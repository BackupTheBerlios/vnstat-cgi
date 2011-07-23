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

#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <time.h>

enum {
	UNITS_IEC,
	UNITS_SI
};
enum {
	VIEW_SUMMARY,
	VIEW_HOURS,
	VIEW_DAYS,
	VIEW_MONTHS
};

struct iface *show_iface;
char mode;

char *lang, *locale;
char units;
struct iface {
	char *name;
	char *text;
	struct iface *next;
} *ifaces;

struct record {
	unsigned char number;
	time_t time;
	unsigned long long rx, tx;
	struct record *next, *prev;
} *top, *months, *days, *hours, *top_last, *months_last, *days_last, *hours_last;
unsigned long long totalrx, totaltx;
time_t last_update;

#endif /* GLOBAL_H_ */
