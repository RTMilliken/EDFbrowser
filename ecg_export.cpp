/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2011, 2012, 2013 Teunis van Beelen
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



#include "ecg_export.h"



#if defined(__APPLE__) || defined(__MACH__) || defined(__APPLE_CC__)

#define fopeno fopen

#else

#define fseeko fseeko64
#define ftello ftello64
#define fopeno fopen64

#endif




UI_ECGExport::UI_ECGExport(QWidget *w_parent)
{
  mainwindow = (UI_Mainwindow *)w_parent;

  myobjectDialog = new QDialog(w_parent);

  myobjectDialog->setMinimumSize(QSize(400, 445));
  myobjectDialog->setMaximumSize(QSize(400, 445));
  myobjectDialog->setWindowTitle("Export RR-intervals");
  myobjectDialog->setModal(TRUE);
  myobjectDialog->setAttribute(Qt::WA_DeleteOnClose, TRUE);

  list = new QListWidget(myobjectDialog);
  list->setGeometry(20, 20, 130, 350);
  list->setSelectionBehavior(QAbstractItemView::SelectRows);
  list->setSelectionMode(QAbstractItemView::SingleSelection);

  groupBox1 = new QGroupBox(myobjectDialog);
  groupBox1->setGeometry(170, 20, 210, 120);
  groupBox1->setTitle("Output format");

  radioButton1 = new QRadioButton("RR interval");
  radioButton2 = new QRadioButton("R Onset + RR interval");
  radioButton3 = new QRadioButton("R Onset");
  radioButton2->setChecked(TRUE);

  checkBox1 = new QCheckBox("Whole recording", myobjectDialog);
  checkBox1->setGeometry(170, 160, 210, 25);
  checkBox1->setTristate(FALSE);

  checkBox2 = new QCheckBox("Don't write to file,\n"
                            "import as annotations instead",
                            myobjectDialog);
  checkBox2->setGeometry(170, 200, 210, 40);
  checkBox2->setTristate(FALSE);

  vbox1 = new QVBoxLayout;
  vbox1->addWidget(radioButton1);
  vbox1->addWidget(radioButton2);
  vbox1->addWidget(radioButton3);

  groupBox1->setLayout(vbox1);

  startButton = new QPushButton(myobjectDialog);
  startButton->setGeometry(20, 400, 150, 25);
  startButton->setText("Import/Export");

  cancelButton = new QPushButton(myobjectDialog);
  cancelButton->setGeometry(280, 400, 100, 25);
  cancelButton->setText("Cancel");

  load_signalcomps();

  QObject::connect(cancelButton, SIGNAL(clicked()), myobjectDialog, SLOT(close()));
  QObject::connect(startButton,  SIGNAL(clicked()), this,           SLOT(Export_RR_intervals()));

  myobjectDialog->exec();
}



