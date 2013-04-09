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




#ifndef SIGNALSFORM1_H
#define SIGNALSFORM1_H



#include <QApplication>
#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QAction>
#include <QMessageBox>
#include <QVariant>
#include <QString>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "popup_messagewindow.h"
#include "mainwindow.h"
#include "colordialog.h"
#include "popup_messagewindow.h"
#include "utc_date_time.h"
#include "special_button.h"
#include "utils.h"



class UI_Mainwindow;




class UI_Signalswindow : public QObject
{
  Q_OBJECT

public:
  UI_Signalswindow(QWidget *parent);

  UI_Mainwindow *mainwindow;

private:

  QDialog      *SignalsDialog;

  QListWidget  *filelist,
               *signallist,
               *compositionlist;

  QLabel       *label1,
               *label2,
               *label3,
               *label4,
               *label5,
               *label6,
               *colorlabel;

  QPushButton  *CloseButton,
               *SelectAllButton,
               *DisplayButton,
               *DisplayCompButton,
               *AddButton,
               *SubtractButton,
               *RemoveButton,
               *HelpButton;

  SpecialButton *ColorButton;

  int smp_per_record,
      curve_color;

  char physdimension[64];

  double bitvalue;

  void strip_types_from_label(char *);

 private slots:

  void show_signals(int);
  void SelectAllButtonClicked();
  void DisplayButtonClicked();
  void DisplayCompButtonClicked();
  void AddButtonClicked();
  void SubtractButtonClicked();
  void RemoveButtonClicked();
  void ColorButtonClicked(SpecialButton *);
  void HelpButtonClicked();
};



#endif // SIGNALSFORM1_H


