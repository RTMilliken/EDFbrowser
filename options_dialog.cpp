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



#include "options_dialog.h"




UI_OptionsDialog::UI_OptionsDialog(QWidget *w_parent)
{
  int i,
      showminimized=0;

  mainwindow = (UI_Mainwindow *)w_parent;

  optionsdialog = new QDialog(w_parent);

  if(QApplication::desktop()->screenGeometry().height() < 900)
  {
    showminimized = 1;
  }

  if(showminimized)
  {
    optionsdialog->setMinimumSize(480, 500);
    optionsdialog->setMaximumSize(480, 820);

    scrollarea = new QScrollArea;
    scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    frame = new QFrame;
    frame->setMinimumSize(440, 780);
    frame->setMaximumSize(440, 780);
  }
  else
  {
    optionsdialog->setMinimumSize(440, 780);
    optionsdialog->setMaximumSize(440, 780);
  }
  optionsdialog->setWindowTitle("Settings");
  optionsdialog->setModal(TRUE);
  optionsdialog->setAttribute(Qt::WA_DeleteOnClose, TRUE);

  tabholder = new QTabWidget;

  CloseButton = new QPushButton;
  CloseButton->setText("Close");

/////////////////////////////////////// tab 1  Colors ///////////////////////////////////////////////////////////////////////

  tab1 = new QWidget;

  label1 = new QLabel(tab1);
  label1->setGeometry(QRect(20, 20, 200, 25));
  label1->setText("Background color");

  BgColorButton = new SpecialButton(tab1);
  BgColorButton->setGeometry(QRect(240, 20, 100, 25));
  BgColorButton->setColor(mainwindow->maincurve->backgroundcolor);

  label2 = new QLabel(tab1);
  label2->setGeometry(QRect(20, 65, 200, 25));
  label2->setText("Small ruler color");

  SrColorButton = new SpecialButton(tab1);
  SrColorButton->setGeometry(QRect(240, 65, 100, 25));
  SrColorButton->setColor(mainwindow->maincurve->small_ruler_color);

  label3 = new QLabel(tab1);
  label3->setGeometry(QRect(20, 110, 200, 25));
  label3->setText("Big ruler color");

  BrColorButton = new SpecialButton(tab1);
  BrColorButton->setGeometry(QRect(240, 110, 100, 25));
  BrColorButton->setColor(mainwindow->maincurve->big_ruler_color);

  label4 = new QLabel(tab1);
  label4->setGeometry(QRect(20, 155, 200, 25));
  label4->setText("Mouse rectangle color");

  MrColorButton = new SpecialButton(tab1);
  MrColorButton->setGeometry(QRect(240, 155, 100, 25));
  MrColorButton->setColor(mainwindow->maincurve->mouse_rect_color);

  label5 = new QLabel(tab1);
  label5->setGeometry(QRect(20, 200, 200, 25));
  label5->setText("Text color");

  TxtColorButton = new SpecialButton(tab1);
  TxtColorButton->setGeometry(QRect(240, 200, 100, 25));
  TxtColorButton->setColor(mainwindow->maincurve->text_color);

  label6 = new QLabel(tab1);
  label6->setGeometry(QRect(20, 245, 200, 25));
  label6->setText("Signals color");

  SigColorButton = new SpecialButton(tab1);
  SigColorButton->setGeometry(QRect(240, 245, 100, 25));
  SigColorButton->setColor((Qt::GlobalColor)mainwindow->maincurve->signal_color);

  label7 = new QLabel(tab1);
  label7->setGeometry(QRect(20, 290, 200, 25));
  label7->setText("Baseline color");

  checkbox3 = new QCheckBox(tab1);
  checkbox3->setGeometry(QRect(200, 293, 20, 20));
  checkbox3->setTristate(FALSE);
  if(mainwindow->show_baselines)
  {
    checkbox3->setCheckState(Qt::Checked);
  }
  else
  {
    checkbox3->setCheckState(Qt::Unchecked);
  }

  BaseColorButton = new SpecialButton(tab1);
  BaseColorButton->setGeometry(QRect(240, 290, 100, 25));
  BaseColorButton->setColor(mainwindow->maincurve->baseline_color);

  label8 = new QLabel(tab1);
  label8->setGeometry(QRect(20, 335, 200, 25));
  label8->setText("Crosshair color");

  Crh1ColorButton = new SpecialButton(tab1);
  Crh1ColorButton->setGeometry(QRect(240, 335, 100, 25));
  Crh1ColorButton->setColor((Qt::GlobalColor)mainwindow->maincurve->crosshair_1.color);

  label9 = new QLabel(tab1);
  label9->setGeometry(QRect(20, 380, 200, 25));
  label9->setText("2th Crosshair color");

  Crh2ColorButton = new SpecialButton(tab1);
  Crh2ColorButton->setGeometry(QRect(240, 380, 100, 25));
  Crh2ColorButton->setColor((Qt::GlobalColor)mainwindow->maincurve->crosshair_2.color);

  label10 = new QLabel(tab1);
  label10->setGeometry(QRect(20, 425, 200, 25));
  label10->setText("Floating ruler color");

  FrColorButton = new SpecialButton(tab1);
  FrColorButton->setGeometry(QRect(240, 425, 100, 25));
  FrColorButton->setColor((Qt::GlobalColor)mainwindow->maincurve->floating_ruler_color);

  label12 = new QLabel(tab1);
  label12->setGeometry(QRect(20, 470, 200, 25));
  label12->setText("Annotation marker");

  checkbox2 = new QCheckBox(tab1);
  checkbox2->setGeometry(QRect(200, 473, 20, 20));
  checkbox2->setTristate(FALSE);
  if(mainwindow->show_annot_markers)
  {
    checkbox2->setCheckState(Qt::Checked);
  }
  else
  {
    checkbox2->setCheckState(Qt::Unchecked);
  }

  AnnotMkrButton = new SpecialButton(tab1);
  AnnotMkrButton->setGeometry(QRect(240, 470, 100, 25));
  AnnotMkrButton->setColor(mainwindow->maincurve->annot_marker_color);

  label11 = new QLabel(tab1);
  label11->setGeometry(QRect(20, 515, 200, 25));
  label11->setText("Print in grayscale");

  checkbox1 = new QCheckBox(tab1);
  checkbox1->setGeometry(QRect(200, 518, 20, 20));
  checkbox1->setTristate(FALSE);
  if(mainwindow->maincurve->blackwhite_printing)
  {
    checkbox1->setCheckState(Qt::Checked);
  }
  else
  {
    checkbox1->setCheckState(Qt::Unchecked);
  }

  label13 = new QLabel(tab1);
  label13->setGeometry(QRect(20, 560, 200, 25));
  label13->setText("Clip signals to pane");

  checkbox4 = new QCheckBox(tab1);
  checkbox4->setGeometry(QRect(200, 563, 20, 20));
  checkbox4->setTristate(FALSE);
  if(mainwindow->clip_to_pane)
  {
    checkbox4->setCheckState(Qt::Checked);
  }
  else
  {
    checkbox4->setCheckState(Qt::Unchecked);
  }

  DefaultButton = new QPushButton(tab1);
  DefaultButton->setGeometry(QRect(140, 620, 140, 25));
  DefaultButton->setText("Restore default");

  QObject::connect(BgColorButton,   SIGNAL(clicked(SpecialButton *)), this,          SLOT(BgColorButtonClicked(SpecialButton *)));
  QObject::connect(SrColorButton,   SIGNAL(clicked(SpecialButton *)), this,          SLOT(SrColorButtonClicked(SpecialButton *)));
  QObject::connect(BrColorButton,   SIGNAL(clicked(SpecialButton *)), this,          SLOT(BrColorButtonClicked(SpecialButton *)));
  QObject::connect(MrColorButton,   SIGNAL(clicked(SpecialButton *)), this,          SLOT(MrColorButtonClicked(SpecialButton *)));
  QObject::connect(TxtColorButton,  SIGNAL(clicked(SpecialButton *)), this,          SLOT(TxtColorButtonClicked(SpecialButton *)));
  QObject::connect(SigColorButton,  SIGNAL(clicked(SpecialButton *)), this,          SLOT(SigColorButtonClicked(SpecialButton *)));
  QObject::connect(BaseColorButton, SIGNAL(clicked(SpecialButton *)), this,          SLOT(BaseColorButtonClicked(SpecialButton *)));
  QObject::connect(Crh1ColorButton, SIGNAL(clicked(SpecialButton *)), this,          SLOT(Crh1ColorButtonClicked(SpecialButton *)));
  QObject::connect(Crh2ColorButton, SIGNAL(clicked(SpecialButton *)), this,          SLOT(Crh2ColorButtonClicked(SpecialButton *)));
  QObject::connect(FrColorButton,   SIGNAL(clicked(SpecialButton *)), this,          SLOT(FrColorButtonClicked(SpecialButton *)));
  QObject::connect(AnnotMkrButton,  SIGNAL(clicked(SpecialButton *)), this,          SLOT(AnnotMkrButtonClicked(SpecialButton *)));
  QObject::connect(checkbox1,       SIGNAL(stateChanged(int)),        this,          SLOT(checkbox1Clicked(int)));
  QObject::connect(checkbox2,       SIGNAL(stateChanged(int)),        this,          SLOT(checkbox2Clicked(int)));
  QObject::connect(checkbox3,       SIGNAL(stateChanged(int)),        this,          SLOT(checkbox3Clicked(int)));
  QObject::connect(checkbox4,       SIGNAL(stateChanged(int)),        this,          SLOT(checkbox4Clicked(int)));
  QObject::connect(DefaultButton,   SIGNAL(clicked()),                this,          SLOT(DefaultButtonClicked()));

  tabholder->addTab(tab1, "Colors");

/////////////////////////////////////// tab 2   Calibration ///////////////////////////////////////////////////////////////////////

  tab2 = new QWidget;

  checkbox2_1 = new QCheckBox("Manually override automatic DPI settings", tab2);
  checkbox2_1->setGeometry(85, 560, 300, 20);
  checkbox2_1->setTristate(FALSE);
  if(mainwindow->auto_dpi)
  {
    checkbox2_1->setCheckState(Qt::Unchecked);
  }
  else
  {
    checkbox2_1->setCheckState(Qt::Checked);
  }

  slabel2_1 = new SpecialButton(tab2);
  slabel2_1->setGeometry(30, 20, 10, 445);
  slabel2_1->setColor(Qt::black);

  slabel2_3 = new SpecialButton(tab2);
  slabel2_3->setGeometry(30, 500, 355, 10);
  slabel2_3->setColor(Qt::black);

  label2_2 = new QLabel(tab2);
  label2_2->setGeometry(85, 50, 220, 50);
  label2_2->setText("Measure the length of the black\nrectangles and enter the values.");

  spinbox2_1 = new QSpinBox(tab2);
  spinbox2_1->setGeometry(85, 240, 100, 20);
  spinbox2_1->setSuffix(" mm");
  spinbox2_1->setMinimum(10);
  spinbox2_1->setMaximum(500);
  spinbox2_1->setValue((int)(4450.0 / (1.0 / mainwindow->pixelsizefactor)));

  spinbox2_2 = new QSpinBox(tab2);
  spinbox2_2->setGeometry(170, 450, 100, 20);
  spinbox2_2->setSuffix(" mm");
  spinbox2_2->setMinimum(10);
  spinbox2_2->setMaximum(500);
  spinbox2_2->setValue((int)(3550.0 / (1.0 / mainwindow->x_pixelsizefactor)));

  ApplyButton = new QPushButton(tab2);
  ApplyButton->setGeometry(QRect(85, 600, 100, 25));
  ApplyButton->setText("Apply");

  if(checkbox2_1->checkState() == Qt::Unchecked)
  {
    spinbox2_1->setEnabled(FALSE);
    spinbox2_2->setEnabled(FALSE);
    ApplyButton->setEnabled(FALSE);
  }

  QObject::connect(ApplyButton, SIGNAL(clicked()),         this, SLOT(ApplyButtonClicked()));
  QObject::connect(checkbox2_1, SIGNAL(stateChanged(int)), this, SLOT(calibrate_checkbox_stateChanged(int)));

  tabholder->addTab(tab2, "Calibration");

/////////////////////////////////////// tab 3  Powerspectrum ///////////////////////////////////////////////////////////////////////

  tab3 = new QWidget;

  label1_3 = new QLabel(tab3);
  label1_3->setGeometry(QRect(20, 20, 330, 25));
  label1_3->setText("Frequency regions of the colorbars:");

  colorBarTable = new QTableWidget(tab3);
  colorBarTable->setGeometry(20, 50, 370, 350);
  colorBarTable->setSelectionMode(QAbstractItemView::NoSelection);
  colorBarTable->setColumnCount(4);
  colorBarTable->setColumnWidth(0, 20);
  colorBarTable->setColumnWidth(1, 140);
  colorBarTable->setColumnWidth(2, 80);
  colorBarTable->setColumnWidth(3, 100);
  colorBarTable->setRowCount(MAXSPECTRUMMARKERS);
  for(i=0; i < MAXSPECTRUMMARKERS; i++)
  {
    colorBarTable->setRowHeight(i, 20);

    colorBarTable->setCellWidget(i, 0, new QCheckBox);
   ((QCheckBox *)(colorBarTable->cellWidget(i, 0)))->setTristate(FALSE);
    if(i < mainwindow->spectrum_colorbar->items)
    {
      ((QCheckBox *)(colorBarTable->cellWidget(i, 0)))->setCheckState(Qt::Checked);
    }
    else
    {
      ((QCheckBox *)(colorBarTable->cellWidget(i, 0)))->setCheckState(Qt::Unchecked);
    }
    QObject::connect(colorBarTable->cellWidget(i, 0), SIGNAL(stateChanged(int)), this, SLOT(checkBoxChanged(int)));

    colorBarTable->setCellWidget(i, 1, new QDoubleSpinBox);
    ((QDoubleSpinBox *)(colorBarTable->cellWidget(i, 1)))->setDecimals(3);
    ((QDoubleSpinBox *)(colorBarTable->cellWidget(i, 1)))->setSuffix(" Hz");
    ((QDoubleSpinBox *)(colorBarTable->cellWidget(i, 1)))->setRange(0.001, 100000.0);
    ((QDoubleSpinBox *)(colorBarTable->cellWidget(i, 1)))->setValue(mainwindow->spectrum_colorbar->freq[i]);
    QObject::connect((QDoubleSpinBox *)(colorBarTable->cellWidget(i, 1)), SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));

    colorBarTable->setCellWidget(i, 2, new SpecialButton);
    ((SpecialButton *)(colorBarTable->cellWidget(i, 2)))->setGlobalColor(mainwindow->spectrum_colorbar->color[i]);
    QObject::connect((SpecialButton *)(colorBarTable->cellWidget(i, 2)), SIGNAL(clicked(SpecialButton *)), this, SLOT(colorBarButtonClicked(SpecialButton *)));

    colorBarTable->setCellWidget(i, 3, new QLineEdit);
    ((QLineEdit *)(colorBarTable->cellWidget(i, 3)))->setText(mainwindow->spectrum_colorbar->label[i]);
    ((QLineEdit *)(colorBarTable->cellWidget(i, 3)))->setMaxLength(16);
    QObject::connect((QLineEdit *)(colorBarTable->cellWidget(i, 3)), SIGNAL(textEdited(const QString  &)), this, SLOT(labelEdited(const QString  &)));
  }

  QStringList horizontallabels;
  horizontallabels += "";
  horizontallabels += "Frequency";
  horizontallabels += "Color";
  horizontallabels += "Label";
  colorBarTable->setHorizontalHeaderLabels(horizontallabels);

  label2_3 = new QLabel(tab3);
  label2_3->setGeometry(QRect(20, 410, 310, 25));
  label2_3->setText("Height of colorbars are relative to the");

  radiobutton1 = new QRadioButton(tab3);
  radiobutton1->setGeometry(QRect(20, 435, 200, 25));
  radiobutton1->setText("sum");
  if(mainwindow->spectrum_colorbar->method == 0)
  {
    radiobutton1->setChecked(TRUE);  // sum
  }

  radiobutton2 = new QRadioButton(tab3);
  radiobutton2->setGeometry(QRect(20, 460, 200, 25));
  radiobutton2->setText("peak");
  if(mainwindow->spectrum_colorbar->method == 1)
  {
    radiobutton2->setChecked(TRUE);  // peak
  }

  radiobutton3 = new QRadioButton(tab3);
  radiobutton3->setGeometry(QRect(20, 485, 200, 25));
  radiobutton3->setText("average");
  if(mainwindow->spectrum_colorbar->method == 2)
  {
    radiobutton3->setChecked(TRUE);  // average
  }

  label3_3 = new QLabel(tab3);
  label3_3->setGeometry(QRect(20, 515, 310, 25));
  label3_3->setText("of the power in the colorbar region.");

  label3_4 = new QLabel(tab3);
  label3_4->setGeometry(QRect(20, 550, 120, 25));
  label3_4->setText("FFT blocksize:");

  spinbox3_1 = new QSpinBox(tab3);
  spinbox3_1->setGeometry(QRect(140, 552, 140, 20));
  spinbox3_1->setSuffix(" samples");
  spinbox3_1->setMinimum(10);
  spinbox3_1->setMaximum(32768);
  spinbox3_1->setSingleStep(2);
  spinbox3_1->setValue(mainwindow->maxdftblocksize);

  DefaultButton2 = new QPushButton(tab3);
  DefaultButton2->setGeometry(QRect(245, 605, 100, 25));
  DefaultButton2->setText("Restore default");

  ApplyButton2 = new QPushButton(tab3);
  ApplyButton2->setGeometry(QRect(20, 605, 100, 25));
  ApplyButton2->setText("Apply");
  ApplyButton2->setEnabled(FALSE);

  QObject::connect(radiobutton1,   SIGNAL(toggled(bool)),     this, SLOT(radioButtonToggled(bool)));
  QObject::connect(radiobutton2,   SIGNAL(toggled(bool)),     this, SLOT(radioButtonToggled(bool)));
  QObject::connect(radiobutton3,   SIGNAL(toggled(bool)),     this, SLOT(radioButtonToggled(bool)));
  QObject::connect(spinbox3_1,     SIGNAL(valueChanged(int)), this, SLOT(spinBox3_3ValueChanged(int)));
  QObject::connect(ApplyButton2,   SIGNAL(clicked()),         this, SLOT(ApplyButton2Clicked()));
  QObject::connect(DefaultButton2, SIGNAL(clicked()),         this, SLOT(DefaultButton2Clicked()));

  tabholder->addTab(tab3, "Powerspectrum");

