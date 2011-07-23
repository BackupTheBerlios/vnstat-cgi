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
#include <string.h>

#include "defines.h"
#include "global.h"
#include "error.h"
#include "vnstat.h"

static unsigned long long str_to_long_long(char *str);
static char *vnstat_read_line(FILE *stream);
static void vnstat_parse_line(char *line);
static void sort_dynamic_list(struct record **first, struct record **last, char (*cmpr) (struct record *current, struct record *next));
void load_vnstat(void);
static char cmpr_time(struct record *current, struct record *next);
static char cmpr_total(struct record *current, struct record *next);
static char count_dynamic_list(struct record *start);

static unsigned long long str_to_long_long(char *str) {
	unsigned long long result;
	if (sscanf(str, "%Lu", &result)==0)
		print_error("Unexpected input from vnstat");
	return result;
}

static char *vnstat_read_line(FILE *stream) {
	long size=1;
	char c, *line = malloc(sizeof(char));

	while (!feof(stream) && (c=fgetc(stream))!='\n' && c!=EOF) {
		line[size-1] = c;
		line = realloc(line, sizeof(char)*++size);
	}
	line[size-1]='\0';
	return line;
}

static void vnstat_parse_line(char *line) {
	char *pchar;
	pchar = strtok (line,";");
	if (NULL != pchar) {
		if (strcmp("totalrx", pchar)==0)
			totalrx+= str_to_long_long(strtok (NULL, ";"))<<20;
		else if (strcmp("totaltx", pchar)==0)
			totaltx+= str_to_long_long(strtok (NULL, ";"))<<20;
		else if (strcmp("totalrxk", pchar)==0)
			totalrx+= str_to_long_long(strtok (NULL, ";"))<<10;
		else if (strcmp("totaltxk", pchar)==0)
			totaltx+= str_to_long_long(strtok (NULL, ";"))<<10;

		else if (strcmp("updated", pchar)==0)
					last_update = (time_t)str_to_long_long(strtok (NULL, ";"));

		else if (strcmp("d",pchar)==0 || strcmp("m",pchar)==0 || strcmp("t",pchar)==0 || strcmp("h",pchar)==0) {
			struct record *rec;
			if ((rec = malloc(sizeof(struct record)))==NULL)
				print_error("Out of memory");
			struct record **list = NULL, **list_last = NULL;
			char type=pchar[0];
			if ( type=='d' ) {
				list=&days;
				list_last=&days_last;
			}
			else if (type=='m') {
				list=&months;
				list_last=&months_last;
			}
			else if (type=='h') {
				list=&hours;
				list_last=&hours_last;
			}
			else if (type=='t') {
				list=&top;
				list_last=&top_last;
			}

			if (*list == NULL) {
				*list = rec;
				*list_last=rec;
				rec->prev = NULL;
			}
			else {
				struct record *last = *list_last;
				last->next=rec;
				rec->prev = *list_last;
				*list_last=rec;
			}
			rec->next=NULL;
			rec->number=(int)str_to_long_long(strtok (NULL, ";"));
			rec->time=(time_t)str_to_long_long(strtok (NULL, ";"));
			if (type!='h') {
				rec->rx=str_to_long_long(strtok (NULL, ";"))<<20;
				rec->tx=str_to_long_long(strtok (NULL, ";"))<<20;
				rec->rx+=str_to_long_long(strtok (NULL, ";"))<<10;
				rec->tx+=str_to_long_long(strtok (NULL, ";"))<<10;
			}
			else {
				rec->rx=str_to_long_long(strtok (NULL, ";"))<<10;
				rec->tx=str_to_long_long(strtok (NULL, ";"))<<10;
			}

		}
	}
}

void load_vnstat(void) {
	totalrx=0;
	totaltx=0;
	last_update=0;
	top=NULL;
	months=NULL;
	days=NULL;
	hours=NULL;
	top_last=NULL;
	months_last=NULL;
	days_last=NULL;
	hours_last=NULL;

	FILE *stream;
	char run[strlen(VNSTAT_COMMAND)-2+strlen(show_iface->name)+1];
	char *line;
	sprintf(run, VNSTAT_COMMAND, show_iface->name);
	if ((stream = popen(run, "r"))==NULL)
		print_error("Can't launch vnstat");
	while (!feof(stream)) {
		line = vnstat_read_line(stream);
		vnstat_parse_line(line);
	}

	pclose(stream);

	if (count_dynamic_list(top)!=10 || count_dynamic_list(days)!=30 ||\
			count_dynamic_list(months)!=12 || count_dynamic_list(hours)!=24)
		print_error("Unexpected input from vnstat");

	sort_dynamic_list(&hours, &hours_last, cmpr_time);
	sort_dynamic_list(&months, &months_last, cmpr_time);
	sort_dynamic_list(&days, &days_last, cmpr_time);
	sort_dynamic_list(&top, &top_last, cmpr_total);
}

static void sort_dynamic_list(struct record **first, struct record **last, char (*cmpr) (struct record *current, struct record *next)) {
	char changed=1;
	struct record *current = *first;
	while (changed == 1) {
		changed=0;
		current=*first;
		while(current->next != NULL) {
			if (((*cmpr)(current, current->next)) == 1) {
				changed=1;
				struct record *temp_next = current->next;
				if (current->prev == NULL)
					*first=current->next;
				else
					current->prev->next = current->next;
				if (current->next->next == NULL)
					*last=current;
				else
					current->next->next->prev=current;
				current->next->prev=current->prev;
				current->next = temp_next->next;
				current->prev = temp_next;
				temp_next->next=current;
			}
			else
				current = current->next;
		}
	}
}

static char cmpr_time(struct record *current, struct record *next) {
	if (current->time < next->time)
		return 1;
	return 0;
}
static char cmpr_total(struct record *current, struct record *next) {
	if ((current->rx+current->tx) < (next->rx+next->tx))
			return 1;
		return 0;
}

static char count_dynamic_list(struct record *start) {
	char result=0;
	while(start != NULL) {
		result++;
		start=start->next;
	}
	return result;
}
