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

#ifndef DEFINES_H_
#define DEFINES_H_

// Configuration
#define CONFIG_PATH "config/config.conf"
// Example: #define CONFIG_PATH "/etc/vnstat-cgi/config.conf"

#define LANGUAGE_PATH "lang/"
// Example: #define LANGUAGE_PATH "/usr/share/vnstat-cgi/lang/"

#define VNSTAT_COMMAND "vnstat -i %s --dumpdb"
// Example: #define VNSTAT_COMMAND "cat /dumpdir/%s"

#define TITLE "vnStat - CGI Frontend"
#define VERSION "0.1"
#define DEFAULT_LANGUAGE "en"
#define DEFAULT_LOCALE "C"
#define DEFAULT_UNITS UNITS_IEC

#endif /* DEFINES_H_ */