void UI_ECGExport::Export_RR_intervals()
{
  int i,
      len,
      signal_nr,
      type=-1,
      beat_cnt,
      samples_cnt,
      progress_steps,
      datarecords,
      whole_recording=0,
      import_as_annots=0,
      filenum=0;

  char path[MAX_PATH_LENGTH],
       str[2048];

  double *beat_interval_list,
         *buf;

  long long *beat_onset_list,
            datrecs,
            smpls_left,
            l_time=0LL;

  struct signalcompblock *signalcomp;


  FILE *outputfile;

  QList<QListWidgetItem *> selectedlist;

  struct annotationblock *annotation;


  selectedlist = list->selectedItems();

  if(selectedlist.size() < 1)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Select a signal first.");
    messagewindow.exec();
    return;
  }

  signal_nr = selectedlist.at(0)->data(Qt::UserRole).toInt();

  if((signal_nr < 0) || (signal_nr >= mainwindow->signalcomps))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Invalid signalcomp number");
    messagewindow.exec();
    return;
  }

  signalcomp = mainwindow->signalcomp[signal_nr];

  if(signalcomp->ecg_filter == NULL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Heart Rate detection is not activated for the selected signal.");
    messagewindow.exec();
    return;
  }

  if(checkBox2->checkState() == Qt::Checked)
  {
    import_as_annots = 1;
  }

  if(checkBox1->checkState() == Qt::Checked)
  {
    whole_recording = 1;

    class FilteredBlockReadClass blockrd;

    buf = blockrd.init_signalcomp(signalcomp, 1, 0);
    if(buf == NULL)
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "Error, can not initialize FilteredBlockReadClass.");
      messagewindow.exec();
      return;
    }

    samples_cnt = blockrd.samples_in_buf();
    if(samples_cnt < 1)
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "Error, samples_cnt is < 1.");
      messagewindow.exec();
      return;
    }

    filenum = signalcomp->filenum;

    reset_ecg_filter(signalcomp->ecg_filter);

    datarecords = signalcomp->edfhdr->datarecords;

    QProgressDialog progress("Processing file...", "Abort", 0, datarecords);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(200);

    progress_steps = datarecords / 100;
    if(progress_steps < 1)
    {
      progress_steps = 1;
    }

    for(i=0; i<signalcomp->edfhdr->datarecords; i++)
    {
      if(!(i%progress_steps))
      {
        progress.setValue(i);

        qApp->processEvents();

        if(progress.wasCanceled() == TRUE)
        {
          return;
        }
      }

      if(blockrd.process_signalcomp(i) != 0)
      {
        progress.reset();
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "Error while reading file.");
        messagewindow.exec();
        return;
      }
    }

    progress.reset();
  }

  beat_cnt = ecg_filter_get_beat_cnt(signalcomp->ecg_filter);

  beat_onset_list = ecg_filter_get_onset_beatlist(signalcomp->ecg_filter);

  beat_interval_list = ecg_filter_get_interval_beatlist(signalcomp->ecg_filter);

  if(beat_cnt < 4)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Error, not enough beats.");
    messagewindow.exec();
    return;
  }

  if(import_as_annots)
  {
    for(i=0; i<beat_cnt; i++)
    {
      if(whole_recording)
      {
        l_time = 0LL;
      }
      else
      {
        l_time = signalcomp->edfhdr->viewtime;
      }

      if(l_time < 0LL)
      {
        l_time = 0LL;
      }

      datrecs = beat_onset_list[i] / signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].smp_per_record;

      smpls_left = beat_onset_list[i] % signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].smp_per_record;

      l_time += (datrecs * signalcomp->edfhdr->long_data_record_duration);

      l_time += ((smpls_left * signalcomp->edfhdr->long_data_record_duration) / signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].smp_per_record);

      if(!whole_recording)
      {
        l_time += (mainwindow->edfheaderlist[mainwindow->sel_viewtime]->viewtime - signalcomp->edfhdr->viewtime);
      }

      annotation = (struct annotationblock *)calloc(1, sizeof(struct annotationblock));
      if(annotation == NULL)
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "A memory allocation error occurred (annotation).");
        messagewindow.exec();
        return;
      }
      annotation->onset = l_time;
      strncpy(annotation->annotation, "R-onset", MAX_ANNOTATION_LEN);
      annotation->annotation[MAX_ANNOTATION_LEN] = 0;
      edfplus_annotation_add_item(&mainwindow->annotationlist[filenum], annotation);
    }

    if(mainwindow->annotations_dock[signalcomp->filenum] == NULL)
    {
      mainwindow->annotations_dock[filenum] = new UI_Annotationswindow(filenum, mainwindow);

      mainwindow->addDockWidget(Qt::RightDockWidgetArea, mainwindow->annotations_dock[filenum]->docklist, Qt::Vertical);

      if(!mainwindow->annotationlist[filenum])
      {
        mainwindow->annotations_dock[filenum]->docklist->hide();
      }
    }

    if(mainwindow->annotationlist[filenum])
    {
      mainwindow->annotations_dock[filenum]->docklist->show();

      mainwindow->annotations_edited = 1;

      mainwindow->annotations_dock[filenum]->updateList();

      mainwindow->save_act->setEnabled(TRUE);
    }
  }
  else
  {
    path[0] = 0;
    if(mainwindow->recent_savedir[0]!=0)
    {
      strcpy(path, mainwindow->recent_savedir);
      strcat(path, "/");
    }
    len = strlen(path);
    get_filename_from_path(path + len, signalcomp->edfhdr->filename, MAX_PATH_LENGTH - len);
    remove_extension_from_filename(path);
    strcat(path, "_RR_interval.txt");

    strcpy(path, QFileDialog::getSaveFileName(0, "Export RR-interval to ASCII", QString::fromLocal8Bit(path), "Text files (*.txt *.TXT)").toLocal8Bit().data());

    if(!strcmp(path, ""))
    {
      return;
    }

    get_directory_from_path(mainwindow->recent_savedir, path, MAX_PATH_LENGTH);

    outputfile = fopeno(path, "wb");
    if(outputfile==NULL)
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "Error, can not open outputfile for writing.");
      messagewindow.exec();
      return;
    }

    if(radioButton1->isChecked() == TRUE)
    {
      type = 1;
    }

    if(radioButton2->isChecked() == TRUE)
    {
      type = 2;
    }

    if(radioButton3->isChecked() == TRUE)
    {
      type = 3;
    }

    if(type == 1)
    {
      for(i=0; i<beat_cnt; i++)
      {
        fprintf(outputfile, "%.4f\n", beat_interval_list[i]);
      }
    }

    if((type == 2) || (type == 3))
    {
      for(i=0; i<beat_cnt; i++)
      {
        if(whole_recording)
        {
          l_time = 0LL;
        }
        else
        {
          l_time = signalcomp->edfhdr->viewtime;
        }

        if(l_time < 0LL)
        {
          l_time = 0LL;
        }

        datrecs = beat_onset_list[i] / signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].smp_per_record;

        smpls_left = beat_onset_list[i] % signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].smp_per_record;

        l_time += (datrecs * signalcomp->edfhdr->long_data_record_duration);

        l_time += ((smpls_left * signalcomp->edfhdr->long_data_record_duration) / signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].smp_per_record);

        if(!whole_recording)
        {
          l_time += (mainwindow->edfheaderlist[mainwindow->sel_viewtime]->viewtime - signalcomp->edfhdr->viewtime);
        }

        if(type == 2)
        {
          fprintf(outputfile, "%.4f\t%.4f\n", ((double)l_time) / TIME_DIMENSION, beat_interval_list[i]);
        }

        if(type == 3)
        {
          fprintf(outputfile, "%.4f\n", ((double)l_time) / TIME_DIMENSION);
        }
      }
    }

    fclose(outputfile);
  }

  myobjectDialog->close();

  if(!import_as_annots)
  {
    sprintf(str, "Done. The R-onsets and/or RR-intervals are exported to:\n\n%s", path);
    QMessageBox messagewindow(QMessageBox::Information, "Ready", QString::fromLocal8Bit(str));
    messagewindow.setIconPixmap(QPixmap(":/images/ok.png"));
    messagewindow.exec();
  }

  reset_ecg_filter(signalcomp->ecg_filter);

  mainwindow->setup_viewbuf();
}


void UI_ECGExport::load_signalcomps(void)
{
  int i;

  QListWidgetItem *item;

  list->clear();

  for(i=0; i<mainwindow->signalcomps; i++)
  {
    item = new QListWidgetItem;
    item->setText(mainwindow->signalcomp[i]->signallabel);
    item->setData(Qt::UserRole, QVariant(i));
    list->addItem(item);
  }
}






















