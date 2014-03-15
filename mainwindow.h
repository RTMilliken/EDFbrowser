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


#ifndef UI_VIEW_MAINFORM_H
#define UI_VIEW_MAINFORM_H

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QFont>
#include <QToolBar>
#include <QSlider>
#include <QPlastiqueStyle>
#include <QGtkStyle>
#include <QWindowsStyle>
#include <QtGlobal>
#include <QCloseEvent>

#ifdef Q_WS_X11
  #include <sys/types.h>
  #include <sys/stat.h>
#endif

#ifdef Q_WS_MAC
  #include <QMacStyle>
  #include <sys/types.h>
  #include <sys/stat.h>
#endif

#ifdef Q_WS_WIN
  #include <windows.h>
  #include <io.h>
  #ifndef CSIDL_COMMON_APPDATA
    #define CSIDL_COMMON_APPDATA 0x0023 // All Users\Application Data
  #endif
  #ifndef CSIDL_APPDATA
    #define CSIDL_APPDATA 0x001a // <username>\Application Data
  #endif
  #ifndef CSIDL_PROGRAM_FILES
    #define CSIDL_PROGRAM_FILES 0x0026 // C:\Program Files
  #endif
#endif

#include <QFileDialog>
#include <QAction>
#include <QActionGroup>
#include <QPixmap>
#include <QSplashScreen>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>
#include <QLibrary>
#include <QString>
#include <QLocale>
#include <QMessageBox>
#include <QProgressDialog>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "global.h"
#include "viewcurve.h"
#include "popup_messagewindow.h"
#include "check_edf_file.h"
#include "show_edf_hdr.h"
#include "signals_dialog.h"
#include "signal_chooser.h"
#include "ascii_export.h"
#include "edf_compat.h"
#include "filter.h"
#include "filter_dialog.h"
#include "jump_dialog.h"
#include "about_dialog.h"
#include "edf_annotations.h"
#include "annotations_dock.h"
#include "options_dialog.h"
#include "nk2edf.h"
#include "xml.h"
#include "save_montage_dialog.h"
#include "load_montage_dialog.h"
#include "view_montage_dialog.h"
#include "show_actual_montage_dialog.h"
#include "print_to_edf.h"
#include "pagetime_dialog.h"
#include "print_to_bdf.h"
#include "ascii2edf.h"
#include "fino2edf.h"
#include "nexfin2edf.h"
#include "edfplusd_cnv.h"
#include "emsa2edf.h"
#include "bdf2edf.h"
#include "edit_annotation_dock.h"
#include "popup_save_cancelwindow.h"
#include "save_annots.h"
#include "utils.h"
#include "spectrumanalyzer.h"
#include "bi9800.h"
#include "export_annotations.h"
#include "edit_predefined_mtg.h"
#include "spectrum_dock.h"
#include "signalcurve.h"
#include "edf_annot_list.h"
#include "reduce_signals.h"
#include "header_editor.h"
#include "biosemi2bdfplus.h"
#include "bdf_triggers.h"
#include "import_annotations.h"
#include "ravg_filter.h"
#include "wav2edf.h"
#include "averager_curve_wnd.h"
#include "ecg_filter.h"
#include "ecg_export.h"
#include "z_score_dialog.h"
#include "z_ratio_filter.h"
#include "raw2edf.h"
#include "check_for_updates.h"
#include "manscan2edf.h"
#include "scp_ecg2edf.h"
#include "unisens2edf.h"

#include "third_party/fidlib/fidlib.h"




class ViewCurve;
class UI_Signalswindow;
class UI_Annotationswindow;
class UI_AnnotationEditwindow;
class UI_SpectrumDockWindow;
class UI_FreqSpectrumWindow;
class UI_AverageCurveWindow;
class UI_ZScoreWindow;


class UI_Mainwindow : public QMainWindow
{
  Q_OBJECT

public:
  UI_Mainwindow();
  ~UI_Mainwindow();

  int files_open,
      signalcomps,
      totalviewbufsize,
      sel_viewtime,
      viewtime_sync,
      print_to_edf_active,
      annot_editor_active,
      show_annot_markers,
      show_baselines,
      annotations_edited,
      exit_in_progress,
      maxdftblocksize,
      dpix,
      dpiy,
      auto_dpi,
      clip_to_pane,
      auto_reload_mtg,
      read_biosemi_status_signal,
      read_nk_trigger_signal,
      live_stream_active,
      live_stream_update_interval,
      signal_averaging_active,
      powerlinefreq,
      mousewheelsens,
      average_ratio,
      average_upsidedown,
      average_bw,
      spectrum_bw,
      spectrum_sqrt,
      spectrum_vlog,
      spectrumdock_sqrt,
      spectrumdock_vlog,
      use_threads,
      check_for_updates,
      amplitude_doubler,
      timescale_doubler,
      viewtime_indicator_type,
      mainwindow_title_type;

