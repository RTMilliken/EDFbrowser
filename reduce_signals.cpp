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



#include "reduce_signals.h"



#if defined(__APPLE__) || defined(__MACH__) || defined(__APPLE_CC__)

#define fopeno fopen

#else

#define fseeko fseeko64
#define ftello ftello64
#define fopeno fopen64

#endif




UI_ReduceSignalsWindow::UI_ReduceSignalsWindow(QWidget *w_parent)
{
  mainwindow = (UI_Mainwindow *)w_parent;

  recent_savedir = mainwindow->recent_savedir;

  myobjectDialog = new QDialog;

  myobjectDialog->setMinimumSize(665, 526);
  myobjectDialog->setMaximumSize(665, 526);
  myobjectDialog->setWindowTitle("Reduce signals and/or duration");
  myobjectDialog->setModal(TRUE);
  myobjectDialog->setAttribute(Qt::WA_DeleteOnClose, TRUE);

  label1 = new QLabel(myobjectDialog);
  label1->setGeometry(20, 20, 635, 25);

  label2 = new QLabel(myobjectDialog);
  label2->setGeometry(435, 307, 100, 25);
  label2->setText("from datarecord");
  label2->setEnabled(FALSE);

  label3 = new QLabel(myobjectDialog);
  label3->setGeometry(435, 372, 100, 25);
  label3->setText("to datarecord");
  label3->setEnabled(FALSE);

  label4 = new QLabel(myobjectDialog);
  label4->setGeometry(555, 332, 100, 25);
  label4->setEnabled(FALSE);

  label5 = new QLabel(myobjectDialog);
  label5->setGeometry(555, 397, 100, 25);
  label5->setEnabled(FALSE);

  radioButton1 = new QRadioButton("whole duration", myobjectDialog);
  radioButton1->setGeometry(435, 247, 100, 25);
  radioButton1->setChecked(TRUE);
  radioButton1->setEnabled(FALSE);

  radioButton2 = new QRadioButton("selection", myobjectDialog);
  radioButton2->setGeometry(435, 272, 100, 25);
  radioButton2->setEnabled(FALSE);

  spinBox1 = new QSpinBox(myobjectDialog);
  spinBox1->setGeometry(435, 332, 100, 25);
  spinBox1->setRange(1, 2147483647);
  spinBox1->setValue(1);
  spinBox1->setEnabled(FALSE);

  spinBox2 = new QSpinBox(myobjectDialog);
  spinBox2->setGeometry(435, 397, 100, 25);
  spinBox2->setRange(1, 2147483647);
  spinBox2->setValue(2147483647);
  spinBox2->setEnabled(FALSE);

  spinBox3 = new QSpinBox(myobjectDialog);
  spinBox3->setGeometry(545, 170, 100, 25);
  spinBox3->setRange(1, 100000);
  spinBox3->setValue(1);
  spinBox3->setEnabled(FALSE);

  SignalsTablewidget = new QTableWidget(myobjectDialog);
  SignalsTablewidget->setGeometry(20, 66, 355, 380);
  SignalsTablewidget->setSelectionMode(QAbstractItemView::NoSelection);
  SignalsTablewidget->setColumnCount(2);
  SignalsTablewidget->setColumnWidth(0, 180);
  SignalsTablewidget->setColumnWidth(1, 130);

  QStringList horizontallabels;
  horizontallabels += "Label";
  horizontallabels += "Samplerate divider";
  SignalsTablewidget->setHorizontalHeaderLabels(horizontallabels);

  pushButton1 = new QPushButton(myobjectDialog);
  pushButton1->setGeometry(QRect(20, 476, 100, 25));
  pushButton1->setText("Select File");

  pushButton2 = new QPushButton(myobjectDialog);
  pushButton2->setGeometry(545, 476, 100, 25);
  pushButton2->setText("Close");

  pushButton3 = new QPushButton(myobjectDialog);
  pushButton3->setGeometry(200, 476, 100, 25);
  pushButton3->setText("Reduce");
  pushButton3->setEnabled(FALSE);

  pushButton4 = new QPushButton(myobjectDialog);
  pushButton4->setGeometry(395, 66, 140, 25);
  pushButton4->setText("Select all signals");
  pushButton4->setEnabled(FALSE);

  pushButton5 = new QPushButton(myobjectDialog);
  pushButton5->setGeometry(395, 118, 140, 25);
  pushButton5->setText("Deselect all signals");
  pushButton5->setEnabled(FALSE);

  pushButton6 = new QPushButton(myobjectDialog);
  pushButton6->setGeometry(395, 170, 140, 25);
  pushButton6->setText("Set samplerate divider:");
  pushButton6->setEnabled(FALSE);

  QObject::connect(pushButton1,    SIGNAL(clicked()),         this,           SLOT(SelectFileButton()));
  QObject::connect(pushButton2,    SIGNAL(clicked()),         myobjectDialog, SLOT(close()));
  QObject::connect(pushButton3,    SIGNAL(clicked()),         this,           SLOT(StartConversion()));
  QObject::connect(pushButton4,    SIGNAL(clicked()),         this,           SLOT(Select_all_signals()));
  QObject::connect(pushButton5,    SIGNAL(clicked()),         this,           SLOT(Deselect_all_signals()));
  QObject::connect(pushButton6,    SIGNAL(clicked()),         this,           SLOT(Set_SRdivider_all_signals()));
  QObject::connect(spinBox1,       SIGNAL(valueChanged(int)), this,           SLOT(spinBox1changed(int)));
  QObject::connect(spinBox2,       SIGNAL(valueChanged(int)), this,           SLOT(spinBox2changed(int)));
  QObject::connect(radioButton1,   SIGNAL(toggled(bool)),     this,           SLOT(radioButton1Toggled(bool)));
  QObject::connect(radioButton2,   SIGNAL(toggled(bool)),     this,           SLOT(radioButton2Toggled(bool)));

  edfhdr = NULL;
  inputfile = NULL;
  outputfile = NULL;
  file_num = -1;

  inputpath[0] = 0;

  myobjectDialog->exec();
}


