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




#ifndef ASCIIEXPORTFORM1_H
#define ASCIIEXPORTFORM1_H



#include <QApplication>
#include <QObject>
#include <QFileDialog>
#include <QDialog>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QProgressDialog>
#include <QMessageBox>
#include <QString>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "mainwindow.h"
#include "popup_messagewindow.h"
#include "utils.h"




class UI_Mainwindow;



class UI_AsciiExportwindow : public QObject
{
  Q_OBJECT

public:
  UI_AsciiExportwindow(QWidget *parent);

  UI_Mainwindow *mainwindow;

private:

  QDialog      *AsciiExportDialog;

  QListWidget  *filelist;

  QPushButton  *CloseButton,
               *ExportButton;


struct asciiedfparamblock{
         int smp_per_record;
         int smp_written;
         int dig_min;
         int dig_max;
         double offset;
         int buf_offset;
         double phys_min;
         double phys_max;
         double time_step;
         double sense;
       } *edfparamascii;


private slots:

  void ExportButtonClicked();

};



#endif // ASCIIEXPORTFORM1_H


