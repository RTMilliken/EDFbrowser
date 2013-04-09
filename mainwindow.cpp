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



#include "mainwindow.h"


#if defined(__APPLE__) || defined(__MACH__) || defined(__APPLE_CC__)

#define fopeno fopen

#else

#define fseeko fseeko64
#define ftello ftello64
#define fopeno fopen64

#endif




UI_Mainwindow::UI_Mainwindow()
{
  int i, j, k;


  live_stream_timer = new QTimer;
  live_stream_timer->setSingleShot(TRUE);
  QObject::connect(live_stream_timer, SIGNAL(timeout()), this, SLOT(live_stream_timer_func()));

  setMinimumSize(QSize(640, 480));
  setWindowTitle(PROGRAM_NAME);
  setWindowIcon(QIcon(":/images/edf.png"));

  myfont = new QFont;

  monofont = new QFont;

#ifdef Q_WS_X11
  QApplication::setFont(*myfont);

  myfont->setFamily("Arial");
  myfont->setPixelSize(12);

  monofont->setFamily("andale mono");
  monofont->setPixelSize(12);
#endif

#ifdef Q_WS_MAC
  QApplication::setFont(*myfont);

  myfont->setFamily("Arial");
  myfont->setPixelSize(12);

  monofont->setFamily("andale mono");
  monofont->setPixelSize(12);
#endif

#ifdef Q_WS_WIN
  myfont->setFamily("Tahoma");
  myfont->setPixelSize(11);

  monofont->setFamily("courier");
  monofont->setPixelSize(12);
#endif

  setlocale(LC_NUMERIC, "C");

  pixelsizefactor = 0.0294382;

  x_pixelsizefactor = 0.0294382;

  check_for_updates = 1;

  use_threads = 1;

  auto_dpi = 1;

  show_annot_markers = 1;

  show_baselines = 1;

  clip_to_pane = 0;

  auto_reload_mtg = 1;

  read_biosemi_status_signal = 1;

  read_nk_trigger_signal = 1;

  maxfilesize_to_readin_annotations = 10485760000LL;

  exit_in_progress = 0;

  live_stream_active = 0;

  signal_averaging_active = 0;

  live_stream_update_interval = 500;

  powerlinefreq = 50;

  mousewheelsens = 10;

  recent_montagedir[0] = 0;
  recent_savedir[0] = 0;
  recent_opendir[0] = 0;
  cfg_app_version[0] = 0;

  for(i=0; i<MAXSPECTRUMDIALOGS; i++)
  {
    spectrumdialog[i] = NULL;
  }

  for(i=0; i<MAXAVERAGECURVEDIALOGS; i++)
  {
    averagecurvedialog[i] = NULL;
  }

  for(i=0; i<MAXZSCOREDIALOGS; i++)
  {
    zscoredialog[i] = NULL;
  }

  spectrum_colorbar = (struct spectrum_markersblock *)calloc(1, sizeof(struct spectrum_markersblock));
  for(i=0; i < MAXSPECTRUMMARKERS; i++)
  {
    spectrum_colorbar->freq[i] = 1.0;
    spectrum_colorbar->color[i] = Qt::white;
  }
  spectrum_colorbar->items = 5;
  spectrum_colorbar->freq[0] = 4.0;
  spectrum_colorbar->freq[1] = 8.0;
  spectrum_colorbar->freq[2] = 12.0;
  spectrum_colorbar->freq[3] = 30.0;
  spectrum_colorbar->freq[4] = 100.0;
  spectrum_colorbar->color[0] = Qt::darkRed;
  spectrum_colorbar->color[1] = Qt::darkGreen;
  spectrum_colorbar->color[2] = Qt::darkBlue;
  spectrum_colorbar->color[3] = Qt::darkCyan;
  spectrum_colorbar->color[4] = Qt::darkMagenta;
  spectrum_colorbar->method = 1;

  maxdftblocksize = 1000;

  import_annotations_var = (import_annotations_var_block *)calloc(1, sizeof(struct import_annotations_var_block));
  strcpy(import_annotations_var->separator, "tab");
  import_annotations_var->format = 1;
  import_annotations_var->onsettimeformat = 0;
  import_annotations_var->onsetcolumn = 1;
  import_annotations_var->descriptioncolumn = 2;
  import_annotations_var->useduration = 0;
  import_annotations_var->durationcolumn = 3;
  import_annotations_var->datastartline = 1;
  import_annotations_var->dceventbittime = 10;
  import_annotations_var->triggerlevel = 500.0;
  import_annotations_var->manualdescription = 0;
  import_annotations_var->description[0] = 0;
  import_annotations_var->ignoreconsecutive = 0;

  export_annotations_var = (export_annotations_var_block *)calloc(1, sizeof(struct export_annotations_var_block));
  export_annotations_var->separator = 0;
  export_annotations_var->format = 1;
  export_annotations_var->duration = 0;

  average_period = 0.3;

  average_ratio = 0;

  average_upsidedown = 0;

  average_bw = 0;

  spectrum_bw = 0;

  spectrum_sqrt = 0;

  z_score_var.crossoverfreq = 7.5;
  z_score_var.z_threshold = 0.0;
  z_score_var.zscore_page_len = 30;
  z_score_var.zscore_error_detection = 80;
  z_score_var.z_hysteresis = 0.0;

  raw2edf_var.sf = 256;
  raw2edf_var.chns = 1;
  raw2edf_var.phys_max = 3000;
  raw2edf_var.straightbinary = 0;
  raw2edf_var.samplesize = 2;
  raw2edf_var.offset = 0;
  raw2edf_var.skipblocksize = 0;
  raw2edf_var.skipbytes = 1;
  strcpy(raw2edf_var.phys_dim, "uV");

  read_general_settings();

  maincurve = new ViewCurve(this);

  dpix = maincurve->logicalDpiX();

  dpiy = maincurve->logicalDpiY();

  if(auto_dpi)
  {
    pixelsizefactor = 1.0 / ((double)dpiy / 2.54);

    x_pixelsizefactor = 1.0 / ((double)dpix / 2.54);
  }

  read_color_settings();

  setCentralWidget(maincurve);

  menubar = menuBar();

  recent_filesmenu = new QMenu(this);
  recent_filesmenu->setTitle("Recent files");
  connect(recent_filesmenu, SIGNAL(triggered(QAction *)), this, SLOT(recent_file_action_func(QAction *)));

  print_img_menu = new QMenu(this);
  print_img_menu->setTitle("to Image");
  print_img_menu->addAction("640 x 480",   this, SLOT(print_to_img_640x480()));
  print_img_menu->addAction("800 x 600",   this, SLOT(print_to_img_800x600()));
  print_img_menu->addAction("1024 x 768",  this, SLOT(print_to_img_1024x768()));
  print_img_menu->addAction("1280 x 1024", this, SLOT(print_to_img_1280x1024()));
  print_img_menu->addAction("1600 x 1200", this, SLOT(print_to_img_1600x1200()));

  printmenu = new QMenu(this);
  printmenu->setTitle("Print");
  printmenu->addAction("to Printer",    maincurve, SLOT(print_to_printer()), QKeySequence::Print);
  printmenu->addAction("to PostScript", maincurve, SLOT(print_to_postscript()));
  printmenu->addAction("to PDF",        maincurve, SLOT(print_to_pdf()));
  printmenu->addMenu(print_img_menu);
  printmenu->addAction("to EDF",        this,      SLOT(print_to_edf()));
  printmenu->addAction("to BDF",        this,      SLOT(print_to_bdf()));


#ifdef BK_MRS_project
// temporary code for private use in a certain project
// do not use this code, it will be removed in future
  bk_mrs_project_file = NULL;

  keyboard_bk_mrs_project_act = new QAction(this);
  keyboard_bk_mrs_project_act->setShortcut(Qt::Key_P);
  this->addAction(keyboard_bk_mrs_project_act);
  connect(keyboard_bk_mrs_project_act, SIGNAL(triggered()), this, SLOT(keyboard_bk_mrs_project_func()));

#endif

  save_act = new QAction("Save as", this);
  save_act->setShortcut(QKeySequence::Save);
  save_act->setEnabled(FALSE);
  connect(save_act, SIGNAL(triggered()), this, SLOT(save_file()));

  filemenu = new QMenu(this);
  filemenu->setTitle("&File");
  filemenu->addAction("Open",         this, SLOT(open_new_file()), QKeySequence::Open);
  filemenu->addSeparator();
  filemenu->addAction("Open stream",  this, SLOT(open_stream()));
  filemenu->addSeparator();
  filemenu->addAction(save_act);
  filemenu->addMenu(recent_filesmenu);
  filemenu->addMenu(printmenu);
  filemenu->addAction("Info",         this, SLOT(show_file_info()));
  filemenu->addAction("Close all",    this, SLOT(close_all_files()), QKeySequence::Close);
  filemenu->addAction("Exit",         this, SLOT(exit_program()), QKeySequence::Quit);
  menubar->addMenu(filemenu);

  signalmenu = new QMenu(this);
  signalmenu->setTitle("&Signals");
  signalmenu->addAction("Properties", this, SLOT(signalproperties_dialog()));
  signalmenu->addAction("Add",        this, SLOT(add_signals_dialog()));
  signalmenu->addAction("Organize",   this, SLOT(organize_signals()));
  signalmenu->addAction("Remove all", this, SLOT(remove_all_signals()));
  menubar->addMenu(signalmenu);

  displaymenu = new QMenu(this);
  displaymenu->setTitle("&Timescale");

  displaymenu->addAction("3 cm/sec", this, SLOT(page_3cmsec()));

  displaymenu->addSeparator();

  page_10m = new QAction("10 mSec/page", this);
  displaymenu->addAction(page_10m);

  page_20m = new QAction("20 mSec/page", this);
  displaymenu->addAction(page_20m);

  page_50m = new QAction("50 mSec/page", this);
  displaymenu->addAction(page_50m);

  page_100m = new QAction("100 mSec/page", this);
  displaymenu->addAction(page_100m);

  page_200m = new QAction("200 mSec/page", this);
  displaymenu->addAction(page_200m);

  page_500m = new QAction("500 mSec/page", this);
  displaymenu->addAction(page_500m);

  page_1 = new QAction("1 Sec/page", this);
  displaymenu->addAction(page_1);

  page_2 = new QAction("2 Sec/page", this);
  displaymenu->addAction(page_2);

  page_5 = new QAction("5 Sec/page", this);
  displaymenu->addAction(page_5);

  page_10 = new QAction("10 Sec/page", this);
  displaymenu->addAction(page_10);

  page_15 = new QAction("15 Sec/page", this);
  displaymenu->addAction(page_15);

  page_20 = new QAction("20 Sec/page", this);
  displaymenu->addAction(page_20);

  page_30 = new QAction("30 Sec/page", this);
  page_30->setChecked(TRUE);
  displaymenu->addAction(page_30);

  page_60 = new QAction("60 Sec/page", this);
  displaymenu->addAction(page_60);

  page_300 = new QAction("5 min/page", this);
  displaymenu->addAction(page_300);

  page_1200 = new QAction("20 min/page", this);
  displaymenu->addAction(page_1200);

  page_3600 = new QAction("1 hour/page", this);
  displaymenu->addAction(page_3600);

  displaymenu->addSeparator();

  page_div2 = new QAction("Timescale / 2", this);
  page_div2->setShortcut(QKeySequence::ZoomIn);
  connect(page_div2, SIGNAL(triggered()), this, SLOT(set_page_div2()));
  displaymenu->addAction(page_div2);

  page_mult2 = new QAction("Timescale x 2", this);
  page_mult2->setShortcut(QKeySequence::ZoomOut);
  connect(page_mult2, SIGNAL(triggered()), this, SLOT(set_page_mult2()));
  displaymenu->addAction(page_mult2);

  displaymenu->addSeparator();

  page_user_defined = new QAction("user defined", this);
  connect(page_user_defined, SIGNAL(triggered()), this, SLOT(set_user_defined_display_time()));
  displaymenu->addAction(page_user_defined);

  displaymenu->addSeparator();

  page_whole_rec = new QAction("whole recording", this);
  connect(page_whole_rec, SIGNAL(triggered()), this, SLOT(set_display_time_whole_rec()));
  displaymenu->addAction(page_whole_rec);

  menubar->addMenu(displaymenu);

  DisplayGroup = new QActionGroup(this);
  DisplayGroup->addAction(page_10m);
  DisplayGroup->addAction(page_20m);
  DisplayGroup->addAction(page_50m);
  DisplayGroup->addAction(page_100m);
  DisplayGroup->addAction(page_200m);
  DisplayGroup->addAction(page_500m);
  DisplayGroup->addAction(page_1);
  DisplayGroup->addAction(page_2);
  DisplayGroup->addAction(page_5);
  DisplayGroup->addAction(page_10);
  DisplayGroup->addAction(page_15);
  DisplayGroup->addAction(page_20);
  DisplayGroup->addAction(page_30);
  DisplayGroup->addAction(page_60);
  DisplayGroup->addAction(page_300);
  DisplayGroup->addAction(page_1200);
  DisplayGroup->addAction(page_3600);
  connect(DisplayGroup, SIGNAL(triggered(QAction *)), this, SLOT(set_display_time(QAction *)));

  amplitudemenu = new QMenu(this);
  amplitudemenu->setTitle("&Amplitude");

  fit_to_pane = new QAction("Fit to pane", this);
  connect(fit_to_pane, SIGNAL(triggered()), this, SLOT(fit_signals_to_pane()));
  amplitudemenu->addAction(fit_to_pane);

  fit_to_dc = new QAction("Adjust offset", this);
  connect(fit_to_dc, SIGNAL(triggered()), this, SLOT(fit_signals_dc_offset()));
  amplitudemenu->addAction(fit_to_dc);

  amplitudemenu->addAction("Offset -> 0", this, SLOT(set_dc_offset_to_zero()));

  amplitudemenu->addSeparator();

  amp_50000 = new QAction("50000", this);
  amplitudemenu->addAction(amp_50000);

  amp_20000 = new QAction("20000", this);
  amplitudemenu->addAction(amp_20000);

  amp_10000 = new QAction("10000", this);
  amplitudemenu->addAction(amp_10000);

  amp_5000 = new QAction("5000", this);
  amplitudemenu->addAction(amp_5000);

  amp_2000 = new QAction("2000", this);
  amplitudemenu->addAction(amp_2000);

  amp_1000 = new QAction("1000", this);
  amplitudemenu->addAction(amp_1000);

  amp_500 = new QAction("500", this);
  amplitudemenu->addAction(amp_500);

  amp_200 = new QAction("200", this);
  amplitudemenu->addAction(amp_200);

  amp_100 = new QAction("100 (50uV/5mm)", this);
  amplitudemenu->addAction(amp_100);

  amp_50 = new QAction("50", this);
  amplitudemenu->addAction(amp_50);

  amp_20 = new QAction("20", this);
  amplitudemenu->addAction(amp_20);

  amp_10 = new QAction("10", this);
  amplitudemenu->addAction(amp_10);

  amp_5 = new QAction("5", this);
  amplitudemenu->addAction(amp_5);

  amp_2 = new QAction("2", this);
  amplitudemenu->addAction(amp_2);

  amp_1 = new QAction("1", this);
  amplitudemenu->addAction(amp_1);

  amp_05 = new QAction("0.5", this);
  amplitudemenu->addAction(amp_05);

  amp_02 = new QAction("0.2", this);
  amplitudemenu->addAction(amp_02);

  amp_01 = new QAction("0.1", this);
  amplitudemenu->addAction(amp_01);

  amp_005 = new QAction("0.05", this);
  amplitudemenu->addAction(amp_005);

  amp_002 = new QAction("0.02", this);
  amplitudemenu->addAction(amp_002);

  amp_001 = new QAction("0.01", this);
  amplitudemenu->addAction(amp_001);

  amp_0005 = new QAction("0.005", this);
  amplitudemenu->addAction(amp_0005);

  amp_0002 = new QAction("0.002", this);
  amplitudemenu->addAction(amp_0002);

  amp_0001 = new QAction("0.001", this);
  amplitudemenu->addAction(amp_0001);

  amp_00005 = new QAction("0.0005", this);
  amplitudemenu->addAction(amp_00005);

  amp_00002 = new QAction("0.0002", this);
  amplitudemenu->addAction(amp_00002);

  amp_00001 = new QAction("0.0001", this);
  amplitudemenu->addAction(amp_00001);

  menubar->addMenu(amplitudemenu);

  AmplitudeGroup = new QActionGroup(this);
  AmplitudeGroup->addAction(amp_00001);
  AmplitudeGroup->addAction(amp_00002);
  AmplitudeGroup->addAction(amp_00005);
  AmplitudeGroup->addAction(amp_0001);
  AmplitudeGroup->addAction(amp_0002);
  AmplitudeGroup->addAction(amp_0005);
  AmplitudeGroup->addAction(amp_001);
  AmplitudeGroup->addAction(amp_002);
  AmplitudeGroup->addAction(amp_005);
  AmplitudeGroup->addAction(amp_01);
  AmplitudeGroup->addAction(amp_02);
  AmplitudeGroup->addAction(amp_05);
  AmplitudeGroup->addAction(amp_1);
  AmplitudeGroup->addAction(amp_2);
  AmplitudeGroup->addAction(amp_5);
  AmplitudeGroup->addAction(amp_10);
  AmplitudeGroup->addAction(amp_20);
  AmplitudeGroup->addAction(amp_50);
  AmplitudeGroup->addAction(amp_100);
  AmplitudeGroup->addAction(amp_200);
  AmplitudeGroup->addAction(amp_500);
  AmplitudeGroup->addAction(amp_1000);
  AmplitudeGroup->addAction(amp_2000);
  AmplitudeGroup->addAction(amp_5000);
  AmplitudeGroup->addAction(amp_10000);
  AmplitudeGroup->addAction(amp_20000);
  AmplitudeGroup->addAction(amp_50000);
  connect(AmplitudeGroup, SIGNAL(triggered(QAction *)), this, SLOT(set_amplitude(QAction *)));

  filtermenu = new QMenu(this);
  filtermenu->setTitle("&Filter");
  filtermenu->addAction("New", this, SLOT(add_new_filter()));
  filtermenu->addAction("Adjust", this, SLOT(filterproperties_dialog()));
  filtermenu->addAction("Remove all", this, SLOT(remove_all_filters()));
  menubar->addMenu(filtermenu);

//   math_func_menu = new QMenu(this);
//   math_func_menu->setTitle("&Math");
//   math_func_menu->addAction("New", this, SLOT(add_new_math_func()));
//   math_func_menu->addAction("Remove all", this, SLOT(remove_all_math_funcs()));
//   menubar->addMenu(math_func_menu);

  load_predefined_mtg_act[0] = new QAction("Empty", this);
  load_predefined_mtg_act[0]->setShortcut(Qt::Key_F1);
  load_predefined_mtg_act[1] = new QAction("Empty", this);
  load_predefined_mtg_act[1]->setShortcut(Qt::Key_F2);
  load_predefined_mtg_act[2] = new QAction("Empty", this);
  load_predefined_mtg_act[2]->setShortcut(Qt::Key_F3);
  load_predefined_mtg_act[3] = new QAction("Empty", this);
  load_predefined_mtg_act[3]->setShortcut(Qt::Key_F4);
  load_predefined_mtg_act[4] = new QAction("Empty", this);
  load_predefined_mtg_act[4]->setShortcut(Qt::Key_F5);
  load_predefined_mtg_act[5] = new QAction("Empty", this);
  load_predefined_mtg_act[5]->setShortcut(Qt::Key_F6);
  load_predefined_mtg_act[6] = new QAction("Empty", this);
  load_predefined_mtg_act[6]->setShortcut(Qt::Key_F7);
  load_predefined_mtg_act[7] = new QAction("Empty", this);
  load_predefined_mtg_act[7]->setShortcut(Qt::Key_F8);
  load_predefined_mtg_group = new QActionGroup(this);
  for(i=0; i < MAXPREDEFINEDMONTAGES; i++)
  {
    load_predefined_mtg_group->addAction(load_predefined_mtg_act[i]);
  }
  connect(load_predefined_mtg_group, SIGNAL(triggered(QAction *)), this, SLOT(load_predefined_mtg(QAction *)));

  montagemenu = new QMenu(this);
  montagemenu->setTitle("&Montage");
  montagemenu->addAction("View this montage", this, SLOT(show_this_montage()));
  montagemenu->addAction("View saved montages", this, SLOT(view_montage()));
  montagemenu->addAction("Save", this, SLOT(save_montage()));
  montagemenu->addAction("Load", this, SLOT(load_montage()));
  montagemenu->addSeparator();
  montagemenu->addAction("Edit key-bindings for montages", this, SLOT(edit_predefined_montages()));
  montagemenu->addSeparator();
  for(i=0; i < MAXPREDEFINEDMONTAGES; i++)
  {
    montagemenu->addAction(load_predefined_mtg_act[i]);
  }
  menubar->addMenu(montagemenu);

//   patternmenu = new QMenu(this);
//   patternmenu->setTitle("&Pattern");
//   patternmenu->addAction("Search", this, SLOT(search_pattern()));
//   menubar->addMenu(patternmenu);

  toolsmenu = new QMenu(this);
  toolsmenu->setTitle("T&ools");
  toolsmenu->addAction("Check EDF/BDF compatibility", this, SLOT(check_edf_compatibility()));
  toolsmenu->addSeparator();
  toolsmenu->addAction("Header editor", this, SLOT(edit_header()));
  toolsmenu->addAction("Reduce signals, duration or samplerate", this, SLOT(reduce_signals()));
  toolsmenu->addAction("Remove duplicates in annotations", this, SLOT(edfplus_annotation_remove_duplicates()));
  toolsmenu->addSeparator();
  toolsmenu->addAction("Import annotations/events", this, SLOT(import_annotations()));
  toolsmenu->addAction("Export annotations/events", this, SLOT(export_annotations()));
  toolsmenu->addAction("Export EDF/BDF to ASCII", this, SLOT(export_to_ascii()));
  toolsmenu->addAction("Export/Import ECG RR-interval", this, SLOT(export_ecg_rr_interval_to_ascii()));
  toolsmenu->addSeparator();
  toolsmenu->addAction("Convert Nihon Kohden to EDF+", this, SLOT(nk2edf_converter()));
  toolsmenu->addAction("Convert ASCII to EDF/BDF", this, SLOT(convert_ascii_to_edf()));
  toolsmenu->addAction("Convert Finometer to EDF", this, SLOT(convert_fino_to_edf()));
  toolsmenu->addAction("Convert Nexfin to EDF", this, SLOT(convert_nexfin_to_edf()));
  toolsmenu->addAction("Convert Emsa to EDF+", this, SLOT(convert_emsa_to_edf()));
  toolsmenu->addAction("Convert EDF+D to EDF+C", this, SLOT(edfd_converter()));
  toolsmenu->addAction("Convert Biosemi to BDF+", this, SLOT(biosemi2bdfplus_converter()));
  toolsmenu->addAction("Convert BDF to EDF", this, SLOT(bdf2edf_converter()));
  toolsmenu->addAction("Convert BI9800TL+3 to EDF", this, SLOT(BI98002edf_converter()));
  toolsmenu->addAction("Convert Wave to EDF", this, SLOT(convert_wave_to_edf()));
  toolsmenu->addAction("Convert Binary/raw data to EDF", this, SLOT(convert_binary_to_edf()));
  toolsmenu->addSeparator();
  toolsmenu->addAction("Options", this, SLOT(show_options_dialog()));
  menubar->addMenu(toolsmenu);

  menubar->addAction("S&ettings", this, SLOT(show_options_dialog()));

  former_page_Act = new QAction("<<", this);
  former_page_Act->setShortcut(QKeySequence::MoveToPreviousPage);
  connect(former_page_Act, SIGNAL(triggered()), this, SLOT(former_page()));
  menubar->addAction(former_page_Act);

  shift_page_left_Act = new QAction("<", this);
  shift_page_left_Act->setShortcut(QKeySequence::MoveToPreviousChar);
  connect(shift_page_left_Act, SIGNAL(triggered()), this, SLOT(shift_page_left()));
  menubar->addAction(shift_page_left_Act);

  shift_page_right_Act = new QAction(">", this);
  shift_page_right_Act->setShortcut(QKeySequence::MoveToNextChar);
  connect(shift_page_right_Act, SIGNAL(triggered()), this, SLOT(shift_page_right()));
  menubar->addAction(shift_page_right_Act);

  next_page_Act = new QAction(">>", this);
  next_page_Act->setShortcut(QKeySequence::MoveToNextPage);
  connect(next_page_Act, SIGNAL(triggered()), this, SLOT(next_page()));
  menubar->addAction(next_page_Act);

  shift_page_up_Act = new QAction("^", this);
  shift_page_up_Act->setShortcut(QKeySequence::MoveToPreviousLine);
  connect(shift_page_up_Act, SIGNAL(triggered()), this, SLOT(shift_page_up()));
  menubar->addAction(shift_page_up_Act);

  shift_page_down_Act = new QAction("v", this);
  shift_page_down_Act->setShortcut(QKeySequence::MoveToNextLine);
  connect(shift_page_down_Act, SIGNAL(triggered()), this, SLOT(shift_page_down()));
  menubar->addAction(shift_page_down_Act);

  zoomback_Act = new QAction("zoomback", this);
  zoomback_Act->setShortcut(Qt::Key_Backspace);
  connect(zoomback_Act, SIGNAL(triggered()), this, SLOT(zoomback()));
  menubar->addAction(zoomback_Act);

  zoomforward_Act = new QAction("zoomforward", this);
  zoomforward_Act->setShortcut(Qt::Key_Insert);
  connect(zoomforward_Act, SIGNAL(triggered()), this, SLOT(forward()));
  menubar->addAction(zoomforward_Act);

  no_timesync_act = new QAction("no timelock", this);
  no_timesync_act->setCheckable(TRUE);

  offset_timesync_act = new QAction("synchronize start of files (offset)", this);
  offset_timesync_act->setCheckable(TRUE);

  absolut_timesync_act = new QAction("synchronize absolute time", this);
  absolut_timesync_act->setCheckable(TRUE);

  user_def_sync_act = new QAction("user defined synchronizing", this);
  user_def_sync_act->setCheckable(TRUE);

  timelock_act_group = new QActionGroup(this);
  timelock_act_group->addAction(no_timesync_act);
  timelock_act_group->addAction(offset_timesync_act);
  timelock_act_group->addAction(absolut_timesync_act);
  timelock_act_group->addAction(user_def_sync_act);
  absolut_timesync_act->setChecked(TRUE);
  connect(timelock_act_group, SIGNAL(triggered(QAction *)), this, SLOT(set_timesync(QAction *)));

  sel_viewtime_act_group = new QActionGroup(this);
  connect(sel_viewtime_act_group, SIGNAL(triggered(QAction *)), this, SLOT(set_timesync_reference(QAction *)));

  timemenu = new QMenu(this);
  timemenu->setTitle("T&imesync");
  timemenu->addAction("Go to start of file", this, SLOT(jump_to_start()), QKeySequence::MoveToStartOfDocument);
  timemenu->addAction("Go to end of file", this, SLOT(jump_to_end()), QKeySequence::MoveToEndOfDocument);
  timemenu->addAction("Jump to", this, SLOT(jump_to_dialog()));
  timemenu->addSeparator()->setText("Timelock");
  timemenu->addAction(no_timesync_act);
  timemenu->addAction(offset_timesync_act);
  timemenu->addAction(absolut_timesync_act);
  timemenu->addAction(user_def_sync_act);
  timemenu->addSeparator();
  timemenu->addAction("synchronize by crosshairs", this, SLOT(sync_by_crosshairs()));
  timemenu->addSeparator()->setText("Time reference");
  menubar->addMenu(timemenu);

  windowmenu = new QMenu(this);
  windowmenu->setTitle("&Window");
  windowmenu->addAction("Annotations", this, SLOT(show_annotations()));
  windowmenu->addAction("Annotation editor", this, SLOT(annotation_editor()));
  windowmenu->addAction("Spectrum", this, SLOT(show_spectrum_dock()));
  menubar->addMenu(windowmenu);

  helpmenu = new QMenu(this);
  helpmenu->setTitle("&Help");
#ifdef Q_WS_X11
  helpmenu->addAction("Manual",  this, SLOT(show_help()));
#endif
#ifdef Q_WS_WIN
  helpmenu->addAction("Manual",  this, SLOT(show_help()));
#endif
  helpmenu->addAction("Keyboard shortcuts", this, SLOT(show_kb_shortcuts()));
  helpmenu->addAction("About EDFbrowser", this, SLOT(show_about_dialog()));
  helpmenu->addAction("Show splashscreen", this, SLOT(show_splashscreen()));
  menubar->addMenu(helpmenu);

  Escape_act = new QAction(this);
  Escape_act->setShortcut(Qt::Key_Escape);
  connect(Escape_act, SIGNAL(triggered()), this, SLOT(Escape_fun()));
  maincurve->addAction(Escape_act);

  positionslider = new QSlider(Qt::Horizontal);
  positionslider->setRange(0, 1000000);
  positionslider->setSingleStep(10000);
  positionslider->setPageStep(100000);

  slidertoolbar = new QToolBar();
  slidertoolbar->setFloatable(FALSE);
  slidertoolbar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
  slidertoolbar->addWidget(positionslider);
  addToolBar(Qt::BottomToolBarArea, slidertoolbar);
  QObject::connect(positionslider, SIGNAL(valueChanged(int)), this, SLOT(slider_moved(int)));
  slidertoolbar->setEnabled(FALSE);
  positionslider->blockSignals(TRUE);

  files_open = 0;
  signalcomps = 0;
  sel_viewtime = 0;
  viewtime_sync = VIEWTIME_SYNCED_ABSOLUT;
  pagetime = 10 * TIME_DIMENSION;
  viewtime_string[0] = 0;
  pagetime_string[0] = 0;
  totalviewbufsize = 0;
  print_to_edf_active = 0;
  annot_editor_active = 0;
  annotations_edited = 0;

  viewbuf = NULL;

  for(i=0; i<MAXFILES; i++)
  {
    annotationlist[i] = NULL;
    annotations_dock[i] = NULL;
  }

  annotationlist_backup = NULL;

  zoomhistory = (struct zoomhistoryblock *)calloc(1, sizeof(struct zoomhistoryblock));

  zoomhistory->history_size_tail = 0;
  zoomhistory->history_size_front = 0;
  for(i=0; i<64; i++)
  {
    zoomhistory->pntr = 0;
    zoomhistory->pagetime[i] = 10 * TIME_DIMENSION;
    for(j=0; j<MAXFILES; j++)
    {
      zoomhistory->viewtime[i][j] = 0;
    }
    for(j=0; j<MAXSIGNALS; j++)
    {
      zoomhistory->voltpercm[i][j] = 70.0;
      zoomhistory->screen_offset[i][j] = 0.0;
      for(k=0; k<MAXSIGNALS; k++)
      {
        zoomhistory->sensitivity[i][j][k] = 0.0475;
      }
    }
  }

  path[0] = 0;
  recent_montagedir[0] = 0;
  recent_savedir[0] = 0;
  recent_opendir[0] = 0;
  montagepath[0] = 0;

  for(i=0; i<MAX_RECENTFILES; i++)
  {
    recent_file_path[i][0] = 0;
    recent_file_mtg_path[i][0] = 0;
  }

  for(i=0; i<MAXPREDEFINEDMONTAGES; i++)
  {
    predefined_mtg_path[i][0] = 0;
  }

  read_recent_file_settings();

  for(i=0; i<MAXPREDEFINEDMONTAGES; i++)
  {
    if(predefined_mtg_path[i][0] != 0)
    {
      load_predefined_mtg_act[i]->setText(predefined_mtg_path[i]);
    }
  }

  annotationEditDock = new UI_AnnotationEditwindow(files_open, this);

  addDockWidget(Qt::BottomDockWidgetArea, annotationEditDock->dockedit, Qt::Horizontal);

  annotationEditDock->dockedit->hide();

  spectrumdock = new UI_SpectrumDockWindow(this);

  addDockWidget(Qt::TopDockWidgetArea, spectrumdock->dock, Qt::Horizontal);

  spectrumdock->dock->hide();

  setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
  setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
  setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
  setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);

  if(QCoreApplication::arguments().size()>1)
  {
    strcpy(path, QCoreApplication::arguments().at(1).toLocal8Bit().data());
    cmdlineargument = 1;

    if(QCoreApplication::arguments().size()>2)
    {
      strcpy(montagepath, QCoreApplication::arguments().at(2).toLocal8Bit().data());
      cmdlineargument = 2;
    }
  }
  else
  {
    cmdlineargument = 0;
  }

  showMaximized();

  oldwindowheight = height();

  if(cmdlineargument)
  {
    open_new_file();
  }

  if(QT_VERSION < MINIMUM_QT_VERSION)
  {
    QMessageBox messagewindow(QMessageBox::Warning, "Warning", "Qt version is too old");
    messagewindow.exec();
  }
  else
  {
    int v_nr;

    char v_str[32];

    strncpy(v_str, qVersion(), 32);
    v_str[31] = 0;

    v_nr = 0x10000 * atoi(v_str);
    v_nr += 0x100 * atoi(v_str + 2);
    v_nr += atoi(v_str + 4);

    if(v_nr < MINIMUM_QT_VERSION)
    {
      QMessageBox messagewindow(QMessageBox::Warning, "Warning", "Qt version is too old");
      messagewindow.exec();
    }
  }

  pixmap = new QPixmap(":/images/splash.png");
  splash = new QSplashScreen(this, *pixmap, Qt::WindowStaysOnTopHint);

  t1 = new QTimer;
  t1->setSingleShot(TRUE);
  QObject::connect(t1, SIGNAL(timeout()), splash, SLOT(close()));

  splash->show();

  t1->start(3000);

  update_checker = NULL;

  if(check_for_updates)
  {
    update_checker = new Check_for_updates;
  }
}



