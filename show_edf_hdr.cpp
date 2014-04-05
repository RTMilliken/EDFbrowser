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



#include "show_edf_hdr.h"



UI_EDFhdrwindow::UI_EDFhdrwindow(QWidget *w_parent)
{
  int i;

  edfplus_layout = 0;

  mainwindow = (UI_Mainwindow *)w_parent;

  EDFhdrDialog = new QDialog;

  EDFhdrDialog->setMinimumSize(QSize(690, 480));
  EDFhdrDialog->setMaximumSize(QSize(690, 480));

  EDFhdrDialog->setWindowTitle("File Info");
  EDFhdrDialog->setModal(true);
  EDFhdrDialog->setAttribute(Qt::WA_DeleteOnClose, true);

  filelist = new QListWidget(EDFhdrDialog);
  filelist->setGeometry(QRect(10, 10, 670, 75));
  for(i=0; i<mainwindow->files_open; i++)
  {
    new QListWidgetItem(QString::fromLocal8Bit(mainwindow->edfheaderlist[i]->filename), filelist);
  }

  label1 = new QLabel(EDFhdrDialog);
  label1->setGeometry(QRect(10, 120, 80, 25));
  label1->setText("Subject");

  label1a = new SpecialButton(EDFhdrDialog);
  label1a->setGeometry(QRect(100, 120, 580, 25));
  label1a->setColor(Qt::white);

  label2 = new QLabel(EDFhdrDialog);
  label2->setGeometry(QRect(10, 155, 80, 25));
  label2->setText("Recording");

  label2a = new SpecialButton(EDFhdrDialog);
  label2a->setGeometry(QRect(100, 155, 580, 25));
  label2a->setColor(Qt::white);

  label3 = new QLabel(EDFhdrDialog);
  label3->setGeometry(QRect(10, 190, 80, 25));
  label3->setText("Start");

  label3a = new SpecialButton(EDFhdrDialog);
  label3a->setGeometry(QRect(100, 190, 180, 25));
  label3a->setColor(Qt::white);

  label4 = new QLabel(EDFhdrDialog);
  label4->setGeometry(QRect(410, 190, 80, 25));
  label4->setText("Duration");

  label4a = new SpecialButton(EDFhdrDialog);
  label4a->setGeometry(QRect(500, 190, 180, 25));
  label4a->setColor(Qt::white);

  label5 = new QLabel(EDFhdrDialog);
  label5->setGeometry(QRect(10, 225, 80, 25));
  label5->setText("Reserved");

  label5a = new SpecialButton(EDFhdrDialog);
  label5a->setGeometry(QRect(100, 225, 350, 25));
  label5a->setColor(Qt::white);

  label6 = new QLabel(EDFhdrDialog);
  label6->setGeometry(QRect(10, 190, 130, 25));
  label6->setText("Birthdate");
  label6->setVisible(false);

  label6a = new SpecialButton(EDFhdrDialog);
  label6a->setGeometry(QRect(150, 190, 530, 25));
  label6a->setColor(Qt::white);
  label6a->setVisible(false);

  label7 = new QLabel(EDFhdrDialog);
  label7->setGeometry(QRect(10, 225, 130, 25));
  label7->setText("Subject name");
  label7->setVisible(false);

  label7a = new SpecialButton(EDFhdrDialog);
  label7a->setGeometry(QRect(150, 225, 530, 25));
  label7a->setColor(Qt::white);
  label7a->setVisible(false);

  label8 = new QLabel(EDFhdrDialog);
  label8->setGeometry(QRect(10, 260, 130, 25));
  label8->setText("Additional info");
  label8->setVisible(false);

  label8a = new SpecialButton(EDFhdrDialog);
  label8a->setGeometry(QRect(150, 260, 530, 25));
  label8a->setColor(Qt::white);
  label8a->setVisible(false);

  label9 = new QLabel(EDFhdrDialog);
  label9->setGeometry(QRect(10, 290, 130, 25));
  label9->setText("Startdate");
  label9->setVisible(false);

  label9a = new SpecialButton(EDFhdrDialog);
  label9a->setGeometry(QRect(150, 290, 530, 25));
  label9a->setColor(Qt::white);
  label9a->setVisible(false);

  label10 = new QLabel(EDFhdrDialog);
  label10->setGeometry(QRect(10, 325, 130, 25));
  label10->setText("Administr. code");
  label10->setVisible(false);

  label10a = new SpecialButton(EDFhdrDialog);
  label10a->setGeometry(QRect(150, 325, 530, 25));
  label10a->setColor(Qt::white);
  label10a->setVisible(false);

  label11 = new QLabel(EDFhdrDialog);
  label11->setGeometry(QRect(10, 360, 130, 25));
  label11->setText("Technician");
  label11->setVisible(false);

  label11a = new SpecialButton(EDFhdrDialog);
  label11a->setGeometry(QRect(150, 360, 530, 25));
  label11a->setColor(Qt::white);
  label11a->setVisible(false);

  label12 = new QLabel(EDFhdrDialog);
  label12->setGeometry(QRect(10, 395, 130, 25));
  label12->setText("Device");
  label12->setVisible(false);

  label12a = new SpecialButton(EDFhdrDialog);
  label12a->setGeometry(QRect(150, 395, 530, 25));
  label12a->setColor(Qt::white);
  label12a->setVisible(false);

  label13 = new QLabel(EDFhdrDialog);
  label13->setGeometry(QRect(10, 430, 130, 25));
  label13->setText("Additional info");
  label13->setVisible(false);

  label13a = new SpecialButton(EDFhdrDialog);
  label13a->setGeometry(QRect(150, 430, 530, 25));
  label13a->setColor(Qt::white);
  label13a->setVisible(false);

  label20 = new QLabel(EDFhdrDialog);
  label20->setGeometry(QRect(10, 260, 130, 25));
  label20->setText("Datarecord duration");

  label20a = new SpecialButton(EDFhdrDialog);
  label20a->setGeometry(QRect(150, 260, 100, 25));
  label20a->setColor(Qt::white);

  label21 = new QLabel(EDFhdrDialog);
  label21->setGeometry(QRect(410, 260, 80, 25));
  label21->setText("Version");

  label21a = new SpecialButton(EDFhdrDialog);
  label21a->setGeometry(QRect(500, 260, 180, 25));
  label21a->setColor(Qt::white);

  signallist = new QTableWidget(EDFhdrDialog);
  signallist->setGeometry(QRect(10, 295, 670, 100));
  signallist->setSelectionMode(QAbstractItemView::NoSelection);
  signallist->setEditTriggers(QAbstractItemView::NoEditTriggers);

  pushButton1 = new QPushButton(EDFhdrDialog);
  pushButton1->setGeometry(QRect(580, 445, 100, 25));
  pushButton1->setText("Close");

  QObject::connect(pushButton1, SIGNAL(clicked()),              EDFhdrDialog, SLOT(close()));
  QObject::connect(filelist,    SIGNAL(currentRowChanged(int)), this,         SLOT(show_params(int)));

  filelist->setCurrentRow(mainwindow->files_open - 1);

  EDFhdrDialog->exec();
}