/////////////////////////////////////// tab 4  Other ///////////////////////////////////////////////////////////////////////

  tab4 = new QWidget;

  label4_1 = new QLabel(tab4);
  label4_1->setGeometry(QRect(20, 20, 280, 25));
  label4_1->setText("Reload last used montage");

  checkbox4_1 = new QCheckBox(tab4);
  checkbox4_1->setGeometry(QRect(325, 23, 20, 20));
  checkbox4_1->setTristate(FALSE);
  if(mainwindow->auto_reload_mtg)
  {
    checkbox4_1->setCheckState(Qt::Checked);
  }
  else
  {
    checkbox4_1->setCheckState(Qt::Unchecked);
  }

  QObject::connect(checkbox4_1, SIGNAL(stateChanged(int)), this, SLOT(checkbox4_1Clicked(int)));

  label4_7 = new QLabel(tab4);
  label4_7->setGeometry(QRect(20, 60, 280, 60));
  label4_7->setText("Do not read annotations, Biosemi Status signal\n"
                    "or Nihon Kohden triggers when filesize\n"
                    "is more than:");

  spinbox4_3 = new QSpinBox(tab4);
  spinbox4_3->setGeometry(QRect(200, 100, 140, 20));
  spinbox4_3->setSuffix(" MB");
  spinbox4_3->setMinimum(100);
  spinbox4_3->setMaximum(10000);
  spinbox4_3->setSingleStep(1);
  spinbox4_3->setValue((int)(mainwindow->maxfilesize_to_readin_annotations / 1048576LL));

  QObject::connect(spinbox4_3, SIGNAL(valueChanged(int)), this, SLOT(spinBox4_3ValueChanged(int)));

  label4_2 = new QLabel(tab4);
  label4_2->setGeometry(QRect(20, 135, 280, 25));
  label4_2->setText("Read Biosemi Status signal");

  checkbox4_2 = new QCheckBox(tab4);
  checkbox4_2->setGeometry(QRect(325, 138, 20, 20));
  checkbox4_2->setTristate(FALSE);
  if(mainwindow->read_biosemi_status_signal)
  {
    checkbox4_2->setCheckState(Qt::Checked);
  }
  else
  {
    checkbox4_2->setCheckState(Qt::Unchecked);
  }

  QObject::connect(checkbox4_2, SIGNAL(stateChanged(int)), this, SLOT(checkbox4_2Clicked(int)));

  label4_6 = new QLabel(tab4);
  label4_6->setGeometry(QRect(20, 175, 280, 25));
  label4_6->setText("Read Nihon Kohden Trigger/Marker signal");

  checkbox4_3 = new QCheckBox(tab4);
  checkbox4_3->setGeometry(QRect(325, 178, 20, 20));
  checkbox4_3->setTristate(FALSE);
  if(mainwindow->read_nk_trigger_signal)
  {
    checkbox4_3->setCheckState(Qt::Checked);
  }
  else
  {
    checkbox4_3->setCheckState(Qt::Unchecked);
  }

  QObject::connect(checkbox4_3, SIGNAL(stateChanged(int)), this, SLOT(checkbox4_3Clicked(int)));

  label4_3 = new QLabel(tab4);
  label4_3->setGeometry(QRect(20, 215, 200, 25));
  label4_3->setText("livestream update interval");

  spinbox4_1 = new QSpinBox(tab4);
  spinbox4_1->setGeometry(QRect(200, 218, 140, 20));
  spinbox4_1->setSuffix(" mSec");
  spinbox4_1->setMinimum(100);
  spinbox4_1->setMaximum(3000);
  spinbox4_1->setSingleStep(1);
  spinbox4_1->setValue(mainwindow->live_stream_update_interval);

  QObject::connect(spinbox4_1, SIGNAL(valueChanged(int)), this, SLOT(spinBox4_1ValueChanged(int)));

  label4_4 = new QLabel(tab4);
  label4_4->setGeometry(QRect(20, 255, 200, 25));
  label4_4->setText("Powerline Frequency");

  combobox4_1 = new QComboBox(tab4);
  combobox4_1->setGeometry(QRect(200, 258, 140, 20));
  combobox4_1->addItem("50 Hz");
  combobox4_1->addItem("60 Hz");
  if(mainwindow->powerlinefreq == 50)
  {
    combobox4_1->setCurrentIndex(0);
  }
  if(mainwindow->powerlinefreq == 60)
  {
    combobox4_1->setCurrentIndex(1);
  }

  QObject::connect(combobox4_1, SIGNAL(currentIndexChanged(int)), this, SLOT(combobox4_1IndexChanged(int)));

  label4_4 = new QLabel(tab4);
  label4_4->setGeometry(QRect(20, 295, 200, 40));
  label4_4->setText("Mousewheel stepsize\n"
                    "(0 means no scroll)");

  spinbox4_2 = new QSpinBox(tab4);
  spinbox4_2->setGeometry(QRect(200, 298, 140, 20));
  spinbox4_2->setPrefix("Timescale / ");
  spinbox4_2->setMinimum(0);
  spinbox4_2->setMaximum(100);
  spinbox4_2->setSingleStep(1);
  spinbox4_2->setValue(mainwindow->mousewheelsens);

  QObject::connect(spinbox4_2, SIGNAL(valueChanged(int)), this, SLOT(spinBox4_2ValueChanged(int)));

  label4_5 = new QLabel(tab4);
  label4_5->setGeometry(QRect(20, 335, 280, 25));
  label4_5->setText("Use Multi-Threading");

  checkbox4_4 = new QCheckBox(tab4);
  checkbox4_4->setGeometry(QRect(325, 338, 20, 20));
  checkbox4_4->setTristate(FALSE);
  if(mainwindow->use_threads)
  {
    checkbox4_4->setCheckState(Qt::Checked);
  }
  else
  {
    checkbox4_4->setCheckState(Qt::Unchecked);
  }

  QObject::connect(checkbox4_4, SIGNAL(stateChanged(int)), this, SLOT(checkbox4_4Clicked(int)));

  label4_8 = new QLabel(tab4);
  label4_8->setGeometry(QRect(20, 375, 310, 25));
  label4_8->setText("Check for updates during startup");

  checkbox4_5 = new QCheckBox(tab4);
  checkbox4_5->setGeometry(QRect(325, 378, 20, 20));
  checkbox4_5->setTristate(FALSE);
  if(mainwindow->check_for_updates)
  {
    checkbox4_5->setCheckState(Qt::Checked);
  }
  else
  {
    checkbox4_5->setCheckState(Qt::Unchecked);
  }

  QObject::connect(checkbox4_5, SIGNAL(stateChanged(int)), this, SLOT(checkbox4_5Clicked(int)));

  tabholder->addTab(tab4, "Other");

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  horLayout = new QHBoxLayout;
  horLayout->addSpacing(300);
  horLayout->addWidget(CloseButton);

  mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tabholder);
  mainLayout->addLayout(horLayout);

  if(showminimized)
  {
    frame->setLayout(mainLayout);

    scrollarea->setWidget(frame);

    scrolllayout = new QGridLayout;
    scrolllayout->addWidget(scrollarea, 0, 0);

    optionsdialog->setLayout(scrolllayout);
  }
  else
  {
    optionsdialog->setLayout(mainLayout);
  }

  QObject::connect(CloseButton, SIGNAL(clicked()), optionsdialog, SLOT(close()));

  optionsdialog->exec();
}