UI_Mainwindow::~UI_Mainwindow()
{
  delete pixmap;
  delete splash;
  delete t1;
  delete myfont;
  delete monofont;
  delete maincurve;
  delete annotationEditDock;
  delete spectrumdock;
  delete live_stream_timer;
  if(update_checker != NULL)  delete update_checker;
}



void UI_Mainwindow::exit_program()
{
  close();
}



void UI_Mainwindow::closeEvent(QCloseEvent *cl_event)
{
  int i,
      button_nr=0;


  if(annotations_edited)
  {
    QMessageBox messagewindow;
    messagewindow.setText("There are unsaved annotations,\n are you sure you want to quit?");
    messagewindow.setIcon(QMessageBox::Question);
    messagewindow.setStandardButtons(QMessageBox::Cancel | QMessageBox::Close);
    messagewindow.setDefaultButton(QMessageBox::Cancel);
    button_nr = messagewindow.exec();
  }

  if(button_nr == QMessageBox::Cancel)
  {
    cl_event->ignore();
  }
  else
  {
    exit_in_progress = 1;

    for(i=0; i<MAXSPECTRUMDIALOGS; i++)
    {
      if(spectrumdialog[i] != NULL)
      {
        delete spectrumdialog[i];

        spectrumdialog[i] = NULL;
      }
    }

    for(i=0; i<MAXAVERAGECURVEDIALOGS; i++)
    {
      if(averagecurvedialog[i] != NULL)
      {
        delete averagecurvedialog[i];

        averagecurvedialog[i] = NULL;
      }
    }

    for(i=0; i<MAXZSCOREDIALOGS; i++)
    {
      if(zscoredialog[i] != NULL)
      {
        delete zscoredialog[i];

        zscoredialog[i] = NULL;
      }
    }

    annotations_edited = 0;

    close_all_files();

    write_settings();

    free(spectrum_colorbar);
    free(zoomhistory);
    free(import_annotations_var);
    free(export_annotations_var);

    cl_event->accept();
  }
}


// void UI_Mainwindow::search_pattern()
// {
//   if(!signalcomps)
//   {
//     return;
//   }
//
//
//
// }


void UI_Mainwindow::Escape_fun()
{
  int i;

  for(i=0; i<signalcomps; i++)
  {
    signalcomp[i]->hascursor1 = 0;
    signalcomp[i]->hascursor2 = 0;
    signalcomp[i]->hasoffsettracking = 0;
  }
  maincurve->crosshair_1.active = 0;
  maincurve->crosshair_2.active = 0;
  maincurve->crosshair_1.moving = 0;
  maincurve->crosshair_2.moving = 0;
  maincurve->use_move_events = 0;
  maincurve->setMouseTracking(FALSE);

  for(i=0; i<signalcomps; i++)
  {
    signalcomp[i]->hasruler = 0;
  }
  maincurve->ruler_active = 0;
  maincurve->ruler_moving = 0;

  maincurve->update();
}


void UI_Mainwindow::open_stream()
{
  if(files_open)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Close all files before opening a stream.");
    messagewindow.exec();
    return;
  }

  live_stream_active = 1;

  open_new_file();

  if(files_open == 1)
  {
    toolsmenu->setEnabled(FALSE);
    timemenu->setEnabled(FALSE);
    windowmenu->setEnabled(FALSE);
    former_page_Act->setEnabled(FALSE);
    shift_page_left_Act->setEnabled(FALSE);
    shift_page_right_Act->setEnabled(FALSE);
    next_page_Act->setEnabled(FALSE);
    shift_page_up_Act->setEnabled(FALSE);
    shift_page_down_Act->setEnabled(FALSE);
    printmenu->setEnabled(FALSE);
    recent_filesmenu->setEnabled(FALSE);

    live_stream_timer->start(live_stream_update_interval);
  }
  else
  {
    live_stream_active = 0;
  }
}


void UI_Mainwindow::live_stream_timer_func()
{
  long long datarecords_old,
            datarecords_new;


  if((!live_stream_active) || (files_open != 1))
  {
    return;
  }

  if(!signalcomps)
  {
    live_stream_timer->start(live_stream_update_interval);

    return;
  }

  datarecords_old = edfheaderlist[0]->datarecords;

  datarecords_new = check_edf_file_datarecords(edfheaderlist[0]);

  if(datarecords_new == 0LL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Stream has no datarecords.");
    messagewindow.exec();

    close_all_files();

    return;
  }

  if((datarecords_new > datarecords_old) && (datarecords_new > 0))
  {
    jump_to_end();
  }

  live_stream_timer->start(live_stream_update_interval);
}


long long UI_Mainwindow::check_edf_file_datarecords(struct edfhdrblock *hdr)
{
  long long datarecords;


  if(fseeko(hdr->file_hdl, 0LL, SEEK_END) == -1LL)
  {
    hdr->datarecords = 0LL;

    return(0LL);
  }

  datarecords = ftello(hdr->file_hdl);

  if(datarecords < 1LL)
  {
    hdr->datarecords = 0LL;

    return(0LL);
  }

  datarecords -= (long long)hdr->hdrsize;

  datarecords /= (long long)hdr->recordsize;

  if(datarecords < 1LL)
  {
    hdr->datarecords = 0LL;

    return(0LL);
  }

  hdr->datarecords = datarecords;

  return(datarecords);
}


#ifdef BK_MRS_project
// temporary code for private use in a certain project
// do not use this code, it will be removed in future

void UI_Mainwindow::keyboard_bk_mrs_project_func()
{
  int i, j,
      len,
      type,
      model,
      order,
      file_present=0;

  char path[MAX_PATH_LENGTH],
       str_signal[1024],
       str_dimension[32];

  double result_mrs,
         result_rms,
         frequency,
         frequency2,
         ripple;

  struct edfhdrblock *hdr;


  hdr = edfheaderlist[0];

  if((!files_open) || (!signalcomps))
  {
    return;
  }

  if(bk_mrs_project_file == NULL)
  {
    get_filename_from_path(path, hdr->filename, MAX_PATH_LENGTH);
    remove_extension_from_filename(path);
    strcat(path, "_mrs.txt");

    strcpy(path, QFileDialog::getSaveFileName(0, "MRS output file", QString::fromLocal8Bit(path), "Text files (*.txt *.TXT)").toLocal8Bit().data());

    if(!strcmp(path, ""))
    {
      return;
    }

    bk_mrs_project_file = fopeno(path, "rb");
    if(bk_mrs_project_file != NULL)
    {
      fclose(bk_mrs_project_file);
      bk_mrs_project_file = NULL;
      file_present = 1;
    }

    if(file_present)
    {
      bk_mrs_project_file = fopeno(path, "ab");
    }
    else
    {
      bk_mrs_project_file = fopeno(path, "wb");
    }

    if(bk_mrs_project_file == NULL)
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "Can not open outputfile for writing.");
      messagewindow.exec();
      return;
    }

    if(!file_present)
    {
      fprintf(bk_mrs_project_file, "starttime,pagetime,signal,number_of_samples,mrs,rms,physical_dimension,file,<filters>\n");
    }
  }

  for(i=0; i<signalcomps; i++)
  {
    strcpy(str_signal, signalcomp[i]->signallabel);
    len = strlen(str_signal);
    for(j=0; j<len; j++)
    {
      if(str_signal[j] == ',')
      {
        str_signal[j] = '.';
      }
    }

    if(signalcomp[i]->stat_cnt)
    {
      result_mrs = (signalcomp[i]->stat_sum_rectified / signalcomp[i]->stat_cnt)  * signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].bitvalue;
      result_rms = sqrt(signalcomp[i]->stat_sum_sqr / signalcomp[i]->stat_cnt)  * signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].bitvalue;
    }
    else
    {
      result_mrs = 0.0;
      result_rms = 0.0;
    }

    strcpy(str_dimension, signalcomp[i]->physdimension);
    len = strlen(str_dimension);
    for(j=0; j<len; j++)
    {
      if(str_dimension[j] == ',')
      {
        str_dimension[j] = '.';
      }
    }

#ifdef Q_WS_WIN
    fprintf(bk_mrs_project_file, "%I64d.%07I64d,%I64d.%07I64d,%s,%i,%f,%f,%s",
            hdr->viewtime / TIME_DIMENSION,
            hdr->viewtime % TIME_DIMENSION,
            pagetime / TIME_DIMENSION,
            pagetime % TIME_DIMENSION,
            str_signal,
            signalcomp[i]->stat_cnt,
            result_mrs,
            result_rms,
            str_dimension);
#else
    fprintf(bk_mrs_project_file, "%lli.%07lli,%lli.%07lli,%s,%i,%f,%f,%s",
            hdr->viewtime / TIME_DIMENSION,
            hdr->viewtime % TIME_DIMENSION,
            pagetime / TIME_DIMENSION,
            pagetime % TIME_DIMENSION,
            str_signal,
            signalcomp[i]->stat_cnt,
            result_mrs,
            result_rms,
            str_dimension);
