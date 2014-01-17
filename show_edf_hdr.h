/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014 Teunis van Beelen
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




#ifndef SHOWEDFHDRFORM1_H
#define SHOWEDFHDRFORM1_H



#include <QApplication>
#include <QObject>
#include <QDialog>
#include <QTableWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QStringList>

#include "global.h"
#include "mainwindow.h"
#include "utc_date_time.h"
#include "special_button.h"
#include "utils.h"


class UI_Mainwindow;



class UI_EDFhdrwindow : public QObject
{
  Q_OBJECT

public:
  UI_EDFhdrwindow(QWidget *parent);

  UI_Mainwindow *mainwindow;

private:

QDialog      *EDFhdrDialog;

QListWidget  *filelist;

QTableWidget *signallist;

QLabel       *label1,
             *label2,
             *label6,
             *label7,
             *label8,
             *label9,
             *label10,
             *label11,
             *label12,
             *label13,
             *label3,
             *label4,
             *label5,
             *label20,
             *label21;

SpecialButton *label1a,
              *label2a,
              *label6a,
              *label7a,
              *label8a,
              *label9a,
              *label10a,
              *label11a,
              *label12a,
              *label13a,
              *label3a,
              *label4a,
              *label5a,
              *label20a,
              *label21a;

QPushButton  *pushButton1;

int edfplus_layout;


private slots:

void show_params(int);

};



#endif // SHOWEDFHDRORM1_H


