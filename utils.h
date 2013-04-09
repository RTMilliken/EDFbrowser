/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2009, 2010, 2011, 2012, 2013 Teunis van Beelen
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







#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif


#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void remove_trailing_spaces(char *);
void remove_leading_spaces(char *);
void remove_trailing_zeros(char *);
void convert_trailing_zeros_to_spaces(char *);
void remove_extension_from_filename(char *);  /* removes extension including the dot */
int get_filename_from_path(char *dest, const char *src, int size);  /* size is size of destination, returns length of filename */
int get_directory_from_path(char *dest, const char *src, int size);  /* size is size of destination, returns length of directory */
void latin1_to_ascii(char *, int);
void latin1_to_utf8(char *, int);
void utf8_to_latin1(char *);
int antoi(const char *, int);
int atoi_nonlocalized(const char *);
double atof_nonlocalized(const char *);
int sprint_number_nonlocalized(char *, double);
long long atoll_x(const char *, int);

/* 3th argument is the minimum digits that will be printed (minus sign not included), leading zero's will be added if necessary */
/* if 4th argument is zero, only negative numbers will have the sign '-' character */
/* if 4th argument is one, the sign '+' or '-' character will always be printed */
/* returns the amount of characters printed */
int sprint_int_number_nonlocalized(char *, int, int, int);
int sprint_ll_number_nonlocalized(char *, long long, int, int);
int fprint_int_number_nonlocalized(FILE *, int, int, int);
int fprint_ll_number_nonlocalized(FILE *, long long, int, int);

/* returns 1 in case the string is not a number */
int is_integer_number(char *);
int is_number(char *);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif


