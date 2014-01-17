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



#include "edf_annot_list.h"



void edfplus_annotation_add_item(struct annotationblock **list, struct annotationblock *annotation)
{
  struct annotationblock *annotlist;

  annotlist = *list;

  annotation->next_annotation = NULL;

  if(annotlist == NULL)
  {
    annotation->former_annotation = NULL;

    *list = annotation;

    return;
  }

  while(annotlist->next_annotation != NULL)
  {
    if(annotlist->next_annotation->onset > annotation->onset)
    {
      break;
    }

    annotlist = annotlist->next_annotation;
  }

  annotation->next_annotation = annotlist->next_annotation;

  if(annotation->next_annotation != NULL)
  {
    annotation->next_annotation->former_annotation = annotation;
  }

  annotlist->next_annotation = annotation;

  annotation->former_annotation = annotlist;
}


void edfplus_annotation_add_copy(struct annotationblock **list, struct annotationblock *annotation)
{
  struct annotationblock *annotlist, *new_annot;

  annotlist = *list;

  new_annot = (struct annotationblock *)calloc(1, sizeof(struct annotationblock));
  if(new_annot == NULL)
  {
    return;
  }

  memcpy(new_annot, annotation, sizeof(struct annotationblock));

  new_annot->next_annotation = NULL;

  if(annotlist == NULL)
  {
    new_annot->former_annotation = NULL;

    *list = new_annot;

    return;
  }

  while(annotlist->next_annotation != NULL)
  {
    annotlist = annotlist->next_annotation;
  }

  annotlist->next_annotation = new_annot;

  new_annot->former_annotation = annotlist;
}


int edfplus_annotation_count(struct annotationblock **list)
{
  int n=0;

  struct annotationblock *annotlist;

  annotlist = *list;

  while(annotlist != NULL)
  {
    annotlist = annotlist->next_annotation;

    n++;
  }

  return(n);
}


void edfplus_annotation_delete(struct annotationblock **list, int n)
{
  struct annotationblock *annotlist;

  annotlist = *list;

  if(annotlist==NULL)
  {
    return;
  }

  while(n)
  {
    if(annotlist->next_annotation==NULL)
    {
      return;
    }

    annotlist = annotlist->next_annotation;

    n--;
  }

  if(annotlist->former_annotation!=NULL)
  {
    annotlist->former_annotation->next_annotation = annotlist->next_annotation;
  }

  if(annotlist->next_annotation!=NULL)
  {
    annotlist->next_annotation->former_annotation = annotlist->former_annotation;
  }

  if(annotlist->former_annotation==NULL)
  {
    *list = annotlist->next_annotation;
  }

  free(annotlist);
}


void edfplus_annotation_delete_list(struct annotationblock **list)
{
  struct annotationblock *annotlist;

  annotlist = *list;

  if(annotlist == NULL)
  {
    return;
  }

  while(annotlist->next_annotation != NULL)
  {
    annotlist = annotlist->next_annotation;

    free(annotlist->former_annotation);
  }

  free(annotlist);

  *list = NULL;
}


struct annotationblock * edfplus_annotation_item(struct annotationblock **list, int n)
{
  struct annotationblock *annotlist;

  annotlist = *list;

  if(annotlist == NULL)
  {
    return(NULL);
  }

  while(n)
  {
    if(annotlist->next_annotation == NULL)
    {
      return(NULL);
    }

    annotlist = annotlist->next_annotation;

    n--;
  }

  return(annotlist);
}


void edfplus_annotation_sort(struct annotationblock **list)  // sort the annotationlist based on the onset time
{
  struct annotationblock *p1, *p2, *p3, *pm, *p5;

  p1 = *list;

  while(p1 != NULL)
  {
    if(p1->next_annotation == NULL)
    {
      break;
    }

    if(p1->next_annotation->onset < p1->onset)
    {
      pm = p1;

      p2 = p1->next_annotation;  // this one we take out

      p3 = p2->next_annotation;

// take out
      p1->next_annotation = p3;

      if(p3 != NULL)
      {
        p3->former_annotation = p1;
      }
// take out done

      while(1)  // search backwards till we find the right place to put it
      {
        p5 = pm->former_annotation;

        if(p5 == NULL)
        {
          pm->former_annotation = p2;

          p2->next_annotation = pm;

          p2->former_annotation = NULL;

          *list = p2;

          break;
        }

        if(p5->onset <= p2->onset)  // insert it here
        {
          pm->former_annotation = p2;

          p2->next_annotation = pm;

          p2->former_annotation = p5;

          p5->next_annotation = p2;

          break;
        }

        pm = p5;
      }
    }
    else
    {
      p1 = p1->next_annotation;  // continue searching the list from where we stopped before
    }
  }
}


