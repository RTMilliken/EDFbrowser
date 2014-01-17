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




#ifndef EDITPREDEFINEDMTGFORM1_H
#define EDITPREDEFINEDMTGFORM1_H



#include <QApplication>
#include <QObject>
#include <QFileDialog>
#include <QDialog>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QMessageBox>
#include <QString>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "mainwindow.h"
#include "popup_messagewindow.h"




class UI_Mainwindow;



class UI_edit_predefined_mtg_window : public QObject
{
  Q_OBJECT

public:
  UI_edit_predefined_mtg_window(QWidget *parent);

  UI_Mainwindow *mainwindow;

private:

  QDialog      *edit_predefined_mtg_Dialog,
               *dialog;

  QListWidget  *mtg_path_list;

  QPushButton  *CloseButton,
               *button1,
               *button2,
               *button3;

  QListWidgetItem *listItem;

  int row;


private slots:

void rowClicked(QListWidgetItem *);
void adEntry();
void removeEntry();

};



#endif


