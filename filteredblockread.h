/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2012, 2013 Teunis van Beelen
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





#ifndef filteredblockread_INCLUDED
#define filteredblockread_INCLUDED



#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "global.h"
#include "filter.h"
#include "ravg_filter.h"

#include "third_party/fidlib/fidlib.h"



class FilteredBlockReadClass
{
private:
  FILE *inputfile;

  struct edfhdrblock *hdr;

  struct signalcompblock *signalcomp;

  int datarecord_cnt,
      samples_per_datrec,
      total_samples,
      skip_filters;

  char *readbuf;

  double *processed_samples_buf,
         bitvalue;


public:
  double * init_signalcomp(struct signalcompblock *, int, int);
  int process_signalcomp(int);
  int samples_in_buf(void);
  FilteredBlockReadClass();
  ~FilteredBlockReadClass();
};

#endif





