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




#ifndef COLORMENUFORM1_H
#define COLORMENUFORM1_H



#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QPushButton>

#include "global.h"
#include "special_button.h"




class UI_Mainwindow;



class UI_ColorMenuDialog : public QObject
{
  Q_OBJECT

public:

  UI_ColorMenuDialog(int *oldColor, QWidget *parent = NULL);

private:

int *color;

QDialog      *colormenu_dialog;

QPushButton  *CloseButton;

SpecialButton *ColorButton1,
              *ColorButton2,
              *ColorButton3,
              *ColorButton4,
              *ColorButton5,
              *ColorButton6,
              *ColorButton7,
              *ColorButton8,
              *ColorButton9,
              *ColorButton10,
              *ColorButton11,
              *ColorButton12,
              *ColorButton13,
              *ColorButton14,
              *ColorButton15,
              *ColorButton16,
              *parentbutton;


private slots:

void ColorButton1_pressed(SpecialButton *);
void ColorButton2_pressed(SpecialButton *);
void ColorButton3_pressed(SpecialButton *);
void ColorButton4_pressed(SpecialButton *);
void ColorButton5_pressed(SpecialButton *);
void ColorButton6_pressed(SpecialButton *);
void ColorButton7_pressed(SpecialButton *);
void ColorButton8_pressed(SpecialButton *);
void ColorButton9_pressed(SpecialButton *);
void ColorButton10_pressed(SpecialButton *);
void ColorButton11_pressed(SpecialButton *);
void ColorButton12_pressed(SpecialButton *);
void ColorButton13_pressed(SpecialButton *);
void ColorButton14_pressed(SpecialButton *);
void ColorButton15_pressed(SpecialButton *);
void ColorButton16_pressed(SpecialButton *);

};



#endif // COLORMENUFORM1_H