#endif

    fprintf(bk_mrs_project_file, ",%s", hdr->filename);

    for(j=0; j<signalcomp[i]->fidfilter_cnt; j++)
    {
      type = signalcomp[i]->fidfilter_type[j];

      model = signalcomp[i]->fidfilter_model[j];

      frequency = signalcomp[i]->fidfilter_freq[j];

      frequency2 = signalcomp[i]->fidfilter_freq2[j];

      order = signalcomp[i]->fidfilter_order[j];

      ripple = signalcomp[i]->fidfilter_ripple[j];

      if(type == 0)
      {
        fprintf(bk_mrs_project_file, ",HP");
      }

      if(type == 1)
      {
        fprintf(bk_mrs_project_file, ",LP");
      }

      if(type == 2)
      {
        fprintf(bk_mrs_project_file, ",N Res %fHz Q-factor %i", frequency, order);
      }

      if(type == 3)
      {
        fprintf(bk_mrs_project_file, ",BP");
      }

      if(type == 4)
      {
        fprintf(bk_mrs_project_file, ",BS");
      }

      if(type != 2)
      {
        if(model == 0)
        {
          fprintf(bk_mrs_project_file, " Bu");
        }

        if(model == 1)
        {
          fprintf(bk_mrs_project_file, " Ch");
        }

        if(model == 2)
        {
          fprintf(bk_mrs_project_file, " Be");
        }

        if((type == 0) || (type == 1))
        {
          fprintf(bk_mrs_project_file, " %fHz %ith order", frequency, order);
        }

        if((type == 3) || (type == 4))
        {
          fprintf(bk_mrs_project_file, " %f-%fHz %ith order", frequency, frequency2, order);
        }

        if(model == 1)
        {
          fprintf(bk_mrs_project_file, " ripple %fdB", ripple);
        }
      }
    }

    fputc('\n', bk_mrs_project_file);
  }

  fflush(bk_mrs_project_file);

  UI_Messagewindow popupmessage("Message", "Values are written.", NULL, 1000);
}

#endif


void UI_Mainwindow::save_file()
{
  int len;

  char f_path[MAX_PATH_LENGTH];

  struct edfhdrblock *hdr;

  struct annotationblock *annot;

  FILE *outputfile;


  if((!annotations_edited)||(!files_open))
  {
    save_act->setEnabled(FALSE);

    return;
  }

  hdr = edfheaderlist[0];

  strcpy(f_path, recent_savedir);
  strcat(f_path, "/");
  len = strlen(f_path);
  get_filename_from_path(f_path + len, hdr->filename, MAX_PATH_LENGTH - len);
  remove_extension_from_filename(f_path);
  if(hdr->edf)
  {
    strcat(f_path, "_edited.edf");
  }
  else
  {
    strcat(f_path, "_edited.bdf");
  }

  strcpy(f_path, QFileDialog::getSaveFileName(this, "Save file", QString::fromLocal8Bit(f_path), "EDF/BDF files (*.edf *.EDF *.bdf *.BDF *.rec *.REC)").toLocal8Bit().data());

  if(!strcmp(f_path, ""))
  {
    return;
  }

  get_directory_from_path(recent_savedir, f_path, MAX_PATH_LENGTH);

  if(file_is_opened(f_path))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Selected file is in use.");
    messagewindow.exec();
    return;
  }

  outputfile = fopeno(f_path, "wb");
  if(outputfile==NULL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Can not create a file for writing.");
    messagewindow.exec();
    return;
  }

  if(save_annotations(this, outputfile, hdr, annotationlist[0]))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "An error occurred during saving.");
    messagewindow.exec();
    fclose(outputfile);
    return;
  }

  fclose(outputfile);

  annot = annotationlist[0];

  annotationlist[0] = annotationlist_backup;

  annotationlist_backup = NULL;

  edfplus_annotation_delete_list(&annot);

  annotations_dock[0]->updateList();

  annotations_edited = 0;

  save_act->setEnabled(FALSE);

  annotationEditDock->dockedit->hide();

  maincurve->update();
}



void UI_Mainwindow::slider_moved(int value)
{
  int i;

  long long new_viewtime,
            tmp;


  if(!signalcomps)  return;

  new_viewtime = edfheaderlist[sel_viewtime]->long_data_record_duration * edfheaderlist[sel_viewtime]->datarecords;

  new_viewtime -= pagetime;

  if(new_viewtime<0)
  {
    new_viewtime = 0;
  }
  else
  {
    new_viewtime /= 1000000;

    new_viewtime *= value;
  }

  if(pagetime >= (300LL * TIME_DIMENSION))
  {
    tmp = new_viewtime % (30LL * TIME_DIMENSION);

    new_viewtime -= tmp;
  }
  else
    if(pagetime >= (60LL * TIME_DIMENSION))
    {
      tmp = new_viewtime % (6LL * TIME_DIMENSION);

      new_viewtime -= tmp;
    }
    else
      if(pagetime >= (30LL * TIME_DIMENSION))
      {
        tmp = new_viewtime % (3LL * TIME_DIMENSION);

        new_viewtime -= tmp;
      }
      else
        if(pagetime >= (20LL * TIME_DIMENSION))
        {
          tmp = new_viewtime % (2LL * TIME_DIMENSION);

          new_viewtime -= tmp;
        }
        else
          if(pagetime >= (10LL * TIME_DIMENSION))
          {
            tmp = new_viewtime % TIME_DIMENSION;

            new_viewtime -= tmp;
          }
          else
            if(pagetime >= TIME_DIMENSION)
            {
              tmp = new_viewtime % (TIME_DIMENSION / 10LL);

              new_viewtime -= tmp;
            }

  if(viewtime_sync==VIEWTIME_SYNCED_OFFSET)
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime = new_viewtime;
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime = new_viewtime;
  }

  if(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)
  {
    edfheaderlist[sel_viewtime]->viewtime = new_viewtime;

    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime = edfheaderlist[sel_viewtime]->viewtime - ((edfheaderlist[i]->utc_starttime - edfheaderlist[sel_viewtime]->utc_starttime) * TIME_DIMENSION);
      }
    }
  }

  if(viewtime_sync==VIEWTIME_USER_DEF_SYNCED)
  {
    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime -= (edfheaderlist[sel_viewtime]->viewtime - new_viewtime);
      }
    }

    edfheaderlist[sel_viewtime]->viewtime = new_viewtime;
  }

  setup_viewbuf();
}



void UI_Mainwindow::set_timesync_reference(QAction *action)
{
  int i;

  for(i=0; i<files_open; i++)
  {
    if(!strcmp(edfheaderlist[i]->filename, action->text().toLocal8Bit().data()))
    {
      break;
    }
  }

  sel_viewtime = i;

  setMainwindowTitle(edfheaderlist[sel_viewtime]);

  setup_viewbuf();
}



void UI_Mainwindow::set_timesync(QAction *)
{
  int i;


  if(no_timesync_act->isChecked())
  {
    viewtime_sync = VIEWTIME_UNSYNCED;
  }

  if(offset_timesync_act->isChecked())
  {
    viewtime_sync = VIEWTIME_SYNCED_OFFSET;

    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime = edfheaderlist[sel_viewtime]->viewtime;
    }
  }

  if(absolut_timesync_act->isChecked())
  {
    viewtime_sync = VIEWTIME_SYNCED_ABSOLUT;

    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime = edfheaderlist[sel_viewtime]->viewtime + edfheaderlist[sel_viewtime]->starttime_offset - ((edfheaderlist[i]->utc_starttime - edfheaderlist[sel_viewtime]->utc_starttime) * TIME_DIMENSION) - edfheaderlist[i]->starttime_offset;
      }
    }
  }

  if(user_def_sync_act->isChecked())
  {
    viewtime_sync = VIEWTIME_USER_DEF_SYNCED;
  }

  setup_viewbuf();
}



void UI_Mainwindow::sync_by_crosshairs()
{
  if(files_open<2)  return;
  if(signalcomps<2)  return;

  if(maincurve->crosshair_1.active&&maincurve->crosshair_2.active)
  {
    if(maincurve->crosshair_1.file_num!=maincurve->crosshair_2.file_num)
    {
      viewtime_sync = VIEWTIME_USER_DEF_SYNCED;

      edfheaderlist[maincurve->crosshair_2.file_num]->viewtime
      -= ((maincurve->crosshair_1.time - edfheaderlist[maincurve->crosshair_1.file_num]->l_starttime - edfheaderlist[maincurve->crosshair_1.file_num]->viewtime - edfheaderlist[maincurve->crosshair_1.file_num]->starttime_offset)
      - (maincurve->crosshair_2.time - edfheaderlist[maincurve->crosshair_2.file_num]->l_starttime - edfheaderlist[maincurve->crosshair_2.file_num]->viewtime - edfheaderlist[maincurve->crosshair_2.file_num]->starttime_offset));

      maincurve->crosshair_2.x_position = maincurve->crosshair_1.x_position;

      user_def_sync_act->setChecked(TRUE);

      viewtime_sync = VIEWTIME_USER_DEF_SYNCED;

      setup_viewbuf();
    }
  }
}


void UI_Mainwindow::show_options_dialog()
{
  UI_OptionsDialog OptionsDialog(this);
}


void UI_Mainwindow::nk2edf_converter()
{
  UI_NK2EDFwindow nk2edf(recent_opendir);
}


void UI_Mainwindow::convert_ascii_to_edf()
{
  UI_ASCII2EDFapp ascii2edf(recent_opendir, recent_savedir);
}


void UI_Mainwindow::convert_binary_to_edf()
{
  UI_RAW2EDFapp bin2edf(&raw2edf_var, recent_opendir, recent_savedir);
}


void UI_Mainwindow::convert_fino_to_edf()
{
  UI_FINO2EDFwindow fino2edf(recent_opendir, recent_savedir);
}


void UI_Mainwindow::convert_wave_to_edf()
{
  UI_WAV2EDFwindow fino2edf(recent_opendir, recent_savedir);
}


void UI_Mainwindow::convert_nexfin_to_edf()
{
  UI_NEXFIN2EDFwindow nexfin2edf(recent_opendir, recent_savedir);
}


void UI_Mainwindow::convert_emsa_to_edf()
{
  UI_EMSA2EDFwindow emsa2edf(recent_opendir, recent_savedir);
}


void UI_Mainwindow::edfd_converter()
{
  UI_EDFDwindow edfplusd2edfplusc(recent_opendir, recent_savedir);
}


void UI_Mainwindow::bdf2edf_converter()
{
  UI_BDF2EDFwindow bdf2edfconv(this);
}


void UI_Mainwindow::biosemi2bdfplus_converter()
{
  UI_BIOSEMI2BDFPLUSwindow biosemi2bdfplusconv(this);
}


void UI_Mainwindow::reduce_signals()
{
  if(!files_open)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "You have to open the file first.");
    messagewindow.exec();
    return;
  }

  UI_ReduceSignalsWindow reduceSignals(this);
}


void UI_Mainwindow::edit_header()
{
  UI_headerEditorWindow header_edit(this);
}


void UI_Mainwindow::BI98002edf_converter()
{
  UI_BI98002EDFwindow BI98002edfconv(recent_opendir, recent_savedir);
}


void UI_Mainwindow::edit_predefined_montages()
{
  UI_edit_predefined_mtg_window edit_predef_mtgs_app(this);
}


void UI_Mainwindow::jump_to_dialog()
{
  UI_JumpMenuDialog jumpmenu(this);
}



void UI_Mainwindow::jump_to_start()
{
  int i;

  if(viewtime_sync==VIEWTIME_SYNCED_OFFSET)
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime = 0;
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime = 0;
  }

  if(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)
  {
    edfheaderlist[sel_viewtime]->viewtime = 0;

    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime = ((edfheaderlist[sel_viewtime]->utc_starttime - edfheaderlist[i]->utc_starttime) * TIME_DIMENSION) + edfheaderlist[sel_viewtime]->starttime_offset - edfheaderlist[i]->starttime_offset;
      }
    }
  }

  if(viewtime_sync==VIEWTIME_USER_DEF_SYNCED)
  {
    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime -= edfheaderlist[sel_viewtime]->viewtime;
      }
    }

    edfheaderlist[sel_viewtime]->viewtime = 0;
  }

  setup_viewbuf();
}



void UI_Mainwindow::jump_to_end()
{
  int i;

  long long new_viewtime;


  new_viewtime = edfheaderlist[sel_viewtime]->datarecords * edfheaderlist[sel_viewtime]->long_data_record_duration - pagetime;

  if(viewtime_sync==VIEWTIME_SYNCED_OFFSET)
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime = new_viewtime;
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime = new_viewtime;
  }

  if(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)
  {
    edfheaderlist[sel_viewtime]->viewtime = new_viewtime;

    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime = edfheaderlist[sel_viewtime]->viewtime + edfheaderlist[sel_viewtime]->starttime_offset - ((edfheaderlist[i]->utc_starttime - edfheaderlist[sel_viewtime]->utc_starttime) * TIME_DIMENSION) - edfheaderlist[i]->starttime_offset;
      }
    }
  }

  if(viewtime_sync==VIEWTIME_USER_DEF_SYNCED)
  {
    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime -= (edfheaderlist[sel_viewtime]->viewtime - new_viewtime);
      }
    }

    edfheaderlist[sel_viewtime]->viewtime = new_viewtime;
  }

  setup_viewbuf();
}



void UI_Mainwindow::add_new_filter()
{
  UI_FilterDialog filterdialog(this);
}



// void UI_Mainwindow::add_new_math_func()
// {
// }



// void UI_Mainwindow::remove_all_math_funcs()
// {
// }



void UI_Mainwindow::zoomback()
{
  int i, j;

  if(!zoomhistory->history_size_tail)
  {
    return;
  }
  zoomhistory->history_size_front++;
  zoomhistory->history_size_tail--;

  for(i=0; i<files_open; i++)
  {
    zoomhistory->viewtime[zoomhistory->pntr][i] = edfheaderlist[i]->viewtime;
  }
  zoomhistory->pagetime[zoomhistory->pntr] = pagetime;
  for(i=0; i<signalcomps; i++)
  {
    zoomhistory->voltpercm[zoomhistory->pntr][i] = signalcomp[i]->voltpercm;
    zoomhistory->screen_offset[zoomhistory->pntr][i] = signalcomp[i]->screen_offset;

    for(j=0; j<signalcomp[i]->num_of_signals; j++)
    {
      zoomhistory->sensitivity[zoomhistory->pntr][i][j] = signalcomp[i]->sensitivity[j];
    }
  }

  zoomhistory->pntr--;
  if(zoomhistory->pntr<0)  zoomhistory->pntr = 63;

  for(i=0; i<files_open; i++)
  {
    edfheaderlist[i]->viewtime = zoomhistory->viewtime[zoomhistory->pntr][i];
  }
  pagetime = zoomhistory->pagetime[zoomhistory->pntr];

  for(i=0; i<signalcomps; i++)
  {
    signalcomp[i]->voltpercm = zoomhistory->voltpercm[zoomhistory->pntr][i];
    signalcomp[i]->screen_offset = zoomhistory->screen_offset[zoomhistory->pntr][i];

    for(j=0; j<signalcomp[i]->num_of_signals; j++)
    {
      signalcomp[i]->sensitivity[j] = zoomhistory->sensitivity[zoomhistory->pntr][i][j];
    }
  }

  setup_viewbuf();
}



void UI_Mainwindow::forward()
{
  int i, j;

  if(!zoomhistory->history_size_front)
  {
    return;
  }
  zoomhistory->history_size_front--;
  zoomhistory->history_size_tail++;

  zoomhistory->pntr++;
  if(zoomhistory->pntr>63)  zoomhistory->pntr = 0;

  for(i=0; i<files_open; i++)
  {
    edfheaderlist[i]->viewtime = zoomhistory->viewtime[zoomhistory->pntr][i];
  }
  pagetime = zoomhistory->pagetime[zoomhistory->pntr];

  for(i=0; i<signalcomps; i++)
  {
    signalcomp[i]->voltpercm = zoomhistory->voltpercm[zoomhistory->pntr][i];
    signalcomp[i]->screen_offset = zoomhistory->screen_offset[zoomhistory->pntr][i];

    for(j=0; j<signalcomp[i]->num_of_signals; j++)
    {
      signalcomp[i]->sensitivity[j] = zoomhistory->sensitivity[zoomhistory->pntr][i][j];
    }
  }

  setup_viewbuf();
}



void UI_Mainwindow::former_page()
{
  int i;

  if((viewtime_sync==VIEWTIME_SYNCED_OFFSET)||(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)||(viewtime_sync==VIEWTIME_USER_DEF_SYNCED))
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime -= pagetime;
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime -= pagetime;
  }

  setup_viewbuf();
}



void UI_Mainwindow::shift_page_left()
{
  int i;

  if((viewtime_sync==VIEWTIME_SYNCED_OFFSET)||(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)||(viewtime_sync==VIEWTIME_USER_DEF_SYNCED))
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime -= (pagetime / 10);
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime -= (pagetime / 10);
  }

  setup_viewbuf();
}



void UI_Mainwindow::shift_page_right()
{
  int i;

  if((viewtime_sync==VIEWTIME_SYNCED_OFFSET)||(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)||(viewtime_sync==VIEWTIME_USER_DEF_SYNCED))
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime += (pagetime / 10);
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime += (pagetime / 10);
  }

  setup_viewbuf();
}



void UI_Mainwindow::next_page()
{
  int i;

  if((viewtime_sync==VIEWTIME_SYNCED_OFFSET)||(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)||(viewtime_sync==VIEWTIME_USER_DEF_SYNCED))
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime += pagetime;
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime += pagetime;
  }

  setup_viewbuf();
}



void UI_Mainwindow::shift_page_up()
{
  int i;

  for(i=0; i<signalcomps; i++)
  {
    signalcomp[i]->screen_offset += (height() / 20.0);

  }

  maincurve->drawCurve_stage_1();
}



void UI_Mainwindow::shift_page_down()
{
  int i;

  for(i=0; i<signalcomps; i++)
  {
    signalcomp[i]->screen_offset -= (height() / 20.0);

  }

  maincurve->drawCurve_stage_1();
}



void UI_Mainwindow::show_annotations()
{
  int i;

  EDF_annotations annotations;

  for(i=0; i<files_open; i++)
  {
    if(edfheaderlist[i]->annots_not_read)
    {
      if((edfheaderlist[i]->edfplus) || (edfheaderlist[i]->bdfplus))
      {
        edfheaderlist[i]->annots_not_read = 0;

        annotations.get_annotations(i, edfheaderlist[i], &annotationlist[i], read_nk_trigger_signal);

        if(edfheaderlist[i]->annots_not_read)
        {
          edfplus_annotation_delete_list(&annotationlist[i]);
        }
        else
        {
          if(annotations_dock[i] == NULL)
          {
            annotations_dock[i] = new UI_Annotationswindow(i, this);

            addDockWidget(Qt::RightDockWidgetArea, annotations_dock[i]->docklist, Qt::Vertical);
          }
        }
      }
    }

    if(annotationlist[i] != NULL)
    {
      if(annotations_dock[i] != NULL)
      {
        annotations_dock[i]->docklist->show();
      }
    }
  }
}



void UI_Mainwindow::annotation_editor()
{
  if(files_open==1)
  {
    if(edfheaderlist[0]->annots_not_read)
    {
      edfplus_annotation_delete_list(&annotationlist[0]);

      if(annotations_dock[0] != NULL)
      {
        annotations_dock[0]->docklist->close();
        delete annotations_dock[0];
        annotations_dock[0] = NULL;
      }

      edfheaderlist[0]->annots_not_read = 0;

      EDF_annotations annotations;

      annotations.get_annotations(0, edfheaderlist[0], &annotationlist[0], read_nk_trigger_signal);

      if(edfheaderlist[0]->annots_not_read)
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "Editing annotations is not possible when you abort the scanning of the file.");
        messagewindow.exec();

        return;
      }
    }

    if(annotations_dock[0]==NULL)
    {
      annotations_dock[0] = new UI_Annotationswindow(0, this);

      addDockWidget(Qt::RightDockWidgetArea, annotations_dock[0]->docklist, Qt::Vertical);
    }

    annotations_dock[0]->docklist->show();

    annotationEditDock->dockedit->show();
  }
  else
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Editing annotations is possible when you have opened one file only.");
    messagewindow.exec();
  }
}



void UI_Mainwindow::show_spectrum_dock()
{
  UI_SignalChooser signalchooserdialog(this, 1);
}