void UI_OptionsDialog::spinBox4_3ValueChanged(int filesize)
{
  mainwindow->maxfilesize_to_readin_annotations = (long long)filesize * 1048576LL;
}


void UI_OptionsDialog::spinBox4_2ValueChanged(int stepsize)
{
  mainwindow->mousewheelsens = stepsize;
}


void UI_OptionsDialog::combobox4_1IndexChanged(int index)
{
  if(index == 0)
  {
    mainwindow->powerlinefreq = 50;
  }

  if(index == 1)
  {
    mainwindow->powerlinefreq = 60;
  }
}


void UI_OptionsDialog::spinBox4_1ValueChanged(int interval)
{
  mainwindow->live_stream_update_interval = interval;
}


void UI_OptionsDialog::calibrate_checkbox_stateChanged(int state)
{
  if(state == Qt::Checked)
  {
    spinbox2_1->setEnabled(TRUE);
    spinbox2_2->setEnabled(TRUE);
    ApplyButton->setEnabled(TRUE);
    mainwindow->auto_dpi = 0;
  }
  else
  {
    spinbox2_1->setEnabled(FALSE);
    spinbox2_2->setEnabled(FALSE);
    ApplyButton->setEnabled(FALSE);
    mainwindow->auto_dpi = 1;
    mainwindow->pixelsizefactor = 1.0 / ((double)mainwindow->dpiy / 2.54);
    mainwindow->x_pixelsizefactor = 1.0 / ((double)mainwindow->dpix / 2.54);

    mainwindow->maincurve->drawCurve_stage_1();
  }
}