void UI_ReduceSignalsWindow::spinBox1changed(int value)
{
  long long seconds,
            milliSec;

  int days;

  char scratchpad[256];

  QObject::disconnect(spinBox2,       SIGNAL(valueChanged(int)), this,           SLOT(spinBox2changed(int)));
  spinBox2->setMinimum(value);
  QObject::connect(spinBox2,       SIGNAL(valueChanged(int)), this,           SLOT(spinBox2changed(int)));

  if(edfhdr == NULL)
  {
    return;
  }
  days = (int)((((value - 1) * edfhdr->long_data_record_duration) / TIME_DIMENSION) / 86400LL);
  seconds = ((value - 1) * edfhdr->long_data_record_duration) / TIME_DIMENSION;
  if(seconds < 0)
  {
    seconds = 0;
  }
  seconds %= 86400LL;
  milliSec = ((value - 1) * edfhdr->long_data_record_duration) % TIME_DIMENSION;
  milliSec /= 10000LL;
  if(days)
  {
    sprintf(scratchpad, "%id %i:%02i:%02i.%03i", days, (int)(seconds / 3600), (int)((seconds % 3600) / 60), (int)(seconds % 60), (int)milliSec);
  }
  else
  {
    sprintf(scratchpad, "%i:%02i:%02i.%03i", (int)(seconds / 3600), (int)((seconds % 3600) / 60), (int)(seconds % 60), (int)milliSec);
  }
  label4->setText(scratchpad);
}


void UI_ReduceSignalsWindow::spinBox2changed(int value)
{
  long long seconds,
            milliSec;

  int days;

  char scratchpad[256];

  QObject::disconnect(spinBox1,       SIGNAL(valueChanged(int)), this,           SLOT(spinBox1changed(int)));
  spinBox1->setMaximum(value);
  QObject::connect(spinBox1,       SIGNAL(valueChanged(int)), this,           SLOT(spinBox1changed(int)));

  if(edfhdr == NULL)
  {
    return;
  }
  days = (int)(((value * edfhdr->long_data_record_duration) / TIME_DIMENSION) / 86400LL);
  seconds = (value * edfhdr->long_data_record_duration) / TIME_DIMENSION;
  seconds %= 86400LL;
  milliSec = (value * edfhdr->long_data_record_duration) % TIME_DIMENSION;
  milliSec /= 10000LL;
  if(days)
  {
    sprintf(scratchpad, "%id %i:%02i:%02i.%03i", days, (int)(seconds / 3600), (int)((seconds % 3600) / 60), (int)(seconds % 60), (int)milliSec);
  }
  else
  {
    sprintf(scratchpad, "%i:%02i:%02i.%03i", (int)(seconds / 3600), (int)((seconds % 3600) / 60), (int)(seconds % 60), (int)milliSec);
  }
  label5->setText(scratchpad);
}


void UI_ReduceSignalsWindow::radioButton1Toggled(bool checked)
{
  long long seconds,
            milliSec;

  int days;

  char scratchpad[256];

  if(checked == TRUE)
  {
    spinBox1->setEnabled(FALSE);
    spinBox2->setEnabled(FALSE);
    label2->setEnabled(FALSE);
    label3->setEnabled(FALSE);
    label4->setEnabled(FALSE);
    label5->setEnabled(FALSE);

    if(edfhdr == NULL)
    {
      return;
    }
    spinBox1->setValue(1);
    spinBox2->setMaximum(edfhdr->datarecords);
    spinBox2->setValue(edfhdr->datarecords);
    spinBox1->setMaximum(edfhdr->datarecords);

    days = (int)(((edfhdr->datarecords * edfhdr->long_data_record_duration) / TIME_DIMENSION) / 86400LL);
    seconds = (edfhdr->datarecords * edfhdr->long_data_record_duration) / TIME_DIMENSION;
    seconds %= 86400LL;
    milliSec = (edfhdr->datarecords * edfhdr->long_data_record_duration) % TIME_DIMENSION;
    milliSec /= 10000LL;

    if(days > 0)
    {
      label4->setText("0d 0:00:00.000");

      sprintf(scratchpad, "%id %i:%02i:%02i.%03i", days, (int)(seconds / 3600), (int)((seconds % 3600) / 60), (int)(seconds % 60), (int)milliSec);
    }
    else
    {
      label4->setText("0:00:00.000");

      sprintf(scratchpad, "%i:%02i:%02i.%03i", (int)(seconds / 3600), (int)((seconds % 3600) / 60), (int)(seconds % 60), (int)milliSec);
    }

    label5->setText(scratchpad);
  }
}


