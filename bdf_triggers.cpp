/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2010, 2011, 2012, 2013 Teunis van Beelen
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



#include "bdf_triggers.h"



#if defined(__APPLE__) || defined(__MACH__) || defined(__APPLE_CC__)

#define fopeno fopen

#else

#define fseeko fseeko64
#define ftello ftello64
#define fopeno fopen64

#endif



int BDF_triggers::get_triggers(struct edfhdrblock *hdr, struct annotationblock **list)
{
  int i, j,
      sf,
      status_signal=0,
      error,
      edfsignals,
      bufsize,
      jump_file,
      trigger_cnt,
      status[24];

  char *buf;

  long long datarecords,
            long_data_record_duration,
            records_read,
            status_sample_duration,
            progress_steps;

  FILE *inputfile;

  struct annotationblock *annotation=NULL;

  struct annotationblock *annotlist;



  *list = NULL;

  annotlist = NULL;

  inputfile = hdr->file_hdl;
  edfsignals = hdr->edfsignals;
  datarecords = hdr->datarecords;
  long_data_record_duration = hdr->long_data_record_duration;

  if(edfsignals < 1)
  {
    return(0);
  }

  if(long_data_record_duration != TIME_DIMENSION)
  {
    return(0);
  }

  sf = hdr->edfparam[0].smp_per_record;

  for(i=1; i<edfsignals; i++)
  {
    if(hdr->edfparam[i].smp_per_record != sf)
    {
      return(0);
    }
  }

  error = 1;

  switch(sf)
  {
    case 16384 : error = 0;
                 break;
    case  8192 : error = 0;
                 break;
    case  4096 : error = 0;
                 break;
    case  2048 : error = 0;
                 break;
    case  1024 : error = 0;
                 break;
    case   512 : error = 0;
                 break;
    case   256 : error = 0;
                 break;
    case   128 : error = 0;
                 break;
    case    64 : error = 0;
                 break;
    case    32 : error = 0;
                 break;
  }

  if(error)
  {
    return(0);
  }

  for(i=0; i<edfsignals; i++)
  {
    if(!(strcmp(hdr->edfparam[i].label, "Status          ")))
    {
      status_signal = i;

      break;
    }
  }

  if(i == edfsignals)
  {
    return(0);
  }

  bufsize = sf * 3;

  buf = (char *)calloc(1, bufsize);
  if(buf==NULL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Memory allocation error occurred when trying to read triggers.\n(buf)");
    messagewindow.exec();
    return(1);
  }

  if(fseeko(inputfile, ((long long)edfsignals + 1LL) * 256LL, SEEK_SET))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "An error occurred when reading inputfile triggers.");
    messagewindow.exec();
    free(buf);
    return(2);
  }

  jump_file = status_signal * sf * 3;

  status_sample_duration = TIME_DIMENSION / (long long)sf;

  trigger_cnt = 0;

  for(i=0; i<24; i++)
  {
    status[i] = 1;
  }

  QProgressDialog progress("Scanning file for triggers...", "Abort", 0, datarecords);
  progress.setWindowModality(Qt::WindowModal);
  progress.setMinimumDuration(200);

  progress_steps = datarecords / 100LL;
  if(progress_steps < 1LL)
  {
    progress_steps = 1LL;
  }

  for(records_read=0LL; records_read<datarecords; records_read++)
  {
    if(!(records_read%progress_steps))
    {
      progress.setValue((int)records_read);

      qApp->processEvents();

      if(progress.wasCanceled() == TRUE)
      {
        break;
      }
    }

    if(trigger_cnt >= 100000)
    {
      break;
    }

    if(fseeko(inputfile, (long long)jump_file, SEEK_CUR))
    {
      progress.reset();
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "An error occurred when reading inputfile triggers.");
      messagewindow.exec();
      free(buf);
      return(2);
    }

    if(fread(buf, bufsize, 1, inputfile)!=1)
    {
      progress.reset();
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "An error occurred when reading inputfile triggers.");
      messagewindow.exec();
      free(buf);
      return(2);
    }

    for(i=0; i<bufsize; i+=3)
    {
      if(buf[i + 2] & 1)
      {
        if(!status[16])  // rising edge detected
        {
          annotation = (struct annotationblock *)calloc(1, sizeof(struct annotationblock));
          if(annotation == NULL)
          {
            progress.reset();
            QMessageBox messagewindow(QMessageBox::Critical, "Error", "Malloc error (annotation).");
            messagewindow.exec();
            *list = annotlist;
            free(buf);
            return(1);
          }
          annotation->file_num = hdr->file_num;
          annotation->onset = (records_read * TIME_DIMENSION) + ((long long)(i / 3) * status_sample_duration);
          annotation->onset += hdr->starttime_offset;
          sprintf(annotation->annotation, "new epoch");
          edfplus_annotation_add_item(&annotlist, annotation);

          trigger_cnt++;

          status[16] = 1;
        }
      }
      else
      {
        if(status[16])  // falling edge detected
        {
          status[16] = 0;
        }
      }

      for(j=0; j<16; j++)
      {
        if(*((unsigned short *)(buf + i)) & (1 << j))
        {
          if(!status[j])  // rising edge detected
          {
            annotation = (struct annotationblock *)calloc(1, sizeof(struct annotationblock));
            if(annotation == NULL)
            {
              progress.reset();
              QMessageBox messagewindow(QMessageBox::Critical, "Error", "Malloc error (annotation).");
              messagewindow.exec();
              *list = annotlist;
              free(buf);
              return(1);
            }
            annotation->onset = (records_read * TIME_DIMENSION) + ((long long)(i / 3) * status_sample_duration);
            annotation->onset += hdr->starttime_offset;
            sprintf(annotation->annotation, "Trigger Input %i", j + 1);
            edfplus_annotation_add_item(&annotlist, annotation);

            trigger_cnt++;

            status[j] = 1;
          }
        }
        else
        {
          if(status[j])  // falling edge detected
          {
            status[j] = 0;
          }
        }
      }
    }
  }

  progress.reset();

  *list = annotlist;

  free(buf);

  hdr->genuine_biosemi = 1;

  return(0);
}