  long long pagetime,
            maxfilesize_to_readin_annotations;

  char *viewbuf,
       viewtime_string[128],
       pagetime_string[64],
       recent_montagedir[MAX_PATH_LENGTH],
       recent_savedir[MAX_PATH_LENGTH],
       recent_opendir[MAX_PATH_LENGTH],
       predefined_mtg_path[MAXPREDEFINEDMONTAGES][MAX_PATH_LENGTH],
       recent_file_mtg_path[MAX_RECENTFILES][MAX_PATH_LENGTH],
       recent_colordir[MAX_PATH_LENGTH],
       cfg_app_version[17];

  double pixelsizefactor,
         x_pixelsizefactor,
         average_period;

  struct{
          double crossoverfreq;
          double z_threshold;
          double z_hysteresis;
          int zscore_page_len;
          int zscore_error_detection;
        } z_score_var;

  struct raw2edf_var_struct raw2edf_var;

  struct edfhdrblock *edfheaderlist[MAXFILES];

  struct signalcompblock *signalcomp[MAXSIGNALS];

  struct zoomhistoryblock *zoomhistory;

  struct annotationblock *annotationlist[MAXFILES];

  struct annotationblock *annotationlist_backup;

  struct spectrum_markersblock *spectrum_colorbar;

  struct import_annotations_var_block *import_annotations_var;

  struct export_annotations_var_block *export_annotations_var;

  UI_Annotationswindow *annotations_dock[MAXFILES];

  UI_AnnotationEditwindow *annotationEditDock;

  UI_FreqSpectrumWindow *spectrumdialog[MAXSPECTRUMDIALOGS];

  UI_AverageCurveWindow *averagecurvedialog[MAXAVERAGECURVEDIALOGS];

  UI_ZScoreWindow *zscoredialog[MAXZSCOREDIALOGS];

  void setup_viewbuf();

  void setMainwindowTitle(struct edfhdrblock *);

  int file_is_opened(const char *);

  void remove_recent_file_mtg_path(const char *);

  void get_rgbcolor_settings(struct xml_handle *, const char *, int, QColor *);

  struct signalcompblock * create_signalcomp_copy(struct signalcompblock *);

  ViewCurve    *maincurve;

  QFont        *myfont,
               *monofont;

  QAction      *save_act,
               *load_predefined_mtg_act[MAXPREDEFINEDMONTAGES],
               *Escape_act;

  QActionGroup *timelock_act_group,
               *sel_viewtime_act_group;

  UI_SpectrumDockWindow *spectrumdock;


private:

  QMenuBar     *menubar;

  QMenu        *filemenu,
               *signalmenu,
               *displaymenu,
               *amplitudemenu,
               *toolsmenu,
               *helpmenu,
               *printmenu,
               *filtermenu,
//               *math_func_menu,
               *timemenu,
               *windowmenu,
               *recent_filesmenu,
               *montagemenu,
               *patternmenu,
               *print_img_menu;

  char path[MAX_PATH_LENGTH],
       montagepath[MAX_PATH_LENGTH],
       recent_file_path[MAX_RECENTFILES][MAX_PATH_LENGTH];

#ifdef BK_MRS_project
// temporary code for private use in a certain project
// do not use this code, it will be removed in future

  QAction  *keyboard_bk_mrs_project_act;

  FILE     *bk_mrs_project_file;

#endif

