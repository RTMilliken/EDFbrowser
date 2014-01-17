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




#ifndef VIEWMONTAGEFORM1_H
#define VIEWMONTAGEFORM1_H



#include <QApplication>
#include <QObject>
#include <QDialog>
#include <QFileDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>
#include <QString>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "mainwindow.h"
#include "xml.h"
#include "popup_messagewindow.h"
#include "utils.h"



class UI_Mainwindow;



class UI_ViewMontagewindow : public QObject
{
  Q_OBJECT

public:
  UI_ViewMontagewindow(QWidget *parent);

  UI_Mainwindow *mainwindow;


private:

  QDialog      *ViewMontageDialog;

  QPushButton  *CloseButton,
               *SelectButton;

  QBoxLayout   *box;

  QHBoxLayout  *hbox;

  QTreeView    *tree;

  QStandardItemModel *t_model;

  char mtg_path[MAX_PATH_LENGTH],
       mtg_dir[MAX_PATH_LENGTH];

private slots:

  void SelectButtonClicked();

};



#endif // VIEWMONTAGEFORM1_H


