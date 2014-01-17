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




#ifndef PAGETIMEFORM1_H
#define PAGETIMEFORM1_H



#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTimeEdit>
#include <QTime>
#include <QSpinBox>
#include <QMessageBox>

#include <string.h>

#include "global.h"
#include "mainwindow.h"
#include "popup_messagewindow.h"



class UI_Mainwindow;



class UI_Userdefined_timepage_Dialog : public QObject
{
  Q_OBJECT

public:

  UI_Userdefined_timepage_Dialog(QWidget *parent=0);

  UI_Mainwindow *mainwindow;

private:

QDialog      *set_diplaytime_dialog;

QLabel       *label1;

QPushButton  *CloseButton,
             *OKButton;

QTimeEdit    *timeEdit1;

QTime        time1;


private slots:

void okbutton_pressed();

};



#endif // PAGETIMEFORM1_H


