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



#include "show_actual_montage_dialog.h"




UI_ShowActualMontagewindow::UI_ShowActualMontagewindow(QWidget *w_parent)
{
  int i, j,
      type,
      model,
      order;

  char txtbuf[2048];

  double frequency,
         frequency2,
         ripple;

  QStandardItem *parentItem,
                *signalItem,
                *filterItem;


  mainwindow = (UI_Mainwindow *)w_parent;

  ShowMontageDialog = new QDialog;

  ShowMontageDialog->setMinimumSize(QSize(500, 300));
  ShowMontageDialog->resize(500, 440);
  ShowMontageDialog->setWindowTitle("Show montage");
  ShowMontageDialog->setModal(TRUE);
  ShowMontageDialog->setAttribute(Qt::WA_DeleteOnClose, TRUE);

  tree = new QTreeView;
  tree->setHeaderHidden(TRUE);
  tree->setIndentation(30);
  tree->setSelectionMode(QAbstractItemView::NoSelection);
  tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tree->setSortingEnabled(FALSE);
  tree->setDragDropMode(QAbstractItemView::NoDragDrop);

  CloseButton = new QPushButton;
  CloseButton->setFixedSize(100, 25);
  CloseButton->setText("Close");

  box = new QBoxLayout(QBoxLayout::TopToBottom, ShowMontageDialog);
  box->addWidget(tree);
  box->addWidget(CloseButton, 0, Qt::AlignRight);

  QObject::connect(CloseButton,  SIGNAL(clicked()), ShowMontageDialog, SLOT(close()));

  t_model = new QStandardItemModel(this);

  parentItem = t_model->invisibleRootItem();

  for(i=0; i<mainwindow->signalcomps; i++)
  {
    txtbuf[0] = 0;

    for(j=0; j<mainwindow->signalcomp[i]->num_of_signals; j++)
    {
      sprintf(txtbuf + strlen(txtbuf), "%+ix %s",
              mainwindow->signalcomp[i]->factor[j],
              mainwindow->signalcomp[i]->edfhdr->edfparam[mainwindow->signalcomp[i]->edfsignal[j]].label);

      remove_trailing_spaces(txtbuf);

      strcat(txtbuf, "   ");
    }

    if(mainwindow->signalcomp[i]->polarity == -1)
    {
      strcat(txtbuf, "Inverted: yes");
    }

    signalItem = new QStandardItem(txtbuf);

    parentItem->appendRow(signalItem);

    sprintf(txtbuf, "amplitude: %f", mainwindow->signalcomp[i]->voltpercm);

    strcat(txtbuf, mainwindow->signalcomp[i]->physdimension);

    remove_trailing_spaces(txtbuf);

    sprintf(txtbuf + strlen(txtbuf), "/cm offset: %f%s",
            mainwindow->signalcomp[i]->screen_offset * mainwindow->pixelsizefactor * mainwindow->signalcomp[i]->voltpercm,
            mainwindow->signalcomp[i]->physdimension);

    remove_trailing_zeros(txtbuf);

    remove_trailing_spaces(txtbuf);

    strcat(txtbuf, "  color: ");

    switch(mainwindow->signalcomp[i]->color)
    {
      case Qt::white       : strcat(txtbuf, "white");
                             break;
      case Qt::black       : strcat(txtbuf, "black");
                             break;
      case Qt::red         : strcat(txtbuf, "red");
                             break;
      case Qt::darkRed     : strcat(txtbuf, "dark red");
                             break;
      case Qt::green       : strcat(txtbuf, "green");
                             break;
      case Qt::darkGreen   : strcat(txtbuf, "dark green");
                             break;
      case Qt::blue        : strcat(txtbuf, "blue");
                             break;
      case Qt::darkBlue    : strcat(txtbuf, "dark blue");
                             break;
      case Qt::cyan        : strcat(txtbuf, "cyan");
                             break;
      case Qt::darkCyan    : strcat(txtbuf, "dark cyan");
                             break;
      case Qt::magenta     : strcat(txtbuf, "magenta");
                             break;
      case Qt::darkMagenta : strcat(txtbuf, "dark magenta");
                             break;
      case Qt::yellow      : strcat(txtbuf, "yellow");
                             break;
      case Qt::darkYellow  : strcat(txtbuf, "dark yellow");
                             break;
      case Qt::gray        : strcat(txtbuf, "gray");
                             break;
      case Qt::darkGray    : strcat(txtbuf, "dark gray");
                             break;
      case Qt::lightGray   : strcat(txtbuf, "light gray");
                             break;
    }

    signalItem->appendRow(new QStandardItem(txtbuf));

    filterItem = new QStandardItem("Filters");

    signalItem->appendRow(filterItem);

    for(j=0; j<mainwindow->signalcomp[i]->filter_cnt; j++)
    {
      if(mainwindow->signalcomp[i]->filter[j]->is_LPF == 1)
      {
        sprintf(txtbuf, "LPF: %fHz", mainwindow->signalcomp[i]->filter[j]->cutoff_frequency);
      }

      if(mainwindow->signalcomp[i]->filter[j]->is_LPF == 0)
      {
        sprintf(txtbuf, "HPF: %fHz", mainwindow->signalcomp[i]->filter[j]->cutoff_frequency);
      }

      remove_trailing_zeros(txtbuf);

      filterItem->appendRow(new QStandardItem(txtbuf));
    }

    for(j=0; j<mainwindow->signalcomp[i]->ravg_filter_cnt; j++)
    {
      if(mainwindow->signalcomp[i]->ravg_filter_type[j] == 0)
      {
        sprintf(txtbuf, "highpass moving average %i smpls", mainwindow->signalcomp[i]->ravg_filter[j]->size);
      }

      if(mainwindow->signalcomp[i]->ravg_filter_type[j] == 1)
      {
        sprintf(txtbuf, "lowpass moving average %i smpls", mainwindow->signalcomp[i]->ravg_filter[j]->size);
      }

      filterItem->appendRow(new QStandardItem(txtbuf));
    }

    for(j=0; j<mainwindow->signalcomp[i]->fidfilter_cnt; j++)
    {
      type = mainwindow->signalcomp[i]->fidfilter_type[j];

      model = mainwindow->signalcomp[i]->fidfilter_model[j];

      frequency = mainwindow->signalcomp[i]->fidfilter_freq[j];

      frequency2 = mainwindow->signalcomp[i]->fidfilter_freq2[j];

      order = mainwindow->signalcomp[i]->fidfilter_order[j];

      ripple = mainwindow->signalcomp[i]->fidfilter_ripple[j];

      if(type == 0)
      {
        if(model == 0)
        {
          sprintf(txtbuf, "highpass Butterworth %fHz %ith order", frequency, order);
        }

        if(model == 1)
        {
          sprintf(txtbuf, "highpass Chebyshev %fHz %ith order %fdB ripple", frequency, order, ripple);
        }

        if(model == 2)
        {
          sprintf(txtbuf, "highpass Bessel %fHz %ith order", frequency, order);
        }
      }

      if(type == 1)
      {
        if(model == 0)
        {
          sprintf(txtbuf, "lowpass Butterworth %fHz %ith order", frequency, order);
        }

        if(model == 1)
        {
          sprintf(txtbuf, "lowpass Chebyshev %fHz %ith order %fdB ripple", frequency, order, ripple);
        }

        if(model == 2)
        {
          sprintf(txtbuf, "lowpass Bessel %fHz %ith order", frequency, order);
        }
      }

      if(type == 2)
      {
        sprintf(txtbuf, "notch %fHz Q-factor %i", frequency, order);
      }

      if(type == 3)
      {
        if(model == 0)
        {
          sprintf(txtbuf, "bandpass Butterworth %f-%fHz %ith order", frequency, frequency2, order);
        }

        if(model == 1)
        {
          sprintf(txtbuf, "bandpass Chebyshev %f-%fHz %ith order %fdB ripple", frequency, frequency2, order, ripple);
        }

        if(model == 2)
        {
          sprintf(txtbuf, "bandpass Bessel %f-%fHz %ith order", frequency, frequency2, order);
        }
      }

      if(type == 4)
      {
        if(model == 0)
        {
          sprintf(txtbuf, "bandstop Butterworth %f-%fHz %ith order", frequency, frequency2, order);
        }

        if(model == 1)
        {
          sprintf(txtbuf, "bandstop Chebyshev %f-%fHz %ith order %fdB ripple", frequency, frequency2, order, ripple);
        }

        if(model == 2)
        {
          sprintf(txtbuf, "bandstop Bessel %f-%fHz %ith order", frequency, frequency2, order);
        }
      }

      remove_trailing_zeros(txtbuf);

      filterItem->appendRow(new QStandardItem(txtbuf));
    }

    if(mainwindow->signalcomp[i]->ecg_filter != NULL)
    {
      sprintf(txtbuf, "ECG heartrate detection");

      filterItem->appendRow(new QStandardItem(txtbuf));
    }

    if(mainwindow->signalcomp[i]->zratio_filter != NULL)
    {
      sprintf(txtbuf, "Z-ratio  cross-over frequency is %.1f Hz", mainwindow->signalcomp[i]->zratio_crossoverfreq);

      filterItem->appendRow(new QStandardItem(txtbuf));
    }
  }

  sprintf(txtbuf, "timescale: %f seconds", (double)mainwindow->pagetime / (double)TIME_DIMENSION);
  remove_trailing_zeros(txtbuf);
  parentItem->appendRow(new QStandardItem(txtbuf));

  tree->setModel(t_model);

  ShowMontageDialog->exec();
}