void UI_ReduceSignalsWindow::radioButton2Toggled(bool checked)
{
  if(checked == TRUE)
  {
    spinBox1->setEnabled(TRUE);
    spinBox2->setEnabled(TRUE);
    label2->setEnabled(TRUE);
    label3->setEnabled(TRUE);
    label4->setEnabled(TRUE);
    label5->setEnabled(TRUE);
  }
}


void UI_ReduceSignalsWindow::Select_all_signals()
{
  int i;

  if(edfhdr==NULL)
  {
    return;
  }

  for(i=0; i<edfhdr->edfsignals; i++)
  {
    if(!edfhdr->edfparam[i].annotation)
    {
      ((QCheckBox *)(SignalsTablewidget->cellWidget(i, 0)))->setCheckState(Qt::Checked);
    }
  }
}



void UI_ReduceSignalsWindow::Deselect_all_signals()
{
  int i;

  if(edfhdr==NULL)
  {
    return;
  }

  for(i=0; i<edfhdr->edfsignals; i++)
  {
    if(!edfhdr->edfparam[i].annotation)
    {
      ((QCheckBox *)(SignalsTablewidget->cellWidget(i, 0)))->setCheckState(Qt::Unchecked);
    }
  }
}



void UI_ReduceSignalsWindow::Set_SRdivider_all_signals()
{
  int i, j, sr, idx;

  sr = spinBox3->value();

  for(i=0; i<edfhdr->edfsignals; i++)
  {
    if(edfhdr->edfparam[i].annotation)
    {
      continue;
    }

    idx = 0;

    for(j=1; j<=edfhdr->edfparam[i].smp_per_record; j++)
    {
      if(!(edfhdr->edfparam[i].smp_per_record % j))
      {
        if(j == sr)
        {
          if(idx < ((QComboBox *)(SignalsTablewidget->cellWidget(i, 1)))->count())
          {
            ((QComboBox *)(SignalsTablewidget->cellWidget(i, 1)))->setCurrentIndex(idx);
          }

          break;
        }

        idx++;
      }
    }
  }
}



void UI_ReduceSignalsWindow::SelectFileButton()
{
  int i, j,
      days;

  long long seconds,
            milliSec;

  char txt_string[2048],
       str[256];

  label1->clear();
  label4->clear();
  label5->clear();

  SignalsTablewidget->setRowCount(0);

  inputfile = NULL;
  outputfile = NULL;

  inputpath[0] = 0;

  edfhdr = NULL;

  file_num = -1;

  pushButton3->setEnabled(FALSE);
  pushButton4->setEnabled(FALSE);
  pushButton5->setEnabled(FALSE);
  pushButton6->setEnabled(FALSE);
  spinBox1->setEnabled(FALSE);
  spinBox2->setEnabled(FALSE);
  spinBox3->setEnabled(FALSE);
  radioButton1->setChecked(TRUE);
  radioButton1->setEnabled(FALSE);
  radioButton2->setEnabled(FALSE);
  label2->setEnabled(FALSE);
  label3->setEnabled(FALSE);
  label4->setEnabled(FALSE);
  label5->setEnabled(FALSE);

  UI_activeFileChooserWindow afchooser(&file_num, mainwindow);

  if(file_num < 0)
  {
    return;
  }

  edfhdr = mainwindow->edfheaderlist[file_num];

  strcpy(inputpath, edfhdr->filename);

  inputfile = edfhdr->file_hdl;
  if(inputfile==NULL)
  {
    snprintf(txt_string, 2048, "Can not open file %s for reading.", inputpath);
    QMessageBox messagewindow(QMessageBox::Critical, "Error", QString::fromLocal8Bit(txt_string));
    messagewindow.exec();

    inputpath[0] = 0;

    edfhdr = NULL;

    file_num = -1;

    return;
  }

  if(edfhdr->datarecords > 2147483647LL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Failure", "This file contains more than 2147483647 datarecords.\n"
                                                                "This tool can not handle more than 2147483647 datarecords.");
    messagewindow.exec();

    inputfile = NULL;

    inputpath[0] = 0;

    edfhdr = NULL;

    file_num = -1;

    return;
  }

