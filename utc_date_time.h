/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2008, 2009, 2010, 2011, 2012, 2013, 2014 Teunis van Beelen
*
* teuniz@gmail.com
*
***************************************************************************
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation version 2 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
***************************************************************************
*
* This version of GPL is at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*
***************************************************************************
*/



#ifndef UTC_DATE_TIME_INCLUDED
#define UTC_DATE_TIME_INCLUDED


#include <string.h>



#ifdef __cplusplus
extern "C" {
#endif




struct date_time_struct{
         int year;
         int month;
         int day;
         int hour;
         int minute;
         int second;
         char month_str[4];
       };


void utc_to_date_time(long long, struct date_time_struct *);

void date_time_to_utc(long long *, struct date_time_struct);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif


