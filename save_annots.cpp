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



#include "save_annots.h"



#if defined(__APPLE__) || defined(__MACH__) || defined(__APPLE_CC__)

#define fopeno fopen

#else

#define fseeko fseeko64
#define ftello ftello64
#define fopeno fopen64

#endif




int save_annotations(UI_Mainwindow *mainwindow, FILE *outputfile, struct edfhdrblock *hdr, struct annotationblock *annotlist)
{
  int i, j, k, n, p=0,
      new_edfsignals=0,
      signalslist[MAXSIGNALS],
      datarecords,
      annot_len,
      annot_smp_per_record,
      annot_recordsize,
      timestamp_decimals,
      timestamp_digits,
      annots_per_datrec,
      space,
      progress_steps;

  char *readbuf,
       scratchpad[256],
       *annot_buf;

  long long time;

  FILE *inputfile;

  struct annotationblock *annot;

  inputfile = hdr->file_hdl;

  for(i=0; i<hdr->edfsignals; i++)
  {
    if(!hdr->edfparam[i].annotation)
    {
      signalslist[new_edfsignals] = i;

      new_edfsignals++;
    }
  }

  datarecords = hdr->datarecords;

  time = (hdr->datarecords * hdr->long_data_record_duration) / TIME_DIMENSION;

  timestamp_decimals = get_tal_timestamp_decimal_cnt(hdr);
  if(timestamp_decimals < 0)
  {
    return(1);
  }

  timestamp_digits = get_tal_timestamp_digit_cnt(hdr);
  if(timestamp_digits < 0)
  {
    return(1);
  }

  annot = annotlist;

  annot_len = get_max_annotation_strlen(&annot);

  i = edfplus_annotation_count(&annot);

  annots_per_datrec = i / datarecords;

  if(i % datarecords)
  {
    annots_per_datrec++;
  }

  annot_recordsize = (annot_len * annots_per_datrec) + timestamp_digits + timestamp_decimals + 4;

  if(timestamp_decimals)
  {
    annot_recordsize++;
  }

  if(hdr->edf)
  {
    annot_smp_per_record = annot_recordsize / 2;

    if(annot_recordsize % annot_smp_per_record)
    {
      annot_smp_per_record++;

      annot_recordsize = annot_smp_per_record * 2;
    }
  }
  else
  {
    annot_smp_per_record = annot_recordsize / 3;

    if(annot_recordsize % annot_smp_per_record)
    {
      annot_smp_per_record++;

      annot_recordsize = annot_smp_per_record * 3;
    }
  }

  readbuf = (char *)malloc(hdr->recordsize);
  if(readbuf==NULL)
  {
    return(1);
  }

  annot_buf = (char *)malloc(annot_recordsize + 10);
  if(annot_buf==NULL)
  {
    free(readbuf);
    return(1);
  }

///////////////////////////////////////////////////////////////////

  rewind(outputfile);

  if(hdr->edf)
  {
    fprintf(outputfile, "0       ");
  }
  else
  {
    fprintf(outputfile, "XBIOSEMI");
    rewind(outputfile);
    fputc(255, outputfile);
    fseeko(outputfile, 0LL, SEEK_END);
  }
  fseeko(inputfile, 8LL, SEEK_SET);
  if((hdr->edfplus)||(hdr->bdfplus))
  {
    if(fread(scratchpad, 176, 1, inputfile)!=1)
    {
      free(readbuf);
      free(annot_buf);
      return(2);
    }

    if((hdr->genuine_nk) && (hdr->nk_triggers_read))
    {
      space = 0;

      for(n=80; n<160; n++)
      {
        if(scratchpad[n] == ' ')
        {
          space++;

          if(space > 3)
          {
            break;
          }
        }
      }

      n += 14;

      if(n<150)
      {
        if(!strncmp(scratchpad + n, "EEG", 3))
        {
          scratchpad[n] = 'e';
          scratchpad[n+1] = 'e';
          scratchpad[n+2] = 'g';
        }
      }
    }

    if(fwrite(scratchpad, 176, 1, outputfile)!=1)
    {
      free(readbuf);
      free(annot_buf);
      return(3);
    }
  }
  else
  {
    sprintf(scratchpad, "X X X X ");
    if(fread(scratchpad + 8, 80, 1, inputfile)!=1)
    {
      free(readbuf);
      free(annot_buf);
      return(2);
    }
    if(fwrite(scratchpad, 80, 1, outputfile)!=1)
    {
      free(readbuf);
      free(annot_buf);
      return(3);
    }
    sprintf(scratchpad, "Startdate X X X X ");
    if(fread(scratchpad + 18, 80, 1, inputfile)!=1)
    {
      free(readbuf);
      free(annot_buf);
      return(2);
    }
    if(fwrite(scratchpad, 80, 1, outputfile)!=1)
    {
      free(readbuf);
      free(annot_buf);
      return(3);
    }
    if(fread(scratchpad, 16, 1, inputfile)!=1)
    {
      free(readbuf);
      free(annot_buf);
      return(2);
    }
    if(fwrite(scratchpad, 16, 1, outputfile)!=1)
    {
      free(readbuf);
      free(annot_buf);
      return(3);
    }
  }
  fprintf(outputfile, "%-8i", (new_edfsignals * 256) + 512);
  if(hdr->edf)
  {
    fprintf(outputfile, "EDF+C");
  }
  else
  {
    fprintf(outputfile, "BDF+C");
  }
  for(i=0; i<39; i++)
  {
    fputc(' ', outputfile);
  }
  fprintf(outputfile, "%-8i", datarecords);
  snprintf(scratchpad, 256, "%.12f", hdr->data_record_duration);
  remove_trailing_zeros(scratchpad);
  strcat(scratchpad, "         ");
  if((!strncmp(scratchpad, "0.", 2)) && (scratchpad[8] != ' '))
  {
    scratchpad[9] = 0;
    fprintf(outputfile, "%s", scratchpad + 1);
  }
  else
  {
    scratchpad[8] = 0;
    fprintf(outputfile, "%s", scratchpad);
  }
  fprintf(outputfile, "%-4i", new_edfsignals + 1);

  for(i=0; i<new_edfsignals; i++)
  {
    fprintf(outputfile, "%s", hdr->edfparam[signalslist[i]].label);
  }
  if(hdr->edf)
  {
    fprintf(outputfile, "%s", "EDF Annotations ");
  }
  else
  {
    fprintf(outputfile, "%s", "BDF Annotations ");
  }
  for(i=0; i<new_edfsignals; i++)
  {
    fprintf(outputfile, "%s", hdr->edfparam[signalslist[i]].transducer);
  }
  for(i=0; i<80; i++)
  {
    fputc(' ', outputfile);
  }
  for(i=0; i<new_edfsignals; i++)
  {
    fprintf(outputfile, "%s", hdr->edfparam[signalslist[i]].physdimension);
  }
  for(i=0; i<8; i++)
  {
    fputc(' ', outputfile);
  }
  for(i=0; i<new_edfsignals; i++)
  {
    snprintf(scratchpad, 256, "%f", hdr->edfparam[signalslist[i]].phys_min);
    for(k=0; k<8; k++)
    {
      if(scratchpad[k]=='.')
      {
        for(j=7; j>=0; j--)
        {
          if((scratchpad[j]!='.')&&(scratchpad[j]!='0'))
          {
            break;
          }

          if(scratchpad[j]=='.')
          {
            scratchpad[j] = ' ';
            break;
          }

          scratchpad[j] = ' ';
        }
        break;
      }
      scratchpad[8] = 0;
    }
    fprintf(outputfile, "%s", scratchpad);
  }
  fprintf(outputfile, "-1      ");
  for(i=0; i<new_edfsignals; i++)
  {
    snprintf(scratchpad, 256, "%f", hdr->edfparam[signalslist[i]].phys_max);
    for(k=0; k<8; k++)
    {
      if(scratchpad[k]=='.')
      {
        for(j=7; j>=0; j--)
        {
          if((scratchpad[j]!='.')&&(scratchpad[j]!='0'))
          {
            break;
          }

          if(scratchpad[j]=='.')
          {
            scratchpad[j] = ' ';
            break;
          }

          scratchpad[j] = ' ';
        }
        break;
      }
    }
    scratchpad[8] = 0;
    fprintf(outputfile, "%s", scratchpad);
  }
  fprintf(outputfile, "1       ");
  for(i=0; i<new_edfsignals; i++)
  {
    fprintf(outputfile, "%-8i", hdr->edfparam[signalslist[i]].dig_min);
  }
  if(hdr->edf)
  {
    fprintf(outputfile, "%s", "-32768  ");
  }
  else
  {
    fprintf(outputfile, "%s", "-8388608");
  }
  for(i=0; i<new_edfsignals; i++)
  {
    fprintf(outputfile, "%-8i", hdr->edfparam[signalslist[i]].dig_max);
  }
  if(hdr->edf)
  {
    fprintf(outputfile, "%s", "32767   ");
  }
  else
  {
    fprintf(outputfile, "%s", "8388607 ");
  }
  for(i=0; i<new_edfsignals; i++)
  {
    fprintf(outputfile, "%s", hdr->edfparam[signalslist[i]].prefilter);
  }
  for(i=0; i<80; i++)
  {
    fputc(' ', outputfile);
  }
  for(i=0; i<new_edfsignals; i++)
  {
    fprintf(outputfile, "%-8i", hdr->edfparam[signalslist[i]].smp_per_record);
  }
  fprintf(outputfile, "%-8i", annot_smp_per_record);
  for(i=0; i<((new_edfsignals * 32) + 32); i++)
  {
   fputc(' ', outputfile);
  }

///////////////////////////////////////////////////////////////////

  QProgressDialog progress("Saving file...", "Abort", 0, datarecords, mainwindow);
  progress.setWindowModality(Qt::WindowModal);
  progress.setMinimumDuration(200);

  progress_steps = datarecords / 100;
  if(progress_steps < 1)
  {
    progress_steps = 1;
  }

  fseeko(inputfile, (long long)(hdr->hdrsize), SEEK_SET);

  annot = annotlist;

  time = hdr->starttime_offset;

  for(k=0; k<datarecords; k++)
  {
    if(!(k%progress_steps))
    {
      progress.setValue(k);

      qApp->processEvents();

      if(progress.wasCanceled() == TRUE)
      {
        free(readbuf);
        free(annot_buf);
        return(4);
      }
    }

    if(fread(readbuf, hdr->recordsize, 1, inputfile) != 1)
    {
      free(readbuf);
      free(annot_buf);
      return(2);
    }

    if(hdr->edf)
    {
      for(i=0; i<new_edfsignals; i++)
      {
        for(j=0; j<hdr->edfparam[signalslist[i]].smp_per_record; j++)
        {
          fputc(*((unsigned char *)(readbuf + hdr->edfparam[signalslist[i]].buf_offset + (j * 2))), outputfile);
          fputc(*((unsigned char *)(readbuf + hdr->edfparam[signalslist[i]].buf_offset + (j * 2) + 1)), outputfile);
        }
      }
    }
    else
    {
      for(i=0; i<new_edfsignals; i++)
      {
        for(j=0; j<hdr->edfparam[signalslist[i]].smp_per_record; j++)
        {
          fputc(*((unsigned char *)(readbuf + hdr->edfparam[signalslist[i]].buf_offset + (j * 3))), outputfile);
          fputc(*((unsigned char *)(readbuf + hdr->edfparam[signalslist[i]].buf_offset + (j * 3) + 1)), outputfile);
          fputc(*((unsigned char *)(readbuf + hdr->edfparam[signalslist[i]].buf_offset + (j * 3) + 2)), outputfile);
        }
      }
    }

#ifdef Q_WS_WIN
    switch(timestamp_decimals)
    {
      case 0 : p = snprintf(annot_buf, 16, "+%I64d", time / TIME_DIMENSION);
                break;
      case 1 : p = snprintf(annot_buf, 16, "+%I64d.%01I64d", time / TIME_DIMENSION, (time % TIME_DIMENSION) / 1000000LL);
                break;
      case 2 : p = snprintf(annot_buf, 16, "+%I64d.%02I64d", time / TIME_DIMENSION, (time % TIME_DIMENSION) / 100000LL);
                break;
      case 3 : p = snprintf(annot_buf, 16, "+%I64d.%03I64d", time / TIME_DIMENSION, (time % TIME_DIMENSION) / 10000LL);
                break;
      case 4 : p = snprintf(annot_buf, 16, "+%I64d.%04I64d", time / TIME_DIMENSION, (time % TIME_DIMENSION) / 1000LL);
                break;
      case 5 : p = snprintf(annot_buf, 16, "+%I64d.%05I64d", time / TIME_DIMENSION, (time % TIME_DIMENSION) / 100LL);
                break;
      case 6 : p = snprintf(annot_buf, 16, "+%I64d.%06I64d", time / TIME_DIMENSION, (time % TIME_DIMENSION) / 10LL);
                break;
      case 7 : p = snprintf(annot_buf, 16, "+%I64d.%07I64d", time / TIME_DIMENSION, time % TIME_DIMENSION);
                break;
    }
#else
    switch(timestamp_decimals)
    {
      case 0 : p = snprintf(annot_buf, 16, "+%lli", time / TIME_DIMENSION);
                break;
      case 1 : p = snprintf(annot_buf, 16, "+%lli.%01lli", time / TIME_DIMENSION, (time % TIME_DIMENSION) / 1000000LL);
                break;
      case 2 : p = snprintf(annot_buf, 16, "+%lli.%02lli", time / TIME_DIMENSION, (time % TIME_DIMENSION) / 100000LL);
                break;
      case 3 : p = snprintf(annot_buf, 16, "+%lli.%03lli", time / TIME_DIMENSION, (time % TIME_DIMENSION) / 10000LL);
                break;
      case 4 : p = snprintf(annot_buf, 16, "+%lli.%04lli", time / TIME_DIMENSION, (time % TIME_DIMENSION) / 1000LL);
                break;
      case 5 : p = snprintf(annot_buf, 16, "+%lli.%05lli", time / TIME_DIMENSION, (time % TIME_DIMENSION) / 100LL);
                break;
      case 6 : p = snprintf(annot_buf, 16, "+%lli.%06lli", time / TIME_DIMENSION, (time % TIME_DIMENSION) / 10LL);
                break;
      case 7 : p = snprintf(annot_buf, 16, "+%lli.%07lli", time / TIME_DIMENSION, time % TIME_DIMENSION);
                break;
    }
#endif
    annot_buf[p++] = 20;
    annot_buf[p++] = 20;
    annot_buf[p++] = 0;

    if(annot!=NULL)
    {
      for(i=0; i<annots_per_datrec; i++)
      {
        if(annot!=NULL)
        {
          if(annot->onset<0)
          {
#ifdef Q_WS_WIN
            p += snprintf(annot_buf + p, 20, "-%I64d.%07I64d", -(annot->onset / TIME_DIMENSION), -(annot->onset % TIME_DIMENSION));
#else
            p += snprintf(annot_buf + p, 20, "-%lli.%07lli", -(annot->onset / TIME_DIMENSION), -(annot->onset % TIME_DIMENSION));
#endif
          }
          else
          {
#ifdef Q_WS_WIN
            p += snprintf(annot_buf + p, 20, "+%I64d.%07I64d", annot->onset / TIME_DIMENSION, annot->onset % TIME_DIMENSION);
#else
            p += snprintf(annot_buf + p, 20, "+%lli.%07lli", annot->onset / TIME_DIMENSION, annot->onset % TIME_DIMENSION);
#endif
          }

          for(j=0; j<7; j++)
          {
            if(annot_buf[p - j - 1] != '0')
            {
              break;
            }
          }

          if(j)
          {
            p -= j;

            if(j == 7)
            {
              p--;
            }
          }

          if(annot->duration[0])
          {
            annot_buf[p++] = 21;

            p += sprintf(annot_buf + p, "%s", annot->duration);
          }

          annot_buf[p++] = 20;

          p += sprintf(annot_buf + p, "%s", annot->annotation);

          annot_buf[p++] = 20;
          annot_buf[p++] = 0;

          annot = annot->next_annotation;
        }
      }
    }

    for(; p<annot_recordsize; p++)
    {
      annot_buf[p] = 0;
    }

    if(fwrite(annot_buf, annot_recordsize, 1, outputfile) != 1)
    {
      free(readbuf);
      free(annot_buf);
      return(3);
    }

    time += hdr->long_data_record_duration;
  }

  progress.reset();

  free(readbuf);
  free(annot_buf);

  return(0);
}