void UI_Mainwindow::open_new_file()
{
  FILE *newfile;

  int i, len, present, position, button_nr=0;

  char str[2048];

  str[0] = 0;

  struct edfhdrblock *edfhdr=NULL;

  if(annot_editor_active&&files_open)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "You can not open multiple files when editing annotations.\n"
                                                              "Close the annotation edit window first.");
    messagewindow.exec();

    cmdlineargument = 0;

    return;
  }

  if((files_open > 0) && (live_stream_active))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "You can not open multiple files while a streaming file is open.\n"
                                                              "Close the streaming file first.");
    messagewindow.exec();

    return;
  }

  if(files_open>=MAXFILES)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "There are too many files opened.");
    messagewindow.exec();
    cmdlineargument = 0;
    return;
  }

  if(cmdlineargument == 0)
  {
    strcpy(path, QFileDialog::getOpenFileName(this, "Open file", QString::fromLocal8Bit(recent_opendir), "EDF/BDF files (*.edf *.EDF *.bdf *.BDF *.rec *.REC)").toLocal8Bit().data());

    if(!strcmp(path, ""))
    {
      return;
    }

    get_directory_from_path(recent_opendir, path, MAX_PATH_LENGTH);
  }

  if((cmdlineargument == 0) || (cmdlineargument == 1))
  {
    montagepath[0] = 0;
  }

  present = 0;

  for(i=0; i<MAX_RECENTFILES; i++)
  {
    if(!strcmp(&recent_file_path[i][0], path))
    {
      present = 1;

      position = i;

      if(cmdlineargument!=2)
      {
        strcpy(montagepath, &recent_file_mtg_path[i][0]);
      }

      break;
    }
  }

  if(present)
  {
    for(i=position; i>0; i--)
    {
      strcpy(&recent_file_path[i][0], &recent_file_path[i-1][0]);
      strcpy(&recent_file_mtg_path[i][0], &recent_file_mtg_path[i-1][0]);
    }
  }
  else
  {
    for(i=MAX_RECENTFILES-1; i>0; i--)
    {
      strcpy(&recent_file_path[i][0], &recent_file_path[i-1][0]);
      strcpy(&recent_file_mtg_path[i][0], &recent_file_mtg_path[i-1][0]);
    }
  }

  strcpy(&recent_file_path[0][0], path);
  strcpy(&recent_file_mtg_path[0][0], montagepath);

  recent_filesmenu->clear();

  for(i=0; i<MAX_RECENTFILES; i++)
  {
    if(recent_file_path[i][0]==0)
    {
      break;
    }
    recent_filesmenu->addAction(QString::fromLocal8Bit(&recent_file_path[i][0]));
  }

  present = 0;

  for(i=0; i<files_open; i++)
  {
    if(!strcmp(edfheaderlist[i]->filename, path))
    {
      present = 1;

      break;
    }
  }

  if(!present)
  {
    len = strlen(path);

    if((strcmp(path + (len - 4), ".edf"))
       &&(strcmp(path + (len - 4), ".EDF"))
       &&(strcmp(path + (len - 4), ".rec"))
       &&(strcmp(path + (len - 4), ".REC"))
       &&(strcmp(path + (len - 4), ".bdf"))
       &&(strcmp(path + (len - 4), ".BDF")))
    {
      snprintf(str, 2048, "File has an unknown extension:  \"%s\"", path + (len - 4));

      QMessageBox messagewindow(QMessageBox::Critical, "Error", QString::fromLocal8Bit(str));
      messagewindow.exec();

      cmdlineargument = 0;

      return;
    }

    newfile = fopeno(path, "rb");
    if(newfile==NULL)
    {
      snprintf(str, 2048, "Can not open file for reading:\n\"%s\"\n"
                          "Check if you have the right permissions.", path);
      QMessageBox messagewindow(QMessageBox::Critical, "Error", QString::fromLocal8Bit(str));
      messagewindow.exec();
      cmdlineargument = 0;
      return;
    }

    strcpy(recent_opendir, path);

    if(len)
    {
      for(i=len-1; i>=0; i--)
      {
        if((path[i] == '/')||(path[i] == '\\'))
        {
          break;
        }
      }
    }

    recent_opendir[i] = 0;

    EDFfileCheck EDFfilechecker;

    edfhdr = EDFfilechecker.check_edf_file(newfile, str, live_stream_active);
    if(edfhdr==NULL)
    {
      fclose(newfile);

      strcat(str, "\n File is not a valid EDF or BDF file.");

      QMessageBox messagewindow(QMessageBox::Critical, "Error", str);
      messagewindow.exec();

      cmdlineargument = 0;

      return;
    }

    if(edfhdr->discontinuous)
    {
      if(edfhdr->edf)
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "EDFbrowser can not show EDF+D (discontiguous) files.\n"
                                                                  "Convert this file to EDF+C first. You can find this converter\n"
                                                                  "in the Tools menu (EDF+D to EDF+C converter).");
        messagewindow.exec();
      }

      if(edfhdr->bdf)
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "EDFbrowser can not show BDF+D (discontiguous) files.\n"
                                                                  "Convert this file to BDF+C first. You can find this converter\n"
                                                                  "in the Tools menu (EDF+D to EDF+C converter).");
        messagewindow.exec();
      }

      free(edfhdr->edfparam);
      free(edfhdr);
      fclose(newfile);
      cmdlineargument = 0;

      return;
    }

    strcpy(edfhdr->filename, path);

    edfhdr->file_hdl = newfile;

    edfhdr->file_num = files_open;

    edfheaderlist[files_open] = edfhdr;

    annotationlist[files_open] = NULL;

    annotations_dock[files_open] = NULL;

    if((edfhdr->edfplus || edfhdr->bdfplus) && (!live_stream_active))
    {
      if((edfhdr->datarecords * (long long)edfhdr->recordsize) <= maxfilesize_to_readin_annotations)
      {
        EDF_annotations annotations;

        annotations.get_annotations(files_open, edfhdr, &annotationlist[files_open], read_nk_trigger_signal);

        if(edfhdr->annots_not_read)
        {
          edfplus_annotation_delete_list(&annotationlist[files_open]);
        }
        else
        {
          annotations_dock[files_open] = new UI_Annotationswindow(files_open, this);

          addDockWidget(Qt::RightDockWidgetArea, annotations_dock[files_open]->docklist, Qt::Vertical);

          if(!annotationlist[files_open])
          {
            annotations_dock[files_open]->docklist->hide();
          }
        }
      }
      else
      {
        edfhdr->annots_not_read = 1;
      }
    }

    if((edfhdr->bdf && (!edfhdr->bdfplus) && read_biosemi_status_signal) && (!live_stream_active))
    {
      if((edfhdr->datarecords * (long long)edfhdr->recordsize) <= maxfilesize_to_readin_annotations)
      {
        BDF_triggers bdf_triggers_obj;

        bdf_triggers_obj.get_triggers(edfhdr, &annotationlist[files_open]);

        annotations_dock[files_open] = new UI_Annotationswindow(files_open, this);

        addDockWidget(Qt::RightDockWidgetArea, annotations_dock[files_open]->docklist, Qt::Vertical);

        if(!annotationlist[files_open])
        {
          annotations_dock[files_open]->docklist->hide();
        }
      }
    }

    if(!files_open)
    {
      edfheaderlist[0]->viewtime = 0;

      setMainwindowTitle(edfhdr);
    }
    else
    {
      if(viewtime_sync==VIEWTIME_SYNCED_OFFSET)
      {
        edfheaderlist[files_open]->viewtime = edfheaderlist[sel_viewtime]->viewtime;
      }

      if(viewtime_sync==VIEWTIME_UNSYNCED)
      {
        edfheaderlist[files_open]->viewtime = 0;
      }

      if(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)
      {
        edfheaderlist[files_open]->viewtime = edfheaderlist[sel_viewtime]->viewtime + edfheaderlist[sel_viewtime]->starttime_offset - edfheaderlist[files_open]->starttime_offset - ((edfheaderlist[files_open]->utc_starttime - edfheaderlist[sel_viewtime]->utc_starttime) * TIME_DIMENSION);
      }

      if(viewtime_sync==VIEWTIME_USER_DEF_SYNCED)
      {
        edfheaderlist[files_open]->viewtime = 0;
      }
    }

    sel_viewtime_act[files_open] = new QAction(QString::fromLocal8Bit(edfhdr->filename), this);
    sel_viewtime_act[files_open]->setCheckable(TRUE);
    if(!files_open)
    {
      sel_viewtime_act[files_open]->setChecked(TRUE);
    }
    sel_viewtime_act_group->addAction(sel_viewtime_act[files_open]);
    timemenu->addAction(sel_viewtime_act[files_open]);

    files_open++;
  }

  if((montagepath[0]!=0)&&(cmdlineargument==2))
  {
    UI_LoadMontagewindow load_mtg(this, montagepath);
    strcpy(&recent_file_mtg_path[0][0], montagepath);
  }
  else
  {
    if((recent_file_mtg_path[0][0] != 0) && (files_open == 1) && auto_reload_mtg)
    {
      QMessageBox messagewindow;
      messagewindow.setText("Load last used montage?");
      messagewindow.setInformativeText(QString::fromLocal8Bit(&recent_file_mtg_path[0][0]));
      messagewindow.setIcon(QMessageBox::Question);
      messagewindow.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      messagewindow.setDefaultButton(QMessageBox::Yes);
      button_nr = messagewindow.exec();

      if(button_nr == QMessageBox::No)
      {
        recent_file_mtg_path[0][0] = 0;

        UI_Signalswindow signalwindow(this);
      }
      else
      {
        UI_LoadMontagewindow load_mtg(this, &recent_file_mtg_path[0][0]);

        if(recent_file_mtg_path[0][0] == 0)
        {
          UI_Signalswindow signalwindow(this);
        }
      }
    }
    else
    {
      UI_Signalswindow signalwindow(this);
    }
  }

  cmdlineargument = 0;
}



void UI_Mainwindow::remove_recent_file_mtg_path(const char *mtg_path)
{
  int i;


  for(i=0; i<MAX_RECENTFILES; i++)
  {
    if(!strcmp(&recent_file_mtg_path[i][0], mtg_path))
    {
      recent_file_mtg_path[i][0] = 0;
    }
  }
}



void UI_Mainwindow::setMainwindowTitle(struct edfhdrblock *edfhdr)
{
  int i, len;

  char str[256];

  struct date_time_struct date_time;


  if(edfhdr==NULL)
  {
    setWindowTitle(PROGRAM_NAME);

    return;
  }

  if(edfhdr->edfplus || edfhdr->bdfplus)
  {
    snprintf(str, 256, PROGRAM_NAME "  subject %s  birthdate %s  startdate %s",
                  edfhdr->plus_patient_name,
                  edfhdr->plus_birthdate,
                  edfhdr->plus_startdate);
  }
  else
  {
    utc_to_date_time(edfhdr->utc_starttime, &date_time);

    date_time.month_str[0] += 32;
    date_time.month_str[1] += 32;
    date_time.month_str[2] += 32;

    snprintf(str, 256, PROGRAM_NAME "  %s  startdate %i %s %i",
                  edfhdr->patient,
                  date_time.day,
                  date_time.month_str,
                  date_time.year);
    len = strlen(str);
    for(i=0; i<len; i++)
    {
      if(str[i]=='_')
      {
        str[i] = ' ';
      }
    }
  }

  setWindowTitle(str);
}



void UI_Mainwindow::signalproperties_dialog()
{
  UI_SignalChooser signalchooserdialog(this, 0);
}



void UI_Mainwindow::filterproperties_dialog()
{
  UI_SignalChooser signalchooserdialog(this, 2);
}



void UI_Mainwindow::add_signals_dialog()
{
  UI_Signalswindow signalwindow(this);
}



void UI_Mainwindow::show_splashscreen()
{
  splash = new QSplashScreen(this, *pixmap, Qt::WindowStaysOnTopHint);

  splash->show();
}



void UI_Mainwindow::show_file_info()
{
  UI_EDFhdrwindow showhdr(this);
}



void UI_Mainwindow::remove_all_filters()
{
  int i, j,
      update_scr=0;

  for(i=0; i<signalcomps; i++)
  {
    for(j=0; j<signalcomp[i]->filter_cnt; j++)
    {
      free(signalcomp[i]->filter[j]);

      update_scr = 1;
    }

    signalcomp[i]->filter_cnt = 0;

    for(j=0; j<signalcomp[i]->fidfilter_cnt; j++)
    {
      free(signalcomp[i]->fidfilter[j]);
      fid_run_free(signalcomp[i]->fid_run[j]);
      fid_run_freebuf(signalcomp[i]->fidbuf[j]);
      fid_run_freebuf(signalcomp[i]->fidbuf2[j]);

      update_scr = 1;
    }

    signalcomp[i]->fidfilter_cnt = 0;

    for(j=0; j<signalcomp[i]->ravg_filter_cnt; j++)
    {
      free_ravg_filter(signalcomp[i]->ravg_filter[j]);

      update_scr = 1;
    }

    signalcomp[i]->ravg_filter_cnt = 0;

    if(signalcomp[i]->ecg_filter != NULL)
    {
      free_ecg_filter(signalcomp[i]->ecg_filter);

      signalcomp[i]->ecg_filter = NULL;

      strcpy(signalcomp[i]->signallabel, signalcomp[i]->signallabel_bu);
      signalcomp[i]->signallabellen = signalcomp[i]->signallabellen_bu;
      strcpy(signalcomp[i]->physdimension, signalcomp[i]->physdimension_bu);
    }

    if(signalcomp[i]->zratio_filter != NULL)
    {
      free_zratio_filter(signalcomp[i]->zratio_filter);

      signalcomp[i]->zratio_filter = NULL;

      strcpy(signalcomp[i]->signallabel, signalcomp[i]->signallabel_bu);
      signalcomp[i]->signallabellen = signalcomp[i]->signallabellen_bu;
      strcpy(signalcomp[i]->physdimension, signalcomp[i]->physdimension_bu);
    }
  }

  if(update_scr)
  {
    setup_viewbuf();
  }
}



void UI_Mainwindow::remove_all_signals()
{
  int i;

  spectrumdock->clear();
  spectrumdock->dock->hide();

  for(i=0; i<MAXSPECTRUMDIALOGS; i++)
  {
    if(spectrumdialog[i] != NULL)
    {
      delete spectrumdialog[i];

      spectrumdialog[i] = NULL;
    }
  }

  for(i=0; i<MAXAVERAGECURVEDIALOGS; i++)
  {
    if(averagecurvedialog[i] != NULL)
    {
      delete averagecurvedialog[i];

      averagecurvedialog[i] = NULL;
    }
  }

  for(i=0; i<MAXZSCOREDIALOGS; i++)
  {
    if(zscoredialog[i] != NULL)
    {
      delete zscoredialog[i];

      zscoredialog[i] = NULL;
    }
  }

  maincurve->crosshair_1.active = 0;
  maincurve->crosshair_2.active = 0;
  maincurve->crosshair_1.moving = 0;
  maincurve->crosshair_2.moving = 0;

  remove_all_filters();

  for(i=0; i<signalcomps; i++)
  {
    free(signalcomp[i]);
  }

  signalcomps = 0;

  if(viewbuf!=NULL)
  {
    free(viewbuf);
    viewbuf = NULL;
  }

  slidertoolbar->setEnabled(FALSE);
  positionslider->blockSignals(TRUE);

  setup_viewbuf();
}


void UI_Mainwindow::close_all_files()
{
  int i, j, k,
      button_nr=0;


  live_stream_active = 0;
  live_stream_timer->stop();
  toolsmenu->setEnabled(TRUE);
  timemenu->setEnabled(TRUE);
  windowmenu->setEnabled(TRUE);
  former_page_Act->setEnabled(TRUE);
  shift_page_left_Act->setEnabled(TRUE);
  shift_page_right_Act->setEnabled(TRUE);
  next_page_Act->setEnabled(TRUE);
  shift_page_up_Act->setEnabled(TRUE);
  shift_page_down_Act->setEnabled(TRUE);
  printmenu->setEnabled(TRUE);
  recent_filesmenu->setEnabled(TRUE);

  if(annotations_edited)
  {
    QMessageBox messagewindow;
    messagewindow.setText("There are unsaved annotations,\n are you sure you want to close this file?");
    messagewindow.setIcon(QMessageBox::Question);
    messagewindow.setStandardButtons(QMessageBox::Cancel | QMessageBox::Close);
    messagewindow.setDefaultButton(QMessageBox::Cancel);
    button_nr = messagewindow.exec();
  }

  if(button_nr == QMessageBox::Cancel)
  {
    return;
  }

  annotations_edited = 0;

#ifdef BK_MRS_project
// temporary code for private use in a certain project
// do not use this code, it will be removed in future

  if(bk_mrs_project_file != NULL)
  {
    fclose(bk_mrs_project_file);
    bk_mrs_project_file = NULL;
  }

#endif

  remove_all_signals();

  while(files_open)
  {
    files_open--;
    if(edfheaderlist[files_open]->file_hdl != NULL)
    {
      fclose(edfheaderlist[files_open]->file_hdl);
    }
    free(edfheaderlist[files_open]->edfparam);
    free(edfheaderlist[files_open]);
    edfplus_annotation_delete_list(&annotationlist[files_open]);

    if(annotations_dock[files_open] != NULL)
    {
      annotations_dock[files_open]->docklist->close();
      delete annotations_dock[files_open];
      annotations_dock[files_open] = NULL;
    }

    delete sel_viewtime_act[files_open];
  }

  edfplus_annotation_delete_list(&annotationlist_backup);

  sel_viewtime = 0;

  pagetime = 10 * TIME_DIMENSION;

  for(i=0; i<64; i++)
  {
    zoomhistory->pntr = 0;
    zoomhistory->pagetime[i] = 10 * TIME_DIMENSION;
    for(j=0; j<MAXFILES; j++)
    {
      zoomhistory->viewtime[i][j] = 0;
    }
    for(j=0; j<MAXSIGNALS; j++)
    {
      zoomhistory->voltpercm[i][j] = 70;
      zoomhistory->screen_offset[i][j] = 0.0;
      for(k=0; k<MAXSIGNALS; k++)
      {
        zoomhistory->sensitivity[i][j][k] = 0.0475;
      }
    }
  }

  annotationEditDock->dockedit->hide();

  save_act->setEnabled(FALSE);

  annotations_edited = 0;

  setWindowTitle(PROGRAM_NAME);

  if(!exit_in_progress)
  {
    maincurve->update();
  }
}


void UI_Mainwindow::show_about_dialog()
{
  UI_Aboutwindow aboutwindow(this);
}


void UI_Mainwindow::page_3cmsec()
{
  double mm;

  if(auto_dpi)
  {
    mm = maincurve->widthMM();

    pagetime = mm * 333333.3333;
  }
  else
  {
    pagetime = (long long)((((double)maincurve->width()) / (1.0 / x_pixelsizefactor) / 3.0) * TIME_DIMENSION);
  }

  setup_viewbuf();
}


void UI_Mainwindow::set_page_div2()
{
  int i;

  if((viewtime_sync==VIEWTIME_SYNCED_OFFSET)||(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)||(viewtime_sync==VIEWTIME_USER_DEF_SYNCED))
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime += (pagetime / 4);
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime += (pagetime / 4);
  }

  pagetime /= 2;

  setup_viewbuf();
}


void UI_Mainwindow::set_page_mult2()
{
  int i;

  if((viewtime_sync==VIEWTIME_SYNCED_OFFSET)||(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)||(viewtime_sync==VIEWTIME_USER_DEF_SYNCED))
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime -= (pagetime / 2);
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime -= (pagetime / 2);
  }

  pagetime *= 2;

  setup_viewbuf();
}


void UI_Mainwindow::set_display_time(QAction *action)
{
  if(action==page_10m)   pagetime = TIME_DIMENSION / 100;
  if(action==page_20m)   pagetime = TIME_DIMENSION / 50;
  if(action==page_50m)   pagetime = TIME_DIMENSION / 20;
  if(action==page_100m)  pagetime = TIME_DIMENSION / 10;
  if(action==page_200m)  pagetime = TIME_DIMENSION / 5;
  if(action==page_500m)  pagetime = TIME_DIMENSION / 2;
  if(action==page_1)     pagetime = TIME_DIMENSION;
  if(action==page_2)     pagetime = TIME_DIMENSION * 2;
  if(action==page_5)     pagetime = TIME_DIMENSION * 5;
  if(action==page_10)    pagetime = TIME_DIMENSION * 10;
  if(action==page_15)    pagetime = TIME_DIMENSION * 15;
  if(action==page_20)    pagetime = TIME_DIMENSION * 20;
  if(action==page_30)    pagetime = TIME_DIMENSION * 30;
  if(action==page_60)    pagetime = TIME_DIMENSION * 60;
  if(action==page_300)   pagetime = TIME_DIMENSION * 300;
  if(action==page_1200)  pagetime = TIME_DIMENSION * 1200;
  if(action==page_3600)  pagetime = TIME_DIMENSION * 3600;

  setup_viewbuf();
}


void UI_Mainwindow::set_user_defined_display_time()
{
  UI_Userdefined_timepage_Dialog set_displaytime_dialog(this);
}


void UI_Mainwindow::set_display_time_whole_rec()
{
  int i;

  if(!files_open)  return;

  if(viewtime_sync==VIEWTIME_SYNCED_OFFSET)
  {
    for(i=0; i<files_open; i++)
    {
      edfheaderlist[i]->viewtime = 0;
    }
  }

  if(viewtime_sync==VIEWTIME_UNSYNCED)
  {
    edfheaderlist[sel_viewtime]->viewtime = 0;
  }

  if(viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)
  {
    edfheaderlist[sel_viewtime]->viewtime = 0;

    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime = ((edfheaderlist[sel_viewtime]->utc_starttime - edfheaderlist[i]->utc_starttime) * TIME_DIMENSION) + edfheaderlist[sel_viewtime]->starttime_offset - - edfheaderlist[i]->starttime_offset;
      }
    }
  }

  if(viewtime_sync==VIEWTIME_USER_DEF_SYNCED)
  {
    for(i=0; i<files_open; i++)
    {
      if(i!=sel_viewtime)
      {
        edfheaderlist[i]->viewtime -= edfheaderlist[sel_viewtime]->viewtime;
      }
    }

    edfheaderlist[sel_viewtime]->viewtime = 0;
  }

  pagetime = edfheaderlist[sel_viewtime]->datarecords * edfheaderlist[sel_viewtime]->long_data_record_duration;

  setup_viewbuf();
}


