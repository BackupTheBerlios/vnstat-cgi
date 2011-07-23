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

void print_error(char *msg) {
	printf(		"Vary: Accept\n" \
			"Content-Type: text/html; charset=utf-8\n" \
			"Cache-Control: no-cache\n" \
			"Expires: 0\n\n" \
			\
			"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\n" \
			"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/Strict.dtd\">\n" \
			"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"auto\" lang=\"auto\">\n" \
			"<head>\n" \
			"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n" \
			"<title>Error</title>\n" \
			"</head>\n" \
			"<body>\n" \
			"%s\n" /* Message */ \
			"</body>\n" \
			"</html>\n", msg);
	exit(EXIT_FAILURE);
}
