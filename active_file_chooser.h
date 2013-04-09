/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2010, 2011, 2012, 2013 Teunis van Beelen
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




#ifndef ACTIVEFILESAVERFORM1_H
#define ACTIVEFILESAVERFORM1_H



#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QString>


#include "global.h"
#include "mainwindow.h"



class UI_Mainwindow;



class UI_activeFileChooserWindow : public QObject
{
  Q_OBJECT

public:
  UI_activeFileChooserWindow(int *, QWidget *);

private:

QDialog      *chooserDialog;

QPushButton  *okButton,
             *cancelButton;

QListWidget  *filelist;

int *file_num;

UI_Mainwindow *mainwindow;


private slots:

void okButtonClicked();

};



#endif