void UI_Mainwindow::fit_signals_to_pane()
{
  int i, j,
      pane_size;

  if(!signalcomps)  return;

  pane_size = maincurve->height() / (signalcomps + 1);

  for(i=0; i<signalcomps; i++)
  {
    for(j=0; j<signalcomp[i]->num_of_signals; j++)
    {
      if(signalcomp[i]->max_dig_value!=signalcomp[i]->min_dig_value)
      {
        signalcomp[i]->sensitivity[j] = (double)pane_size / (double)(signalcomp[i]->max_dig_value - signalcomp[i]->min_dig_value);
      }
      else
      {
        signalcomp[i]->sensitivity[j] = pane_size;
      }
    }

    signalcomp[i]->voltpercm =
     signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].bitvalue
     / (signalcomp[i]->sensitivity[0] * pixelsizefactor);

    signalcomp[i]->screen_offset = ((signalcomp[i]->max_dig_value + signalcomp[i]->min_dig_value) / 2.0) * signalcomp[i]->sensitivity[0];
  }

  maincurve->drawCurve_stage_1();
}


void UI_Mainwindow::fit_signals_dc_offset()
{
  int i;

  if(!signalcomps)  return;

  for(i=0; i<signalcomps; i++)
  {
    signalcomp[i]->screen_offset = ((signalcomp[i]->max_dig_value + signalcomp[i]->min_dig_value) / 2.0) * signalcomp[i]->sensitivity[0];
  }

  maincurve->drawCurve_stage_1();
}


void UI_Mainwindow::set_amplitude(QAction *action)
{
  int i, j;

  double value=100.0, original_value, value2=100.0;


  if(action==amp_00001) value = 0.0001;
  if(action==amp_00002) value = 0.0002;
  if(action==amp_00005) value = 0.0005;
  if(action==amp_0001)  value = 0.001;
  if(action==amp_0002)  value = 0.002;
  if(action==amp_0005)  value = 0.005;
  if(action==amp_001)   value = 0.01;
  if(action==amp_002)   value = 0.02;
  if(action==amp_005)   value = 0.05;
  if(action==amp_01)    value = 0.1;
  if(action==amp_02)    value = 0.2;
  if(action==amp_05)    value = 0.5;
  if(action==amp_1)     value = 1.0;
  if(action==amp_2)     value = 2.0;
  if(action==amp_5)     value = 5.0;
  if(action==amp_10)    value = 10.0;
  if(action==amp_20)    value = 20.0;
  if(action==amp_50)    value = 50.0;
  if(action==amp_100)   value = 100.0;
  if(action==amp_200)   value = 200.0;
  if(action==amp_500)   value = 500.0;
  if(action==amp_1000)  value = 1000.0;
  if(action==amp_2000)  value = 2000.0;
  if(action==amp_5000)  value = 5000.0;
  if(action==amp_10000) value = 10000.0;
  if(action==amp_20000) value = 20000.0;
  if(action==amp_50000) value = 50000.0;

  for(i=0; i<signalcomps; i++)
  {
    value2 = value;

    if(signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].bitvalue < 0.0)
    {
      value2 *= -1.0;
    }

    for(j=0; j<signalcomp[i]->num_of_signals; j++)
    {
      signalcomp[i]->sensitivity[j] = (signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[j]].bitvalue / value2) / pixelsizefactor;
    }

    original_value = signalcomp[i]->voltpercm;

    signalcomp[i]->voltpercm = value2;

    signalcomp[i]->screen_offset = signalcomp[i]->screen_offset * (original_value / value2);
  }

  maincurve->drawCurve_stage_1();
}


void UI_Mainwindow::load_predefined_mtg(QAction *action)
{
  int i;

  if(!files_open)
  {
    return;
  }

  for(i=0; i < MAXPREDEFINEDMONTAGES; i++)
  {
    if(action==load_predefined_mtg_act[i])
    {
      if(predefined_mtg_path[i][0] != 0)
      {
        strcpy(montagepath, &predefined_mtg_path[i][0]);

        UI_LoadMontagewindow load_mtg(this, montagepath);

        return;
      }
    }
  }
}


void UI_Mainwindow::setup_viewbuf()
{
  int i, j, k, s,
      temp=0,
      skip,
      totalsize,
      hasprefilter=0,
      readsize=0,
      dif;

  double pre_time=0.0,
         d_temp,
         dig_value;

  long long l_temp,
            datarecords;

  union {
          unsigned int one;
          signed int one_signed;
          unsigned short two[2];
          signed short two_signed[2];
          unsigned char four[4];
        } var;


  for(i=0; i<files_open; i++) edfheaderlist[i]->prefiltertime = 0;

  for(i=0; i<signalcomps; i++)
  {
    if(signalcomp[i]->filter_cnt)
    {
      hasprefilter = 1;

      for(k=0; k<signalcomp[i]->filter_cnt; k++)
      {
        if(pre_time < (1.0 / signalcomp[i]->filter[k]->cutoff_frequency))
        {
          pre_time = (1.0 / signalcomp[i]->filter[k]->cutoff_frequency);
        }
      }
    }

    if(signalcomp[i]->ravg_filter_cnt)
    {
      hasprefilter = 1;

      for(k=0; k<signalcomp[i]->ravg_filter_cnt; k++)
      {
        if(pre_time < ((double)(signalcomp[i]->ravg_filter[k]->size + 3) / ((double)signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].smp_per_record / signalcomp[i]->edfhdr->data_record_duration)))
        {
          pre_time = (double)(signalcomp[i]->ravg_filter[k]->size + 3) / ((double)signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].smp_per_record / signalcomp[i]->edfhdr->data_record_duration);
        }
      }
    }

    if(signalcomp[i]->ecg_filter != NULL)
    {
      hasprefilter = 1;

      if(pre_time < 10.0)
      {
        pre_time = 10.0;
      }
    }

    if(signalcomp[i]->zratio_filter != NULL)
    {
      hasprefilter = 1;

      if(pre_time < 4.0)
      {
        pre_time = 4.0;
      }
    }

    if(signalcomp[i]->fidfilter_cnt)
    {
      hasprefilter = 1;

      for(k=0; k<signalcomp[i]->fidfilter_cnt; k++)
      {
        if(pre_time < ((2.0 * signalcomp[i]->fidfilter_order[k]) / signalcomp[i]->fidfilter_freq[k]))
        {
          pre_time = (2.0 * signalcomp[i]->fidfilter_order[k]) / signalcomp[i]->fidfilter_freq[k];
        }
      }
    }
  }

  if(hasprefilter)
  {
    for(i=0; i<signalcomps; i++)
    {
      if((signalcomp[i]->filter_cnt) || (signalcomp[i]->ravg_filter_cnt) || (signalcomp[i]->fidfilter_cnt) || (signalcomp[i]->ecg_filter != NULL) || (signalcomp[i]->zratio_filter != NULL))
      {
        signalcomp[i]->edfhdr->prefiltertime = (long long)(pre_time * ((double)TIME_DIMENSION));
        if(signalcomp[i]->edfhdr->prefiltertime>signalcomp[i]->edfhdr->viewtime)
        {
          signalcomp[i]->edfhdr->prefiltertime = signalcomp[i]->edfhdr->viewtime;
          if(signalcomp[i]->edfhdr->prefiltertime<0) signalcomp[i]->edfhdr->prefiltertime = 0;
        }
      }
    }

    totalsize = 0;

    for(i=0; i<signalcomps; i++)
    {
      if(signalcomp[i]->edfhdr->prefiltertime)  signalcomp[i]->records_in_viewbuf = (signalcomp[i]->edfhdr->viewtime / signalcomp[i]->edfhdr->long_data_record_duration) - ((signalcomp[i]->edfhdr->viewtime - signalcomp[i]->edfhdr->prefiltertime) / signalcomp[i]->edfhdr->long_data_record_duration) + 1;
      else signalcomp[i]->records_in_viewbuf = 0;

      signalcomp[i]->viewbufsize = signalcomp[i]->records_in_viewbuf * signalcomp[i]->edfhdr->recordsize;

      if(signalcomp[i]->edfhdr->prefiltertime)
      {
        signalcomp[i]->samples_in_prefilterbuf = (signalcomp[i]->records_in_viewbuf - 1) * signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].smp_per_record;

        signalcomp[i]->samples_in_prefilterbuf
        += (int)(((double)(signalcomp[i]->edfhdr->viewtime % signalcomp[i]->edfhdr->long_data_record_duration)
        / (double)signalcomp[i]->edfhdr->long_data_record_duration)
        * (double)signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].smp_per_record);
      }
      else
      {
        signalcomp[i]->samples_in_prefilterbuf = 0;
      }

      if(!i)
      {
        signalcomp[i]->viewbufoffset = 0;
        totalsize = signalcomp[i]->viewbufsize;
      }
      else
      {
        skip = 0;

        for(j=0; j<i; j++)
        {
          if(signalcomp[i]->edfhdr->file_hdl==signalcomp[j]->edfhdr->file_hdl)
          {
            skip = 1;
            signalcomp[i]->viewbufoffset = signalcomp[j]->viewbufoffset;
            signalcomp[i]->records_in_viewbuf = signalcomp[j]->records_in_viewbuf;
            signalcomp[i]->viewbufsize = signalcomp[j]->viewbufsize;
            break;
          }
        }

        if(!skip)
        {
          signalcomp[i]->viewbufoffset = signalcomp[i-1]->viewbufoffset + signalcomp[i-1]->viewbufsize;
          totalsize += signalcomp[i]->viewbufsize;
        }
      }
    }

    if(viewbuf!=NULL)
    {
      free(viewbuf);
      viewbuf = NULL;
    }

    viewbuf = (char *)malloc(totalsize);
    if(viewbuf==NULL)
    {
      live_stream_active = 0;
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "Internal error: Memory allocation error:\n\"prefilterbuf\"");
      messagewindow.exec();
      remove_all_signals();
      return;
    }

    for(i=0; i<signalcomps; i++)
    {
      if(!i)
      {
        datarecords = (signalcomp[i]->edfhdr->viewtime - signalcomp[i]->edfhdr->prefiltertime) / signalcomp[i]->edfhdr->long_data_record_duration;

        signalcomp[i]->prefilter_starttime = datarecords * signalcomp[i]->edfhdr->long_data_record_duration;

        if((signalcomp[i]->viewbufsize>0)&&(datarecords<signalcomp[i]->edfhdr->datarecords))
        {
          fseeko(signalcomp[i]->edfhdr->file_hdl, (long long)(signalcomp[i]->edfhdr->hdrsize + (datarecords * signalcomp[i]->edfhdr->recordsize)), SEEK_SET);

          if(signalcomp[i]->viewbufsize>((signalcomp[i]->edfhdr->datarecords - datarecords) * signalcomp[i]->edfhdr->recordsize))
          {
            signalcomp[i]->viewbufsize = (signalcomp[i]->edfhdr->datarecords - datarecords) * signalcomp[i]->edfhdr->recordsize;
          }

          if(fread(viewbuf + signalcomp[i]->viewbufoffset, signalcomp[i]->viewbufsize, 1, signalcomp[i]->edfhdr->file_hdl)!=1)
          {
            live_stream_active = 0;
            QMessageBox messagewindow(QMessageBox::Critical, "Error", "A read error occurred. 2");
            messagewindow.exec();
            remove_all_signals();
            return;
          }
        }
      }
      else
      {
        skip = 0;

        for(j=0; j<i; j++)
        {
          if(signalcomp[i]->edfhdr->file_hdl==signalcomp[j]->edfhdr->file_hdl)
          {
            skip = 1;
            break;
          }
        }

        if(!skip)
        {
          datarecords = (signalcomp[i]->edfhdr->viewtime - signalcomp[i]->edfhdr->prefiltertime) / signalcomp[i]->edfhdr->long_data_record_duration;

          signalcomp[i]->prefilter_starttime = datarecords * signalcomp[i]->edfhdr->long_data_record_duration;

          if((signalcomp[i]->viewbufsize>0)&&(datarecords<signalcomp[i]->edfhdr->datarecords))
          {
            fseeko(signalcomp[i]->edfhdr->file_hdl, (long long)(signalcomp[i]->edfhdr->hdrsize + (datarecords * signalcomp[i]->edfhdr->recordsize)), SEEK_SET);

            if(signalcomp[i]->viewbufsize>((signalcomp[i]->edfhdr->datarecords - datarecords) * signalcomp[i]->edfhdr->recordsize))
            {
              signalcomp[i]->viewbufsize = (signalcomp[i]->edfhdr->datarecords - datarecords) * signalcomp[i]->edfhdr->recordsize;
            }

            if(fread(viewbuf + signalcomp[i]->viewbufoffset, signalcomp[i]->viewbufsize, 1, signalcomp[i]->edfhdr->file_hdl)!=1)
            {
              live_stream_active = 0;
              QMessageBox messagewindow(QMessageBox::Critical, "Error", "A read error occurred. 3");
              messagewindow.exec();
              remove_all_signals();
              return;
            }
          }
        }
      }
    }

    for(i=0; i<signalcomps; i++)
    {
      if(signalcomp[i]->zratio_filter != NULL)
      {
        l_temp = signalcomp[i]->prefilter_starttime % (TIME_DIMENSION * 2LL); // necessary for the Z-ratio filter
        if(l_temp != 0L)
        {
          temp = (TIME_DIMENSION * 2LL) - l_temp;

          l_temp = temp;

          signalcomp[i]->prefilter_reset_sample = (l_temp / signalcomp[i]->edfhdr->long_data_record_duration)
          * signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].smp_per_record;

          signalcomp[i]->prefilter_reset_sample
          += (int)(((double)(l_temp % signalcomp[i]->edfhdr->long_data_record_duration)
          / (double)signalcomp[i]->edfhdr->long_data_record_duration)
          * (double)signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].smp_per_record);
        }
        else
        {
          signalcomp[i]->prefilter_reset_sample = 0;
        }

