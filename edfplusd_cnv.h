/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2009, 2010, 2011, 2012, 2013, 2014 Teunis van Beelen
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


#ifndef UI_EDFD2EDFCFORM_H
#define UI_EDFD2EDFCFORM_H


#include <QApplication>
#include <QDialog>
#include <QPushButton>
#include <QObject>
#include <QTextEdit>
#include <QFileDialog>
#include <QProgressDialog>
#include <QStyle>
#include <QPlastiqueStyle>
#include <QWindowsStyle>
#include <QString>
#include <QByteArray>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "check_edf_file.h"
#include "edf_annotations.h"
#include "utils.h"




class UI_EDFDwindow : public QObject
{
  Q_OBJECT

public:

  UI_EDFDwindow(char *, char *);

private:

QPushButton  *pushButton1,
             *pushButton2;

QTextEdit    *textEdit1;

QDialog      *myobjectDialog;

char *recent_opendir,
     *recent_savedir;

long long get_datarecord_timestamp(char *);
void write_values_to_hdr(FILE *, long long, int, struct edfhdrblock *);
void free_annotations(struct annotationblock *);

private slots:

void SelectFileButton();

};




#endif


