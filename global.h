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


#ifndef VIEWER_GLOBAL_H
#define VIEWER_GLOBAL_H


#define PROGRAM_NAME "EDFbrowser"
#define PROGRAM_VERSION "1.53"
#define MINIMUM_QT_VERSION 0x040701
#define MAXFILES 32
#define MAXSIGNALS 512
#define MAXFILTERS 8
#define TIME_DIMENSION (10000000LL)
#define MAX_ANNOTATION_LEN 512
#define VIEWTIME_SYNCED_OFFSET 0
#define VIEWTIME_SYNCED_ABSOLUT 1
#define VIEWTIME_UNSYNCED 2
#define VIEWTIME_USER_DEF_SYNCED 3
#define MAX_PATH_LENGTH 1024
#define MAX_RECENTFILES 32
#define MAX_ACTIVE_ANNOT_MARKERS 64
#define MAXSPECTRUMDIALOGS 32
#define MAXPREDEFINEDMONTAGES 12
#define MAXAVERAGECURVEDIALOGS 32
#define MAXZSCOREDIALOGS 32

#define ANNOT_ID_NK_TRIGGER   0
#define ANNOT_ID_BS_TRIGGER   1

// #define BK_MRS_project
// This define activates temporary code for private use in a certain project.
// Do not use this code, it will be removed in future.
// When this code is activated, on pressing the 'p'-key, some values will be written
// to a file.

#include <stdio.h>

#include "filter.h"
#include "third_party/fidlib/fidlib.h"
#include "ravg_filter.h"
#include "ecg_filter.h"
#include "z_ratio_filter.h"



struct edfparamblock{
        char   label[17];
        char   transducer[81];
        char   physdimension[9];
        double phys_min;
        double phys_max;
        int    dig_min;
        int    dig_max;
        char   prefilter[81];
        int    smp_per_record;
        char   reserved[33];
        double offset;
        int    buf_offset;
        double bitvalue;
        int    annotation;
      };

struct edfhdrblock{
        FILE      *file_hdl;
        int       file_num;
        char      version[32];
        char      filename[MAX_PATH_LENGTH];
        char      patient[81];
        char      recording[81];
        char      plus_patientcode[81];
        char      plus_gender[16];
        char      plus_birthdate[16];
        char      plus_patient_name[81];
        char      plus_patient_additional[81];
        char      plus_startdate[16];
        char      plus_admincode[81];
        char      plus_technician[81];
        char      plus_equipment[81];
        char      plus_recording_additional[81];
        long long l_starttime;
        long long utc_starttime;
        char      reserved[45];
        int       hdrsize;
        int       edfsignals;
        long long datarecords;
        int       recordsize;
        int       annot_ch[256];
        int       nr_annot_chns;
        int       edf;
        int       edfplus;
        int       bdf;
        int       bdfplus;
        int       discontinuous;
        int       genuine_nk;
        int       nk_triggers_read;
        int       genuine_biosemi;
        double    data_record_duration;
        long long long_data_record_duration;
        long long viewtime;
        long long starttime_offset;
        long long prefiltertime;
        int       annots_not_read;
        struct edfparamblock *edfparam;
      };

struct signalcompblock{
        int filenum;
        struct edfhdrblock *edfhdr;
        int num_of_signals;
        int viewbufsize;
        int viewbufoffset;
        long long records_in_viewbuf;
        long long samples_in_viewbuf;
        long long samples_on_screen;
        long long sample_start;
        long long sample_stop;
        int timeoffset;
        int sample_timeoffset;
        int pixels_shift;
        double sample_timeoffset_part;
        double sample_pixel_ratio;
        int edfsignal[MAXSIGNALS];
        int factor[MAXSIGNALS];
        int polarity;
        double sensitivity[MAXSIGNALS];
        int oldvalue;
        long long oldsmplnr;
        long long file_duration;
        char signallabel[512];
        char alias[17];
        int signallabellen;
        char signallabel_bu[512];
        int signallabellen_bu;
        int hascursor1;
        int hascursor2;
        int hasoffsettracking;
        int hasgaintracking;
        int hasruler;
        double screen_offset;
        double voltpercm;
        char physdimension[9];
        char physdimension_bu[9];
        int color;
        int filter_cnt;
        int samples_in_prefilterbuf;
        long long prefilter_starttime;
        int prefilter_reset_sample;
        double filterpreset_a[MAXFILTERS];
        double filterpreset_b[MAXFILTERS];
        struct filter_settings *filter[MAXFILTERS];
        int max_dig_value;
        int min_dig_value;
        int fidfilter_cnt;
        int fidfilter_type[MAXFILTERS];
        double fidfilter_freq[MAXFILTERS];
        double fidfilter_freq2[MAXFILTERS];
        double fidfilter_ripple[MAXFILTERS];
        int fidfilter_order[MAXFILTERS];
        int fidfilter_model[MAXFILTERS];
        FidFilter *fidfilter[MAXFILTERS];
        FidRun *fid_run[MAXFILTERS];
        FidFunc *fidfuncp[MAXFILTERS];
        void *fidbuf[MAXFILTERS];
        void *fidbuf2[MAXFILTERS];
        int fidfilter_setup[MAXFILTERS];
        int stat_cnt;
        int stat_zero_crossing_cnt;
        double stat_sum;
        double stat_sum_sqr;
        double stat_sum_rectified;
        int ravg_filter_cnt;
        int ravg_filter_type[MAXFILTERS];
        int ravg_filter_size[MAXFILTERS];
        int ravg_filter_setup[MAXFILTERS];
        struct ravg_filter_settings *ravg_filter[MAXFILTERS];
        struct ecg_filter_settings *ecg_filter;
        struct zratio_filter_settings *zratio_filter;
        double zratio_crossoverfreq;
        int spectr_dialog[MAXSPECTRUMDIALOGS];
        int avg_dialog[MAXAVERAGECURVEDIALOGS];
        int zscoredialog[MAXZSCOREDIALOGS];
      };

struct zoomhistoryblock{
        int pntr;
        int history_size_tail;
        int history_size_front;
        long long viewtime[64][MAXFILES];
        long long pagetime[64];
        double voltpercm[64][MAXSIGNALS];
        double sensitivity[64][MAXSIGNALS][MAXSIGNALS];
        double screen_offset[64][MAXSIGNALS];
       };

struct annotationblock{
        int file_num;
        long long onset;
        char duration[16];
        char annotation[MAX_ANNOTATION_LEN + 1];
        struct annotationblock *former_annotation;
        struct annotationblock *next_annotation;
        int modified;
        int x_pos;
        int selected;
        int jump;
        int hided;
        int hided_in_list;
        unsigned int ident;
       };

struct active_markersblock{
        int file_num;
        struct annotationblock *list[MAX_ACTIVE_ANNOT_MARKERS];
        int count;
        int selected;
       };


struct graphicLineStruct{
        int x1;
        int y1;
        int x2;
        int y2;
        };

struct graphicBufStruct{
        struct graphicLineStruct graphicLine[MAXSIGNALS];
        };

struct crossHairStruct{
        int color;
        int file_num;
        int active;
        int moving;
        int position;
        int x_position;
        int y_position;
        int y_value;
        double value;
        long long time;
        long long time_relative;
       };

#endif



