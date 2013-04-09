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




#include "signal_chooser.h"



UI_SignalChooser::UI_SignalChooser(QWidget *w_parent, int job, int *sgnl_nr)
{
  task = job;

  signal_nr = sgnl_nr;

  mainwindow = (UI_Mainwindow *)w_parent;

  signalchooser_dialog = new QDialog(w_parent);

  if(task == 3)
  {
    signalchooser_dialog->setMinimumSize(QSize(265, 400));
    signalchooser_dialog->setMaximumSize(QSize(265, 400));
    signalchooser_dialog->setWindowTitle("Organize signals");
  }
  else
  {
    signalchooser_dialog->setMinimumSize(QSize(200, 400));
    signalchooser_dialog->setMaximumSize(QSize(200, 400));
    signalchooser_dialog->setWindowTitle("Signals");
  }
  signalchooser_dialog->setModal(TRUE);
  signalchooser_dialog->setAttribute(Qt::WA_DeleteOnClose, TRUE);

  list = new QListWidget(signalchooser_dialog);
  list->setGeometry(5, 5, 130, 365);
  list->setSelectionBehavior(QAbstractItemView::SelectRows);
  if(task == 3)
  {
    list->setSelectionMode(QAbstractItemView::ExtendedSelection);
  }
  else
  {
    list->setSelectionMode(QAbstractItemView::SingleSelection);
  }

  CloseButton = new QPushButton(signalchooser_dialog);
  CloseButton->setGeometry(5, 375, 130, 20);
  CloseButton->setText("Close");

  if(task == 3)
  {
    UpButton = new QPushButton(signalchooser_dialog);
    UpButton->setGeometry(150, 180, 100, 20);
    UpButton->setText("Up");

    DownButton = new QPushButton(signalchooser_dialog);
    DownButton->setGeometry(150, 220, 100, 20);
    DownButton->setText("Down");

    InvertButton = new QPushButton(signalchooser_dialog);
    InvertButton->setGeometry(150, 260, 100, 20);
    InvertButton->setText("Invert");

    DeleteButton = new QPushButton(signalchooser_dialog);
    DeleteButton->setGeometry(150, 300, 100, 20);
    DeleteButton->setText("Remove");
  }

  load_signalcomps();

  QObject::connect(CloseButton, SIGNAL(clicked()), signalchooser_dialog, SLOT(close()));

  if(task == 3)
  {
    if(list->count() > 0)
    {
      list->setCurrentRow(0);

      QObject::connect(UpButton,     SIGNAL(clicked()), this, SLOT(signalUp()));
      QObject::connect(DownButton,   SIGNAL(clicked()), this, SLOT(signalDown()));
      QObject::connect(InvertButton, SIGNAL(clicked()), this, SLOT(signalInvert()));
      QObject::connect(DeleteButton, SIGNAL(clicked()), this, SLOT(signalDelete()));
    }
  }
  else
  {
    QObject::connect(list, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(call_sidemenu(QListWidgetItem *)));
  }

  signalchooser_dialog->exec();
}



void UI_SignalChooser::load_signalcomps(void)
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



void UI_SignalChooser::call_sidemenu(QListWidgetItem *)
{
  if(task == 3) return;

  signalchooser_dialog->hide();

  if(task == 0)
  {
    mainwindow->maincurve->exec_sidemenu(list->currentRow());
  }

  if(task == 1)
  {
    mainwindow->spectrumdock->init(list->currentRow());
  }

  if(task == 2)
  {
    AdjustFilterSettings filtersettings(mainwindow->signalcomp[list->currentRow()], mainwindow->maincurve);
  }

  if(task == 4)
  {
    if(signal_nr != NULL)
    {
      *signal_nr = list->currentRow();
    }
  }

  signalchooser_dialog->close();
}


