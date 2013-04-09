/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2008, 2009, 2010, 2011, 2012, 2013 Teunis van Beelen
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


#ifndef UI_FINO2EDFFORM_H
#define UI_FINO2EDFFORM_H


#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QObject>
#include <QFileDialog>
#include <QCheckBox>
#include <QCursor>
#include <QStyle>
#include <QPlastiqueStyle>
#include <QWindowsStyle>
#include <QMessageBox>
#include <QString>
#include <QPixmap>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "popup_messagewindow.h"
#include "utils.h"


#define MAX_SIGNALS 32



class UI_FINO2EDFwindow : public QObject
{
  Q_OBJECT

public:
  UI_FINO2EDFwindow(char *recent_dir=NULL, char *save_dir=NULL);

private:

QDialog       *myobjectDialog;

QPushButton   *pushButton1,
              *pushButton2;

QLabel        *PatientnameLabel,
              *RecordingLabel,
              *DatetimeLabel;

QLineEdit     *PatientnameLineEdit,
              *RecordingLineEdit;

QDateTimeEdit *StartDatetimeedit;

char *recent_opendir,
     *recent_savedir;

void enable_widgets(bool);


private slots:

void SelectFileButton();

};




#endif


