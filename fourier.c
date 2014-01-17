/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2010, 2011, 2012, 2013, 2014 Teunis van Beelen
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


/*
*****************************************************************************
*
* frequency analysis (power spectrum)
*
* Signal X sampled to N samples in X0, X1, X2, ... Xn-1
* Sample interval t(tau) seconds (i.e. 0.01 at 100 Hz samplefreq.)
*
* Powerspectrum P
* Calculated for N/2 frequencypoints op P0, P1, P2, ... Pn/2-1
* Frequencyresolution dF(delta F) = 1/Nt (i.e. 0.1 with 10 seconds signal (N = 1000))
*
* Calculate Pi, thus for frequency i * dF (i = 0, 1, 2, ... N/2-1)
*
* REi = {X0 * cos((2*pi*i/N) * 0)  +  X1 * cos((2*pi*i/N) * 1)  +  X2 * cos((2*pi*i/N) * 2)  +  ...  +  XN-1 * cos((2*pi*i/N) * (N-1))}
*
* IMi = {X0 * sin((2*pi*i/N) * 0)  +  X1 * sin((2*pi*i/N) * 1)  +  X2 * sin((2*pi*i/N) * 2)  +  ...  +  XN-1 * sin((2*pi*i/N) * (N-1))}
*
* Pi = ( (2/Fs) * {REi*REi + IMi*IMi} ) / N
*
*****************************************************************************
*/



#include "fourier.h"


#define _GNU_SOURCE


#define DUB_PI 6.28318530717958623199592693708837032318115234375



float fourier_transform_get_power_float(int k, float *buf, int bufsize, float samplefreq)
{
  int n;

  float var,
        rei=0.0,
        imi=0.0;

  var = (DUB_PI * k) / bufsize;

  for(n=0; n<bufsize; n++)
  {
    rei += (buf[n] * cos(var * n));
    imi += (buf[n] * sin(var * n));
  }

  if(!k)    /* DC! */
  {
    return(((1.0 / samplefreq) * ((rei * rei) + (imi * imi))) / bufsize);
  }

  return(((2.0 / samplefreq) * ((rei * rei) + (imi * imi))) / bufsize);
}


double fourier_transform_get_power_double(int k, double *buf, int bufsize, double samplefreq)
{
  int n;

  double var,
         rei=0.0,
         imi=0.0;

  var = (DUB_PI * k) / bufsize;

  for(n=0; n<bufsize; n++)
  {
    rei += (buf[n] * cos(var * n));
    imi += (buf[n] * sin(var * n));
  }

  if(!k)    /* DC! */
  {
    return(((1.0 / samplefreq) * ((rei * rei) + (imi * imi))) / bufsize);
  }

  return(((2.0 / samplefreq) * ((rei * rei) + (imi * imi))) / bufsize);
}