void UI_OptionsDialog::ApplyButtonClicked()
{
  int i, j;


  mainwindow->pixelsizefactor = 1.0 / (4450.0 / spinbox2_1->value());

  mainwindow->x_pixelsizefactor = 1.0 / (3550.0 / spinbox2_2->value());

  for(i=0; i<mainwindow->signalcomps; i++)
  {
    for(j=0; j<mainwindow->signalcomp[i]->num_of_signals; j++)
    {
      mainwindow->signalcomp[i]->sensitivity[j] =
       mainwindow->signalcomp[i]->edfhdr->edfparam[mainwindow->signalcomp[i]->edfsignal[j]].bitvalue
       / ((double)mainwindow->signalcomp[i]->voltpercm
       * mainwindow->pixelsizefactor);
    }
  }

  mainwindow->maincurve->drawCurve_stage_1();
}


void UI_OptionsDialog::labelEdited(const QString  &)
{
  ApplyButton2->setEnabled(TRUE);
}


void UI_OptionsDialog::spinBox3_3ValueChanged(int)
{
  ApplyButton2->setEnabled(TRUE);
}


void UI_OptionsDialog::radioButtonToggled(bool)
{
  ApplyButton2->setEnabled(TRUE);
}


void UI_OptionsDialog::spinBoxValueChanged(double)
{
  ApplyButton2->setEnabled(TRUE);
}


