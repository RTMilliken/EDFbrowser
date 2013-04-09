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
* The current version of GPL is at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*
***************************************************************************
*/




#include "filter.h"



struct filter_settings * create_filter(int is_LPF, double cutoff_frequency, double sample_frequency)
{
  struct filter_settings *settings;

  settings = (struct filter_settings *) calloc(1, sizeof(struct filter_settings));
  if(settings==NULL)  return(NULL);

  settings->is_LPF = is_LPF;
  settings->cutoff_frequency = cutoff_frequency;
  settings->sample_frequency = sample_frequency;
  settings->factor = get_filter_factor(cutoff_frequency, sample_frequency);
  settings->old_input = 0.0;
  settings->old_output = 0.0;
  settings->first_sample = 1;

  return(settings);
}


struct filter_settings * create_filter_copy(struct filter_settings *src)
{
  struct filter_settings *settings;

  settings = (struct filter_settings *)calloc(1, sizeof(struct filter_settings));
  if(settings==NULL)
  {
    return(NULL);
  }
  memcpy(settings, src, sizeof(struct filter_settings));

  return(settings);
}


void reset_filter(int offset, struct filter_settings *settings)
{
  settings->first_sample = 0;
  settings->old_input = offset;
  if(settings->is_LPF)
  {
    settings->old_output = offset;
  }
  else
  {
    settings->old_output = 0.0;
  }
}


int first_order_filter(int new_input, struct filter_settings *settings)
{
  double new_output;


  if(settings->first_sample)
  {
    settings->first_sample = 0;

    settings->old_input = new_input;

    if(settings->is_LPF)
    {
      settings->old_output = new_input;

      return(new_input);
    }
    else
    {
      settings->old_output = 0.0;

      return(0);
    }
  }

  if(settings->is_LPF)
  {
    new_output = settings->old_output + ((settings->old_input - settings->old_output) * settings->factor);
  }
  else
  {
    new_output = settings->old_output * (1.0 - settings->factor);
    new_output += new_input - settings->old_input;
  }

  settings->old_input = new_input;
  settings->old_output = new_output;

  return((int)new_output);
}


double get_filter_factor(double cutoff_frequency, double sample_frequency)
{
  double t;

  t = (cutoff_frequency * 2.0 * M_PI) / sample_frequency;
  t = exp(-t);
  t = 1.0 - t; // now t will be in the range 0.001 (at high sample-frequency and low cut-off)
               // to 0.999 (at low sample-frequency and high cut-off)

  return(t);
}