void UI_SignalChooser::signalUp()
{
  int i, n,
      size,
      sigcomp_nr,
      selected_signals[MAXSIGNALS];

  QList<QListWidgetItem *> selectedlist;

  struct signalcompblock *signalcomp;


  selectedlist = list->selectedItems();

  n = selectedlist.size();

  if(n < 1)
  {
    return;
  }

  size = list->count();

  if(size < (n + 1))
  {
    return;
  }

  if(selectedlist.at(0)->data(Qt::UserRole).toInt() < 1)
  {
    return;
  }

  for(i=0; i<n; i++)
  {
    sigcomp_nr = selectedlist.at(i)->data(Qt::UserRole).toInt();

    signalcomp = mainwindow->signalcomp[sigcomp_nr];

    mainwindow->signalcomp[sigcomp_nr] = mainwindow->signalcomp[sigcomp_nr - 1];

    mainwindow->signalcomp[sigcomp_nr - 1] = signalcomp;

    selected_signals[i] = sigcomp_nr;
  }

  load_signalcomps();

  for(i=0; i<n; i++)
  {
    list->item(selected_signals[i] - 1)->setSelected(TRUE);
  }

  mainwindow->setup_viewbuf();
}


void UI_SignalChooser::signalDown()
{
  int i, n,
      size,
      sigcomp_nr,
      selected_signals[MAXSIGNALS];

  QList<QListWidgetItem *> selectedlist;

  struct signalcompblock *signalcomp;


  selectedlist = list->selectedItems();

  n = selectedlist.size();

  if(n < 1)
  {
    return;
  }

  size = list->count();

  if(size < (n + 1))
  {
    return;
  }

  if(selectedlist.at(n - 1)->data(Qt::UserRole).toInt() > (size - 2))
  {
    return;
  }

  for(i=(n-1); i>=0; i--)
  {
    sigcomp_nr = selectedlist.at(i)->data(Qt::UserRole).toInt();

    signalcomp = mainwindow->signalcomp[sigcomp_nr];

    mainwindow->signalcomp[sigcomp_nr] = mainwindow->signalcomp[sigcomp_nr + 1];

    mainwindow->signalcomp[sigcomp_nr + 1] = signalcomp;

    selected_signals[i] = sigcomp_nr;
  }

  load_signalcomps();

  for(i=0; i<n; i++)
  {
    list->item(selected_signals[i] + 1)->setSelected(TRUE);
  }

  mainwindow->setup_viewbuf();
}