// printf("records_in_viewbuf is %lli\n"
//       "samples_in_prefilterbuf is %i\n"
//       "l_temp is %lli\n"
//       "temp is %i\n"
//       "prefilter_reset_sample is %i\n\n",
//       signalcomp[i]->records_in_viewbuf,
//       signalcomp[i]->samples_in_prefilterbuf,
//       l_temp,
//       temp,
//       signalcomp[i]->prefilter_reset_sample);

      }
    }

    for(i=0; i<signalcomps; i++)
    {
      if((!signalcomp[i]->filter_cnt) && (!signalcomp[i]->ravg_filter_cnt) && (!signalcomp[i]->fidfilter_cnt) && (signalcomp[i]->ecg_filter == NULL) && (signalcomp[i]->zratio_filter == NULL)) continue;

      for(s=0; s<signalcomp[i]->samples_in_prefilterbuf; s++)
      {
        dig_value = 0.0;

        for(k=0; k<signalcomp[i]->num_of_signals; k++)
        {
          if(signalcomp[i]->edfhdr->bdf)
          {
            var.two[0] = *((unsigned short *)(
              viewbuf
              + signalcomp[i]->viewbufoffset
              + (signalcomp[i]->edfhdr->recordsize * (s / signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[k]].smp_per_record))
              + signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[k]].buf_offset
              + ((s % signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[k]].smp_per_record) * 3)));

            var.four[2] = *((unsigned char *)(
              viewbuf
              + signalcomp[i]->viewbufoffset
              + (signalcomp[i]->edfhdr->recordsize * (s / signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[k]].smp_per_record))
              + signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[k]].buf_offset
              + ((s % signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[k]].smp_per_record) * 3)
              + 2));

            if(var.four[2]&0x80)
            {
              var.four[3] = 0xff;
            }
            else
            {
              var.four[3] = 0x00;
            }

            temp = var.one_signed;
          }

          if(signalcomp[i]->edfhdr->edf)
          {
            temp = *(((short *)(
            viewbuf
            + signalcomp[i]->viewbufoffset
            + (signalcomp[i]->edfhdr->recordsize * (s / signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[k]].smp_per_record))
            + signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[k]].buf_offset))
            + (s % signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[k]].smp_per_record));
          }

          temp += signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[k]].offset;
          temp *= signalcomp[i]->factor[k];

          dig_value += temp;
        }

        for(j=0; j<signalcomp[i]->filter_cnt; j++)
        {
          dig_value = first_order_filter(dig_value, signalcomp[i]->filter[j]);
        }

        for(j=0; j<signalcomp[i]->ravg_filter_cnt; j++)
        {
          dig_value = run_ravg_filter(dig_value, signalcomp[i]->ravg_filter[j]);
        }

        for(j=0; j<signalcomp[i]->fidfilter_cnt; j++)
        {
          dig_value = signalcomp[i]->fidfuncp[j](signalcomp[i]->fidbuf[j], dig_value);
        }

        if(signalcomp[i]->ecg_filter != NULL)
        {
          if(s == 0)
          {
            reset_ecg_filter(signalcomp[i]->ecg_filter);
          }

          dig_value = run_ecg_filter(dig_value, signalcomp[i]->ecg_filter);
        }

        if(signalcomp[i]->zratio_filter != NULL)
        {
          if(s == signalcomp[i]->prefilter_reset_sample)
          {
            reset_zratio_filter(signalcomp[i]->zratio_filter);
          }

          dig_value = run_zratio_filter(dig_value, signalcomp[i]->zratio_filter);
        }
      }
    }

    for(i=0; i<signalcomps; i++)
    {
      if(signalcomp[i]->samples_in_prefilterbuf > 0)
      {
        for(j=0; j<signalcomp[i]->filter_cnt; j++)
        {
          signalcomp[i]->filterpreset_a[j] = signalcomp[i]->filter[j]->old_input;
          signalcomp[i]->filterpreset_b[j] = signalcomp[i]->filter[j]->old_output;
        }

        for(j=0; j<signalcomp[i]->ravg_filter_cnt; j++)
        {
          ravg_filter_save_buf(signalcomp[i]->ravg_filter[j]);
        }

        for(j=0; j<signalcomp[i]->fidfilter_cnt; j++)
        {
          memcpy(signalcomp[i]->fidbuf2[j], signalcomp[i]->fidbuf[j], fid_run_bufsize(signalcomp[i]->fid_run[j]));
        }

        if(signalcomp[i]->ecg_filter != NULL)
        {
          ecg_filter_save_buf(signalcomp[i]->ecg_filter);
        }

        if(signalcomp[i]->zratio_filter != NULL)
        {
          zratio_filter_save_buf(signalcomp[i]->zratio_filter);
        }
      }
    }
  }

  totalsize = 0;

  for(i=0; i<signalcomps; i++)
  {
    if(signalcomp[i]->edfhdr->viewtime>=0)  signalcomp[i]->records_in_viewbuf = ((pagetime + (signalcomp[i]->edfhdr->viewtime % signalcomp[i]->edfhdr->long_data_record_duration)) / signalcomp[i]->edfhdr->long_data_record_duration) + 1;
    else  signalcomp[i]->records_in_viewbuf = ((pagetime + ((-(signalcomp[i]->edfhdr->viewtime)) % signalcomp[i]->edfhdr->long_data_record_duration)) / signalcomp[i]->edfhdr->long_data_record_duration) + 1;

    signalcomp[i]->viewbufsize = signalcomp[i]->records_in_viewbuf * signalcomp[i]->edfhdr->recordsize;

    signalcomp[i]->samples_on_screen = (int)(((double)pagetime / (double)signalcomp[i]->edfhdr->long_data_record_duration) * (double)signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].smp_per_record);

    if(signalcomp[i]->edfhdr->viewtime<0)
    {
      d_temp =
        (((double)(-(signalcomp[i]->edfhdr->viewtime)))
        / (double)signalcomp[i]->edfhdr->long_data_record_duration)
        * (double)signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].smp_per_record;

      if(d_temp>2147483648.0)
      {
        signalcomp[i]->sample_start = 2147483647LL;
      }
      else
      {
        signalcomp[i]->sample_start = (int)d_temp;
      }
    }
    else
    {
      signalcomp[i]->sample_start = 0;
    }

    if(signalcomp[i]->edfhdr->viewtime>=0)
    {
      signalcomp[i]->sample_timeoffset_part = ((double)(signalcomp[i]->edfhdr->viewtime % signalcomp[i]->edfhdr->long_data_record_duration) / (double)signalcomp[i]->edfhdr->long_data_record_duration) * (double)signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].smp_per_record;
      signalcomp[i]->sample_timeoffset = (int)(signalcomp[i]->sample_timeoffset_part);
      signalcomp[i]->sample_timeoffset_part -= signalcomp[i]->sample_timeoffset;
    }
    else
    {
      signalcomp[i]->sample_timeoffset_part = 0.0;
      signalcomp[i]->sample_timeoffset = 0;
    }

    if(!i)
    {
      signalcomp[i]->viewbufoffset = 0;
      totalsize = signalcomp[i]->viewbufsize;
    }
    else
    {
      skip = 0;

      for(j=0; j<i; j++)
      {
        if(signalcomp[i]->edfhdr->file_hdl==signalcomp[j]->edfhdr->file_hdl)
        {
          skip = 1;
          signalcomp[i]->viewbufoffset = signalcomp[j]->viewbufoffset;
          signalcomp[i]->records_in_viewbuf = signalcomp[j]->records_in_viewbuf;
          signalcomp[i]->viewbufsize = signalcomp[j]->viewbufsize;
          break;
        }
      }

      if(!skip)
      {
        signalcomp[i]->viewbufoffset = signalcomp[i-1]->viewbufoffset + signalcomp[i-1]->viewbufsize;
        totalsize += signalcomp[i]->viewbufsize;
      }
    }
  }

  if(viewbuf!=NULL)
  {
    free(viewbuf);
    viewbuf = NULL;
  }

  if(totalsize)
  {
    viewbuf = (char *)malloc(totalsize);
    if(viewbuf==NULL)
    {
      live_stream_active = 0;
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "The system was not able to provide enough resources (memory) to perform the requested action.\n"
                                                                "Decrease the timescale and try again.");
      messagewindow.exec();

      remove_all_signals();
      return;
    }
  }

  for(i=0; i<signalcomps; i++)
  {
    if(!i)
    {
      if(signalcomp[i]->edfhdr->viewtime>=0)
      {
        datarecords = signalcomp[i]->edfhdr->viewtime / signalcomp[i]->edfhdr->long_data_record_duration;
      }
      else
      {
        datarecords = 0;
      }

      dif = signalcomp[i]->edfhdr->datarecords - datarecords;

      if(dif<=0)
      {
        memset(viewbuf + signalcomp[i]->viewbufoffset, 0, signalcomp[i]->records_in_viewbuf * signalcomp[i]->edfhdr->recordsize);

        signalcomp[i]->sample_stop = 0;
      }
      else
      {
        if(dif<signalcomp[i]->records_in_viewbuf)
        {
          readsize = dif * signalcomp[i]->edfhdr->recordsize;

          memset(viewbuf + signalcomp[i]->viewbufoffset + readsize, 0, (signalcomp[i]->records_in_viewbuf * signalcomp[i]->edfhdr->recordsize) - readsize);

          signalcomp[i]->sample_stop = (dif * signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].smp_per_record) - signalcomp[i]->sample_timeoffset;
        }
        else
        {
          readsize = signalcomp[i]->records_in_viewbuf * signalcomp[i]->edfhdr->recordsize;

          signalcomp[i]->sample_stop = signalcomp[i]->samples_on_screen;
        }

        l_temp = signalcomp[i]->edfhdr->hdrsize;
        l_temp += (datarecords * signalcomp[i]->edfhdr->recordsize);

        fseeko(signalcomp[i]->edfhdr->file_hdl, l_temp, SEEK_SET);

        if(fread(viewbuf + signalcomp[i]->viewbufoffset, readsize, 1, signalcomp[i]->edfhdr->file_hdl)!=1)
        {
          live_stream_active = 0;
          QMessageBox messagewindow(QMessageBox::Critical, "Error", "A read error occurred. 5");
          messagewindow.exec();
          remove_all_signals();
          return;
        }
      }
    }
    else
    {
      skip = 0;

      for(j=0; j<i; j++)
      {
        if(signalcomp[i]->edfhdr->file_hdl==signalcomp[j]->edfhdr->file_hdl)
        {
          skip = 1;
          break;
        }
      }

      if(signalcomp[i]->edfhdr->viewtime>=0)
      {
        datarecords = signalcomp[i]->edfhdr->viewtime / signalcomp[i]->edfhdr->long_data_record_duration;
      }
      else
      {
        datarecords = 0;
      }

      dif = signalcomp[i]->edfhdr->datarecords - datarecords;

      if(dif<=0)
      {
        if(!skip)
        {
          memset(viewbuf + signalcomp[i]->viewbufoffset, 0, signalcomp[i]->records_in_viewbuf * signalcomp[i]->edfhdr->recordsize);
        }

        signalcomp[i]->sample_stop = 0;
      }
      else
      {
        if(dif<signalcomp[i]->records_in_viewbuf)
        {
          if(!skip)
          {
            readsize = dif * signalcomp[i]->edfhdr->recordsize;

            memset(viewbuf + signalcomp[i]->viewbufoffset + readsize, 0, (signalcomp[i]->records_in_viewbuf * signalcomp[i]->edfhdr->recordsize) - readsize);
          }

          signalcomp[i]->sample_stop = (dif * signalcomp[i]->edfhdr->edfparam[signalcomp[i]->edfsignal[0]].smp_per_record) - signalcomp[i]->sample_timeoffset;
        }
        else
        {
          if(!skip)
          {
            readsize = signalcomp[i]->records_in_viewbuf * signalcomp[i]->edfhdr->recordsize;
          }

          signalcomp[i]->sample_stop = signalcomp[i]->samples_on_screen;
        }

        if(!skip)
        {
          l_temp = signalcomp[i]->edfhdr->hdrsize;
          l_temp += (datarecords * signalcomp[i]->edfhdr->recordsize);

          fseeko(signalcomp[i]->edfhdr->file_hdl, l_temp, SEEK_SET);

          if(fread(viewbuf + signalcomp[i]->viewbufoffset, readsize, 1, signalcomp[i]->edfhdr->file_hdl)!=1)
          {
            live_stream_active = 0;
            QMessageBox messagewindow(QMessageBox::Critical, "Error", "A read error occurred. 6");
            messagewindow.exec();
            remove_all_signals();
            return;
          }
        }
      }
    }

    signalcomp[i]->sample_stop += signalcomp[i]->sample_start;
  }

  if(signalcomps && (!signal_averaging_active))
  {
    if((edfheaderlist[sel_viewtime]->viewtime + edfheaderlist[sel_viewtime]->starttime_offset)>=0LL)
    {
      snprintf(viewtime_string, 32, "%2i:%02i:%02i.%04i",
              (int)((((edfheaderlist[sel_viewtime]->l_starttime + edfheaderlist[sel_viewtime]->viewtime + edfheaderlist[sel_viewtime]->starttime_offset) / TIME_DIMENSION)/ 3600LL) % 24LL),
              (int)((((edfheaderlist[sel_viewtime]->l_starttime + edfheaderlist[sel_viewtime]->viewtime + edfheaderlist[sel_viewtime]->starttime_offset) / TIME_DIMENSION) % 3600LL) / 60LL),
              (int)(((edfheaderlist[sel_viewtime]->l_starttime + edfheaderlist[sel_viewtime]->viewtime + edfheaderlist[sel_viewtime]->starttime_offset) / TIME_DIMENSION) % 60LL),
              (int)(((edfheaderlist[sel_viewtime]->l_starttime + edfheaderlist[sel_viewtime]->viewtime + edfheaderlist[sel_viewtime]->starttime_offset) % TIME_DIMENSION) / 1000LL));

      snprintf(viewtime_string + strlen(viewtime_string), 32, " (%i:%02i:%02i.%04i)",
              (int)((edfheaderlist[sel_viewtime]->viewtime / TIME_DIMENSION)/ 3600LL),
              (int)(((edfheaderlist[sel_viewtime]->viewtime / TIME_DIMENSION) % 3600LL) / 60LL),
              (int)((edfheaderlist[sel_viewtime]->viewtime / TIME_DIMENSION) % 60LL),
              (int)((edfheaderlist[sel_viewtime]->viewtime % TIME_DIMENSION) / 1000LL));
    }
    else
    {
      l_temp = edfheaderlist[sel_viewtime]->l_starttime + ((edfheaderlist[sel_viewtime]->viewtime + edfheaderlist[sel_viewtime]->starttime_offset) % (86400LL * TIME_DIMENSION));
      if(l_temp<=0)
      {
        l_temp = (86400LL * TIME_DIMENSION) + l_temp;
      }
      snprintf(viewtime_string, 32, "%2i:%02i:%02i.%04i",
              (int)((((l_temp) / TIME_DIMENSION)/ 3600LL) % 24LL),
              (int)((((l_temp) / TIME_DIMENSION) % 3600LL) / 60LL),
              (int)(((l_temp) / TIME_DIMENSION) % 60LL),
              (int)(((l_temp) % TIME_DIMENSION) / 1000LL));

      l_temp = -edfheaderlist[sel_viewtime]->viewtime;

      snprintf(viewtime_string + strlen(viewtime_string), 32, " (-%i:%02i:%02i.%04i)",
              (int)((l_temp / TIME_DIMENSION)/ 3600LL),
              (int)(((l_temp / TIME_DIMENSION) % 3600LL) / 60LL),
              (int)((l_temp / TIME_DIMENSION) % 60LL),
              (int)((l_temp % TIME_DIMENSION) / 1000LL));
    }

    if(pagetime >= (3600LL * TIME_DIMENSION))
    {
      snprintf(pagetime_string, 32, "%i:%02i:%02i.%04i",
              ((int)(pagetime / TIME_DIMENSION)) / 3600,
              (((int)(pagetime / TIME_DIMENSION)) % 3600) / 60,
              ((int)(pagetime / TIME_DIMENSION)) % 60,
              (int)((pagetime % TIME_DIMENSION) / 1000LL));
    }
    else
    {
      if(pagetime >= (60LL * TIME_DIMENSION))
      {
        snprintf(pagetime_string, 32, "%i:%02i.%04i",
                ((int)(pagetime / TIME_DIMENSION)) / 60,
                ((int)(pagetime / TIME_DIMENSION)) % 60,
                (int)((pagetime % TIME_DIMENSION) / 1000LL));
      }
      else
      {
        snprintf(pagetime_string, 32, "%i.%04i sec",
                (int)(pagetime / TIME_DIMENSION),
                (int)((pagetime % TIME_DIMENSION) / 1000LL));
      }
    }

    remove_trailing_zeros(viewtime_string);
    remove_trailing_zeros(pagetime_string);
  }

  if(!signal_averaging_active)
  {
    if(print_to_edf_active)
    {
      print_to_edf_active = 0;
    }
    else
    {
      if(signalcomps && (!live_stream_active))
      {
        positionslider->blockSignals(TRUE);

        long long record_duration = edfheaderlist[sel_viewtime]->long_data_record_duration * edfheaderlist[sel_viewtime]->datarecords;

        record_duration -= pagetime;

        if(edfheaderlist[sel_viewtime]->viewtime<=0)
        {
          positionslider->setValue(0);
        }
        else
        {
          if(edfheaderlist[sel_viewtime]->viewtime>=record_duration)
          {
            positionslider->setValue(1000000);
          }
          else
          {
            if(record_duration<pagetime)
            {
              positionslider->setValue(1000000);
            }
            else
            {
              positionslider->setValue(edfheaderlist[sel_viewtime]->viewtime * 1000000LL / record_duration);
            }
          }
        }

        slidertoolbar->setEnabled(TRUE);
      }
      else
      {
        slidertoolbar->setEnabled(FALSE);

        positionslider->blockSignals(TRUE);
      }

      maincurve->drawCurve_stage_1();

      if(signalcomps && (!live_stream_active))
      {
        positionslider->blockSignals(FALSE);
      }
    }

    if(spectrumdock->dock->isVisible())
    {
      spectrumdock->rescan();
    }
  }
}


void UI_Mainwindow::export_to_ascii()
{
  if(!files_open)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "You have to open a file first.");
    messagewindow.exec();
    return;
  }

  UI_AsciiExportwindow exportdialog(this);
}


void UI_Mainwindow::export_ecg_rr_interval_to_ascii()
{
  UI_ECGExport ECG_export(this);
}


void UI_Mainwindow::export_annotations()
{
  if(!files_open)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "You have to open a file first.");
    messagewindow.exec();
    return;
  }

  UI_ExportAnnotationswindow exportAnnotsDialog(this);
}


void UI_Mainwindow::import_annotations()
{
  UI_ImportAnnotationswindow importAnnotsDialog(this);
}


void UI_Mainwindow::check_edf_compatibility()
{
  if(!files_open)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "You have to open the file first.");
    messagewindow.exec();
    return;
  }

  UI_EDFCompatwindow edfcompatdialog(this);
}


void UI_Mainwindow::print_to_img_640x480()
{
  maincurve->print_to_image(640, 480);
}


void UI_Mainwindow::print_to_img_800x600()
{
  maincurve->print_to_image(800, 600);
}


void UI_Mainwindow::print_to_img_1024x768()
{
  maincurve->print_to_image(1024, 768);
}


void UI_Mainwindow::print_to_img_1280x1024()
{
  maincurve->print_to_image(1280, 1024);
}


void UI_Mainwindow::print_to_img_1600x1200()
{
  maincurve->print_to_image(1600, 1200);
}


long long UI_Mainwindow::get_long_time(char *str)
{
  int i, len, hasdot=0, dotposition=0;

  long long value=0, radix;

  str = str + 1;

  len = strlen(str);

  for(i=0; i<len; i++)
  {
    if(str[i]=='.')
    {
      hasdot = 1;
      dotposition = i;
      break;
    }
  }

  if(hasdot)
  {
    radix = TIME_DIMENSION;

    for(i=dotposition-1; i>=0; i--)
    {
        value += ((long long)(str[i] - 48)) * radix;
        radix *= 10;
    }

    radix = TIME_DIMENSION / 10;

    for(i=dotposition+1; i<len; i++)
    {
        value += ((long long)(str[i] - 48)) * radix;
        radix /= 10;
    }
  }
  else
  {
    radix = TIME_DIMENSION;

    for(i=len-1; i>=0; i--)
    {
        value += ((long long)(str[i] - 48)) * radix;
        radix *= 10;
    }
  }

  if(str[-1]=='-')  value = -value;

  return(value);
}


void UI_Mainwindow::get_rgbcolor_settings(struct xml_handle *xml_hdl, const char *id, int cnt, QColor *rgb_color)
{
  char *result;

  QColor tmp_color;


  if(xml_goto_nth_element_inside(xml_hdl, id, cnt))
  {
    return;
  }

  if(xml_goto_nth_element_inside(xml_hdl, "red", 0))
  {
    return;
  }
  result = xml_get_content_of_element(xml_hdl);
  if(result==NULL)
  {
    return;
  }
  tmp_color.setRed(atoi(result));
  free(result);

  xml_go_up(xml_hdl);

  if(xml_goto_nth_element_inside(xml_hdl, "green", 0))
  {
    return;
  }
  result = xml_get_content_of_element(xml_hdl);
  if(result==NULL)
  {
    return;
  }
  tmp_color.setGreen(atoi(result));
  free(result);

  xml_go_up(xml_hdl);

  if(xml_goto_nth_element_inside(xml_hdl, "blue", 0))
  {
    return;
  }
  result = xml_get_content_of_element(xml_hdl);
  if(result==NULL)
  {
    return;
  }
  tmp_color.setBlue(atoi(result));
  free(result);

  *rgb_color = tmp_color;

  xml_go_up(xml_hdl);
  xml_go_up(xml_hdl);
}


void UI_Mainwindow::read_color_settings()
{
  char cfg_path[MAX_PATH_LENGTH],
       *result;

  struct xml_handle *xml_hdl;

  cfg_path[0] = 0;

#ifdef Q_WS_X11
  strcpy(cfg_path, getenv("HOME"));
  strcat(cfg_path, "/.");
  strcat(cfg_path, PROGRAM_NAME);
  strcat(cfg_path, "/settings.xml");
#endif
#ifdef Q_WS_MAC
  strcpy(cfg_path, getenv("HOME"));
  strcat(cfg_path, "/.");
  strcat(cfg_path, PROGRAM_NAME);
  strcat(cfg_path, "/settings.xml");
#endif
#ifdef Q_WS_WIN
  strcpy(cfg_path, specialFolder(CSIDL_APPDATA).toLocal8Bit().data());
  strcat(cfg_path, "\\");
  strcat(cfg_path, PROGRAM_NAME);
  strcat(cfg_path, "\\settings.xml");
#endif

  xml_hdl = xml_get_handle(cfg_path);
  if(!xml_hdl)
  {
    return;
  }

  if(strcmp(xml_hdl->elementname, "config"))
  {
    xml_close(xml_hdl);
    return;
  }

  if(xml_goto_nth_element_inside(xml_hdl, "UI", 0))
  {
    xml_close(xml_hdl);
    return;
  }

  if(xml_goto_nth_element_inside(xml_hdl, "colors", 0))
  {
    xml_close(xml_hdl);
    return;
  }

  get_rgbcolor_settings(xml_hdl, "backgroundcolor", 0, &maincurve->backgroundcolor);

  get_rgbcolor_settings(xml_hdl, "small_ruler_color", 0, &maincurve->small_ruler_color);

  get_rgbcolor_settings(xml_hdl, "big_ruler_color", 0, &maincurve->big_ruler_color);

  get_rgbcolor_settings(xml_hdl, "mouse_rect_color", 0, &maincurve->mouse_rect_color);

  get_rgbcolor_settings(xml_hdl, "text_color", 0, &maincurve->text_color);

  get_rgbcolor_settings(xml_hdl, "baseline_color", 0, &maincurve->baseline_color);

  get_rgbcolor_settings(xml_hdl, "annot_marker_color", 0, &maincurve->annot_marker_color);

  if(xml_goto_nth_element_inside(xml_hdl, "signal_color", 0))
  {
    xml_close(xml_hdl);
    return;
  }
  result = xml_get_content_of_element(xml_hdl);
  if(result==NULL)
  {
    xml_close(xml_hdl);
    return;
  }
  maincurve->signal_color = atoi(result);
  free(result);

  xml_go_up(xml_hdl);

  if(xml_goto_nth_element_inside(xml_hdl, "floating_ruler_color", 0))
  {
    xml_close(xml_hdl);
    return;
  }
  result = xml_get_content_of_element(xml_hdl);
  if(result==NULL)
  {
    xml_close(xml_hdl);
    return;
  }
  maincurve->floating_ruler_color = atoi(result);
  free(result);

  xml_go_up(xml_hdl);

  if(xml_goto_nth_element_inside(xml_hdl, "blackwhite_printing", 0))
  {
    xml_close(xml_hdl);
    return;
  }
  result = xml_get_content_of_element(xml_hdl);
  if(result==NULL)
  {
    xml_close(xml_hdl);
    return;
  }
  maincurve->blackwhite_printing = atoi(result);
  free(result);

  xml_go_up(xml_hdl);

  if(xml_goto_nth_element_inside(xml_hdl, "show_annot_markers", 0))
  {
    xml_close(xml_hdl);
    return;
  }
  result = xml_get_content_of_element(xml_hdl);
  if(result==NULL)
  {
    xml_close(xml_hdl);
    return;
  }
  show_annot_markers = atoi(result);
  free(result);

  xml_go_up(xml_hdl);

  if(xml_goto_nth_element_inside(xml_hdl, "show_baselines", 0))
  {
    xml_close(xml_hdl);
    return;
  }
  result = xml_get_content_of_element(xml_hdl);
  if(result==NULL)
  {
    xml_close(xml_hdl);
    return;
  }
  show_baselines = atoi(result);
  free(result);

  xml_go_up(xml_hdl);

  if(xml_goto_nth_element_inside(xml_hdl, "crosshair_1_color", 0))
  {
    xml_close(xml_hdl);
    return;
  }
  result = xml_get_content_of_element(xml_hdl);
  if(result==NULL)
  {
    xml_close(xml_hdl);
    return;
  }
  maincurve->crosshair_1.color = atoi(result);
  free(result);

  xml_go_up(xml_hdl);

  if(xml_goto_nth_element_inside(xml_hdl, "crosshair_2_color", 0))
  {
    xml_close(xml_hdl);
    return;
  }
  result = xml_get_content_of_element(xml_hdl);
  if(result==NULL)
  {
    xml_close(xml_hdl);
    return;
  }
  maincurve->crosshair_2.color = atoi(result);
  free(result);

  xml_close(xml_hdl);
}



void UI_Mainwindow::read_recent_file_settings()
{
  int i;

  char cfg_path[MAX_PATH_LENGTH],
       *result;

  struct xml_handle *xml_hdl;

  cfg_path[0] = 0;

#ifdef Q_WS_X11
  strcpy(cfg_path, getenv("HOME"));
  strcat(cfg_path, "/.");
  strcat(cfg_path, PROGRAM_NAME);
  strcat(cfg_path, "/settings.xml");
#endif
#ifdef Q_WS_MAC
  strcpy(cfg_path, getenv("HOME"));
  strcat(cfg_path, "/.");
  strcat(cfg_path, PROGRAM_NAME);
  strcat(cfg_path, "/settings.xml");
#endif
#ifdef Q_WS_WIN
  strcpy(cfg_path, specialFolder(CSIDL_APPDATA).toLocal8Bit().data());
  strcat(cfg_path, "\\");
  strcat(cfg_path, PROGRAM_NAME);
  strcat(cfg_path, "\\settings.xml");
#endif

  xml_hdl = xml_get_handle(cfg_path);
  if(xml_hdl==NULL)
  {
    return;
  }

  if(strcmp(xml_hdl->elementname, "config"))
  {
    xml_close(xml_hdl);
    return;
  }

  if(xml_goto_nth_element_inside(xml_hdl, "UI", 0))
  {
    xml_close(xml_hdl);
    return;
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "recent_montagedir", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    strncpy(recent_montagedir, result, MAX_PATH_LENGTH);
    recent_montagedir[MAX_PATH_LENGTH - 1] = 0;
    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "recent_savedir", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    strncpy(recent_savedir, result, MAX_PATH_LENGTH);
    recent_savedir[MAX_PATH_LENGTH - 1] = 0;
    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "recent_opendir", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    strncpy(recent_opendir, result, MAX_PATH_LENGTH);
    recent_opendir[MAX_PATH_LENGTH - 1] = 0;
    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "recent_file", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }
    if(result[0] != 0)
    {
      strncpy(&recent_file_path[0][0], result, MAX_PATH_LENGTH);
      recent_file_path[0][MAX_PATH_LENGTH - 1] = 0;
      recent_filesmenu->addAction(QString::fromLocal8Bit(&recent_file_path[0][0]));
      free(result);

      for(i=1; i<MAX_RECENTFILES; i++)
      {
        if(xml_goto_next_element_with_same_name(xml_hdl))
        {
          break;
        }
        result = xml_get_content_of_element(xml_hdl);
        if(result==NULL)
        {
          xml_close(xml_hdl);
          return;
        }
        if(result[0] == 0)
        {
          free(result);
          break;
        }
        strncpy(&recent_file_path[i][0], result, MAX_PATH_LENGTH);
        recent_file_path[i][MAX_PATH_LENGTH - 1] = 0;
        recent_filesmenu->addAction(QString::fromLocal8Bit(&recent_file_path[i][0]));
        free(result);
      }
    }
    else
    {
      free(result);
    }

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "recent_file_mtg", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }
    strncpy(&recent_file_mtg_path[0][0], result, MAX_PATH_LENGTH);
    recent_file_mtg_path[0][MAX_PATH_LENGTH - 1] = 0;
    free(result);
    for(i=1; i<MAX_RECENTFILES; i++)
    {
      if(xml_goto_next_element_with_same_name(xml_hdl))
      {
        break;
      }
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }
      strncpy(&recent_file_mtg_path[i][0], result, MAX_PATH_LENGTH);
      recent_file_mtg_path[i][MAX_PATH_LENGTH - 1] = 0;
      free(result);
    }

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "predefined_mtg_path", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }
    strncpy(&predefined_mtg_path[0][0], result, MAX_PATH_LENGTH);
    predefined_mtg_path[0][MAX_PATH_LENGTH - 1] = 0;
    free(result);

    for(i=1; i < MAXPREDEFINEDMONTAGES; i++)
    {
      if(xml_goto_next_element_with_same_name(xml_hdl))
      {
        break;
      }
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }
      strncpy(&predefined_mtg_path[i][0], result, MAX_PATH_LENGTH);
      predefined_mtg_path[i][MAX_PATH_LENGTH - 1] = 0;
      free(result);
    }

    xml_go_up(xml_hdl);
  }

  xml_close(xml_hdl);
}



