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


#ifndef UI_REDUCESIGNALSFORM_H
#define UI_REDUCESIGNALSFORM_H


#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QObject>
#include <QTableWidget>
#include <QFileDialog>
#include <QCursor>
#include <QString>
#include <QSpinBox>
#include <QRadioButton>
#include <QMessageBox>
#include <QComboBox>
#include <QVariant>
#include <QProgressDialog>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "mainwindow.h"
#include "check_edf_file.h"
#include "popup_messagewindow.h"
#include "utils.h"
#include "utc_date_time.h"
#include "active_file_chooser.h"
#include "edf_annot_list.h"




class UI_Mainwindow;



class UI_ReduceSignalsWindow : public QObject
{
  Q_OBJECT

public:

  UI_ReduceSignalsWindow(QWidget *parent);

  UI_Mainwindow *mainwindow;

private:

QLabel       *label1,
             *label2,
             *label3,
             *label4,
             *label5;

QPushButton  *pushButton1,
             *pushButton2,
             *pushButton3,
             *pushButton4,
             *pushButton5,
             *pushButton6;

QSpinBox     *spinBox1,
             *spinBox2,
             *spinBox3;

QRadioButton *radioButton1,
             *radioButton2;

QTableWidget *SignalsTablewidget;

QDialog      *myobjectDialog;

int  signalslist[MAXSIGNALS],
     dividerlist[MAXSIGNALS],
     file_num;

char  inputpath[MAX_PATH_LENGTH],
      outputpath[MAX_PATH_LENGTH],
      *recent_savedir;

FILE *inputfile,
     *outputfile;

struct edfhdrblock *edfhdr;

void showpopupmessage(const char *, const char *);

private slots:

void SelectFileButton();
void StartConversion();
void Select_all_signals();
void Deselect_all_signals();
void Set_SRdivider_all_signals();
void spinBox1changed(int);
void spinBox2changed(int);
void radioButton1Toggled(bool);
void radioButton2Toggled(bool);

};




#endif


