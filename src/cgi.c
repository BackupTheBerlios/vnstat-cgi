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
#include "cgi.h"

static char *get_GET(void);
void parse_GET();

static char *get_GET(void) {
	char *puffer, *cgi_string, *request = getenv("REQUEST_METHOD");

	if(NULL == request)
		return NULL;
	else if(strcmp(request, "GET")==0) {
		cgi_string = getenv("QUERY_STRING");
		if(NULL==cgi_string)
			return NULL;
		else {
			if ((puffer = malloc(sizeof(char)*strlen(cgi_string)+1))==NULL)
				print_error("Out of memory");
			strcpy(puffer, cgi_string);
			return puffer;
		}
	}
	return NULL;
}

void parse_GET(void) {
	mode = VIEW_SUMMARY;
	show_iface = ifaces;
	char *cgi_string = get_GET();

	if (cgi_string!=NULL && strlen(cgi_string)>=1) {
		switch(cgi_string[0]) {
			case 's':
				mode=VIEW_SUMMARY;
				break;
			case 'm':
				mode=VIEW_MONTHS;
				break;
			case 'd':
				mode=VIEW_DAYS;
				break;
			case 'h':
				mode=VIEW_HOURS;
				break;
			default:
				return;
		}
		if (strlen(cgi_string)>=2) {
			struct iface *current = ifaces;
			while(current!=NULL) {
				if (strcmp(cgi_string+1, current->name)==0) {
					show_iface=current;
					break;
				}
				current=current->next;
			}
		}
	}
	free(cgi_string);

}
