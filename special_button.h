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



#ifndef SPECIALBUTTON_H
#define SPECIALBUTTON_H

#include <string.h>

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>



class SpecialButton: public QWidget
{
  Q_OBJECT

public:
  SpecialButton(QWidget *parent=0);

  QSize sizeHint() const {return minimumSizeHint(); }
  QSize minimumSizeHint() const {return QSize(30,10); }

public slots:
  void setColor(QColor);
  QColor color();
  void setGlobalColor(int);
  int globalColor();
  void setText(const char *);

protected:
  void paintEvent(QPaintEvent *);
  QColor ButtonColor;
  void mouseReleaseEvent(QMouseEvent *);

private:
  char buttonText[2048];

  int global_Color;

signals:
     void clicked(SpecialButton *);
};


#endif


