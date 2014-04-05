/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2011, 2012, 2013, 2014 Teunis van Beelen
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




#include "statistics_dialog.h"



UI_StatisticWindow::UI_StatisticWindow(struct signalcompblock *signalcomp, long long pagetime)
{
  int i,
      tmp,
      NN50,
      pNN50;

  char stat_str[2048];

  double d_tmp,
         average_bpm,
         average_rr,
         sdnn_bpm,
         sdnn_rr,
         *buf_bpm,
         rmssd_rr,
         *beat_interval_list;

  StatDialog = new QDialog;
  StatDialog->setWindowTitle("Statistics");
  StatDialog->setModal(true);
  StatDialog->setAttribute(Qt::WA_DeleteOnClose, true);
  StatDialog->setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
  StatDialog->setWindowIcon(QIcon(":/images/edf.png"));

  if(signalcomp->ecg_filter != NULL)
  {
    StatDialog->setMinimumSize(600, 400);
    StatDialog->setSizeGripEnabled(true);

    startSlider = new QSlider;
    startSlider->setOrientation(Qt::Horizontal);
    startSlider->setMinimum(0);
    startSlider->setMaximum(295);
    startSlider->setValue(0);

    stopSlider = new QSlider;
    stopSlider->setOrientation(Qt::Horizontal);
    stopSlider->setMinimum(5);
    stopSlider->setMaximum(300);
    stopSlider->setValue(300);

    curve1 = new SignalCurve(StatDialog);
    curve1->setSignalColor(Qt::darkGreen);
    curve1->setBackgroundColor(Qt::black);
    curve1->setRasterColor(Qt::gray);
    curve1->setTraceWidth(0);
    curve1->setH_label(signalcomp->physdimension);
    curve1->setLowerLabel("HR (beats/min)");
    curve1->setDashBoardEnabled(false);
    curve1->setUpperLabel1("Distribution");
    curve1->setFillSurfaceEnabled(true);

    vlayout2_1 = new QVBoxLayout;
    vlayout2_1->setSpacing(20);
    vlayout2_1->addWidget(curve1);
    vlayout2_1->addWidget(startSlider);
    vlayout2_1->addWidget(stopSlider);
  }
  else
  {
    StatDialog->setMinimumSize(300, 400);
    StatDialog->setMaximumSize(300, 400);
  }

  Label1 = new QLabel(StatDialog);
  Label1->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  pushButton1 = new QPushButton(StatDialog);
  pushButton1->setMinimumSize(100, 25);
  pushButton1->setText("&Close");

  hlayout1_1_1 = new QHBoxLayout;
  hlayout1_1_1->addWidget(pushButton1);
  hlayout1_1_1->addStretch(100);

  vlayout1_1 = new QVBoxLayout;
  vlayout1_1->setSpacing(20);
  vlayout1_1->addWidget(Label1);
  vlayout1_1->addStretch(100);
  vlayout1_1->addLayout(hlayout1_1_1);

  hlayout1 = new QHBoxLayout;
  hlayout1->addLayout(vlayout1_1, 1);
  if(signalcomp->ecg_filter != NULL)
  {
    hlayout1->addLayout(vlayout2_1, 100);
  }

  StatDialog->setLayout(hlayout1);

  QObject::connect(pushButton1, SIGNAL(clicked()), StatDialog, SLOT(close()));

  for(i=0; i<300; i++)
  {
    bpm_distribution[i] = 0;
  }

  if(signalcomp->ecg_filter == NULL)
  {
    if((signalcomp->stat_cnt < 1) || (pagetime < 10LL))
    {
      if(signalcomp->alias[0] != 0)
      {
        sprintf(stat_str, "Signal:  %s\n\nSamples:   0\n\nSum:       0 %s\n\nMean:      0 %s\n\nRMS:       0 %s\n\nMRS:       0 %s\n\nZero crossings:  0\n\nFrequency:  0 Hz",
                signalcomp->alias,
                signalcomp->physdimension,
                signalcomp->physdimension,
                signalcomp->physdimension,
                signalcomp->physdimension);
      }
      else
      {
        sprintf(stat_str, "Signal:  %s\n\nSamples:   0\n\nSum:       0 %s\n\nMean:      0 %s\n\nRMS:       0 %s\n\nMRS:       0 %s\n\nZero crossings:  0\n\nFrequency:  0 Hz",
                signalcomp->signallabel,
                signalcomp->physdimension,
                signalcomp->physdimension,
                signalcomp->physdimension,
                signalcomp->physdimension);
      }
    }
    else
    {
      if(signalcomp->alias[0] != 0)
      {
        sprintf(stat_str, "Signal:  %s\n\nSamples:   %i\n\nSum:       %f %s\n\nMean:      %f %s\n\nRMS:       %f %s\n\nMRS:       %f %s\n\nZero crossings:  %i\n\nFrequency:  %f Hz",
                signalcomp->alias,
                signalcomp->stat_cnt,
                signalcomp->stat_sum * signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].bitvalue,
                signalcomp->physdimension,
                (signalcomp->stat_sum * signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].bitvalue) / signalcomp->stat_cnt,
                signalcomp->physdimension,
                sqrt(signalcomp->stat_sum_sqr / signalcomp->stat_cnt) * signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].bitvalue,
                signalcomp->physdimension,
                (signalcomp->stat_sum_rectified / signalcomp->stat_cnt) * signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].bitvalue,
                signalcomp->physdimension,
                signalcomp->stat_zero_crossing_cnt,
                (((double)signalcomp->stat_zero_crossing_cnt / 2.0)) / ((double)pagetime / (double)TIME_DIMENSION)
              );
      }
      else
      {
        sprintf(stat_str, "Signal:  %s\n\nSamples:   %i\n\nSum:       %f %s\n\nMean:      %f %s\n\nRMS:       %f %s\n\nMRS:       %f %s\n\nZero crossings:  %i\n\nFrequency:  %f Hz",
                signalcomp->signallabel,
                signalcomp->stat_cnt,
                signalcomp->stat_sum * signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].bitvalue,
                signalcomp->physdimension,
                (signalcomp->stat_sum * signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].bitvalue) / signalcomp->stat_cnt,
                signalcomp->physdimension,
                sqrt(signalcomp->stat_sum_sqr / signalcomp->stat_cnt) * signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].bitvalue,
                signalcomp->physdimension,
                (signalcomp->stat_sum_rectified / signalcomp->stat_cnt) * signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].bitvalue,
                signalcomp->physdimension,
                signalcomp->stat_zero_crossing_cnt,
                (((double)signalcomp->stat_zero_crossing_cnt / 2.0)) / ((double)pagetime / (double)TIME_DIMENSION)
              );
      }
    }
  }
  else
  {
    beat_cnt = ecg_filter_get_beat_cnt(signalcomp->ecg_filter);

    beat_interval_list = ecg_filter_get_interval_beatlist(signalcomp->ecg_filter);

    if(beat_cnt < 3)
    {
      sprintf(stat_str, "Not enough beats.");
    }
    else
    {
      average_bpm = 0.0;
      average_rr = 0.0;
      sdnn_bpm = 0.0;
      sdnn_rr = 0.0;
      rmssd_rr = 0.0;
      NN50 = 0;

      buf_bpm = (double *)malloc(sizeof(double) * beat_cnt);
      if(buf_bpm == NULL)
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "The system was not able to provide enough resources (memory) to perform the requested action.");
        messagewindow.exec();
        return;
      }

      for(i=0; i<beat_cnt; i++)
      {
        buf_bpm[i] = 60.0 / beat_interval_list[i];

        average_bpm += buf_bpm[i];
        average_rr += beat_interval_list[i];

        if(i < (beat_cnt - 1))
        {
          d_tmp = (beat_interval_list[i] - beat_interval_list[i + 1]) * 1000.0;

          rmssd_rr += (d_tmp * d_tmp);

          if(((beat_interval_list[i] - beat_interval_list[i + 1]) > 0.05 ) || ((beat_interval_list[i + 1] - beat_interval_list[i]) > 0.05 ))
          {
            NN50++;
          }
        }
      }

      average_bpm /= beat_cnt;
      average_rr /= beat_cnt;
      rmssd_rr /= beat_cnt;
      rmssd_rr = sqrt(rmssd_rr);

      pNN50 = (NN50 * 100) / (beat_cnt - 1);

      for(i=0; i<beat_cnt; i++)
      {
        sdnn_bpm += (buf_bpm[i] - average_bpm) * (buf_bpm[i] - average_bpm);
        sdnn_rr += (beat_interval_list[i] - average_rr) * (beat_interval_list[i] - average_rr);
      }

      sdnn_bpm = sqrt(sdnn_bpm / beat_cnt);
      sdnn_rr = sqrt(sdnn_rr / beat_cnt);

      sprintf(stat_str,
              "Heart Rate\n\n"
              "Beats:    %3i\n\n"
              "Mean RR:  %3i ms\n\n"
              "SDNN RR:  %3i ms\n\n"
              "RMSSD RR: %3i ms\n\n"
              "Mean HR:  %3.3f bpm\n\n"
              "SDNN HR:  %3.3f bpm\n\n"
              "NN50:     %3i\n\n"
              "pNN50:    %3i %%\n\n",
              beat_cnt,
              (int)(average_rr * 1000.0),
              (int)(sdnn_rr * 1000.0),
              (int)rmssd_rr,
              average_bpm,
              sdnn_bpm,
              NN50,
              pNN50);

      free(buf_bpm);

      for(i=0; i<beat_cnt; i++)
      {
        tmp = 60.0 / beat_interval_list[i];

        if((tmp > 0) && (tmp < 301))
        {
          bpm_distribution[tmp-1]++;
        }
      }

      max_val = 1;

      for(i=0; i<300; i++)
      {
        if(bpm_distribution[i] > max_val)
        {
          max_val = bpm_distribution[i];
        }
      }

      for(i=0; i<300; i++)
      {
        if(bpm_distribution[i] > (max_val / 70))
        {
          start_ruler = i;

          break;
        }
      }

      for(i=299; i>=0; i--)
      {
        if(bpm_distribution[i] > (max_val / 70))
        {
          end_ruler = i + 1;

          if(end_ruler > 300)
          {
            end_ruler = 300;
          }

          break;
        }
      }

      if(start_ruler >= end_ruler)
      {
        start_ruler = 0;

        end_ruler = 300;
      }

      startSlider->setValue(start_ruler);
      stopSlider->setValue(end_ruler);

      curve1->setH_RulerValues(start_ruler + 1, end_ruler + 1);

      curve1->drawCurve(bpm_distribution + start_ruler, end_ruler - start_ruler, (int)(max_val * 1.1) + 1, 0.0);

      QObject::connect(startSlider, SIGNAL(valueChanged(int)), this, SLOT(startSliderMoved(int)));
      QObject::connect(stopSlider,  SIGNAL(valueChanged(int)), this, SLOT(stopSliderMoved(int)));
    }
  }

  Label1->setText(stat_str);

  StatDialog->exec();
}


