/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2011, 2012, 2013 Teunis van Beelen
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



#ifndef AVERAGERCURVEWND_H
#define AVERAGERCURVEWND_H



#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QString>
#include <QMessageBox>
#include <QCheckBox>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "global.h"
#include "mainwindow.h"
#include "signalcurve.h"
#include "edflib.h"
#include "utils.h"
#include "flywheel.h"




class UI_Mainwindow;




class UI_AverageCurveWindow : public QWidget
{
  Q_OBJECT

public:
  UI_AverageCurveWindow(struct signalcompblock *, QWidget *, int,
                        double *,
                        double,
                        double,
                        long long,
                        long long,
                        int,
                        int,
                        char *,
                        double);

  ~UI_AverageCurveWindow();

  UI_Mainwindow *mainwindow;

  QDialog *averager_curve_dialog;

  struct signalcompblock *signalcomp;

private:

  int averager_curve_dialog_is_destroyed,
      averagecurvedialognumber,
      avg_cnt,
      avg_trigger_position_ratio,
      flywheel_value;

  double pagetime,
         *avgbuf,
         avg_max_value,
         avg_min_value,
         avg_period;

  long long avg_samples_on_screen;

  char avg_annotation[MAX_ANNOTATION_LEN + 1];

  QHBoxLayout *hlayout1,
              *hlayout2,
              *hlayout3,
              *hlayout4;

  QVBoxLayout *vlayout1,
              *vlayout2,
              *vlayout3;

  SignalCurve *curve1;

  QSlider *amplitudeSlider,
          *spanSlider,
          *centerSlider;

  QLabel *spanLabel,
         *centerLabel,
         *amplitudeLabel;

  QCheckBox *inversionCheckBox,
            *BWCheckBox;

  UI_Flywheel *flywheel1;


private slots:

void sliderMoved(int);
void averager_curve_dialogDestroyed(QObject *);
void export_edf(void);
void update_flywheel(int);

};


#endif



