void UI_OptionsDialog::ApplyButton2Clicked()
{
  int row;

  char str[256];

  for(row = 1; row < MAXSPECTRUMMARKERS; row++)
  {
    if(((QCheckBox *)(colorBarTable->cellWidget(row, 0)))->checkState() == Qt::Checked)
    {
      if(((QDoubleSpinBox *)(colorBarTable->cellWidget(row - 1, 1)))->value() >= ((QDoubleSpinBox *)(colorBarTable->cellWidget(row, 1)))->value())
      {
        sprintf(str, "Row %i must have a higher frequency than row %i", row + 1, row);

        QMessageBox messagewindow(QMessageBox::Critical, "Error", str);
        messagewindow.exec();

        return;
      }
    }
    else
    {
      break;
    }
  }

  for(row = 0; row < MAXSPECTRUMMARKERS; row++)
  {
    if(((QCheckBox *)(colorBarTable->cellWidget(row, 0)))->checkState() == Qt::Checked)
    {
      mainwindow->spectrum_colorbar->freq[row] = ((QDoubleSpinBox *)(colorBarTable->cellWidget(row, 1)))->value();
      mainwindow->spectrum_colorbar->color[row] = ((SpecialButton *)(colorBarTable->cellWidget(row, 2)))->globalColor();
      strncpy(mainwindow->spectrum_colorbar->label[row], ((QLineEdit *)(colorBarTable->cellWidget(row, 3)))->text().toLatin1().data(), 16);
      mainwindow->spectrum_colorbar->label[row][16] = 0;
    }
    else
    {
      break;
    }
  }

  mainwindow->spectrum_colorbar->items = row;

  for(; row < MAXSPECTRUMMARKERS; row++)
  {
    mainwindow->spectrum_colorbar->freq[row] = ((QDoubleSpinBox *)(colorBarTable->cellWidget(row, 1)))->value();
    mainwindow->spectrum_colorbar->color[row] = ((SpecialButton *)(colorBarTable->cellWidget(row, 2)))->globalColor();
  }

  if(radiobutton1->isChecked()) // sum
  {
    mainwindow->spectrum_colorbar->method = 0;
  }
  else
    if(radiobutton2->isChecked()) // peak
    {
      mainwindow->spectrum_colorbar->method = 1;
    }
    else
      if(radiobutton3->isChecked()) // average
      {
        mainwindow->spectrum_colorbar->method = 2;
      }

  mainwindow->maxdftblocksize = spinbox3_1->value();
  if(mainwindow->maxdftblocksize & 1)
  {
    mainwindow->maxdftblocksize--;
  }

  ApplyButton2->setEnabled(FALSE);
}