struct annotationblock * edfplus_annotation_copy_list(struct annotationblock **list)
{
  struct annotationblock *annot, *annot2, *annot3;

  annot = *list;

  if(annot==NULL)
  {
    return(NULL);
  }

  annot2 = (struct annotationblock *)calloc(1, sizeof(struct annotationblock));

  annot3 = annot2;

  memcpy(annot2, annot, sizeof(struct annotationblock));

  annot = annot->next_annotation;

  while(annot!=NULL)
  {
    annot2->next_annotation = (struct annotationblock *)calloc(1, sizeof(struct annotationblock));

    memcpy(annot2->next_annotation, annot, sizeof(struct annotationblock));

    annot2->next_annotation->former_annotation = annot2;

    annot2 = annot2->next_annotation;

    annot = annot->next_annotation;
  }

  return(annot3);
}


int get_tal_timestamp_digit_cnt(struct edfhdrblock *hdr)
{
  int timestamp_digits;

  char scratchpad[256];

  long long time;


  if(hdr==NULL)
  {
    return(-1);
  }

  time = (hdr->datarecords * hdr->long_data_record_duration) / TIME_DIMENSION;

#ifdef _WIN32
  timestamp_digits = snprintf(scratchpad, 256, "%I64d", time);
#else
  timestamp_digits = snprintf(scratchpad, 256, "%lli", time);
#endif

  return(timestamp_digits);
}


int get_tal_timestamp_decimal_cnt(struct edfhdrblock *hdr)
{
  int i, j,
      timestamp_decimals;


  if(hdr==NULL)
  {
    return(-1);
  }

  j = 10;

  for(timestamp_decimals=7; timestamp_decimals>0; timestamp_decimals--)
  {
    if(hdr->long_data_record_duration % j)
    {
      break;
    }

    j *= 10;
  }

  if((hdr->edfplus==1)||(hdr->bdfplus==1))
  {
    j = 10;

    for(i=7; i>0; i--)
    {
      if(hdr->starttime_offset % j)
      {
        break;
      }

      j *= 10;
    }

    if(i > timestamp_decimals)
    {
      timestamp_decimals = i;
    }
  }

  return(timestamp_decimals);
}


int get_max_annotation_strlen(struct annotationblock **list)
{
  int j,
      len,
      annot_descr_len=0,
      annot_duration_len=0,
      timestamp_digits=0,
      timestamp_decimals=0;

  char scratchpad[256];

  struct annotationblock *annot;


  annot = list[0];

  if(annot==NULL)
  {
    return(0);
  }

  while(annot!=NULL)
  {
#ifdef _WIN32
    len = snprintf(scratchpad, 256, "%I64d", annot->onset / TIME_DIMENSION);
#else
    len = snprintf(scratchpad, 256, "%lli", annot->onset / TIME_DIMENSION);
#endif

    if(len > timestamp_digits)
    {
      timestamp_digits = len;
    }

    j = 10;

    for(len=7; len>0; len--)
    {
      if(annot->onset % j)
      {
        break;
      }

      j *= 10;
    }

    if(len > timestamp_decimals)
    {
      timestamp_decimals = len;
    }

    len = strlen(annot->annotation);

    if(len>annot_descr_len)
    {
      annot_descr_len = len;
    }

    len = strlen(annot->duration);

    if(len>annot_duration_len)
    {
      annot_duration_len = len;
    }

    annot = annot->next_annotation;
  }

  if(annot_duration_len)
  {
    annot_duration_len++;
  }

  if(timestamp_decimals)
  {
    timestamp_decimals++;
  }

// printf("annot_descr_len is %i\n"
//        "annot_duration_len is %i\n"
//        "timestamp_digits is %i\n"
//        "timestamp_decimals is %i\n",
//        annot_descr_len,
//        annot_duration_len,
//        timestamp_digits,
//        timestamp_decimals);

  return(annot_descr_len + annot_duration_len + timestamp_digits + timestamp_decimals + 4);
}







