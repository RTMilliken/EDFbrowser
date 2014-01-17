/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2009, 2010, 2011, 2012, 2013, 2014 Teunis van Beelen
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



#include "special_button.h"


SpecialButton::SpecialButton(QWidget *w_parent) : QWidget(w_parent)
{
  ButtonColor = QColor(Qt::white);
  global_Color = Qt::white;

  buttonText[0] = 0;
}


void SpecialButton::paintEvent(QPaintEvent *)
{
  int w, h;

  QPainter painter(this);

  painter.save();

  w = width();
  h = height();

  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::black);
  painter.drawRect(0, 0, w-1, h-1);
  painter.fillRect(1, 1, w-2, h-2, ButtonColor);
  if(buttonText[0]!=0)  painter.drawText(5, h/2+5, buttonText);
  painter.restore();
}


void SpecialButton::setColor(QColor newColor)
{
  ButtonColor = newColor;
  update();
}


QColor SpecialButton::color()
{
  return(ButtonColor);
}


void SpecialButton::setGlobalColor(int newColor)
{
  ButtonColor = (Qt::GlobalColor)newColor;
  global_Color = newColor;
  update();
}


int SpecialButton::globalColor()
{
  return(global_Color);
}


void SpecialButton::setText(const char *str)
{
  strncpy(buttonText, str, 2047);
  buttonText[2047] = 0;
  update();
}


void SpecialButton::mouseReleaseEvent(QMouseEvent *release_event)
{
  if(release_event->button()==Qt::LeftButton)
  {
    emit clicked(this);
  }
}