void UI_OptionsDialog::checkBoxChanged(int state)
{
  int i,
      row,
      lastrow=0;

  if(state == Qt::Checked)
  {
    for(row = MAXSPECTRUMMARKERS - 1; row >= 0; row--)
    {
      if(((QCheckBox *)(colorBarTable->cellWidget(row, 0)))->checkState() == Qt::Checked)
      {
        lastrow = row;

        if(row)
        {
          for(i=row-1; i>=0; i--)
          {
            ((QCheckBox *)(colorBarTable->cellWidget(i, 0)))->setCheckState(Qt::Checked);
          }
        }

        break;
      }
    }
  }
  else
  {
    for(row = 0; row < MAXSPECTRUMMARKERS; row++)
    {
      if(((QCheckBox *)(colorBarTable->cellWidget(row, 0)))->checkState() == Qt::Unchecked)
      {
        lastrow = row - 1;

        for(; row < MAXSPECTRUMMARKERS; row++)
        {
          ((QCheckBox *)(colorBarTable->cellWidget(row, 0)))->setCheckState(Qt::Unchecked);
        }

        break;
      }
    }
  }

  for(row=0; row < lastrow; row++)
  {
    if(((QDoubleSpinBox *)(colorBarTable->cellWidget(row, 1)))->value() >= ((QDoubleSpinBox *)(colorBarTable->cellWidget(row + 1, 1)))->value())
    {
      ((QDoubleSpinBox *)(colorBarTable->cellWidget(row + 1, 1)))->setValue(((QDoubleSpinBox *)(colorBarTable->cellWidget(row, 1)))->value() + 1.0);
    }
  }

  ApplyButton2->setEnabled(TRUE);
}


void UI_OptionsDialog::DefaultButton2Clicked()
{
  int i;

  for(i=0; i<5; i++)
  {
    ((QCheckBox *)(colorBarTable->cellWidget(i, 0)))->setCheckState(Qt::Checked);
    ((QLineEdit *)(colorBarTable->cellWidget(i, 3)))->clear();
  }

  ((QDoubleSpinBox *)(colorBarTable->cellWidget(0, 1)))->setValue(4.0);
  ((SpecialButton *)(colorBarTable->cellWidget(0, 2)))->setGlobalColor(Qt::darkRed);

  ((QDoubleSpinBox *)(colorBarTable->cellWidget(1, 1)))->setValue(8.0);
  ((SpecialButton *)(colorBarTable->cellWidget(1, 2)))->setGlobalColor(Qt::darkGreen);

  ((QDoubleSpinBox *)(colorBarTable->cellWidget(2, 1)))->setValue(12.0);
  ((SpecialButton *)(colorBarTable->cellWidget(2, 2)))->setGlobalColor(Qt::darkBlue);

  ((QDoubleSpinBox *)(colorBarTable->cellWidget(3, 1)))->setValue(30.0);
  ((SpecialButton *)(colorBarTable->cellWidget(3, 2)))->setGlobalColor(Qt::darkCyan);

  ((QDoubleSpinBox *)(colorBarTable->cellWidget(4, 1)))->setValue(100.0);
  ((SpecialButton *)(colorBarTable->cellWidget(4, 2)))->setGlobalColor(Qt::darkMagenta);

  for(i=5; i<MAXSPECTRUMMARKERS; i++)
  {
    ((QCheckBox *)(colorBarTable->cellWidget(i, 0)))->setCheckState(Qt::Unchecked);
    ((QDoubleSpinBox *)(colorBarTable->cellWidget(i, 1)))->setValue(1.0);
    ((SpecialButton *)(colorBarTable->cellWidget(i, 2)))->setGlobalColor(Qt::white);
    ((QLineEdit *)(colorBarTable->cellWidget(i, 3)))->clear();
  }

  spinbox3_1->setValue(1000);

  radiobutton2->setChecked(TRUE);

  ApplyButton2->setEnabled(TRUE);
}