void UI_SignalChooser::signalDelete()
{
  int i, j, k, n, p,
      sigcomp_nr;

  QListWidgetItem *item;

  QList<QListWidgetItem *> selectedlist;


  selectedlist = list->selectedItems();

  n = selectedlist.size();

  if(n < 1)
  {
    return;
  }

  for(k=0; k<n; k++)
  {
    item = selectedlist.at(k);

    sigcomp_nr = item->data(Qt::UserRole).toInt();

    sigcomp_nr -= k;

    if(mainwindow->spectrumdock->signalcomp == mainwindow->signalcomp[sigcomp_nr])
    {
      mainwindow->spectrumdock->clear();
      mainwindow->spectrumdock->dock->hide();
    }

    for(i=0; i<MAXSPECTRUMDIALOGS; i++)
    {
      p = mainwindow->signalcomp[sigcomp_nr]->spectr_dialog[i];

      if(p != 0)
      {
        delete mainwindow->spectrumdialog[p - 1];

        mainwindow->spectrumdialog[p - 1] = NULL;
      }
    }

    for(i=0; i<MAXAVERAGECURVEDIALOGS; i++)
    {
      p = mainwindow->signalcomp[sigcomp_nr]->avg_dialog[i];

      if(p != 0)
      {
        delete mainwindow->averagecurvedialog[p - 1];

        mainwindow->averagecurvedialog[p - 1] = NULL;
      }
    }

    if(mainwindow->signalcomp[sigcomp_nr]->hascursor2)
    {
      mainwindow->maincurve->crosshair_2.active = 0;
      mainwindow->maincurve->crosshair_2.moving = 0;
    }

    if(mainwindow->signalcomp[sigcomp_nr]->hascursor1)
    {
      mainwindow->maincurve->crosshair_1.active = 0;
      mainwindow->maincurve->crosshair_2.active = 0;
      mainwindow->maincurve->crosshair_1.moving = 0;
      mainwindow->maincurve->crosshair_2.moving = 0;

      for(i=0; i<mainwindow->signalcomps; i++)
      {
        mainwindow->signalcomp[i]->hascursor2 = 0;
      }
    }

    if(mainwindow->signalcomp[sigcomp_nr]->hasruler)
    {
      mainwindow->maincurve->ruler_active = 0;
      mainwindow->maincurve->ruler_moving = 0;
    }

    for(j=0; j<mainwindow->signalcomp[sigcomp_nr]->filter_cnt; j++)
    {
      free(mainwindow->signalcomp[sigcomp_nr]->filter[j]);
    }

    mainwindow->signalcomp[sigcomp_nr]->filter_cnt = 0;

    for(j=0; j<mainwindow->signalcomp[sigcomp_nr]->ravg_filter_cnt; j++)
    {
      free_ravg_filter(mainwindow->signalcomp[sigcomp_nr]->ravg_filter[j]);
    }

    mainwindow->signalcomp[sigcomp_nr]->ravg_filter_cnt = 0;

    if(mainwindow->signalcomp[sigcomp_nr]->ecg_filter != NULL)
    {
      free_ecg_filter(mainwindow->signalcomp[sigcomp_nr]->ecg_filter);

      mainwindow->signalcomp[sigcomp_nr]->ecg_filter = NULL;

      strcpy(mainwindow->signalcomp[sigcomp_nr]->signallabel, mainwindow->signalcomp[sigcomp_nr]->signallabel_bu);
      mainwindow->signalcomp[sigcomp_nr]->signallabellen = mainwindow->signalcomp[sigcomp_nr]->signallabellen_bu;
      strcpy(mainwindow->signalcomp[sigcomp_nr]->physdimension, mainwindow->signalcomp[sigcomp_nr]->physdimension_bu);
    }

    for(j=0; j<mainwindow->signalcomp[sigcomp_nr]->fidfilter_cnt; j++)
    {
      free(mainwindow->signalcomp[sigcomp_nr]->fidfilter[j]);
      fid_run_free(mainwindow->signalcomp[sigcomp_nr]->fid_run[j]);
      fid_run_freebuf(mainwindow->signalcomp[sigcomp_nr]->fidbuf[j]);
      fid_run_freebuf(mainwindow->signalcomp[sigcomp_nr]->fidbuf2[j]);
    }

    mainwindow->signalcomp[sigcomp_nr]->fidfilter_cnt = 0;

    free(mainwindow->signalcomp[sigcomp_nr]);

    for(i=sigcomp_nr; i<mainwindow->signalcomps - 1; i++)
    {
      mainwindow->signalcomp[i] = mainwindow->signalcomp[i + 1];
    }

    mainwindow->signalcomps--;
  }

  load_signalcomps();

  mainwindow->setup_viewbuf();
}


void UI_SignalChooser::signalInvert()
{
  int i, j, k, n,
      selected_signals[MAXSIGNALS];

  QList<QListWidgetItem *> selectedlist;


  selectedlist = list->selectedItems();

  n = selectedlist.size();

  if(n < 1)
  {
    return;
  }

  for(k=0; k<n; k++)
  {
    j = selectedlist.at(k)->data(Qt::UserRole).toInt();

    selected_signals[k] = j;

    if(mainwindow->signalcomp[j]->polarity != 1)
    {
      mainwindow->signalcomp[j]->polarity = 1;
    }
    else
    {
      mainwindow->signalcomp[j]->polarity = -1;
    }
  }

  load_signalcomps();

  for(i=0; i<n; i++)
  {
    list->item(selected_signals[i])->setSelected(TRUE);
  }

  mainwindow->setup_viewbuf();
}


void UI_SignalChooser::strip_types_from_label(char *label)
{
  int i,
      len;


  len = strlen(label);
  if(len<16)
  {
    return;
  }

  if((!(strncmp(label, "EEG ", 4)))
   ||(!(strncmp(label, "ECG ", 4)))
   ||(!(strncmp(label, "EOG ", 4)))
   ||(!(strncmp(label, "ERG ", 4)))
   ||(!(strncmp(label, "EMG ", 4)))
   ||(!(strncmp(label, "MEG ", 4)))
   ||(!(strncmp(label, "MCG ", 4))))
  {
    if(label[4]!=' ')
    {
      for(i=0; i<(len-4); i++)
      {
        label[i] = label[i+4];
      }

      for(; i<len; i++)
      {
        label[i] = ' ';
      }
    }
  }
}






