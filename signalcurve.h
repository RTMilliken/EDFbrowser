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



#ifndef SIGNALCURVE_H
#define SIGNALCURVE_H


#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include <QPixmap>
#include <QPen>
#include <QString>
#include <QStringList>
#include <QFont>
#include <QMessageBox>

#include <string.h>


#define MAXSPECTRUMMARKERS 16
#define SC_MAX_PATH_LEN 1024


struct spectrum_markersblock{
        int items;
        double freq[MAXSPECTRUMMARKERS];
        int color[MAXSPECTRUMMARKERS];
        char label[MAXSPECTRUMMARKERS][17];
        double value[MAXSPECTRUMMARKERS];
        int method;
       };





class SignalCurve: public QWidget
{
  Q_OBJECT

public:
  SignalCurve(QWidget *parent=0);

  QSize sizeHint() const {return minimumSizeHint(); }
  QSize minimumSizeHint() const {return QSize(30,10); }

  void setSignalColor(QColor);
  void setTraceWidth(int);
  void setBackgroundColor(QColor);
  void setRasterColor(QColor);
  void setBorderColor(QColor);
  void setTextColor(QColor);
  void setBorderSize(int);
  void setH_RulerValues(double, double);
  void setH_label(const char *);
  void setV_label(const char *);
  void setUpperLabel1(const char *);
  void setUpperLabel2(const char *);
  void setLowerLabel(const char *);
  void drawCurve(double *, int , double , double );
  void drawCurve(int *, int , double , double );
  void drawCurve( float *, int , double , double );
  void drawLine(int, double, int, double, QColor);
  void setLineEnabled(bool);
  void create_button(const char *);
  void setCursorEnabled(bool);
  bool isCursorEnabled(void);
  bool isCursorActive(void);
  void setPrintEnabled(bool);
  void setDashBoardEnabled(bool);
  void setMarker1Enabled(bool);
  void setMarker1MovableEnabled(bool);
  void setMarker1Position(double);
  void setMarker1Color(QColor);
  double getMarker1Position(void);
  void setMarker2Enabled(bool);
  void setMarker2MovableEnabled(bool);
  void setMarker2Position(double);
  void setMarker2Color(QColor);
  double getMarker2Position(void);
  void setCrosshairColor(QColor);
  void clear();
  void setUpdatesEnabled(bool);
  void enableSpectrumColors(struct spectrum_markersblock *);
  void disableSpectrumColors();
  void setFillSurfaceEnabled(bool);
  void setV_rulerEnabled(bool);
  void setUpsidedownEnabled(bool);
  int getCursorPosition(void);
  void shiftCursorPixelsLeft(int);
  void shiftCursorPixelsRight(int);
  void shiftCursorIndexLeft(int);
  void shiftCursorIndexRight(int);

signals:
  void extra_button_clicked();
  void dashBoardClicked();
  void markerHasMoved();

private slots:
  void exec_sidemenu();
  void print_to_postscript();
  void print_to_pdf();
  void print_to_image();
  void print_to_printer();
  void print_to_ascii();
  void send_button_event();

private:
  QDialog     *sidemenu;

  QPushButton *sidemenuButton1,
              *sidemenuButton2,
              *sidemenuButton3,
              *sidemenuButton4,
              *sidemenuButton5,
              *sidemenuButton6;

  QColor SignalColor,
         BackgroundColor,
         RasterColor,
         BorderColor,
         RulerColor,
         TextColor,
         crosshair_1_color,
         line1Color,
         backup_color_1,
         backup_color_2,
         backup_color_3,
         backup_color_4,
         backup_color_5,
         backup_color_6;

  QPrinter *printer;

  QPen Marker1Pen,
       Marker2Pen;

  double max_value,
         min_value,
         *dbuf,
         h_ruler_startvalue,
         h_ruler_endvalue,
         printsize_x_factor,
         printsize_y_factor,
         crosshair_1_value,
         crosshair_1_value_2,
         crosshair_1_x_position,
         line1_start_y,
         line1_end_y,
         marker_1_position,
         marker_2_position;

  float *fbuf;

  int bufsize,
      bordersize,
      h_ruler_precision,
      drawHruler,
      drawVruler,
      drawcurve_before_raster,
      tracewidth,
      extra_button,
      use_move_events,
      mouse_x,
      mouse_y,
      mouse_old_x,
      mouse_old_y,
      crosshair_1_active,
      crosshair_1_moving,
      crosshair_1_y_position,
      crosshair_1_y_value,
      marker_1_moving,
      marker_1_x_position,
      marker_2_moving,
      marker_2_x_position,
      line1_start_x,
      line1_end_x,
      w,
      h,
      old_w,
      updates_enabled,
      fillsurface,
      *ibuf;

  char h_label[32],
       v_label[16],
       upperlabel1[64],
       upperlabel2[64],
       lowerlabel[64],
       extra_button_txt[16],
       recent_savedir[SC_MAX_PATH_LEN];

  bool printEnabled,
       cursorEnabled,
       dashBoardEnabled,
       Marker1Enabled,
       Marker1MovableEnabled,
       Marker2Enabled,
       Marker2MovableEnabled,
       curveUpSideDown,
       line1Enabled;

  struct spectrum_markersblock *spectrum_color;

  void backup_colors_for_printing();
  void restore_colors_after_printing();
  void drawWidget(QPainter *, int, int);
  void drawWidget_to_printer(QPainter *, int, int);
  int get_directory_from_path(char *, const char *, int);

protected:
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void resizeEvent(QResizeEvent *);

};


#endif