void UI_OptionsDialog::colorBarButtonClicked(SpecialButton *button)
{
  int color;

  UI_ColorMenuDialog colormenudialog(&color);

  if(color < 0)  return;

  button->setGlobalColor(color);

  ApplyButton2->setEnabled(TRUE);
}


void UI_OptionsDialog::checkbox1Clicked(int state)
{
  if(state==Qt::Checked)
  {
    mainwindow->maincurve->blackwhite_printing = 1;
  }

  if(state==Qt::Unchecked)
  {
    mainwindow->maincurve->blackwhite_printing = 0;
  }
}


void UI_OptionsDialog::checkbox2Clicked(int state)
{
  if(state==Qt::Checked)
  {
    mainwindow->show_annot_markers = 1;
  }

  if(state==Qt::Unchecked)
  {
    mainwindow->show_annot_markers = 0;
  }

  mainwindow->maincurve->update();
}


void UI_OptionsDialog::checkbox3Clicked(int state)
{
  if(state==Qt::Checked)
  {
    mainwindow->show_baselines = 1;
  }

  if(state==Qt::Unchecked)
  {
    mainwindow->show_baselines = 0;
  }

  mainwindow->maincurve->update();
}


void UI_OptionsDialog::checkbox4Clicked(int state)
{
  if(state==Qt::Checked)
  {
    mainwindow->clip_to_pane = 1;
  }

  if(state==Qt::Unchecked)
  {
    mainwindow->clip_to_pane = 0;
  }

  mainwindow->maincurve->update();
}


void UI_OptionsDialog::checkbox4_1Clicked(int state)
{
  if(state==Qt::Checked)
  {
    mainwindow->auto_reload_mtg = 1;
  }

  if(state==Qt::Unchecked)
  {
    mainwindow->auto_reload_mtg = 0;
  }
}


void UI_OptionsDialog::checkbox4_2Clicked(int state)
{
  if(state==Qt::Checked)
  {
    mainwindow->read_biosemi_status_signal = 1;
  }

  if(state==Qt::Unchecked)
  {
    mainwindow->read_biosemi_status_signal = 0;
  }
}


void UI_OptionsDialog::checkbox4_3Clicked(int state)
{
  if(state==Qt::Checked)
  {
    mainwindow->read_nk_trigger_signal = 1;
  }

  if(state==Qt::Unchecked)
  {
    mainwindow->read_nk_trigger_signal = 0;
  }
}


void UI_OptionsDialog::checkbox4_4Clicked(int state)
{
  if(state==Qt::Checked)
  {
    mainwindow->use_threads = 1;
  }

  if(state==Qt::Unchecked)
  {
    mainwindow->use_threads = 0;
  }
}


void UI_OptionsDialog::checkbox4_5Clicked(int state)
{
  if(state==Qt::Checked)
  {
    mainwindow->check_for_updates = 1;
  }

  if(state==Qt::Unchecked)
  {
    mainwindow->check_for_updates = 0;
  }
}


void UI_OptionsDialog::DefaultButtonClicked()
{
  int i;

  QPalette palette;

  mainwindow->maincurve->backgroundcolor = Qt::gray;
  BgColorButton->setColor(mainwindow->maincurve->backgroundcolor);

  mainwindow->maincurve->small_ruler_color = Qt::black;
  SrColorButton->setColor(mainwindow->maincurve->small_ruler_color);

  mainwindow->maincurve->big_ruler_color = Qt::darkGray;
  BrColorButton->setColor(mainwindow->maincurve->big_ruler_color);

  mainwindow->maincurve->mouse_rect_color = Qt::black;
  MrColorButton->setColor(mainwindow->maincurve->mouse_rect_color);

  mainwindow->maincurve->text_color = Qt::black;
  TxtColorButton->setColor(mainwindow->maincurve->text_color);

  mainwindow->maincurve->signal_color = Qt::blue;
  SigColorButton->setColor(Qt::blue);

  mainwindow->maincurve->baseline_color = Qt::darkGray;
  BaseColorButton->setColor(Qt::darkGray);
  mainwindow->show_baselines = 1;
  checkbox3->setCheckState(Qt::Checked);

  mainwindow->maincurve->crosshair_1.color = Qt::red;
  Crh1ColorButton->setColor(Qt::red);

  mainwindow->maincurve->crosshair_2.color = Qt::cyan;
  Crh2ColorButton->setColor(Qt::cyan);

  mainwindow->maincurve->floating_ruler_color = Qt::red;
  FrColorButton->setColor(Qt::red);

  mainwindow->maincurve->annot_marker_color = Qt::white;
  AnnotMkrButton->setColor(Qt::white);
  mainwindow->show_annot_markers = 1;
  checkbox2->setCheckState(Qt::Checked);

  palette.setColor(QPalette::Text, mainwindow->maincurve->text_color);
  palette.setColor(QPalette::Base, mainwindow->maincurve->backgroundcolor);

  for(i=0; i<mainwindow->files_open; i++)
  {
    if(mainwindow->annotations_dock[i])
    {
      mainwindow->annotations_dock[i]->list->setPalette(palette);
    }
  }

  for(i=0; i<mainwindow->signalcomps; i++)
  {
    mainwindow->signalcomp[i]->color = mainwindow->maincurve->signal_color;
  }

  mainwindow->maincurve->blackwhite_printing = 1;

  checkbox1->setCheckState(Qt::Checked);

  checkbox4->setCheckState(Qt::Unchecked);

  mainwindow->maincurve->update();
}



