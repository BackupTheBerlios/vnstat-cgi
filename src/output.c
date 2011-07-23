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
#include <math.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#include "defines.h"
#include "global.h"
#include "language.h"
#include "output.h"
#include "error.h"

static void print_html_menu(void);
static void print_html_graph(void);
static void print_html_summary(void);
static void print_html_top(void);
static void print_html_css(void);
void print_html(void);
static char *add_unit(long long value);

char units_array[9][4];

static void print_html_css(void) {
	printf("<style type=\"text/css\" media=\"screen\">\n" \
			"body\n" \
			"{\n" \
			"font-family:Verdana;\n" \
			"font-size:12px;\n" \
			"color:black;\n" \
			"background-color:white;\n" \
			"}\n" \
			"h1 {font-size:18px;}\n" \
			"h2 {font-size:16px;}\n" \
			"a:link, a:visited, a:active, a:hover\n" \
			"{\n" \
			"text-decoration:none;\n" \
			"font-weight:bold;\n" \
			"font-size:12px;\n" \
			"color:#0c07e1;\n" \
			"}\n" \
			"a:active, a:hover\n" \
			"{\n" \
			"text-decoration:underline;\n" \
			"}\n" \
			"#left {\n" \
			"width:150px;\n" \
			"float:left;\n" \
			"line-height:18px;\n" \
			"}\n" \
			".menutitle\n" \
			"{\n" \
			"font-weight:bold;\n" \
			"margin-top:15px;\n" \
			"margin-bottom:5px;\n" \
			"}\n" \
			"#content {\n" \
			"width:100%%;\n" \
			"float:left;\n" \
			"}\n" \
			"table\n" \
			"{\n" \
			"border-collapse:collapse;\n" \
			"}\n" \
			"table,th, td\n" \
			"{\n" \
			"border: 1px solid black;\n" \
			"}\n" \
			"th {font-size:14px;}\n" \
			"</style>\n");

}

static void print_html_menu(void) {
	struct iface *current=ifaces;
	while(current!=NULL) {
		printf("<p class=\"menutitle\">%s</p>\n", current->text==NULL?current->name:current->text);
		printf("&raquo; <a href=\"?s%s\">%s</a><br />\n", current->name, translate(LANG_SIDEBAR_SUMMARY));
		printf("&raquo; <a href=\"?h%s\">%s</a><br />\n", current->name, translate(LANG_SIDEBAR_HOURS));
		printf("&raquo; <a href=\"?d%s\">%s</a><br />\n", current->name, translate(LANG_SIDEBAR_DAYS));
		printf("&raquo; <a href=\"?m%s\">%s</a><br />\n", current->name, translate(LANG_SIDEBAR_MONTHS));
		current=current->next;
	}
}

