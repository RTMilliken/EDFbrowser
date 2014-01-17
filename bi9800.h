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


#ifndef UI_BI98002EDFFORM_H
#define UI_BI98002EDFFORM_H


#include <QApplication>
#include <QObject>
#include <QDialog>
#include <QPushButton>
#include <QFileDialog>
#include <QCursor>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QProgressDialog>
#include <QMessageBox>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "global.h"
#include "popup_messagewindow.h"
#include "edflib.h"
#include "utils.h"



class UI_BI98002EDFwindow : public QObject
{
  Q_OBJECT

public:

  UI_BI98002EDFwindow(char *recent_dir=NULL, char *save_dir=NULL);

private:

QPushButton  *pushButton1,
             *pushButton2;

QDialog      *myobjectDialog;

char  *recent_opendir,
      *recent_savedir;



void showpopupmessage(const char *, const char *);

private slots:

void SelectFileButton();

};




#endif


