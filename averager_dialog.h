/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2011, 2012, 2013 Teunis van Beelen
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




#ifndef AVERAGER_FORM1_H
#define AVERAGER_FORM1_H



#include <QApplication>
#include <QObject>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QProgressDialog>
#include <QComboBox>
#include <QList>
#include <QTime>
#include <QTimeEdit>
#include <QDoubleSpinBox>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "mainwindow.h"
#include "edf_annot_list.h"
#include "averager_curve_wnd.h"
#include "utils.h"



class UI_Mainwindow;



class UI_AveragerWindow : public QObject
{
  Q_OBJECT

public:
  UI_AveragerWindow(QWidget *, int);

  ~UI_AveragerWindow();

  UI_Mainwindow *mainwindow;


private:

QDialog     *averager_dialog;

QLabel      *annotLabel,
            *annotNameLabel,
            *signalLabel,
            *ratioLabel,
            *bufsizeLabel,
            *time1Label,
            *time2Label;

QPushButton *CloseButton,
            *StartButton;

QListWidget *list;

QComboBox   *ratioBox;

QTimeEdit    *timeEdit1,
             *timeEdit2;

QTime        time1,
             time2;

QDoubleSpinBox *avg_periodspinbox;

char annot_str[MAX_ANNOTATION_LEN + 1];

int signal_nr;

double *avgbuf;


void process_avg(struct signalcompblock *);

private slots:

void startButtonClicked();

};



#endif // AVERAGER_FORM1_H