/***************** load signalproperties ******************************/

  label1->setText(inputpath);

  SignalsTablewidget->setRowCount(edfhdr->edfsignals);

  for(i=0; i<edfhdr->edfsignals; i++)
  {
    SignalsTablewidget->setRowHeight(i, 20);

    SignalsTablewidget->setCellWidget(i, 0, new QCheckBox(edfhdr->edfparam[i].label));
    ((QCheckBox *)(SignalsTablewidget->cellWidget(i, 0)))->setTristate(FALSE);
    ((QCheckBox *)(SignalsTablewidget->cellWidget(i, 0)))->setCheckState(Qt::Checked);

    if(edfhdr->edfparam[i].annotation)
    {
      ((QCheckBox *)(SignalsTablewidget->cellWidget(i, 0)))->setEnabled(FALSE);
    }

    if(!(edfhdr->edfparam[i].annotation))
    {
      SignalsTablewidget->setCellWidget(i, 1, new QComboBox);
      ((QComboBox *)(SignalsTablewidget->cellWidget(i, 1)))->setEditable(FALSE);

      for(j=1; j<=edfhdr->edfparam[i].smp_per_record; j++)
      {
        if(!(edfhdr->edfparam[i].smp_per_record % j))
        {
          snprintf(str, 256, "%i  (%f", j, ((double)(edfhdr->edfparam[i].smp_per_record / j)) / edfhdr->data_record_duration);
          remove_trailing_zeros(str);
          strcat(str, " Hz)");
          ((QComboBox *)(SignalsTablewidget->cellWidget(i, 1)))->addItem(str, QVariant(j));
        }
      }
    }
  }

  pushButton3->setEnabled(TRUE);
  pushButton4->setEnabled(TRUE);
  pushButton5->setEnabled(TRUE);
  pushButton6->setEnabled(TRUE);

  spinBox1->setValue(1);
  spinBox2->setMaximum(edfhdr->datarecords);
  spinBox2->setValue(edfhdr->datarecords);
  spinBox1->setMaximum(edfhdr->datarecords);
  spinBox3->setEnabled(TRUE);

  radioButton1->setEnabled(TRUE);
  radioButton2->setEnabled(TRUE);

  label4->setText("0:00:00.000");
  days = (int)(((edfhdr->datarecords * edfhdr->long_data_record_duration) / TIME_DIMENSION) / 86400LL);
  seconds = (edfhdr->datarecords * edfhdr->long_data_record_duration) / TIME_DIMENSION;
  seconds %= 86400LL;
  milliSec = (edfhdr->datarecords * edfhdr->long_data_record_duration) % TIME_DIMENSION;
  milliSec /= 10000LL;
  if(days)
  {
    sprintf(txt_string, "%id %i:%02i:%02i.%03i", days, (int)(seconds / 3600), (int)((seconds % 3600) / 60), (int)(seconds % 60), (int)milliSec);
  }
  else
  {
    sprintf(txt_string, "%i:%02i:%02i.%03i", (int)(seconds / 3600), (int)((seconds % 3600) / 60), (int)(seconds % 60), (int)milliSec);
  }
  label5->setText(txt_string);
}



