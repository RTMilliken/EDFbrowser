/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2009, 2010, 2011, 2012, 2013 Teunis van Beelen
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




#ifndef ANNOTATION_EDIT_FORM1_H
#define ANNOTATION_EDIT_FORM1_H



#include <QApplication>
#include <QObject>
#include <QDockWidget>
#include <QByteArray>
#include <QDialog>
#include <QLineEdit>
#include <QTime>
#include <QTimeEdit>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QSpinBox>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "mainwindow.h"
#include "annotations_dock.h"
#include "popup_save_cancelwindow.h"
#include "edf_annot_list.h"



class UI_Mainwindow;




class UI_AnnotationEditwindow : public QObject
{
  Q_OBJECT

public:
  UI_AnnotationEditwindow(int, QWidget *parent);

  UI_Mainwindow *mainwindow;

  QDockWidget  *dockedit;

  void annotEditSetOnset(long long);

  void annotEditSetDuration(long long);

  void set_selected_annotation(int, int);

  void set_selected_annotation(struct annotationblock *);

private:

  struct annotationblock *annotation;

  int file_num,
      annot_num;

  QDialog *annot_edit_dialog;

  QLabel *onsetLabel,
         *durationLabel,
         *descriptionLabel;

  QLineEdit *annot_descript_lineEdit;

  QTimeEdit *onset_timeEdit;

  QSpinBox  *onset_daySpinbox;

  QDoubleSpinBox *duration_spinbox;

  QPushButton *modifybutton,
              *deletebutton,
              *createbutton;

  QComboBox *posNegTimebox;

private slots:

  void modifyButtonClicked();
  void deleteButtonClicked();
  void createButtonClicked();

  void open_close_dock(bool);
};



#endif




