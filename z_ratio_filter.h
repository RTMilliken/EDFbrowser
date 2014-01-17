/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2012, 2013, 2014 Teunis van Beelen
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







#ifndef zratio_filter_INCLUDED
#define zratio_filter_INCLUDED



#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "global.h"

#include "third_party/kiss_fft/kiss_fftr.h"





struct zratio_filter_settings{
  double crossoverfreq;
  int dftblocksize;
  double samplefreq;
  int fft_outputbufsize;
  double freqstep;
  double f1;
  double f2;
  double f3;
  double f4;
  double *fft_inputbuf;
  double *fft_outputbuf;
  kiss_fftr_cfg cfg;
  kiss_fft_cpx *kiss_fftbuf;
  int smpls_in_inputbuf;
  double zratio_value;
  double *fft_inputbuf_bu;
  int smpls_in_inputbuf_bu;
  double zratio_value_bu;
  double bitvalue;
};



struct zratio_filter_settings * create_zratio_filter(int, long long, double, double);
double run_zratio_filter(double, struct zratio_filter_settings *);
void free_zratio_filter(struct zratio_filter_settings *);
void reset_zratio_filter(struct zratio_filter_settings *);
void zratio_filter_save_buf(struct zratio_filter_settings *);
void zratio_filter_restore_buf(struct zratio_filter_settings *);
#endif