void UI_ReduceSignalsWindow::StartConversion()
{
  int i, j, k,
      new_edfsignals,
      datarecords=0,
      annot_smp_per_record,
      annot_recordsize,
      timestamp_digits=0,
      timestamp_decimals=0,
      annot_len,
      tallen=0,
      len,
      annot_cnt,
      annots_per_datrec=0,
      smplrt,
      tmp,
      progress_steps,
      datrecs_processed;

  char *readbuf=NULL,
       scratchpad[256];

  long long new_starttime,
            time_diff,
            onset_diff,
            taltime,
            l_temp,
            endtime=0,
            l_tmp;

  struct date_time_struct dts;

  struct annotationblock *new_annot_list=NULL,
                         *root_new_annot_list=NULL,
                         *annot_list=NULL;

  union {
          unsigned int one;
          signed int one_signed;
          unsigned short two[2];
          signed short two_signed[2];
          unsigned char four[4];
        } var;

  QProgressDialog progress("Processing file...", "Abort", 0, 1);
  progress.setWindowModality(Qt::WindowModal);
  progress.setMinimumDuration(200);


  pushButton3->setEnabled(FALSE);
  pushButton4->setEnabled(FALSE);
  pushButton5->setEnabled(FALSE);
  pushButton6->setEnabled(FALSE);
  spinBox1->setEnabled(FALSE);
  spinBox2->setEnabled(FALSE);
  spinBox3->setEnabled(FALSE);
  radioButton1->setEnabled(FALSE);
  radioButton2->setEnabled(FALSE);
  label2->setEnabled(FALSE);
  label3->setEnabled(FALSE);

  if(edfhdr==NULL)
  {
    return;
  }

  if(file_num < 0)
  {
    return;
  }

  new_edfsignals = 0;

  annot_smp_per_record = 0;

  annot_cnt = 0;

  time_diff = (long long)(spinBox1->value() - 1) * edfhdr->long_data_record_duration;

  taltime = (time_diff + edfhdr->starttime_offset) % TIME_DIMENSION;

  endtime = (long long)(spinBox2->value() - (spinBox1->value() - 1)) * edfhdr->long_data_record_duration + taltime;

  for(i=0; i<edfhdr->edfsignals; i++)
  {
    if(!edfhdr->edfparam[i].annotation)
    {
      if(((QCheckBox *)(SignalsTablewidget->cellWidget(i, 0)))->checkState()==Qt::Checked)
      {
        signalslist[new_edfsignals] = i;

        dividerlist[new_edfsignals] = ((QComboBox *)(SignalsTablewidget->cellWidget(i, 1)))->itemData(((QComboBox *)(SignalsTablewidget->cellWidget(i, 1)))->currentIndex()).toInt();

        new_edfsignals++;
      }
    }
  }

  datarecords = spinBox2->value() - spinBox1->value() + 1;

  if(edfhdr->edfplus || edfhdr->bdfplus)
  {
    timestamp_decimals = get_tal_timestamp_decimal_cnt(edfhdr);
    if(timestamp_decimals < 0)
    {
      showpopupmessage("Error", "Internal error, get_tal_timestamp_decimal_cnt(");
      goto END_1;
    }

    timestamp_digits = get_tal_timestamp_digit_cnt(edfhdr);
    if(timestamp_digits < 0)
    {
      showpopupmessage("Error", "Internal error, get_tal_timestamp_digit_cnt(");
      goto END_1;
    }

    annot_list = mainwindow->annotationlist[file_num];

    while(annot_list != NULL)
    {
      l_temp = annot_list->onset - time_diff;

      if((l_temp >= 0LL) && (l_temp <= endtime))
      {
        annot_cnt++;

        edfplus_annotation_add_copy(&new_annot_list, annot_list);
      }

      annot_list = annot_list->next_annotation;
    }

    annot_list = new_annot_list;

    root_new_annot_list = new_annot_list;

    new_starttime = edfhdr->utc_starttime + ((time_diff + edfhdr->starttime_offset) / TIME_DIMENSION);

    onset_diff = (new_starttime - edfhdr->utc_starttime) * TIME_DIMENSION;

    while(annot_list != NULL)
    {
      annot_list->onset -= onset_diff;

      annot_list = annot_list->next_annotation;
    }

    edfplus_annotation_sort(&new_annot_list);

    annots_per_datrec = annot_cnt / datarecords;

    if(annot_cnt % datarecords)
    {
      annots_per_datrec++;
    }

    annot_len = get_max_annotation_strlen(&new_annot_list);

    if(!annot_cnt)
    {
      annots_per_datrec = 0;
    }

    annot_recordsize = (annot_len * annots_per_datrec) + timestamp_digits + timestamp_decimals + 4;

    if(timestamp_decimals)
    {
      annot_recordsize++;
    }

    if(edfhdr->edf)
    {
      annot_smp_per_record = annot_recordsize / 2;

      if(annot_recordsize % annot_smp_per_record)
      {
        annot_smp_per_record++;

        annot_recordsize = annot_smp_per_record * 2;
      }
    }
    else
    {
      annot_smp_per_record = annot_recordsize / 3;

      if(annot_recordsize % annot_smp_per_record)
      {
        annot_smp_per_record++;

        annot_recordsize = annot_smp_per_record * 3;
      }
    }
  }
  else
  {
    annot_smp_per_record = 0;

    annot_recordsize = 0;
  }

  readbuf = (char *)malloc(edfhdr->recordsize);
  if(readbuf==NULL)
  {
    showpopupmessage("Error", "Malloc error, (readbuf).");
    goto END_2;
  }

///////////////////////////////////////////////////////////////////

  outputpath[0] = 0;
  if(recent_savedir[0]!=0)
  {
    strcpy(outputpath, recent_savedir);
    strcat(outputpath, "/");
  }
  len = strlen(outputpath);
  get_filename_from_path(outputpath + len, inputpath, MAX_PATH_LENGTH - len);
  remove_extension_from_filename(outputpath);
  if(edfhdr->edf)
  {
    strcat(outputpath, "_reduced.edf");

    strcpy(outputpath, QFileDialog::getSaveFileName(0, "Save file", QString::fromLocal8Bit(outputpath), "EDF files (*.edf *.EDF)").toLocal8Bit().data());
  }
  else
  {
    strcat(outputpath, "_reduced.bdf");

    strcpy(outputpath, QFileDialog::getSaveFileName(0, "Save file", QString::fromLocal8Bit(outputpath), "BDF files (*.bdf *.BDF)").toLocal8Bit().data());
  }

  if(!strcmp(outputpath, ""))
  {
    goto END_2;
  }

  get_directory_from_path(recent_savedir, outputpath, MAX_PATH_LENGTH);

  if(mainwindow->file_is_opened(outputpath))
  {
    showpopupmessage("Reduce signals", "Error, selected file is in use.");
    goto END_2;
  }

  outputfile = fopeno(outputpath, "wb");
  if(outputfile==NULL)
  {
    showpopupmessage("Error", "Can not open outputfile for writing.");
    goto END_2;
  }

  new_starttime = edfhdr->utc_starttime + ((time_diff + edfhdr->starttime_offset) / TIME_DIMENSION);

  utc_to_date_time(new_starttime, &dts);

  rewind(inputfile);
  if(fread(scratchpad, 168, 1, inputfile)!=1)
  {
    showpopupmessage("Error", "Read error (1).");
    goto END_3;
  }

  if(edfhdr->edfplus || edfhdr->bdfplus)
  {
    if(scratchpad[98] != 'X')
    {
      sprintf(scratchpad + 98, "%02i-%s-%04i", dts.day, dts.month_str, dts.year);

      scratchpad[109] = ' ';
    }
  }

  if(fwrite(scratchpad, 168, 1, outputfile)!=1)
  {
    showpopupmessage("Error", "Write error (1).");
    goto END_3;
  }

  fprintf(outputfile, "%02i.%02i.%02i%02i.%02i.%02i",
          dts.day,
          dts.month,
          dts.year % 100,
          dts.hour,
          dts.minute,
          dts.second);

  if(edfhdr->edfplus || edfhdr->bdfplus)
  {
    fprintf(outputfile, "%-8i", new_edfsignals * 256 + 512);
  }
  else
  {
    fprintf(outputfile, "%-8i", new_edfsignals * 256 + 256);
  }
  if(edfhdr->edfplus)
  {
    fprintf(outputfile, "EDF+C");
    for(i=0; i<39; i++)
    {
      fputc(' ', outputfile);
    }
  }
  if(edfhdr->bdfplus)
  {
    fprintf(outputfile, "BDF+C");
    for(i=0; i<39; i++)
    {
      fputc(' ', outputfile);
    }
  }
  if((!edfhdr->edfplus) && (!edfhdr->bdfplus))
  {
    for(i=0; i<44; i++)
    {
      fputc(' ', outputfile);
    }
  }
  fprintf(outputfile, "%-8i", datarecords);
  snprintf(scratchpad, 256, "%f", edfhdr->data_record_duration);
  convert_trailing_zeros_to_spaces(scratchpad);
  if(scratchpad[7]=='.')
  {
    scratchpad[7] = ' ';
  }
  scratchpad[8] = 0;

  fprintf(outputfile, "%s", scratchpad);
  if(edfhdr->edfplus || edfhdr->bdfplus)
  {
    fprintf(outputfile, "%-4i", new_edfsignals + 1);
  }
  else
  {
    fprintf(outputfile, "%-4i", new_edfsignals);
  }

  for(i=0; i<new_edfsignals; i++)
  {
    fprintf(outputfile, "%s", edfhdr->edfparam[signalslist[i]].label);
  }
  if(edfhdr->edfplus)
  {
    fprintf(outputfile, "EDF Annotations ");
  }
  if(edfhdr->bdfplus)
  {
    fprintf(outputfile, "BDF Annotations ");
  }
  for(i=0; i<new_edfsignals; i++)
  {
    fprintf(outputfile, "%s", edfhdr->edfparam[signalslist[i]].transducer);
  }
  if(edfhdr->edfplus || edfhdr->bdfplus)
  {
    for(i=0; i<80; i++)
    {
      fputc(' ', outputfile);
    }
  }
  for(i=0; i<new_edfsignals; i++)
  {
    fprintf(outputfile, "%s", edfhdr->edfparam[signalslist[i]].physdimension);
  }
  if(edfhdr->edfplus || edfhdr->bdfplus)
  {
    for(i=0; i<8; i++)
    {
      fputc(' ', outputfile);
    }
  }
  for(i=0; i<new_edfsignals; i++)
  {
    snprintf(scratchpad, 256, "%f", edfhdr->edfparam[signalslist[i]].phys_min);
    convert_trailing_zeros_to_spaces(scratchpad);
    if(scratchpad[7]=='.')
    {
      scratchpad[7] = ' ';
    }
    scratchpad[8] = 0;
    fprintf(outputfile, "%s", scratchpad);
  }
  if(edfhdr->edfplus || edfhdr->bdfplus)
  {
    fprintf(outputfile, "-1      ");
  }
  for(i=0; i<new_edfsignals; i++)
  {
    snprintf(scratchpad, 256, "%f", edfhdr->edfparam[signalslist[i]].phys_max);
    convert_trailing_zeros_to_spaces(scratchpad);
    if(scratchpad[7]=='.')
    {
      scratchpad[7] = ' ';
    }
    scratchpad[8] = 0;
    fprintf(outputfile, "%s", scratchpad);
  }
  if(edfhdr->edfplus || edfhdr->bdfplus)
  {
    fprintf(outputfile, "1       ");
  }
  for(i=0; i<new_edfsignals; i++)
  {
    fprintf(outputfile, "%-8i", edfhdr->edfparam[signalslist[i]].dig_min);
  }
  if(edfhdr->edfplus)
  {
    fprintf(outputfile, "-32768  ");
  }
  if(edfhdr->bdfplus)
  {
    fprintf(outputfile, "-8388608");
  }
  for(i=0; i<new_edfsignals; i++)
  {
    fprintf(outputfile, "%-8i", edfhdr->edfparam[signalslist[i]].dig_max);
  }
  if(edfhdr->edfplus)
  {
    fprintf(outputfile, "32767   ");
  }
  if(edfhdr->bdfplus)
  {
    fprintf(outputfile, "8388607 ");
  }
  for(i=0; i<new_edfsignals; i++)
  {
    fprintf(outputfile, "%s", edfhdr->edfparam[signalslist[i]].prefilter);
  }
  if(edfhdr->edfplus || edfhdr->bdfplus)
  {
    for(i=0; i<80; i++)
    {
      fputc(' ', outputfile);
    }
  }
  for(i=0; i<new_edfsignals; i++)
  {
    fprintf(outputfile, "%-8i", edfhdr->edfparam[signalslist[i]].smp_per_record / dividerlist[i]);
  }
  if(edfhdr->edfplus || edfhdr->bdfplus)
  {
    fprintf(outputfile, "%-8i", annot_smp_per_record);
  }
  for(i=0; i<(new_edfsignals * 32); i++)
  {
   fputc(' ', outputfile);
  }
  if(edfhdr->edfplus || edfhdr->bdfplus)
  {
    for(i=0; i<32; i++)
    {
      fputc(' ', outputfile);
    }
  }
///////////////////////////////////////////////////////////////////

  progress.setRange(0, datarecords);
  progress.setValue(0);

  progress_steps = datarecords / 100;
  if(progress_steps < 1)
  {
    progress_steps = 1;
  }

  fseeko(inputfile, (long long)edfhdr->hdrsize + ((long long)(spinBox1->value() - 1) * (long long)edfhdr->recordsize), SEEK_SET);

  for(datrecs_processed=0; datrecs_processed<datarecords; datrecs_processed++)
  {
    if(!(datrecs_processed % progress_steps))
    {
      progress.setValue(datrecs_processed);

      qApp->processEvents();

      if(progress.wasCanceled() == TRUE)
      {
        goto END_3;
      }
    }

    if(fread(readbuf, edfhdr->recordsize, 1, inputfile) != 1)
    {
      progress.reset();
      showpopupmessage("Error", "Read error (2).");
      goto END_3;
    }

    if(edfhdr->edf)
    {
      for(i=0; i<new_edfsignals; i++)
      {
        if(dividerlist[i] == 1)
        {
          smplrt = edfhdr->edfparam[signalslist[i]].smp_per_record;

          for(j=0; j<smplrt; j++)
          {
            fputc(readbuf[edfhdr->edfparam[signalslist[i]].buf_offset + (j * 2)], outputfile);
            if(fputc(readbuf[edfhdr->edfparam[signalslist[i]].buf_offset + (j * 2) + 1], outputfile)==EOF)
            {
              progress.reset();
              showpopupmessage("Error", "Write error (4).");
              goto END_3;
            }
          }
        }
        else
        {
          smplrt = edfhdr->edfparam[signalslist[i]].smp_per_record / dividerlist[i];

          for(j=0; j<smplrt; j++)
          {
            tmp = 0;

            for(k=0; k<dividerlist[i]; k++)
            {
              tmp += *(((signed short *)(readbuf + edfhdr->edfparam[signalslist[i]].buf_offset)) + (dividerlist[i] * j) + k);
            }

            tmp /= dividerlist[i];

            fputc(tmp & 0xff, outputfile);
            if(fputc((tmp >> 8) & 0xff, outputfile)==EOF)
            {
              progress.reset();
              showpopupmessage("Error", "Write error (4).");
              goto END_3;
            }
          }
        }
      }
    }
    else
    {
      for(i=0; i<new_edfsignals; i++)
      {
        if(dividerlist[i] == 1)
        {
          smplrt = edfhdr->edfparam[signalslist[i]].smp_per_record;

          for(j=0; j<smplrt; j++)
          {
            fputc(readbuf[edfhdr->edfparam[signalslist[i]].buf_offset + (j * 3)], outputfile);
            fputc(readbuf[edfhdr->edfparam[signalslist[i]].buf_offset + (j * 3) + 1], outputfile);
            if(fputc(readbuf[edfhdr->edfparam[signalslist[i]].buf_offset + (j * 3) + 2], outputfile)==EOF)
            {
              progress.reset();
              showpopupmessage("Error", "Write error (4).");
              goto END_3;
            }
          }
        }
        else
        {
          smplrt = edfhdr->edfparam[signalslist[i]].smp_per_record / dividerlist[i];

          for(j=0; j<smplrt; j++)
          {
            l_tmp = 0LL;

            for(k=0; k<dividerlist[i]; k++)
            {
              var.two[0] = *((unsigned short *)(readbuf + edfhdr->edfparam[signalslist[i]].buf_offset + (dividerlist[i] * j * 3) + (k * 3)));
              var.four[2] = *((unsigned char *)(readbuf + edfhdr->edfparam[signalslist[i]].buf_offset + (dividerlist[i] * j * 3) + (k * 3) + 2));

              if(var.four[2]&0x80)
              {
                var.four[3] = 0xff;
              }
              else
              {
                var.four[3] = 0x00;
              }

              l_tmp += var.one_signed;
            }

            l_tmp /= dividerlist[i];

            fputc(l_tmp & 0xff, outputfile);
            fputc((l_tmp >> 8) & 0xff, outputfile);
            if(fputc((l_tmp >> 16) & 0xff, outputfile)==EOF)
            {
              progress.reset();
              showpopupmessage("Error", "Write error (4).");
              goto END_3;
            }
          }
        }
      }
    }

    if(edfhdr->edfplus || edfhdr->bdfplus)
    {
      switch(timestamp_decimals)
      {
        case 0 : tallen = fprintf(outputfile, "+%i", (int)(taltime / TIME_DIMENSION));
                  break;
        case 1 : tallen = fprintf(outputfile, "+%i.%01i", (int)(taltime / TIME_DIMENSION), (int)((taltime % TIME_DIMENSION) / 1000000LL));
                  break;
        case 2 : tallen = fprintf(outputfile, "+%i.%02i", (int)(taltime / TIME_DIMENSION), (int)((taltime % TIME_DIMENSION) / 100000LL));
                  break;
        case 3 : tallen = fprintf(outputfile, "+%i.%03i", (int)(taltime / TIME_DIMENSION), (int)((taltime % TIME_DIMENSION) / 10000LL));
                  break;
        case 4 : tallen = fprintf(outputfile, "+%i.%04i", (int)(taltime / TIME_DIMENSION), (int)((taltime % TIME_DIMENSION) / 1000LL));
                  break;
        case 5 : tallen = fprintf(outputfile, "+%i.%05i", (int)(taltime / TIME_DIMENSION), (int)((taltime % TIME_DIMENSION) / 100LL));
                  break;
        case 6 : tallen = fprintf(outputfile, "+%i.%06i", (int)(taltime / TIME_DIMENSION), (int)((taltime % TIME_DIMENSION) / 10LL));
                  break;
        case 7 : tallen = fprintf(outputfile, "+%i.%07i", (int)(taltime / TIME_DIMENSION), (int)(taltime % TIME_DIMENSION));
                  break;
      }

      fputc(20, outputfile);
      fputc(20, outputfile);
      fputc(0, outputfile);

      tallen += 3;

      if(new_annot_list != NULL)
      {
        for(i=0; i<annots_per_datrec; i++)
        {
          if(new_annot_list != NULL)
          {
            len = snprintf(scratchpad, 256, "%+i.%07i",
            (int)(new_annot_list->onset / TIME_DIMENSION),
            (int)(new_annot_list->onset % TIME_DIMENSION));

            for(j=0; j<7; j++)
            {
              if(scratchpad[len - j - 1] != '0')
              {
                break;
              }
            }

            if(j)
            {
              len -= j;

              if(j == 7)
              {
                len--;
              }
            }

            if(fwrite(scratchpad, len, 1, outputfile) != 1)
            {
              progress.reset();
              showpopupmessage("Error", "Write error (5).");
              goto END_3;
            }

            tallen += len;

            if(new_annot_list->duration[0]!=0)
            {
              fputc(21, outputfile);
              tallen++;

              tallen += fprintf(outputfile, "%s", new_annot_list->duration);
            }

            fputc(20, outputfile);
            tallen++;

            tallen += fprintf(outputfile, "%s", new_annot_list->annotation);

            fputc(20, outputfile);
            fputc(0, outputfile);
            tallen += 2;

            new_annot_list = new_annot_list->next_annotation;
          }
        }
      }

      for(k=tallen; k<annot_recordsize; k++)
      {
        fputc(0, outputfile);
      }

      taltime += edfhdr->long_data_record_duration;
    }
  }

  progress.reset();
  showpopupmessage("Ready", "Done.");

END_3:

  fclose(outputfile);
  outputfile = NULL;

END_2:

  if(readbuf != NULL)
  {
    free(readbuf);
    readbuf = NULL;
  }

END_1:

  inputfile = NULL;

  inputpath[0] = 0;

  label1->clear();
  label4->clear();
  label5->clear();

  file_num = -1;

  edfhdr = NULL;

  edfplus_annotation_delete_list(&root_new_annot_list);

  root_new_annot_list = NULL;

  SignalsTablewidget->setRowCount(0);
}


void UI_ReduceSignalsWindow::showpopupmessage(const char *str1, const char *str2)
{
  UI_Messagewindow popupmessage(str1, str2);
}



















