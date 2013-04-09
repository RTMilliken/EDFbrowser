/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2007, 2008, 2009, 2010, 2011, 2012, 2013 Teunis van Beelen
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







#ifndef filter_INCLUDED
#define filter_INCLUDED



#include <stdlib.h>
#include <string.h>
#include <math.h>




struct filter_settings{
                       double old_input;
                       double old_output;
                       double factor;
                       double sample_frequency;
                       double cutoff_frequency;
                       int    is_LPF;
                       int first_sample;
                      };



struct filter_settings * create_filter(int, double, double);
void reset_filter(int, struct filter_settings *);
int first_order_filter(int, struct filter_settings *);
double get_filter_factor(double, double);
struct filter_settings * create_filter_copy(struct filter_settings *);



#endif