static void print_html_summary(void) {
	printf("<h2>%s</h2>\n", translate(LANG_TABLE_HEADER_SUMMARY));
	printf("<table width=\"100%%\" cellspacing=\"0\">\n" \
			"<colgroup>\n" \
			"<col width=\"25%%\">\n" \
			"<col width=\"25%%\">\n" \
			"<col width=\"25%%\">\n" \
			"<col width=\"25%%\">\n" \
			"</colgroup>\n" \
			"<thead>\n" \
			"<tr>\n" \
			"<th></th>\n" \
			"<th>%s</th>\n" /* LANG_TABLE_IN */ \
			"<th>%s</th>\n" /* LANG_TABLE_OUT */ \
			"<th>%s</th>\n" /* LANG_TABLE_TOTAL */ \
			"</tr>\n" \
			"</thead>\n" \
			"<tbody>\n", translate(LANG_TABLE_IN), translate(LANG_TABLE_OUT), translate(LANG_TABLE_TOTAL));
	time_t tnow;
	time(&tnow);
	struct tm tm_now;
	struct tm *tm_record;
	memcpy(&tm_now, localtime(&tnow), sizeof(struct tm));

	tm_record = localtime(&hours->time);
	printf("<tr>\n");
	printf("<td>%s</td>\n", translate(LANG_SUMMARY_THIS_HOUR));
	if (tm_record->tm_hour==tm_now.tm_hour && tm_record->tm_isdst==tm_now.tm_isdst && tm_record->tm_year==tm_now.tm_year && tm_record->tm_yday==tm_now.tm_yday) {
		printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(hours->rx));
		printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(hours->tx));
		printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(hours->rx+hours->tx));
	}
	else
		printf("<td style=\"text-align: right;\">%s</td>\n" \
				"<td style=\"text-align: right;\">%s</td>\n" \
				"<td style=\"text-align: right;\">%s</td>\n", translate(LANG_NO_DATA), translate(LANG_NO_DATA), translate(LANG_NO_DATA));
	printf("</tr>\n");

	tm_record = localtime(&(days->time));
	printf("<tr>\n");
	printf("<td>%s</td>\n", translate(LANG_SUMMARY_THIS_DAY));
	if (tm_record->tm_year==tm_now.tm_year && tm_record->tm_yday==tm_now.tm_yday) {
		printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(days->rx));
		printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(days->tx));
		printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(days->rx+days->tx));
	}
	else
		printf("<td style=\"text-align: right;\">%s</td>\n" \
						"<td style=\"text-align: right;\">%s</td>\n" \
						"<td style=\"text-align: right;\">%s</td>\n", translate(LANG_NO_DATA), translate(LANG_NO_DATA), translate(LANG_NO_DATA));
	printf("</tr>\n");

	tm_record = localtime(&months->time);
	printf("<tr>\n");
	printf("<td>%s</td>\n", translate(LANG_SUMMARY_THIS_MONTH));
	if (tm_record->tm_mon==tm_now.tm_mon && tm_record->tm_year==tm_now.tm_year) {
		printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(months->rx));
		printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(months->tx));
		printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(months->rx+months->tx));
	}
	else
		printf("<td style=\"text-align: right;\">%s</td>\n" \
						"<td style=\"text-align: right;\">%s</td>\n" \
						"<td style=\"text-align: right;\">%s</td>\n", translate(LANG_NO_DATA), translate(LANG_NO_DATA), translate(LANG_NO_DATA));
	printf("</tr>\n");

	printf("<tr>\n");
	printf("<td>%s</td>\n", translate(LANG_SUMMARY_ALL_TIME));
	printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(totalrx));
	printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(totaltx));
	printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(totalrx+totaltx));
	printf("</tr>\n");

	printf("</tbody>\n</table>\n");
}

static void print_html_top(void) {
	printf("<h2>%s</h2>\n", translate(LANG_TABLE_HEADER_TOP_10_DAYS));
	printf("<table width=\"100%%\" cellspacing=\"0\">\n" \
				"<colgroup>\n" \
				"<col width=\"25%%\">\n" \
				"<col width=\"25%%\">\n" \
				"<col width=\"25%%\">\n" \
				"<col width=\"25%%\">\n" \
				"</colgroup>\n" \
				"<thead>\n" \
				"<tr>\n" \
				"<th></th>\n" \
				"<th>%s</th>\n" /* LANG_TABLE_IN */ \
				"<th>%s</th>\n" /* LANG_TABLE_OUT */ \
				"<th>%s</th>\n" /* LANG_TABLE_TOTAL */ \
				"</tr>\n" \
				"</thead>\n" \
				"<tbody>\n", translate(LANG_TABLE_IN), translate(LANG_TABLE_OUT), translate(LANG_TABLE_TOTAL));

		struct record *current=top;
		while(current!=NULL) {
			if (current->time!=0) {
				printf("<tr>\n");
				char buf[80];
				struct tm *tm_record = localtime(&current->time);
				strftime(buf, sizeof(buf), translate(LANG_DATE_TOP), tm_record);

				printf("<td>%s</td>\n", buf);
				printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(current->rx));
				printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(current->tx));
				printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(current->tx+current->rx));
				printf("</tr>\n");
			}
			current=current->next;
		}

		printf("</tbody>\n" \
				"</table>\n");
}

