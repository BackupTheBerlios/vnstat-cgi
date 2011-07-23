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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "defines.h"
#include "global.h"
#include "error.h"
#include "language.h"

void load_language(void);
char *translate(unsigned char index);
static char *read_line(FILE *stream);

char *translate(unsigned char index) {
	return translations[index];
}

void load_language(void) {
	FILE *stream;
	char path[strlen(LANGUAGE_PATH)+strlen(lang)+1];
	strcpy(path, LANGUAGE_PATH);
	strcpy(path+strlen(LANGUAGE_PATH), lang);

	if ((stream = fopen(path, "r")) == NULL)
		print_error("Can't open language file");
	unsigned char c=0;
	while (!feof(stream)) {
		char *line = read_line(stream);
		if (strlen(line)!=0 && line[0]!='#') {
			if (c==LANG_LENGTH)
				print_error("Unexpected input from language file");
			translations[c]=line;
			c++;
		}
	}
	if (c!=LANG_LENGTH)
		print_error("Unexpected input from language file");
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
