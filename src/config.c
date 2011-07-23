/*
 * Copyright (C) 2011 Sebastian P.
 *
 * This file is part of vnStat CGI Frontend.
 *
 * vnStat CGI Frontend is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * vnStat CGI Frontend is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with vnStat CGI Frontend.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <regex.h>
#include <string.h>
#include <locale.h>

#include "defines.h"
#include "global.h"
#include "error.h"
#include "config.h"

void load_config(void);
static char *read_line(FILE *stream);
static void parse_line(char const *line, char **directive, char **value);
static void parse_interface(char const *value);
static void parse_units(char *value);
static void parse_locale(char *value);
static void parse_language(char *value);
static void lowerstr(char *value);

static void parse_interface(char const *value) {
	char *name, *text=NULL;
	regex_t pregex;
	char *pattern = "^([^[:blank:]]+)[[:blank:]]*(.*)$";
	size_t nmatch = 3;
	regmatch_t pmatch[3];

	regcomp(&pregex, pattern, REG_EXTENDED);

	if (0 == regexec(&pregex, value, nmatch, pmatch, 0)) {
		size_t size=pmatch[1].rm_eo - pmatch[1].rm_so + 1;
		if ((name=malloc(sizeof(char)*size)) == NULL)
			print_error("Out of memory");
		memcpy(name, value+pmatch[1].rm_so, size-1);
		name[size-1] = '\0';

		size=pmatch[2].rm_eo - pmatch[2].rm_so + 1;
		if (size!=1) {
			if ((text=malloc(sizeof(char)*size)) == NULL)
				print_error("Out of memory");;
			memcpy(text, value+pmatch[2].rm_so, size);
		}

		struct iface *temp, *last = ifaces;
		while (last != NULL) {
			if ( strcmp(last->name, name)==0 ) {
				free(name);
				if (text!=NULL)
					free(text);
				regfree(&pregex);
				return;
			}
			if (last->next == NULL)
				break;
			last=last->next;
		}

		if ((temp = malloc(sizeof(struct iface)))==NULL)
			print_error("Out of memory");
		temp->name = name;
		temp->text = text;
		temp->next = NULL;
		if (NULL == last)
			ifaces=temp;
		else
			last->next=temp;
	}
	regfree(&pregex);
}

static void lowerstr(char *value) {
	int i;
	for (i=0; i<strlen(value); i++)
		value[i]=tolower(value[i]);
}

static void parse_units(char *value) {
	lowerstr(value);
	if (strcmp(value, "iec")==0)
		units= UNITS_IEC;
	else if (strcmp(value, "si")==0)
		units= UNITS_SI;
}

static void parse_language(char *value) {
	if (strlen(value) != 0) {
		if ((realloc(lang, strlen(value)+1))==NULL)
				print_error("Out of memory");
			strcpy(lang, value);
	}
}

static void parse_locale(char *value) {
	if (strlen(value) != 0) {
		if ((realloc(locale, strlen(value)+1))==NULL)
				print_error("Out of memory");
			strcpy(locale, value);
	}
}

void load_config(void) {
	if ((lang = malloc(strlen(DEFAULT_LANGUAGE)+1))==NULL)
		print_error("Out of memory");
	strcpy(lang, DEFAULT_LANGUAGE);
	if ((locale = malloc(strlen(DEFAULT_LOCALE)+1))==NULL)
		print_error("Out of memory");
	strcpy(locale, DEFAULT_LOCALE);
	units = DEFAULT_UNITS;
	ifaces = NULL;
	FILE *stream;

	if ((stream = fopen(CONFIG_PATH, "r")) == NULL)
		print_error("Can't open config file");;
	while (!feof(stream)) {
		char *line = read_line(stream);
		char *directive, *value;
		parse_line(line, &directive, &value);

		if (directive!=NULL) {
			lowerstr(directive);
			if (strcmp(directive, "language")== 0) {
				parse_language(value);
			}
			else if (strcmp(directive, "locale")== 0) {
				parse_locale(value);
			}
			else if (strcmp(directive, "units")==0) {
				parse_units(value);
			}
			else if (strcmp(directive, "interface")==0) {
				parse_interface(value);
			}
			free(directive);
			free(value);
		}
		free(line);
	}

	if (ifaces == NULL) {
		print_error("No interface specified in config file.");
	}
}

static char *read_line(FILE *stream) {
	long size=1;
	char c, *line;

	if ((line = malloc(sizeof(char)))==NULL)
		print_error("Out of memory");
	while (!feof(stream) && (c=fgetc(stream))!='\n' && c!=EOF) {
		line[size-1] = c;
		if ((line = realloc(line, sizeof(char)*++size))==NULL)
			print_error("Out of memory");;
	}
	line[size-1]='\0';
	return line;
}

static void parse_line(char const *line, char **directive, char **value) {
	regex_t pregex;
	char *pattern = "^[[:blank:]]*([[:punct:][:alnum:]]+)[[:blank:]]*=[[:blank:]]*([^#]+)";
	size_t nmatch = 3;
	regmatch_t pmatch[3];

	regcomp(&pregex, pattern, REG_EXTENDED);

	if (0 == regexec(&pregex, line, nmatch, pmatch, 0)) {
		size_t size=pmatch[1].rm_eo - pmatch[1].rm_so + 1;
		if ((*directive=malloc(sizeof(char)*size)) == NULL)
			print_error("Out of memory");;
		memcpy(*directive, line+pmatch[1].rm_so, size-1);
		(*directive)[size-1] = '\0';

		size=pmatch[2].rm_eo - pmatch[2].rm_so + 1;
		size_t i;
		for (i=size-2; i>0; i--) {
			if (*(line+pmatch[2].rm_so+i)==' ' || *(line+pmatch[2].rm_so+i)=='\t')
				size--;
			else
				break;
		}
		if ((*value=malloc(sizeof(char)*size))==NULL)
			print_error("Out of memory");
		memcpy(*value, line+pmatch[2].rm_so, size-1);
		(*value)[size-1] = '\0';
	}
	else *value=*directive=NULL;

	regfree(&pregex);

}