static void print_html_graph(void) {
	printf("<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" style=\"height:220px; width:60px; float:left;\">\n");
	struct record *data, *data_last;

	char length;
	switch (mode) {
		case VIEW_MONTHS:
			data_last=months_last;
			data=months;
			length=12;
			break;
		case VIEW_DAYS:
			data_last=days_last;
			data=days;
			length=30;
			break;
		case VIEW_HOURS:
			data_last=hours_last;
			data=hours;
			length=24;
			break;
	}
	struct record *current = data_last;
	long double biggest = 0;
	long long divisor=1;
	char pdp=0;
	unsigned char graph_unit=0;
	while(current!=NULL) {
		if ((current->rx+current->tx) > biggest)
			biggest=current->tx+current->rx;
		current=current->prev;
	}
	while (biggest>=((units==UNITS_IEC)?1024:1000)) {
		divisor *= ((units==UNITS_IEC)?1024:1000);
		biggest /= ((units==UNITS_IEC)?1024:1000);
		graph_unit++;
	}

	if (biggest>=1000) {
		short temp = ceil(biggest/100);
		if ((temp%2)==1)
			temp+=1;
		biggest=temp*100;
	}
	else if (biggest>=100) {
		short temp =ceil(biggest/10);
		if ((temp%2)==1)
			temp+=1;
		biggest=temp*10;
	}
	else if (biggest>=10) {
		short temp =ceil(biggest);
		if ((temp%2)==1)
			temp+=1;
		biggest=temp;
	}
	else {
		short temp=ceil(biggest*10);
		if ((temp%2)==1)
			temp+=1;
		biggest=temp/10.;
		pdp=1;
	}

	printf("<text fill=\"black\" stroke=\"none\" x=\"55\" y=\"10\" style=\"text-anchor:end;\">%.*Lf %s</text>\n", pdp, biggest, units_array[graph_unit]);
	printf("<text fill=\"black\" stroke=\"none\" x=\"55\" y=\"105\" style=\"text-anchor:end;\">%.*Lf %s</text>\n", pdp, biggest/2, units_array[graph_unit]);
	printf("<text fill=\"black\" stroke=\"none\" x=\"55\" y=\"200\" style=\"text-anchor:end;\">0 B</text>\n");
	printf("<line x1=\"59\" y1=\"0\" x2=\"59\" y2=\"200\" style=\"stroke:black;\" /></svg>\n");
	printf("<div style=\"padding-left:60px;\"><svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" style=\"width:100%%; height: 220px;\" >\n");
	printf("<line x1=\"0\" y1=\"100\" x2=\"100%%\" y2=\"100\" style=\"stroke:black;stroke-dasharray:3,5;\" />\n");
	printf("<line x1=\"0\" y1=\"1\" x2=\"100%%\" y2=\"1\" style=\"stroke:black;stroke-dasharray:3,5;\" />\n");
	printf("<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" preserveAspectRatio=\"none\" viewBox=\"0 0 %d 100\" width=\"100%%\" height=\"200px\" >\n", length*5);

	current=data_last;
	char height,height_tx, height_rx,c=0;
	while(current!=NULL) {
		if (current->time != 0) {
			height=round(((current->tx+current->rx)*100)/(biggest*divisor)); //TODO: Error handling (Division by 0)
			height_tx=round(((current->tx)*100)/(biggest*divisor)); //TODO: Error handling (Division by 0)
			height_rx=round(((current->rx)*100)/(biggest*divisor)); //TODO: Error handling (Division by 0)
			printf("<rect x=\"%d\" y=\"%d\" width=\"4\" height=\"%d\" style=\"fill:#DCDCE6;\"/>\n", c*5+1, 100-height, height);
			printf("<rect x=\"%d\" y=\"%d\" width=\"2\" height=\"%d\" style=\"fill:#BEBE14;\"/>\n", c*5+1, 100-height_rx, height_rx);
			printf("<rect x=\"%d\" y=\"%d\" width=\"2\" height=\"%d\" style=\"fill:#82A064;\"/>\n", c*5+3, 100-height_tx, height_tx);
		}
		c++;
		current=current->prev;
	}
	printf("</svg>\n");

	printf("<line x1=\"0\" y1=\"100\" x2=\"100%%\" y2=\"100\" style=\"stroke:black;stroke-dasharray:3,5;stroke-opacity:0.3;\"/>\n");
	printf("<line x1=\"0\" y1=\"1\" x2=\"100%%\" y2=\"1\" style=\"stroke:black;stroke-dasharray:3,5;stroke-opacity:0.3;\"/>\n");
	printf("<line x1=\"0\" y1=\"200\" x2=\"100%%\" y2=\"200\" style=\"stroke:black;\" />\n");
	current=data_last;
	c=0;
	while(current!=NULL) {
		char buf[80];
		if (current->time != 0) {
			struct tm *tm_record = localtime(&current->time);
			switch (mode) {
				case VIEW_MONTHS:
					strftime(buf, sizeof(buf), translate(LANG_DATE_GRAPH_MONTHS), tm_record);
					break;
				case VIEW_DAYS:
					strftime(buf, sizeof(buf), translate(LANG_DATE_GRAPH_DAYS), tm_record);
					break;
				case VIEW_HOURS:
					strftime(buf, sizeof(buf), translate(LANG_DATE_GRAPH_HOURS), tm_record);
					break;
			}
		}
		else
			strncpy(buf, translate(LANG_NO_DATA_GRAPH), sizeof(buf));
		setlocale(LC_ALL, "C"); // use . as decimal point
		printf("<text fill=\"black\" stroke=\"none\" x=\"%f%%\" y=\"215\" style=\"text-anchor:middle;\" >%s</text>",100./length*(3./5+c), buf);
		setlocale(LC_ALL, locale);
		c++;
		current=current->prev;
	}
	printf("</svg>\n");
	printf("<span style=\"color:#BEBE14; font-weight:bold;\">&bull;</span> %s&nbsp;&nbsp;&nbsp;\n", translate(LANG_GRAPH_IN));
	printf("<span style=\"color:#82A064; font-weight:bold;\">&bull;</span> %s&nbsp;&nbsp;&nbsp;\n", translate(LANG_GRAPH_OUT));
	printf("<span style=\"color:#DCDCE6; font-weight:bold;\">&bull;</span>  %s\n", translate(LANG_GRAPH_TOTAL));
	printf("</div>\n");
	printf("<div style=\"clear:both;\"/>\n");

	switch (mode) {
		case VIEW_MONTHS:
			printf("<h2>%s</h2>\n", translate(LANG_TABLE_HEADER_LAST_12_MONTHS));
			break;
		case VIEW_DAYS:
			printf("<h2>%s</h2>\n", translate(LANG_TABLE_HEADER_LAST_30_DAYS));
			break;
		case VIEW_HOURS:
			printf("<h2>%s</h2>\n", translate(LANG_TABLE_HEADER_LAST_24_HOURS));
			break;
	}

	printf("<table width=\"100%%\" cellspacing=\"0\">\n" \
				"<colgroup>\n" \
				"<col width=\"25%%\">\n" \
				"<col width=\"25%%\">\n" \
				"<col width=\"25%%\">\n" \
				"<col width=\"25%%\">\n" \
				"</colgroup>\n" \
				"<thead>\n" \
				"<tr>\n" \
				"<th></th>\n" \
				"<th>%s</th>\n" /* LANG_TABLE_IN */ \
				"<th>%s</th>\n" /* LANG_TABLE_OUT */ \
				"<th>%s</th>\n" /* LANG_TABLE_TOTAL */ \
				"</tr>\n" \
				"</thead>\n" \
				"<tbody>\n", translate(LANG_TABLE_IN), translate(LANG_TABLE_OUT), translate(LANG_TABLE_TOTAL));

	current=data;
	while(current!=NULL) {
		if (current->time!=0) {
			printf("<tr>\n");
			char buf[80];
			struct tm *tm_record = localtime(&current->time);
			switch (mode) {
				case VIEW_MONTHS:
					strftime(buf, sizeof(buf), translate(LANG_DATE_MONTHS), tm_record);
					break;
				case VIEW_DAYS:
					strftime(buf, sizeof(buf), translate(LANG_DATE_DAYS), tm_record);
					break;
				case VIEW_HOURS:
					strftime(buf, sizeof(buf), translate(LANG_DATE_HOURS), tm_record);
					break;
			}
			printf("<td>%s</td>\n", buf);
			printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(current->rx));
			printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(current->tx));
			printf("<td style=\"text-align: right;\">%s</td>\n", add_unit(current->tx+current->rx));
			printf("</tr>\n");
		}
		current=current->next;
	}
	printf("</tbody>\n" \
			"</table>\n");
}