void UI_StatisticWindow::startSliderMoved(int)
{
  startSlider->blockSignals(true);
  stopSlider->blockSignals(true);

  start_ruler = startSlider->value();
  end_ruler = stopSlider->value();

  if(end_ruler < (start_ruler + 5))
  {
    end_ruler = start_ruler + 5;

    stopSlider->setValue(end_ruler);
  }

  curve1->setH_RulerValues(start_ruler + 1, end_ruler + 1);

  curve1->drawCurve(bpm_distribution + start_ruler, end_ruler - start_ruler, (int)(max_val * 1.1) + 1, 0.0);

  startSlider->blockSignals(false);
  stopSlider->blockSignals(false);
}


void UI_StatisticWindow::stopSliderMoved(int)
{
  startSlider->blockSignals(true);
  stopSlider->blockSignals(true);

  start_ruler = startSlider->value();
  end_ruler = stopSlider->value();

  if(start_ruler > (end_ruler - 5))
  {
    start_ruler = end_ruler - 5;

    startSlider->setValue(start_ruler);
  }

  curve1->setH_RulerValues(start_ruler + 1, end_ruler + 1);

  curve1->drawCurve(bpm_distribution + start_ruler, end_ruler - start_ruler, (int)(max_val * 1.1) + 1, 0.0);

  startSlider->blockSignals(false);
  stopSlider->blockSignals(false);
}




