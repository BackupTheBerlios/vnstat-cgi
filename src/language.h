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

#ifndef LANGUAGE_H_
#define LANGUAGE_H_

enum {
	LANG_SIDEBAR_SUMMARY,
	LANG_SIDEBAR_HOURS,
	LANG_SIDEBAR_DAYS,
	LANG_SIDEBAR_MONTHS,

	LANG_TABLE_HEADER_SUMMARY,
	LANG_TABLE_HEADER_TOP_10_DAYS,
	LANG_TABLE_HEADER_LAST_24_HOURS,
	LANG_TABLE_HEADER_LAST_30_DAYS,
	LANG_TABLE_HEADER_LAST_12_MONTHS,

	LANG_TABLE_IN,
	LANG_TABLE_OUT,
	LANG_TABLE_TOTAL,

	LANG_SUMMARY_THIS_HOUR,
	LANG_SUMMARY_THIS_DAY,
	LANG_SUMMARY_THIS_MONTH,
	LANG_SUMMARY_ALL_TIME,

	LANG_HEADER,
	LANG_HEADER_SHORT,
	LANG_LAST_UPDATE,

	LANG_GRAPH_IN,
	LANG_GRAPH_OUT,
	LANG_GRAPH_TOTAL,

	LANG_DATE_DAYS,
	LANG_DATE_GRAPH_DAYS,
	LANG_DATE_MONTHS,
	LANG_DATE_GRAPH_MONTHS,
	LANG_DATE_HOURS,
	LANG_DATE_GRAPH_HOURS,
	LANG_DATE_TOP, // Top 10 days
	LANG_DATE_LAST_UPDATE,

	LANG_NO_DATA,
	LANG_NO_DATA_GRAPH,

	LANG_LENGTH
};

char *translations[LANG_LENGTH];

void load_language(void);
char *translate(unsigned char index);

#endif /* LANGUAGE_H_ */