void UI_OptionsDialog::BgColorButtonClicked(SpecialButton *)
{
  int i;

  QColor temp;

  QPalette palette;

  temp = QColorDialog::getColor(mainwindow->maincurve->backgroundcolor, tab1);

  if(temp.isValid())
  {
    mainwindow->maincurve->backgroundcolor = temp;

    BgColorButton->setColor(mainwindow->maincurve->backgroundcolor);

    palette.setColor(QPalette::Text, mainwindow->maincurve->text_color);
    palette.setColor(QPalette::Base, mainwindow->maincurve->backgroundcolor);

    for(i=0; i<mainwindow->files_open; i++)
    {
      if(mainwindow->annotations_dock[i])
      {
        mainwindow->annotations_dock[i]->list->setPalette(palette);
      }
    }

    mainwindow->maincurve->update();
  }
}



void UI_OptionsDialog::SrColorButtonClicked(SpecialButton *)
{
  QColor temp;

  temp = QColorDialog::getColor(mainwindow->maincurve->small_ruler_color, tab1);

  if(temp.isValid())
  {
    mainwindow->maincurve->small_ruler_color = temp;

    SrColorButton->setColor(mainwindow->maincurve->small_ruler_color);

    mainwindow->maincurve->update();
  }
}



void UI_OptionsDialog::BrColorButtonClicked(SpecialButton *)
{
  QColor temp;

  temp = QColorDialog::getColor(mainwindow->maincurve->big_ruler_color, tab1);

  if(temp.isValid())
  {
    mainwindow->maincurve->big_ruler_color = temp;

    BrColorButton->setColor(mainwindow->maincurve->big_ruler_color);

    mainwindow->maincurve->update();
  }
}



void UI_OptionsDialog::MrColorButtonClicked(SpecialButton *)
{
  QColor temp;

  temp = QColorDialog::getColor(mainwindow->maincurve->mouse_rect_color, tab1);

  if(temp.isValid())
  {
    mainwindow->maincurve->mouse_rect_color = temp;

    MrColorButton->setColor(mainwindow->maincurve->mouse_rect_color);

    mainwindow->maincurve->update();
  }
}



void UI_OptionsDialog::TxtColorButtonClicked(SpecialButton *)
{
  int i;

  QColor temp;

  QPalette palette;

  temp = QColorDialog::getColor(mainwindow->maincurve->text_color, tab1);

  if(temp.isValid())
  {
    mainwindow->maincurve->text_color = temp;

    TxtColorButton->setColor(mainwindow->maincurve->text_color);

    palette.setColor(QPalette::Text, mainwindow->maincurve->text_color);
    palette.setColor(QPalette::Base, mainwindow->maincurve->backgroundcolor);

    for(i=0; i<mainwindow->files_open; i++)
    {
      if(mainwindow->annotationlist[i])
      {
        mainwindow->annotations_dock[i]->list->setPalette(palette);
      }
    }

    mainwindow->maincurve->update();
  }
}



void UI_OptionsDialog::SigColorButtonClicked(SpecialButton *)
{
  int i, color;

  UI_ColorMenuDialog colormenudialog(&color);

  if(color < 0)  return;

  SigColorButton->setColor((Qt::GlobalColor)color);

  mainwindow->maincurve->signal_color = color;

  for(i=0; i<mainwindow->signalcomps; i++)
  {
    mainwindow->signalcomp[i]->color = color;
  }

  mainwindow->maincurve->update();
}



void UI_OptionsDialog::BaseColorButtonClicked(SpecialButton *)
{
  QColor temp;

  temp = QColorDialog::getColor(mainwindow->maincurve->baseline_color, tab1);

  if(temp.isValid())
  {
    mainwindow->maincurve->baseline_color = temp;

    BaseColorButton->setColor(mainwindow->maincurve->baseline_color);

    mainwindow->maincurve->update();
  }
}



void UI_OptionsDialog::Crh1ColorButtonClicked(SpecialButton *)
{
  int color;

  UI_ColorMenuDialog colormenudialog(&color);

  if(color < 0)  return;

  Crh1ColorButton->setColor((Qt::GlobalColor)color);

  mainwindow->maincurve->crosshair_1.color = color;

  mainwindow->maincurve->update();
}



void UI_OptionsDialog::Crh2ColorButtonClicked(SpecialButton *)
{
  int color;

  UI_ColorMenuDialog colormenudialog(&color);

  if(color < 0)  return;

  Crh2ColorButton->setColor((Qt::GlobalColor)color);

  mainwindow->maincurve->crosshair_2.color = color;

  mainwindow->maincurve->update();
}


void UI_OptionsDialog::FrColorButtonClicked(SpecialButton *)
{
  int color;

  UI_ColorMenuDialog colormenudialog(&color);

  if(color < 0)  return;

  FrColorButton->setColor((Qt::GlobalColor)color);

  mainwindow->maincurve->floating_ruler_color = color;

  mainwindow->maincurve->update();
}


void UI_OptionsDialog::AnnotMkrButtonClicked(SpecialButton *)
{
  QColor temp;

  temp = QColorDialog::getColor(mainwindow->maincurve->annot_marker_color, tab1);

  if(temp.isValid())
  {
    mainwindow->maincurve->annot_marker_color = temp;

    AnnotMkrButton->setColor(mainwindow->maincurve->annot_marker_color);

    mainwindow->maincurve->update();
  }
}