static char *add_unit(long long input) {
	static char res[13]; // strlen("1,000.00 YiB")+1
	long double value = input;
	unsigned char c=0;

	while (value>=((units==UNITS_IEC)?1024:1000)) {
		value/=((units==UNITS_IEC)?1024:1000);
		c++;
	}
	snprintf(res, sizeof(res), "%.2Lf %s", value, units_array[c]);

	return res;
}

void print_html(void) {
	if (setlocale(LC_ALL, locale) == NULL)
		print_error("Can't set locale");
	if (units==UNITS_IEC)
		memcpy(units_array,"B\0  KiB\0MiB\0GiB\0TiB\0PiB\0EiB\0ZiB\0YiB\0", 36);
	else
		memcpy(units_array,"B\0  kB\0 MB\0 GB\0 TB\0 PB\0 EB\0 ZB\0 YB\0 ", 36);

	printf(		"Vary: Accept\n" \
			"Content-Type: text/html; charset=utf-8\n" \
			"Cache-Control: no-cache\n" \
			"Expires: 0\n\n" \
			\
			"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\n" \
			"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/Strict.dtd\">\n" \
			"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"auto\" lang=\"auto\">\n" \
			"<head>\n" \
			"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n");

	print_html_css();

	printf("<title>%s (v%s)</title>\n" /* TITLE and VERSION */ \
			"</head>\n" \
			"<body>\n" \
			"<div id=\"left\">\n", TITLE, VERSION);

	print_html_menu();

	printf("</div>\n" \
			"<div style=\"padding-left:180px;\">\n" \
			"<div id=\"content\">\n");

	printf("<h1 style=\"margin-bottom: 0px;\">");
	if (NULL==show_iface->text)
		printf(translate(LANG_HEADER_SHORT), show_iface->name);
	else
		printf(translate(LANG_HEADER), show_iface->text, show_iface->name);
	printf("</h1>\n");

	{ // Show Last Update
		char buf[80];
		struct tm *tm_last_update = localtime(&last_update);
		strftime(buf, sizeof(buf), translate(LANG_DATE_LAST_UPDATE), tm_last_update);
		printf("<p style=\"margin-top: 0px;\">");
		printf(translate(LANG_LAST_UPDATE), buf);
		printf("</p>\n");
	}

	if (mode != VIEW_SUMMARY) print_html_graph();
	else {
		print_html_summary();
		print_html_top();
	}

	printf("<br />\n" \
			"</div>\n" \
			"</div>\n" \
			"</body>\n" \
			"</html>\n");
}