void UI_Mainwindow::read_general_settings()
{
  int i;

  char cfg_path[MAX_PATH_LENGTH],
       *result;

  struct xml_handle *xml_hdl;

  cfg_path[0] = 0;

#ifdef Q_WS_X11
  strcpy(cfg_path, getenv("HOME"));
  strcat(cfg_path, "/.");
  strcat(cfg_path, PROGRAM_NAME);
  strcat(cfg_path, "/settings.xml");
#endif
#ifdef Q_WS_MAC
  strcpy(cfg_path, getenv("HOME"));
  strcat(cfg_path, "/.");
  strcat(cfg_path, PROGRAM_NAME);
  strcat(cfg_path, "/settings.xml");
#endif
#ifdef Q_WS_WIN
  strcpy(cfg_path, specialFolder(CSIDL_APPDATA).toLocal8Bit().data());
  strcat(cfg_path, "\\");
  strcat(cfg_path, PROGRAM_NAME);
  strcat(cfg_path, "\\settings.xml");
#endif

  xml_hdl = xml_get_handle(cfg_path);
  if(xml_hdl==NULL)
  {
    return;
  }

  if(strcmp(xml_hdl->elementname, "config"))
  {
    xml_close(xml_hdl);
    return;
  }

  if(!xml_goto_nth_element_inside(xml_hdl, "cfg_app_version", 0))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    strncpy(cfg_app_version, result, 16);
    cfg_app_version[16] = 0;

    free(result);

    xml_go_up(xml_hdl);
  }

  if(xml_goto_nth_element_inside(xml_hdl, "UI", 0))
  {
    xml_close(xml_hdl);
    return;
  }

  if(xml_goto_nth_element_inside(xml_hdl, "pixelsizefactor", 0))
  {
    xml_close(xml_hdl);
    return;
  }

  result = xml_get_content_of_element(xml_hdl);
  if(result==NULL)
  {
    xml_close(xml_hdl);
    return;
  }

  pixelsizefactor = atof(result);
  if(pixelsizefactor < 0.00001)
  {
    pixelsizefactor = 0.0294382;
  }
  free(result);

  xml_go_up(xml_hdl);

  if(!(xml_goto_nth_element_inside(xml_hdl, "auto_dpi", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    auto_dpi = atoi(result);
    if((auto_dpi < 0) || (auto_dpi > 1))
    {
      auto_dpi = 1;
    }
    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "x_pixelsizefactor", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    x_pixelsizefactor = atof(result);
    if(x_pixelsizefactor < 0.00001)
    {
      x_pixelsizefactor = 0.0294382;
    }
    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "clip_to_pane", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    clip_to_pane = atoi(result);
    if((clip_to_pane < 0) || (clip_to_pane > 1))
    {
      clip_to_pane = 0;
    }
    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "auto_reload_mtg", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    auto_reload_mtg = atoi(result);
    if((auto_reload_mtg < 0) || (auto_reload_mtg > 1))
    {
      auto_reload_mtg = 1;
    }
    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "read_biosemi_status_signal", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    read_biosemi_status_signal = atoi(result);
    if((read_biosemi_status_signal < 0) || (read_biosemi_status_signal > 1))
    {
      read_biosemi_status_signal = 1;
    }
    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "read_nk_trigger_signal", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    read_nk_trigger_signal = atoi(result);
    if((read_nk_trigger_signal < 0) || (read_nk_trigger_signal > 1))
    {
      read_nk_trigger_signal = 1;
    }
    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "maxfilesize_to_readin_annotations", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    maxfilesize_to_readin_annotations = atoll(result);
    if((maxfilesize_to_readin_annotations < 104857600LL) || (maxfilesize_to_readin_annotations > 10485760000LL))
    {
      maxfilesize_to_readin_annotations = 10485760000LL;
    }
    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "use_threads", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    use_threads = atoi(result);
    if((use_threads < 0) || (use_threads > 1))
    {
      use_threads = 1;
    }
    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "spectrummarkerblock", 0)))
  {
    if(xml_goto_nth_element_inside(xml_hdl, "items", 0))
    {
      xml_close(xml_hdl);
      return;
    }

    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    if((atoi(result) > MAXSPECTRUMMARKERS) || (atoi(result) < 0))
    {
      xml_close(xml_hdl);
      free(result);
      return;
    }

    spectrum_colorbar->items = atoi(result);
    free(result);

    xml_go_up(xml_hdl);

    if(xml_goto_nth_element_inside(xml_hdl, "method", 0))
    {
      xml_close(xml_hdl);
      return;
    }

    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    if((atoi(result) > 2) || (atoi(result) < 0))
    {
      xml_close(xml_hdl);
      free(result);
      return;
    }

    spectrum_colorbar->method = atoi(result);
    free(result);

    xml_go_up(xml_hdl);
    if(xml_goto_nth_element_inside(xml_hdl, "frequency", 0))
    {
      xml_close(xml_hdl);
      return;
    }

    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    if((atof(result) > 1000000.0) || (atof(result) < 0.00001))
    {
      xml_close(xml_hdl);
      free(result);
      return;
    }

    spectrum_colorbar->freq[0] = atof(result);
    free(result);

    for(i=1; i < spectrum_colorbar->items; i++)
    {
      if(xml_goto_next_element_with_same_name(xml_hdl))
      {
        xml_close(xml_hdl);
        return;
      }

      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      if(i < spectrum_colorbar->items)
      {
        if((atof(result) > 1000000.0) || (atof(result) <= spectrum_colorbar->freq[i-1]))
        {
          xml_close(xml_hdl);
          free(result);
          return;
        }
      }

      spectrum_colorbar->freq[i] = atof(result);
      free(result);
    }

    xml_go_up(xml_hdl);

    if(xml_goto_nth_element_inside(xml_hdl, "color", 0))
    {
      xml_close(xml_hdl);
      return;
    }

    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    if((atoi(result) > 18) || (atoi(result) < 2))
    {
      xml_close(xml_hdl);
      free(result);
      return;
    }

    spectrum_colorbar->color[0] = atoi(result);
    free(result);

    for(i=1; i < spectrum_colorbar->items; i++)
    {
      if(xml_goto_next_element_with_same_name(xml_hdl))
      {
        xml_close(xml_hdl);
        return;
      }

      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      if((atoi(result) > 18) || (atoi(result) < 2))
      {
        xml_close(xml_hdl);
        free(result);
        return;
      }

      spectrum_colorbar->color[i] = atoi(result);
      free(result);
    }

    xml_go_up(xml_hdl);

    if(!(xml_goto_nth_element_inside(xml_hdl, "label", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      strncpy(spectrum_colorbar->label[0], result, 16);
      spectrum_colorbar->label[0][16] = 0;
      free(result);

      for(i=1; i < spectrum_colorbar->items; i++)
      {
        if(xml_goto_next_element_with_same_name(xml_hdl))
        {
          xml_close(xml_hdl);
          return;
        }

        result = xml_get_content_of_element(xml_hdl);
        if(result==NULL)
        {
          xml_close(xml_hdl);
          return;
        }

        strncpy(spectrum_colorbar->label[i], result, 16);
        spectrum_colorbar->label[i][16] = 0;
        free(result);
      }

      xml_go_up(xml_hdl);
    }
  }

  xml_go_up(xml_hdl);

  if(!(xml_goto_nth_element_inside(xml_hdl, "maxdftblocksize", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    if(atoi(result) < 10)
    {
      xml_close(xml_hdl);
      free(result);
      return;
    }

    maxdftblocksize = atoi(result);

    free(result);
  }

  xml_go_up(xml_hdl);

  if(!(xml_goto_nth_element_inside(xml_hdl, "annotations_import_var", 0)))
  {
    if(!(xml_goto_nth_element_inside(xml_hdl, "format", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      import_annotations_var->format = atoi(result);

      if((import_annotations_var->format < 0) || (import_annotations_var->format > 3))
      {
        import_annotations_var->format = 1;
      }

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "onsettimeformat", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      import_annotations_var->onsettimeformat = atoi(result);

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "onsetcolumn", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      import_annotations_var->onsetcolumn = atoi(result);

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "descriptioncolumn", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      import_annotations_var->descriptioncolumn = atoi(result);

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "useduration", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      import_annotations_var->useduration = atoi(result);

      if(import_annotations_var->useduration != 1)
      {
        import_annotations_var->useduration = 0;
      }

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "durationcolumn", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      import_annotations_var->durationcolumn = atoi(result);

      if(import_annotations_var->durationcolumn < 0)
      {
        import_annotations_var->durationcolumn = 0;
      }

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "datastartline", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      import_annotations_var->datastartline = atoi(result);

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "separator", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      strncpy(import_annotations_var->separator, result, 3);

      import_annotations_var->separator[3] = 0;

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "dceventbittime", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      import_annotations_var->dceventbittime = atoi(result);

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "triggerlevel", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      import_annotations_var->triggerlevel = atof(result);

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "manualdescription", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      import_annotations_var->manualdescription = atoi(result);

      if((import_annotations_var->manualdescription < 0) || (import_annotations_var->manualdescription > 1))
      {
        import_annotations_var->manualdescription = 0;
      }

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "description", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      strncpy(import_annotations_var->description, result, 20);

      import_annotations_var->description[20] = 0;

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "ignoreconsecutive", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      import_annotations_var->ignoreconsecutive = atoi(result);

      if(import_annotations_var->ignoreconsecutive != 1)
      {
        import_annotations_var->ignoreconsecutive = 0;
      }

      free(result);

      xml_go_up(xml_hdl);
    }

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "annotations_export_var", 0)))
  {
    if(!(xml_goto_nth_element_inside(xml_hdl, "separator", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      export_annotations_var->separator = atoi(result);

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "format", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      export_annotations_var->format = atoi(result);

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "duration", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      export_annotations_var->duration = atoi(result);

      free(result);

      xml_go_up(xml_hdl);
    }

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "live_stream_update_interval", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    live_stream_update_interval = atoi(result);
    if((live_stream_update_interval < 100) || (live_stream_update_interval > 3000))
    {
      live_stream_update_interval = 500;
    }

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "powerlinefreq", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    powerlinefreq = atoi(result);
    if((powerlinefreq != 50) && (powerlinefreq != 60))
    {
      powerlinefreq = 50;
    }

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "mousewheelsens", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    mousewheelsens = atoi(result);
    if((mousewheelsens < 0) || (mousewheelsens > 100))
    {
      mousewheelsens = 10;
    }

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "average_period", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    average_period = atof(result);

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "average_ratio", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    average_ratio = atoi(result);
    if((average_ratio < 0) || (average_ratio > 2))
    {
      average_ratio = 0;
    }

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "average_upsidedown", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    if(atoi(result) == 1)
    {
      average_upsidedown = 1;
    }
    else
    {
      average_upsidedown = 0;
    }

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "average_bw", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    if(atoi(result) == 1)
    {
      average_bw = 1;
    }
    else
    {
      average_bw = 0;
    }

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "spectrum_bw", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    if(atoi(result) == 1)
    {
      spectrum_bw = 1;
    }
    else
    {
      spectrum_bw = 0;
    }

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "spectrum_sqrt", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    if(atoi(result) == 1)
    {
      spectrum_sqrt = 1;
    }
    else
    {
      spectrum_sqrt = 0;
    }

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "z_score_var.zscore_page_len", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    z_score_var.zscore_page_len = atoi(result);

    if((z_score_var.zscore_page_len < 10) || (z_score_var.zscore_page_len > 60))
    {
      z_score_var.zscore_page_len = 30;
    }

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "z_score_var.z_threshold", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    z_score_var.z_threshold = atof(result);

    if((z_score_var.z_threshold < -0.5) || (z_score_var.z_threshold > 0.5))
    {
      z_score_var.z_threshold = 0.0;
    }

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "z_score_var.crossoverfreq", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    z_score_var.crossoverfreq = atof(result);

    if((z_score_var.crossoverfreq < 5.0) || (z_score_var.crossoverfreq > 9.5))
    {
      z_score_var.crossoverfreq = 7.5;
    }

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "z_score_var.z_hysteresis", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    z_score_var.z_hysteresis = atof(result);

    if((z_score_var.z_hysteresis < 0.0) || (z_score_var.z_hysteresis > 0.25))
    {
      z_score_var.z_hysteresis = 0.0;
    }

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "z_score_var.zscore_error_detection", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    z_score_var.zscore_error_detection = atoi(result);

    if((z_score_var.zscore_error_detection < 50) || (z_score_var.zscore_error_detection > 100))
    {
      z_score_var.zscore_error_detection = 50;
    }

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "raw2edf_var", 0)))
  {
    if(!(xml_goto_nth_element_inside(xml_hdl, "sf", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      raw2edf_var.sf = atoi(result);
      if(raw2edf_var.sf < 1)  raw2edf_var.sf = 1;
      if(raw2edf_var.sf > 1000000)  raw2edf_var.sf = 1000000;

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "chns", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      raw2edf_var.chns = atoi(result);
      if(raw2edf_var.chns < 1)  raw2edf_var.chns = 1;
      if(raw2edf_var.chns > 256)  raw2edf_var.chns = 256;

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "phys_max", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      raw2edf_var.phys_max = atoi(result);
      if(raw2edf_var.phys_max < 1)  raw2edf_var.phys_max = 1;
      if(raw2edf_var.phys_max > 10000000)  raw2edf_var.phys_max = 10000000;

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "straightbinary", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      raw2edf_var.straightbinary = atoi(result);
      if(raw2edf_var.straightbinary < 0)  raw2edf_var.straightbinary = 0;
      if(raw2edf_var.straightbinary > 1)  raw2edf_var.straightbinary = 1;

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "samplesize", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      raw2edf_var.samplesize = atoi(result);
      if(raw2edf_var.samplesize < 1)  raw2edf_var.samplesize = 1;
      if(raw2edf_var.samplesize > 2)  raw2edf_var.samplesize = 2;

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "offset", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      raw2edf_var.offset = atoi(result);
      if(raw2edf_var.offset < 0)  raw2edf_var.offset = 0;
      if(raw2edf_var.offset > 1000000)  raw2edf_var.offset = 1000000;

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "skipblocksize", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      raw2edf_var.skipblocksize = atoi(result);
      if(raw2edf_var.skipblocksize < 0)  raw2edf_var.skipblocksize = 0;
      if(raw2edf_var.skipblocksize > 1000000)  raw2edf_var.skipblocksize = 1000000;

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "skipbytes", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      raw2edf_var.skipbytes = atoi(result);
      if(raw2edf_var.skipbytes < 1)  raw2edf_var.skipbytes = 1;
      if(raw2edf_var.skipbytes > 1000000)  raw2edf_var.skipbytes = 1000000;

      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "phys_dim", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result==NULL)
      {
        xml_close(xml_hdl);
        return;
      }

      strncpy(raw2edf_var.phys_dim, result, 16);
      raw2edf_var.phys_dim[15] = 0;
      latin1_to_ascii(raw2edf_var.phys_dim, 16);
      remove_leading_spaces(raw2edf_var.phys_dim);
      remove_trailing_spaces(raw2edf_var.phys_dim);

      free(result);

      xml_go_up(xml_hdl);
    }

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "check_for_updates", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    check_for_updates = atoi(result);
    if((check_for_updates < 0) || (check_for_updates > 1))  check_for_updates = 1;

    free(result);

    xml_go_up(xml_hdl);
  }

  xml_close(xml_hdl);
}



void UI_Mainwindow::recent_file_action_func(QAction *action)
{
  strcpy(path, action->text().toLocal8Bit().data());

  cmdlineargument = 1;

  open_new_file();
}



void UI_Mainwindow::write_settings()
{
  int i, len;

  char cfg_path[MAX_PATH_LENGTH],
       str[1024];

  FILE *cfgfile;

  cfg_path[0] = 0;

#ifdef Q_WS_X11
  strcpy(cfg_path, getenv("HOME"));
  strcat(cfg_path, "/.");
  strcat(cfg_path, PROGRAM_NAME);
  mkdir(cfg_path,  S_IRWXU);
  strcat(cfg_path, "/settings.xml");
#endif
#ifdef Q_WS_MAC
  strcpy(cfg_path, getenv("HOME"));
  strcat(cfg_path, "/.");
  strcat(cfg_path, PROGRAM_NAME);
  mkdir(cfg_path,  S_IRWXU);
  strcat(cfg_path, "/settings.xml");
#endif
#ifdef Q_WS_WIN
  strcpy(cfg_path, specialFolder(CSIDL_APPDATA).toLocal8Bit().data());
  strcat(cfg_path, "\\");
  strcat(cfg_path, PROGRAM_NAME);
  _mkdir(cfg_path);
  strcat(cfg_path, "\\settings.xml");
#endif

  cfgfile = fopeno(cfg_path, "wb");
  if(cfgfile)
  {
    fprintf(cfgfile, "<?xml version=\"1.0\"?>\n"
                     "<config>\n"
                     "  <cfg_app_version>" PROGRAM_NAME " " PROGRAM_VERSION "</cfg_app_version>\n"
                     "  <UI>\n"
                     "    <colors>\n");

    fprintf(cfgfile, "      <backgroundcolor>\n"
                    "        <red>%i</red>\n"
                    "        <green>%i</green>\n"
                    "        <blue>%i</blue>\n"
                    "      </backgroundcolor>\n",
                    maincurve->backgroundcolor.red(),
                    maincurve->backgroundcolor.green(),
                    maincurve->backgroundcolor.blue());

    fprintf(cfgfile, "      <small_ruler_color>\n"
                    "        <red>%i</red>\n"
                    "        <green>%i</green>\n"
                    "        <blue>%i</blue>\n"
                    "      </small_ruler_color>\n",
                    maincurve->small_ruler_color.red(),
                    maincurve->small_ruler_color.green(),
                    maincurve->small_ruler_color.blue());

    fprintf(cfgfile, "      <big_ruler_color>\n"
                    "        <red>%i</red>\n"
                    "        <green>%i</green>\n"
                    "        <blue>%i</blue>\n"
                    "      </big_ruler_color>\n",
                    maincurve->big_ruler_color.red(),
                    maincurve->big_ruler_color.green(),
                    maincurve->big_ruler_color.blue());

    fprintf(cfgfile, "      <mouse_rect_color>\n"
                    "        <red>%i</red>\n"
                    "        <green>%i</green>\n"
                    "        <blue>%i</blue>\n"
                    "      </mouse_rect_color>\n",
                    maincurve->mouse_rect_color.red(),
                    maincurve->mouse_rect_color.green(),
                    maincurve->mouse_rect_color.blue());

    fprintf(cfgfile, "      <text_color>\n"
                    "        <red>%i</red>\n"
                    "        <green>%i</green>\n"
                    "        <blue>%i</blue>\n"
                    "      </text_color>\n",
                    maincurve->text_color.red(),
                    maincurve->text_color.green(),
                    maincurve->text_color.blue());

    fprintf(cfgfile, "      <baseline_color>\n"
                    "        <red>%i</red>\n"
                    "        <green>%i</green>\n"
                    "        <blue>%i</blue>\n"
                    "      </baseline_color>\n",
                    maincurve->baseline_color.red(),
                    maincurve->baseline_color.green(),
                    maincurve->baseline_color.blue());

    fprintf(cfgfile, "      <annot_marker_color>\n"
                    "        <red>%i</red>\n"
                    "        <green>%i</green>\n"
                    "        <blue>%i</blue>\n"
                    "      </annot_marker_color>\n",
                    maincurve->annot_marker_color.red(),
                    maincurve->annot_marker_color.green(),
                    maincurve->annot_marker_color.blue());

    fprintf(cfgfile, "      <signal_color>%i</signal_color>\n",
                    maincurve->signal_color);

    fprintf(cfgfile, "      <crosshair_1_color>%i</crosshair_1_color>\n",
                    maincurve->crosshair_1.color);

    fprintf(cfgfile, "      <crosshair_2_color>%i</crosshair_2_color>\n",
                    maincurve->crosshair_2.color);

    fprintf(cfgfile, "      <floating_ruler_color>%i</floating_ruler_color>\n",
                    maincurve->floating_ruler_color);

    fprintf(cfgfile, "      <blackwhite_printing>%i</blackwhite_printing>\n",
                    maincurve->blackwhite_printing);

    fprintf(cfgfile, "      <show_annot_markers>%i</show_annot_markers>\n",
                    show_annot_markers);

    fprintf(cfgfile, "      <show_baselines>%i</show_baselines>\n",
                    show_baselines);

    fprintf(cfgfile, "      <clip_to_pane>%i</clip_to_pane>\n",
                    clip_to_pane);

    fprintf(cfgfile, "      <auto_reload_mtg>%i</auto_reload_mtg>\n",
                    auto_reload_mtg);

    fprintf(cfgfile, "      <read_biosemi_status_signal>%i</read_biosemi_status_signal>\n",
                    read_biosemi_status_signal);

    fprintf(cfgfile, "      <read_nk_trigger_signal>%i</read_nk_trigger_signal>\n",
                    read_nk_trigger_signal);

    fprintf(cfgfile, "      <use_threads>%i</use_threads>\n",
                    use_threads);

#ifdef Q_WS_WIN
    fprintf(cfgfile, "      <maxfilesize_to_readin_annotations>%I64d</maxfilesize_to_readin_annotations>\n",
                    maxfilesize_to_readin_annotations);
#else
    fprintf(cfgfile, "      <maxfilesize_to_readin_annotations>%lli</maxfilesize_to_readin_annotations>\n",
                    maxfilesize_to_readin_annotations);
#endif

    len = strlen(path);
    for(i=len-1; i>=0; i--)
    {
      if((path[i] == '/')||(path[i] == '\\'))
      {
        break;
      }
    }
    path[i+1] = 0;

    fprintf(cfgfile, "    </colors>\n    <pixelsizefactor>%.10f</pixelsizefactor>\n    <auto_dpi>%i</auto_dpi>\n    <x_pixelsizefactor>%.10f</x_pixelsizefactor>\n    <recent_dir>",
                     pixelsizefactor, auto_dpi, x_pixelsizefactor);

    xml_fwrite_encode_entity(cfgfile, path);

    fprintf(cfgfile, "</recent_dir>\n");

    for(i=0; i<MAX_RECENTFILES; i++)
    {
      fprintf(cfgfile, "    <recent_file>");

      xml_fwrite_encode_entity(cfgfile, &recent_file_path[i][0]);

      fprintf(cfgfile, "</recent_file>\n");
    }

    for(i=0; i<MAX_RECENTFILES; i++)
    {
      fprintf(cfgfile, "    <recent_file_mtg>");

      xml_fwrite_encode_entity(cfgfile, &recent_file_mtg_path[i][0]);

      fprintf(cfgfile, "</recent_file_mtg>\n");
    }

    fprintf(cfgfile, "    <recent_montagedir>");

    xml_fwrite_encode_entity(cfgfile, recent_montagedir);

    fprintf(cfgfile, "</recent_montagedir>\n");

    fprintf(cfgfile, "    <recent_savedir>");

    xml_fwrite_encode_entity(cfgfile, recent_savedir);

    fprintf(cfgfile, "</recent_savedir>\n");

    fprintf(cfgfile, "    <recent_opendir>");

    xml_fwrite_encode_entity(cfgfile, recent_opendir);

    fprintf(cfgfile, "</recent_opendir>\n");

    for(i=0; i < MAXPREDEFINEDMONTAGES; i++)
    {
      fprintf(cfgfile, "    <predefined_mtg_path>");

      xml_fwrite_encode_entity(cfgfile, &predefined_mtg_path[i][0]);

      fprintf(cfgfile, "</predefined_mtg_path>\n");
    }

    fprintf(cfgfile, "    <spectrummarkerblock>\n");

    fprintf(cfgfile, "      <items>%i</items>\n", spectrum_colorbar->items);

    for(i=0; i < MAXSPECTRUMMARKERS; i++)
    {
      fprintf(cfgfile, "      <frequency>%f</frequency>\n", spectrum_colorbar->freq[i]);
    }

    for(i=0; i < MAXSPECTRUMMARKERS; i++)
    {
      fprintf(cfgfile, "      <color>%i</color>\n", spectrum_colorbar->color[i]);
    }

    for(i=0; i < MAXSPECTRUMMARKERS; i++)
    {
      fprintf(cfgfile, "      <label>");

      xml_fwrite_encode_entity(cfgfile, spectrum_colorbar->label[i]);

      fprintf(cfgfile, "</label>\n");
    }

    fprintf(cfgfile, "      <method>%i</method>\n", spectrum_colorbar->method);

    fprintf(cfgfile, "    </spectrummarkerblock>\n");

    fprintf(cfgfile, "    <maxdftblocksize>%i</maxdftblocksize>\n", maxdftblocksize);

    fprintf(cfgfile, "    <annotations_import_var>\n");

    fprintf(cfgfile, "      <format>%i</format>\n", import_annotations_var->format);

    fprintf(cfgfile, "      <onsettimeformat>%i</onsettimeformat>\n", import_annotations_var->onsettimeformat);

    fprintf(cfgfile, "      <onsetcolumn>%i</onsetcolumn>\n", import_annotations_var->onsetcolumn);

    fprintf(cfgfile, "      <descriptioncolumn>%i</descriptioncolumn>\n", import_annotations_var->descriptioncolumn);

    fprintf(cfgfile, "      <useduration>%i</useduration>\n", import_annotations_var->useduration);

    fprintf(cfgfile, "      <durationcolumn>%i</durationcolumn>\n", import_annotations_var->durationcolumn);

    fprintf(cfgfile, "      <datastartline>%i</datastartline>\n", import_annotations_var->datastartline);

    xml_strcpy_encode_entity(str, import_annotations_var->separator);

    fprintf(cfgfile, "      <separator>%s</separator>\n", str);

    fprintf(cfgfile, "      <dceventbittime>%i</dceventbittime>\n", import_annotations_var->dceventbittime);

    fprintf(cfgfile, "      <triggerlevel>%.12f</triggerlevel>\n", import_annotations_var->triggerlevel);

    fprintf(cfgfile, "      <manualdescription>%i</manualdescription>\n", import_annotations_var->manualdescription);

    xml_strcpy_encode_entity(str, import_annotations_var->description);

    fprintf(cfgfile, "      <description>%s</description>\n", str);

    fprintf(cfgfile, "      <ignoreconsecutive>%i</ignoreconsecutive>\n", import_annotations_var->ignoreconsecutive);

    fprintf(cfgfile, "    </annotations_import_var>\n");

    fprintf(cfgfile, "    <annotations_export_var>\n");

    fprintf(cfgfile, "      <separator>%i</separator>\n", export_annotations_var->separator);

    fprintf(cfgfile, "      <format>%i</format>\n", export_annotations_var->format);

    fprintf(cfgfile, "      <duration>%i</duration>\n", export_annotations_var->duration);

    fprintf(cfgfile, "    </annotations_export_var>\n");

    fprintf(cfgfile, "    <live_stream_update_interval>%i</live_stream_update_interval>\n", live_stream_update_interval);

    fprintf(cfgfile, "    <powerlinefreq>%i</powerlinefreq>\n", powerlinefreq);

    fprintf(cfgfile, "    <mousewheelsens>%i</mousewheelsens>\n", mousewheelsens);

    fprintf(cfgfile, "    <average_period>%f</average_period>\n", average_period);

    fprintf(cfgfile, "    <average_ratio>%i</average_ratio>\n", average_ratio);

    fprintf(cfgfile, "    <average_upsidedown>%i</average_upsidedown>\n", average_upsidedown);

    fprintf(cfgfile, "    <average_bw>%i</average_bw>\n", average_bw);

    fprintf(cfgfile, "    <spectrum_bw>%i</spectrum_bw>\n", spectrum_bw);

    fprintf(cfgfile, "    <spectrum_sqrt>%i</spectrum_sqrt>\n", spectrum_sqrt);

    fprintf(cfgfile, "    <z_score_var.crossoverfreq>%f</z_score_var.crossoverfreq>\n", z_score_var.crossoverfreq);

    fprintf(cfgfile, "    <z_score_var.z_threshold>%f</z_score_var.z_threshold>\n", z_score_var.z_threshold);

    fprintf(cfgfile, "    <z_score_var.zscore_page_len>%i</z_score_var.zscore_page_len>\n", z_score_var.zscore_page_len);

    fprintf(cfgfile, "    <z_score_var.zscore_error_detection>%i</z_score_var.zscore_error_detection>\n", z_score_var.zscore_error_detection);

    fprintf(cfgfile, "    <z_score_var.z_hysteresis>%f</z_score_var.z_hysteresis>\n", z_score_var.z_hysteresis);

    fprintf(cfgfile, "    <raw2edf_var>\n");

    fprintf(cfgfile, "      <sf>%i</sf>\n", raw2edf_var.sf);

    fprintf(cfgfile, "      <chns>%i</chns>\n", raw2edf_var.chns);

    fprintf(cfgfile, "      <phys_max>%i</phys_max>\n", raw2edf_var.phys_max);

    fprintf(cfgfile, "      <straightbinary>%i</straightbinary>\n", raw2edf_var.straightbinary);

    fprintf(cfgfile, "      <samplesize>%i</samplesize>\n", raw2edf_var.samplesize);

    fprintf(cfgfile, "      <offset>%i</offset>\n", raw2edf_var.offset);

    fprintf(cfgfile, "      <skipblocksize>%i</skipblocksize>\n", raw2edf_var.skipblocksize);

    fprintf(cfgfile, "      <skipbytes>%i</skipbytes>\n", raw2edf_var.skipbytes);

    xml_strcpy_encode_entity(str, raw2edf_var.phys_dim);

    fprintf(cfgfile, "      <phys_dim>%s</phys_dim>\n", str);

    fprintf(cfgfile, "    </raw2edf_var>\n");

    fprintf(cfgfile, "    <check_for_updates>%i</check_for_updates>\n", check_for_updates);

    fprintf(cfgfile, "  </UI>\n</config>\n");

    fclose(cfgfile);
  }
}




void UI_Mainwindow::save_montage()
{
  if(!files_open)  return;

  if(!signalcomps)  return;

  UI_SaveMontagewindow save_mtg(this);
}




void UI_Mainwindow::load_montage()
{
  if(!files_open)  return;

  UI_LoadMontagewindow load_mtg(this);
}



void UI_Mainwindow::view_montage()
{
  UI_ViewMontagewindow view_mtg(this);
}



void UI_Mainwindow::show_this_montage()
{
  UI_ShowActualMontagewindow show_mtg(this);
}


void UI_Mainwindow::print_to_edf()
{
  print_screen_to_edf(this);
}


void UI_Mainwindow::print_to_bdf()
{
  print_screen_to_bdf(this);
}


void UI_Mainwindow::set_dc_offset_to_zero()
{
  int i;

  if(!signalcomps)
  {
    return;
  }

  for(i=0; i<signalcomps; i++)
  {
    signalcomp[i]->screen_offset = 0.0;
  }

  maincurve->drawCurve_stage_1();
}


void UI_Mainwindow::show_help()
{
#ifdef Q_WS_X11
  QDesktopServices::openUrl(QUrl("file:///usr/share/doc/edfbrowser/manual.html"));
#endif

#ifdef Q_WS_WIN
  char path[MAX_PATH_LENGTH];

  strcpy(path, "file:///");
  strcat(path, specialFolder(CSIDL_PROGRAM_FILES).toLocal8Bit().data());
  strcat(path, "\\EDFbrowser\\manual.html");
  QDesktopServices::openUrl(QUrl(path));
#endif
}



void UI_Mainwindow::show_kb_shortcuts()
{
  UI_Messagewindow popuperror("Keyboard shortcuts",
   "PgDn\tnext page\n"
   "PgUp\tformer page\n"
   "Right Arrow\tshift right one tenth of pagetime\n"
   "Left Arrow\tshift left one tenth of pagetime\n"
   "Up Arrow\tshift up\n"
   "Down Arrow\tshift down\n"
   "Ctrl-Home\tgo to start of file\n"
   "Ctrl-End\tgo to end of file\n"
   "Ctrl++\tzoom in\n"
   "Ctrl+-\tzoom out\n"
   "F1 - F8\tload predefined montage\n"
   "Esc\tremove crosshairs or floating ruler\n"

   "\nafter zooming in by dragging a rectangle:\n"
   "Backspace\tzoom back\n"
   "Insert\tzoom in\n"
#ifdef Q_WS_WIN
   "\nCtrl+O\tOpen a file\n"
   "Ctrl+F4\tClose all files\n"
   "Alt+F4\tExit program\n"
#else
   "\nCtrl+O\tOpen a file\n"
   "Ctrl+W\tClose all files\n"
   "Ctrl+Q\tExit program\n"
#endif
   "\nMousewheel\tshift left or right\n"
   "Ctrl+Mousewheel\tzoom in or out");
}



#ifdef Q_WS_WIN

QString UI_Mainwindow::specialFolder(int type)
{
    QString result;

    QLibrary library(QLatin1String("shell32"));
    QT_WA( {
        typedef BOOL (WINAPI*GetSpecialFolderPath)(HWND, LPTSTR, int, BOOL);
        GetSpecialFolderPath SHGetSpecialFolderPath = (GetSpecialFolderPath)library.resolve("SHGetSpecialFolderPathW");
        if (SHGetSpecialFolderPath) {
            TCHAR path[MAX_PATH];
            SHGetSpecialFolderPath(0, path, type, FALSE);
            result = QString::fromUtf16((ushort*)path);
        }
    } , {
        typedef BOOL (WINAPI*GetSpecialFolderPath)(HWND, char*, int, BOOL);
        GetSpecialFolderPath SHGetSpecialFolderPath = (GetSpecialFolderPath)library.resolve("SHGetSpecialFolderPathA");
        if (SHGetSpecialFolderPath) {
            char path[MAX_PATH];
            SHGetSpecialFolderPath(0, path, type, FALSE);
            result = QString::fromLocal8Bit(path);
        }
    } );

    if (result.isEmpty()) {
         result = QLatin1String("C:\\temp");
    }

    return result;
}

#endif



int UI_Mainwindow::file_is_opened(const char *o_path)
{
  int i;


  for(i=0; i<files_open; i++)
  {
    if(!strcmp(edfheaderlist[i]->filename, o_path))
    {
      return(1);
    }
  }

  return(0);
}


void UI_Mainwindow::organize_signals()
{
  UI_SignalChooser signalchooser(this, 3);
}


void UI_Mainwindow::edfplus_annotation_remove_duplicates()
{
  int i, j, k, list_size, dup_cnt=0;

  char str[256];

  struct annotationblock **list, *annot, *annot_cmp;


  QProgressDialog progress("Checking for duplicates...", "Abort", 0, 10, this);
  progress.setWindowModality(Qt::WindowModal);
  progress.setMinimumDuration(200);

  for(k=0; k<files_open; k++)
  {
    list = &annotationlist[k];

    if(*list==NULL)
    {
      continue;
    }

    list_size = edfplus_annotation_count(list);

    if(list_size < 2)  continue;

    progress.setMaximum(list_size);

    for(j=0; j<list_size; j++)
    {
      progress.setValue(j);

      qApp->processEvents();

      if(progress.wasCanceled() == TRUE)
      {
        break;
      }

      annot_cmp = edfplus_annotation_item(list, j);

      for(i=j; i<list_size; i++)
      {
        if(i!=j)
        {
          annot = edfplus_annotation_item(list, i);

          if(annot->onset != annot_cmp->onset)  continue;

          if(annot->file_num != annot_cmp->file_num)  continue;

          if(strcmp(annot->annotation, annot_cmp->annotation))  continue;

          if(strcmp(annot->duration, annot_cmp->duration))  continue;

          edfplus_annotation_delete(list, i);

          dup_cnt++;

          list_size--;

          if(j)  j--;

          if(i)  i--;
        }
      }
    }
  }

  progress.reset();

  if(dup_cnt)
  {
    for(k=0; k<files_open; k++)
    {
      if(annotations_dock[k] != NULL)
      {
        annotations_dock[k]->updateList();
      }
    }

    annotations_edited = 1;

    save_act->setEnabled(TRUE);
  }

  snprintf(str, 256, "Removed %i duplicates from the annotationlist(s)", dup_cnt);
  QMessageBox messagewindow(QMessageBox::Information, "Ready", str);
  messagewindow.exec();
}



struct signalcompblock * UI_Mainwindow::create_signalcomp_copy(struct signalcompblock *original_signalcomp)
{
  int i;

  char spec_str[256],
       *filter_spec,
       *err;

  struct signalcompblock *newsignalcomp;

  if(signalcomps >= MAXSIGNALS)
  {
    return(NULL);
  }

  if(original_signalcomp->ecg_filter != NULL)
  {
    return(NULL);
  }

  if(original_signalcomp->zratio_filter != NULL)
  {
    return(NULL);
  }

  newsignalcomp = (struct signalcompblock *)calloc(1, sizeof(struct signalcompblock));
  if(newsignalcomp == NULL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "malloc() error");
    messagewindow.exec();
    return(NULL);
  }

  memcpy(newsignalcomp, original_signalcomp, sizeof(struct signalcompblock));

  for(i=0; i<newsignalcomp->filter_cnt; i++)
  {
    newsignalcomp->filter[i] = create_filter_copy(original_signalcomp->filter[i]);
    if(newsignalcomp->filter[i] == NULL)
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "malloc() error");
      messagewindow.exec();
      free(signalcomp);
      return(NULL);
    }
  }

  for(i=0; i<newsignalcomp->ravg_filter_cnt; i++)
  {
    newsignalcomp->ravg_filter[i] = create_ravg_filter_copy(original_signalcomp->ravg_filter[i]);
    if(newsignalcomp->ravg_filter[i] == NULL)
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "malloc() error");
      messagewindow.exec();
      free(signalcomp);
      return(NULL);
    }
  }

  for(i=0; i<newsignalcomp->fidfilter_cnt; i++)
  {
    if(newsignalcomp->fidfilter_type[i] == 0)
    {
      if(newsignalcomp->fidfilter_model[i] == 0)
      {
        sprintf(spec_str, "HpBu%i/%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i]);
      }

      if(newsignalcomp->fidfilter_model[i] == 1)
      {
        sprintf(spec_str, "HpCh%i/%f/%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_ripple[i], newsignalcomp->fidfilter_freq[i]);
      }

      if(newsignalcomp->fidfilter_model[i] == 2)
      {
        sprintf(spec_str, "HpBe%i/%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i]);
      }
    }

    if(newsignalcomp->fidfilter_type[i] == 1)
    {
      if(newsignalcomp->fidfilter_model[i] == 0)
      {
        sprintf(spec_str, "LpBu%i/%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i]);
      }

      if(newsignalcomp->fidfilter_model[i] == 1)
      {
        sprintf(spec_str, "LpCh%i/%f/%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_ripple[i], newsignalcomp->fidfilter_freq[i]);
      }

      if(newsignalcomp->fidfilter_model[i] == 2)
      {
        sprintf(spec_str, "LpBe%i/%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i]);
      }
    }

    if(newsignalcomp->fidfilter_type[i] == 2)
    {
      if(newsignalcomp->fidfilter_model[i] == 0)
      {
        sprintf(spec_str, "BsRe/%i/%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i]);
      }
    }

    if(newsignalcomp->fidfilter_type[i] == 3)
    {
      if(newsignalcomp->fidfilter_model[i] == 0)
      {
        sprintf(spec_str, "BpBu%i/%f-%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i], newsignalcomp->fidfilter_freq2[i]);
      }

      if(newsignalcomp->fidfilter_model[i] == 1)
      {
        sprintf(spec_str, "BpCh%i/%f/%f-%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_ripple[i], newsignalcomp->fidfilter_freq[i], newsignalcomp->fidfilter_freq2[i]);
      }

      if(newsignalcomp->fidfilter_model[i] == 2)
      {
        sprintf(spec_str, "BpBe%i/%f-%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i], newsignalcomp->fidfilter_freq2[i]);
      }
    }

    if(newsignalcomp->fidfilter_type[i] == 4)
    {
      if(newsignalcomp->fidfilter_model[i] == 0)
      {
        sprintf(spec_str, "BsBu%i/%f-%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i], newsignalcomp->fidfilter_freq2[i]);
      }

      if(newsignalcomp->fidfilter_model[i] == 1)
      {
        sprintf(spec_str, "BsCh%i/%f/%f-%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_ripple[i], newsignalcomp->fidfilter_freq[i], newsignalcomp->fidfilter_freq2[i]);
      }

      if(newsignalcomp->fidfilter_model[i] == 2)
      {
        sprintf(spec_str, "BsBe%i/%f-%f", newsignalcomp->fidfilter_order[i], newsignalcomp->fidfilter_freq[i], newsignalcomp->fidfilter_freq2[i]);
      }
    }

    filter_spec = spec_str;

    err = fid_parse(((double)(newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[0]].smp_per_record)) / newsignalcomp->edfhdr->data_record_duration,
                    &filter_spec,
                    &newsignalcomp->fidfilter[i]);

    if(err != NULL)
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", err);
      messagewindow.exec();
      free(err);
      free(signalcomp);
      return(NULL);
    }

    newsignalcomp->fid_run[i] = fid_run_new(newsignalcomp->fidfilter[i], &newsignalcomp->fidfuncp[i]);

    newsignalcomp->fidbuf[i] = fid_run_newbuf(newsignalcomp->fid_run[i]);
    newsignalcomp->fidbuf2[i] = fid_run_newbuf(newsignalcomp->fid_run[i]);

//    memcpy(newsignalcomp->fidbuf[i], original_signalcomp->fidbuf[i], fid_run_bufsize(original_signalcomp->fid_run[i]));
//    memcpy(newsignalcomp->fidbuf2[i], original_signalcomp->fidbuf2[i], fid_run_bufsize(original_signalcomp->fid_run[i]));
  }

  signalcomp[signalcomps] = newsignalcomp;

  signalcomps++;

  return(newsignalcomp);
}