void UI_EDFhdrwindow::show_params(int row)
{
  int i,
      signal_cnt;

  char str[512];

  long long file_duration;

  struct date_time_struct date_time;


  if(row<0)  return;

  if(mainwindow->edfheaderlist[row]->edfplus || mainwindow->edfheaderlist[row]->bdfplus)
  {
    edfplus_layout = 1;

    EDFhdrDialog->setMinimumSize(QSize(690, 765));
    EDFhdrDialog->setMaximumSize(QSize(690, 765));

    pushButton1->setGeometry(QRect(580, 730, 100, 25));

    signallist->setGeometry(QRect(10, 580, 670, 100));

    label20a->setGeometry(QRect(150, 545, 100, 25));

    label20->setGeometry(QRect(10, 545, 130, 25));

    label21->setGeometry(QRect(410, 545, 80, 25));

    label21a->setGeometry(QRect(500, 545, 180, 25));

    label5a->setGeometry(QRect(150, 500, 350, 25));

    label5->setGeometry(QRect(10, 500, 80, 25));

    label4a->setGeometry(QRect(500, 465, 180, 25));

    label4->setGeometry(QRect(410, 465, 80, 25));

    label3a->setGeometry(QRect(150, 465, 180, 25));

    label3->setGeometry(QRect(10, 465, 80, 25));

    label1a->setGeometry(QRect(150, 120, 530, 25));

    label1a->setText(mainwindow->edfheaderlist[row]->plus_patientcode);

    label1->setGeometry(QRect(10, 120, 130, 25));
    label1->setText("Subject code");

    label2a->setGeometry(QRect(150, 155, 530, 25));
    label2a->setText(mainwindow->edfheaderlist[row]->plus_gender);

    label2->setGeometry(QRect(10, 155, 130, 25));
    label2->setText("Sex");

    label6->setVisible(true);
    label6a->setVisible(true);
    label6a->setText(mainwindow->edfheaderlist[row]->plus_birthdate);

    label7->setVisible(true);
    label7a->setVisible(true);
    label7a->setText(mainwindow->edfheaderlist[row]->plus_patient_name);

    label8->setVisible(true);
    label8a->setVisible(true);
    label8a->setText(mainwindow->edfheaderlist[row]->plus_patient_additional);

    label9->setVisible(true);
    label9a->setVisible(true);
    label9a->setText(mainwindow->edfheaderlist[row]->plus_startdate);

    label10->setVisible(true);
    label10a->setVisible(true);
    label10a->setText(mainwindow->edfheaderlist[row]->plus_admincode);

    label11->setVisible(true);
    label11a->setVisible(true);
    label11a->setText(mainwindow->edfheaderlist[row]->plus_technician);

    label12->setVisible(true);
    label12a->setVisible(true);
    label12a->setText(mainwindow->edfheaderlist[row]->plus_equipment);

    label13->setVisible(true);
    label13a->setVisible(true);
    label13a->setText(mainwindow->edfheaderlist[row]->plus_recording_additional);
  }
  else
  {
    label6->setVisible(false);
    label6a->setVisible(false);
    label7->setVisible(false);
    label7a->setVisible(false);
    label8->setVisible(false);
    label8a->setVisible(false);
    label9->setVisible(false);
    label9a->setVisible(false);
    label10->setVisible(false);
    label10a->setVisible(false);
    label11->setVisible(false);
    label11a->setVisible(false);
    label12->setVisible(false);
    label12a->setVisible(false);
    label13->setVisible(false);
    label13a->setVisible(false);

    EDFhdrDialog->setMinimumSize(QSize(690, 480));
    EDFhdrDialog->setMaximumSize(QSize(690, 480));

    label1->setGeometry(QRect(10, 120, 80, 25));
    label1->setText("Subject");
    label1a->setGeometry(QRect(100, 120, 580, 25));
    label1a->setText(mainwindow->edfheaderlist[row]->patient);
    label2->setGeometry(QRect(10, 155, 80, 25));
    label2->setText("Recording");
    label2a->setGeometry(QRect(100, 155, 580, 25));
    label2a->setText(mainwindow->edfheaderlist[row]->recording);
    label3a->setGeometry(QRect(100, 190, 180, 25));
    label3->setGeometry(QRect(10, 190, 80, 25));
    label4a->setGeometry(QRect(500, 190, 180, 25));
    label4->setGeometry(QRect(410, 190, 80, 25));
    label5a->setGeometry(QRect(100, 225, 350, 25));
    label5->setGeometry(QRect(10, 225, 80, 25));
    label20->setGeometry(QRect(10, 260, 130, 25));
    label20a->setGeometry(QRect(150, 260, 100, 25));
    label21->setGeometry(QRect(410, 260, 80, 25));
    label21a->setGeometry(QRect(500, 260, 180, 25));
    signallist->setGeometry(QRect(10, 295, 670, 100));
    pushButton1->setGeometry(QRect(580, 445, 100, 25));
  }

  utc_to_date_time(mainwindow->edfheaderlist[row]->utc_starttime, &date_time);

  date_time.month_str[0] += 32;
  date_time.month_str[1] += 32;
  date_time.month_str[2] += 32;

  snprintf(str, 400, "%i %s %i  %2i:%02i:%02i",
          date_time.day,
          date_time.month_str,
          date_time.year,
          date_time.hour,
          date_time.minute,
          date_time.second);

  if(mainwindow->edfheaderlist[row]->starttime_offset != 0LL)
  {
#ifdef Q_OS_WIN32
    snprintf(str + strlen(str), 100, ".%07I64d", mainwindow->edfheaderlist[row]->starttime_offset);
#else
    snprintf(str + strlen(str), 100, ".%07lli", mainwindow->edfheaderlist[row]->starttime_offset);
#endif

    remove_trailing_zeros(str);
  }

  label3a->setText(str);

  file_duration = mainwindow->edfheaderlist[row]->long_data_record_duration * mainwindow->edfheaderlist[row]->datarecords;

  snprintf(str, 512,
          "%2i:%02i:%02i",
          (int)((file_duration / TIME_DIMENSION)/ 3600LL),
          (int)(((file_duration / TIME_DIMENSION) % 3600LL) / 60LL),
          (int)((file_duration / TIME_DIMENSION) % 60LL));

  label4a->setText(str);

  label5a->setText(mainwindow->edfheaderlist[row]->reserved);

  snprintf(str,  512, "%.12f", mainwindow->edfheaderlist[row]->data_record_duration);

  remove_trailing_zeros(str);

  label20a->setText(str);

  label21a->setText(mainwindow->edfheaderlist[row]->version);

  signal_cnt = mainwindow->edfheaderlist[row]->edfsignals;

  signallist->setColumnCount(10);
  signallist->setRowCount(signal_cnt);
  signallist->setSelectionMode(QAbstractItemView::NoSelection);
  signallist->setColumnWidth(0, 180);
  signallist->setColumnWidth(1, 120);
  signallist->setColumnWidth(2, 120);
  signallist->setColumnWidth(3, 120);
  signallist->setColumnWidth(4, 120);
  signallist->setColumnWidth(5, 120);
  signallist->setColumnWidth(6, 120);
  signallist->setColumnWidth(7, 120);
  signallist->setColumnWidth(8, 520);
  signallist->setColumnWidth(9, 520);
  QStringList horizontallabels;
  horizontallabels += "Label";
  horizontallabels += "Samplefrequency";
  horizontallabels += "Physical maximum";
  horizontallabels += "Physical minimum";
  horizontallabels += "Physical dimension";
  horizontallabels += "Digital maximum";
  horizontallabels += "Digital minimum";
  horizontallabels += "Samples per record";
  horizontallabels += "Prefilter";
  horizontallabels += "Transducer";
  signallist->setHorizontalHeaderLabels(horizontallabels);

  for(i=0; i<signal_cnt; i++)
  {
    signallist->setRowHeight(i, 20);
    signallist->setCellWidget(i, 0, new QLabel(mainwindow->edfheaderlist[row]->edfparam[i].label));
    snprintf(str, 512, "%f", (double)mainwindow->edfheaderlist[row]->edfparam[i].smp_per_record / mainwindow->edfheaderlist[row]->data_record_duration);
    strcat(str, " Hz");
    remove_trailing_zeros(str);
    signallist->setCellWidget(i, 1, new QLabel(str));
    snprintf(str, 512, "%+f", mainwindow->edfheaderlist[row]->edfparam[i].phys_max);
    remove_trailing_zeros(str);
    signallist->setCellWidget(i, 2, new QLabel(str));
    snprintf(str, 512, "%+f", mainwindow->edfheaderlist[row]->edfparam[i].phys_min);
    remove_trailing_zeros(str);
    signallist->setCellWidget(i, 3, new QLabel(str));
    signallist->setCellWidget(i, 4, new QLabel(mainwindow->edfheaderlist[row]->edfparam[i].physdimension));
    snprintf(str, 512, "%+i", mainwindow->edfheaderlist[row]->edfparam[i].dig_max);
    signallist->setCellWidget(i, 5, new QLabel(str));
    snprintf(str, 512, "%+i", mainwindow->edfheaderlist[row]->edfparam[i].dig_min);
    signallist->setCellWidget(i, 6, new QLabel(str));
    snprintf(str, 512, "%i", mainwindow->edfheaderlist[row]->edfparam[i].smp_per_record);
    signallist->setCellWidget(i, 7, new QLabel(str));
    signallist->setCellWidget(i, 8, new QLabel(mainwindow->edfheaderlist[row]->edfparam[i].prefilter));
    signallist->setCellWidget(i, 9, new QLabel(mainwindow->edfheaderlist[row]->edfparam[i].transducer));
  }
}










