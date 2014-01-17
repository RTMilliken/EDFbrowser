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
* The current version of GPL is at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*
***************************************************************************
*/




#ifndef ADJUSTFILTERSETTINGS_H
#define ADJUSTFILTERSETTINGS_H



#include <QApplication>
#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>

#include "global.h"
#include "mainwindow.h"
#include "viewcurve.h"
#include "popup_messagewindow.h"
#include "ravg_filter.h"

#include "third_party/fidlib/fidlib.h"


class UI_Mainwindow;
class ViewCurve;



class AdjustFilterSettings : public QObject
{
  Q_OBJECT

public:

AdjustFilterSettings(struct signalcompblock *, QWidget *);


private:

int filter_nr,
    filter_cnt,
    type,
    model,
    order,
    size,
    brand[MAXFILTERS];

double frequency1,
       frequency2,
       ripple;

struct signalcompblock * signalcomp;

UI_Mainwindow  *mainwindow;

ViewCurve      *maincurve;

QDialog        *filtersettings_dialog;

QLabel         *label[5];

QComboBox      *filterbox,
               *stepsizebox;

QSpinBox       *orderbox;

QDoubleSpinBox *freq1box,
               *freq2box;

QPushButton    *CloseButton;

void update_filter(void);


private slots:

void freqbox1valuechanged(double);
void freqbox2valuechanged(double);
void orderboxvaluechanged(int);
void stepsizeboxchanged(int);
void filterboxchanged(int);

};



#endif // ADJUSTFILTERSETTINGS_H


