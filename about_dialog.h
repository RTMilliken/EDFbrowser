/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2007, 2008, 2009, 2010, 2011, 2012, 2013 Teunis van Beelen
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




#ifndef ABOUT_FORM1_H
#define ABOUT_FORM1_H



#include <QApplication>
#include <QObject>
#include <QDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtGlobal>

#include "global.h"
#include "mainwindow.h"



class UI_Aboutwindow : public QObject
{
  Q_OBJECT

public:
  UI_Aboutwindow(UI_Mainwindow *);

private:

QDialog      *AboutDialog;

QPushButton  *pushButton1;

QTextEdit    *textedit1;

QHBoxLayout  *hlayout1;

QVBoxLayout  *vlayout1;

};



#endif // ABOUT_FORM1_H


