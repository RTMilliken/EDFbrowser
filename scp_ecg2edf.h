/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2013 Teunis van Beelen
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


#ifndef UI_SCPECG2EDFFORM_H
#define UI_SCPECG2EDFFORM_H


#include <QApplication>
#include <QDialog>
#include <QPushButton>
#include <QObject>
#include <QTextEdit>
#include <QFileDialog>
#include <QCursor>
#include <QStyle>
#include <QPlastiqueStyle>
#include <QWindowsStyle>
#include <QProgressDialog>
#include <QString>
#include <QByteArray>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "utils.h"
#include "edflib.h"
#include "utc_date_time.h"



class UI_SCPECG2EDFwindow : public QObject
{
  Q_OBJECT

public:
  UI_SCPECG2EDFwindow(char *recent_dir=NULL, char *save_dir=NULL);

private:


QPushButton  *pushButton1,
             *pushButton2;

QTextEdit    *textEdit1;

QDialog      *myobjectDialog;

char  *recent_opendir,
      *recent_savedir;

unsigned short crc_ccitt_table[256];

struct section_prop_struct{
        int present;
        long long file_offset;
        unsigned short crc;
        int section_id;
        int section_length;
        int section_version;
        int section_protocol_version;
        char reserved[6];
        } sp[12];

struct lead_prop_struct{
        int start;
        int end;
        int samples;
        int bytes;
        unsigned char label;
        } lp[256];

struct huffmantable_struct{
        int h_tables_cnt;
        int code_structs_cnt;
        int prefix_bits;
        int total_bits;
        int table_mode_switch;
        int base_value;
        int base_code;
        } ht;

struct patient_data_struct{
        char pat_id[21];
        int startdate_year;
        int startdate_month;
        int startdate_day;
        int starttime_hour;
        int starttime_minute;
        int starttime_second;
        char last_name[21];
        char first_name[21];
        int birthdate_year;
        int birthdate_month;
        int birthdate_day;
        int sex;
        char device_model[6];
        char device_serial[49];
        char device_ident[49];
        char manufacturer[49];
        unsigned char lang_code;
        } pat_dat;

int read_data_section_zero(FILE *, char *, long long);

int read_section_header(int, FILE *, long long, char *);

int check_crc(FILE *, long long, long long, unsigned short, char *);

void crc_ccitt_init(void);

unsigned short crc_ccitt(const unsigned char *, int, unsigned short);

inline unsigned char reverse_bitorder(unsigned char);

void lead_label_lookup(unsigned char, char *);

int get_patient_data(FILE *);

private slots:

void SelectFileButton();

};




#endif