  QAction  *former_page_Act,
           *shift_page_left_Act,
           *shift_page_right_Act,
           *next_page_Act,
           *shift_page_up_Act,
           *shift_page_down_Act,
           *page_div2,
           *page_mult2,
           *page_10m,
           *page_20m,
           *page_50m,
           *page_100m,
           *page_200m,
           *page_500m,
           *page_1,
           *page_2,
           *page_5,
           *page_10,
           *page_15,
           *page_20,
           *page_30,
           *page_60,
           *page_300,
           *page_1200,
           *page_3600,
           *page_user_defined,
           *page_whole_rec,
           *fit_to_pane,
           *fit_to_dc,
           *amp_00001,
           *amp_00002,
           *amp_00005,
           *amp_0001,
           *amp_0002,
           *amp_0005,
           *amp_001,
           *amp_002,
           *amp_005,
           *amp_01,
           *amp_02,
           *amp_05,
           *amp_1,
           *amp_2,
           *amp_5,
           *amp_10,
           *amp_20,
           *amp_50,
           *amp_100,
           *amp_200,
           *amp_500,
           *amp_1000,
           *amp_2000,
           *amp_5000,
           *amp_10000,
           *amp_20000,
           *amp_50000,
           *amp_plus,
           *amp_minus,
           *zoomback_Act,
           *zoomforward_Act,
           *recent_file[MAX_RECENTFILES],
           *offset_timesync_act,
           *absolut_timesync_act,
           *no_timesync_act,
           *user_def_sync_act,
           *sel_viewtime_act[MAXFILES];

  QActionGroup *AmplitudeGroup,
               *DisplayGroup,
               *load_predefined_mtg_group;

  QTimer   *live_stream_timer;

  QSplashScreen *splash;

  QPixmap  *pixmap;

  QToolBar *slidertoolbar;

  QSlider  *positionslider;

  int cmdlineargument,
      oldwindowheight;

  Check_for_updates *update_checker;

  void read_color_settings();
  void read_recent_file_settings();
  void read_general_settings();
  void write_settings();
#ifdef Q_WS_WIN
  QString specialFolder(int);
#endif
  long long check_edf_file_datarecords(struct edfhdrblock *);

public slots:
  void remove_all_signals();
  void edfplus_annotation_remove_duplicates();

private slots:
  void open_new_file();
  void show_file_info();
  void close_all_files();
  void exit_program();
  void signalproperties_dialog();
  void filterproperties_dialog();
  void add_signals_dialog();
  void show_about_dialog();
  void set_display_time(QAction *);
  void set_page_div2();
  void set_page_mult2();
  void set_display_time_whole_rec();
  void set_amplitude(QAction *);
  void set_amplitude_div2();
  void set_amplitude_mult2();
  void fit_signals_to_pane();
  void fit_signals_dc_offset();
  void former_page();
  void shift_page_left();
  void shift_page_right();
  void next_page();
  void shift_page_up();
  void shift_page_down();
  void zoomback();
  void forward();
  void show_splashscreen();
  void export_to_ascii();
  void check_edf_compatibility();
  void add_new_filter();
  void remove_all_filters();
//  void add_new_math_func();
//  void remove_all_math_funcs();
  void jump_to_dialog();
  void jump_to_start();
  void jump_to_end();
  void show_annotations();
  void show_options_dialog();
  long long get_long_time(char *);
  void nk2edf_converter();
  void set_timesync(QAction *);
  void set_timesync_reference(QAction *);
  void recent_file_action_func(QAction *);
  void sync_by_crosshairs();
  void save_montage();
  void load_montage();
  void view_montage();
  void show_this_montage();
  void show_help();
  void show_kb_shortcuts();
  void print_to_edf();
  void set_user_defined_display_time();
  void print_to_bdf();
  void print_to_img_640x480();
  void print_to_img_800x600();
  void print_to_img_1024x768();
  void print_to_img_1280x1024();
  void print_to_img_1600x1200();
  void convert_ascii_to_edf();
  void convert_fino_to_edf();
  void convert_wave_to_edf();
  void convert_nexfin_to_edf();
  void edfd_converter();
  void slider_moved(int);
  void convert_emsa_to_edf();
  void bdf2edf_converter();
  void set_dc_offset_to_zero();
  void annotation_editor();
  void save_file();
  void unisens2edf_converter();
  void BI98002edf_converter();
  void export_annotations();
  void load_predefined_mtg(QAction *);
  void edit_predefined_montages();
  void show_spectrum_dock();
  void page_3cmsec();
  void reduce_signals();
  void edit_header();
  void biosemi2bdfplus_converter();
  void import_annotations();
  void open_stream();
  void live_stream_timer_func();
  void organize_signals();
  void Escape_fun();
  void export_ecg_rr_interval_to_ascii();
  void convert_binary_to_edf();
  void convert_manscan_to_edf();
  void convert_scpecg_to_edf();
//  void search_pattern();

#ifdef BK_MRS_project
// temporary code for private use in a certain project
// do not use this code, it will be removed in future

  void keyboard_bk_mrs_project_func();

#endif

protected:
  void closeEvent(QCloseEvent *);

};


#endif



