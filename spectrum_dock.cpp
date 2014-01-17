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



#include "spectrum_dock.h"



UI_SpectrumDockWindow::UI_SpectrumDockWindow(QWidget *w_parent)
{
  char str[600];

  buf1 = NULL;
  buf2 = NULL;
  buf3 = NULL;

  busy = 0;

  dashboard = 0;

  signalcomp = NULL;

  mainwindow = (UI_Mainwindow *)w_parent;

  spectrum_color = mainwindow->spectrum_colorbar;

  sqrt_powerspectrum = 0;

  SpectrumDialog = new QDialog;

  dock = new QDockWidget("Power Spectral Density", w_parent);
  dock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
  dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
  dock->setMinimumHeight(300);
  if(dashboard)
  {
    dock->setWidget(SpectrumDialog);
  }

  curve1 = new SignalCurve;
  curve1->setSignalColor(Qt::green);
  curve1->setBackgroundColor(Qt::black);
  curve1->setRasterColor(Qt::gray);
  curve1->setTraceWidth(0);
  curve1->setH_label("Hz");
  curve1->setLowerLabel("Frequency");
  if(sqrt_powerspectrum)
  {
    curve1->setV_label(physdimension);
  }
  else
  {
    snprintf(str, 512, "(%s)^2/Hz", physdimension);
    curve1->setV_label(str);
  }
  curve1->create_button("to Text");

  if(!dashboard)
  {
    dock->setWidget(curve1);
  }

  amplitudeSlider = new QSlider;
  amplitudeSlider->setOrientation(Qt::Vertical);
  amplitudeSlider->setMinimum(1);
  amplitudeSlider->setMaximum(2000);
  amplitudeSlider->setValue(1000);
  amplitudeSlider->setInvertedAppearance(TRUE);
  amplitudeSlider->setMinimumSize(15, 110);

  amplitudeLabel = new QLabel;
  amplitudeLabel->setText("Amplitude");
  amplitudeLabel->setMinimumSize(100, 15);
  amplitudeLabel->setAlignment(Qt::AlignHCenter);

  sqrtButton = new QRadioButton;
  sqrtButton->setMinimumSize(50, 20);
  sqrtButton->setText("Amplitude");
  sqrtButton->setAutoExclusive(FALSE);

  colorBarButton = new QRadioButton;
  colorBarButton->setMinimumSize(50, 20);
  colorBarButton->setText("Colorbar");
  colorBarButton->setAutoExclusive(FALSE);

  vlayout2 = new QVBoxLayout;
  vlayout2->setSpacing(20);
  vlayout2->addStretch(100);
  vlayout2->addWidget(amplitudeLabel, 0, Qt::AlignHCenter);
  vlayout2->addWidget(amplitudeSlider, 0, Qt::AlignHCenter);
  vlayout2->addWidget(sqrtButton);
  vlayout2->addWidget(colorBarButton);

  spanSlider = new QSlider;
  spanSlider->setOrientation(Qt::Horizontal);
  spanSlider->setMinimum(10);
  spanSlider->setMaximum(1000);
  spanSlider->setValue(1000);
  spanSlider->setMinimumSize(500, 15);

  spanLabel = new QLabel;
  spanLabel->setText("Span");
  spanLabel->setMinimumSize(100, 15);
  spanLabel->setAlignment(Qt::AlignHCenter);

  centerSlider = new QSlider;
  centerSlider->setOrientation(Qt::Horizontal);
  centerSlider->setMinimum(0);
  centerSlider->setMaximum(1000);
  centerSlider->setValue(0);
  centerSlider->setMinimumSize(500, 15);

  centerLabel = new QLabel;
  centerLabel->setText("Center");
  centerLabel->setMinimumSize(100, 15);
  centerLabel->setAlignment(Qt::AlignHCenter);

  hlayout1 = new QHBoxLayout;
  hlayout1->setSpacing(20);
  hlayout1->addLayout(vlayout2);
  if(dashboard)
  {
    hlayout1->addWidget(curve1, 100);
  }

  hlayout2 = new QHBoxLayout;
  hlayout2->setSpacing(20);
  hlayout2->addWidget(spanLabel);
  hlayout2->addWidget(spanSlider);
  hlayout2->addStretch(100);

  hlayout3 = new QHBoxLayout;
  hlayout3->setSpacing(20);
  hlayout3->addWidget(centerLabel);
  hlayout3->addWidget(centerSlider);
  hlayout3->addStretch(100);

  vlayout1 = new QVBoxLayout;
  vlayout1->setSpacing(20);
  vlayout1->addLayout(hlayout1);
  vlayout1->addLayout(hlayout2);
  vlayout1->addLayout(hlayout3);

  SpectrumDialog->setLayout(vlayout1);

  t1 = new QTimer(this);
  t1->setSingleShot(TRUE);

  QObject::connect(t1,              SIGNAL(timeout()),              this, SLOT(update_curve()));
  QObject::connect(amplitudeSlider, SIGNAL(valueChanged(int)),      this, SLOT(sliderMoved(int)));
  QObject::connect(spanSlider,      SIGNAL(valueChanged(int)),      this, SLOT(sliderMoved(int)));
  QObject::connect(centerSlider,    SIGNAL(valueChanged(int)),      this, SLOT(sliderMoved(int)));
  QObject::connect(sqrtButton,      SIGNAL(toggled(bool)),          this, SLOT(sqrtButtonClicked(bool)));
  QObject::connect(colorBarButton,  SIGNAL(toggled(bool)),          this, SLOT(colorBarButtonClicked(bool)));
  QObject::connect(curve1,          SIGNAL(extra_button_clicked()), this, SLOT(print_to_txt()));
  QObject::connect(curve1,          SIGNAL(dashBoardClicked()),     this, SLOT(setdashboard()));
}


void UI_SpectrumDockWindow::setdashboard()
{
  if(dashboard)
  {
    dashboard = 0;
    dock->setWidget(curve1);
  }
  else
  {
    dashboard = 1;
    hlayout1->addWidget(curve1, 100);
    dock->setWidget(SpectrumDialog);
  }
}


void UI_SpectrumDockWindow::print_to_txt()
{
  int i;

  char str[1024],
       path[MAX_PATH_LENGTH];

  FILE *outputfile;


  path[0] = 0;
  if(mainwindow->recent_savedir[0]!=0)
  {
    strcpy(path, mainwindow->recent_savedir);
    strcat(path, "/");
  }
  strcat(path, "spectrum.txt");

  strcpy(path, QFileDialog::getSaveFileName(0, "Export to text (ASCII)", QString::fromLocal8Bit(path), "Text files (*.txt *.TXT)").toLocal8Bit().data());

  if(!strcmp(path, ""))
  {
    return;
  }

  get_directory_from_path(mainwindow->recent_savedir, path, MAX_PATH_LENGTH);

  outputfile = fopen(path, "wb");
  if(outputfile == NULL)
  {
    snprintf(str, 1024, "Can not open file %s for writing.", path);
    QMessageBox messagewindow(QMessageBox::Critical, "Error", QString::fromLocal8Bit(str));
    messagewindow.exec();
    return;
  }

  sprintf(str, "FFT Power Spectral Density (Power/%fHz)\n", freqstep);
  remove_trailing_zeros(str);
  fprintf(outputfile, "%s", str);
  fprintf(outputfile, "Signal: %s\n", signalcomp->signallabel);
  sprintf(str, "FFT blocksize: %i\n", mainwindow->maxdftblocksize);
  sprintf(str + strlen(str), "FFT resolution: %f Hz\n", freqstep);
  sprintf(str + strlen(str), "Data Samples: %i\n", samples);
  sprintf(str + strlen(str), "Power Samples: %i\n", steps);
  sprintf(str + strlen(str), "Samplefrequency: %f Hz\n", (double)signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].smp_per_record / ((double)signalcomp->edfhdr->long_data_record_duration / TIME_DIMENSION));
  remove_trailing_zeros(str);
  fprintf(outputfile, "%s", str);

  for(i=0; i<steps; i++)
  {
    fprintf(outputfile, "%.16f\t%.16f\n", freqstep * i, buf2[i]);
  }

  fclose (outputfile);
}


void UI_SpectrumDockWindow::colorBarButtonClicked(bool value)
{
  if(value == TRUE)
  {
    curve1->enableSpectrumColors(spectrum_color);
  }
  else
  {
    curve1->disableSpectrumColors();
  }
}


void UI_SpectrumDockWindow::sqrtButtonClicked(bool value)
{
  char str[600];

  if(value == FALSE)
  {
    sqrt_powerspectrum = 0;

    sprintf(str, "Power Spectral Density %s", signallabel);

    dock->setWindowTitle(str);

    sprintf(str, "(%s)^2/Hz", physdimension);

    curve1->setV_label(str);
  }
  else
  {
    sqrt_powerspectrum = 1;

    sprintf(str, "Amplitude Spectrum %s", signallabel);

    curve1->setV_label(physdimension);

    dock->setWindowTitle(str);
  }

  sliderMoved(0);
}


void UI_SpectrumDockWindow::sliderMoved(int)
{
  int startstep,
      stopstep,
      precision,
      spanstep;

  double max_freq,
         start_freq;


  spanstep = spanSlider->value() * steps / 1000;

  startstep = centerSlider->value() * (steps - spanstep) / 1000;

  stopstep = startstep + spanstep;

  if(sqrt_powerspectrum)
  {
    curve1->drawCurve(buf3 + startstep, stopstep - startstep, (maxvalue_sqrt * 1.05 * (double)amplitudeSlider->value()) / 1000.0, 0.0);
  }
  else
  {
    curve1->drawCurve(buf2 + startstep, stopstep - startstep, (maxvalue * 1.05 * (double)amplitudeSlider->value()) / 1000.0, 0.0);
  }

  max_freq = ((double)samplefreq / 2.0) * stopstep / steps;

  precision = 0;
  if(max_freq < 10.0)
  {
    precision = 1;
  }
  if(max_freq < 1.0)
  {
    precision = 2;
  }
  if(max_freq < 0.1)
  {
    precision = 3;
  }
  if(max_freq < 0.01)
  {
    precision = 4;
  }

  start_freq = ((double)samplefreq / 2.0) * startstep / steps;

  curve1->setH_RulerValues(start_freq, max_freq);

  centerLabel->setText(QString::number(start_freq + ((max_freq - start_freq) / 2.0), 'f', precision).append(" Hz").prepend("Center "));

  spanLabel->setText(QString::number(max_freq - start_freq, 'f', precision).append(" Hz").prepend("Span "));
}


void UI_SpectrumDockWindow::init(int signal_nr)
{
  char str[600];


  if(signal_nr < 0)
  {
    signalcomp = NULL;

    viewbuf = NULL;
  }
  else
  {
    signalcomp = mainwindow->signalcomp[signal_nr];

    viewbuf = mainwindow->viewbuf;

    strcpy(signallabel, signalcomp->signallabel);

    strcpy(physdimension, signalcomp->physdimension);

    if(sqrt_powerspectrum)
    {
      curve1->setV_label(physdimension);
    }
    else
    {
      snprintf(str, 512, "(%s)^2/Hz", physdimension);
      curve1->setV_label(str);
    }

    dock->show();

    t1->start(100);
  }
}


void UI_SpectrumDockWindow::rescan()
{
  t1->start(100);
}


void UI_SpectrumDockWindow::clear()
{
  int i;

  signalcomp = NULL;

  viewbuf = NULL;

  if(buf1 != NULL)
  {
    free(buf1);
    buf1 = NULL;
  }

  if(buf2 != NULL)
  {
    free(buf2);
    buf2 = NULL;
  }

  if(buf3 != NULL)
  {
    free(buf3);
    buf3 = NULL;
  }

  if(spectrum_color != NULL)
  {
    for(i=0; i < spectrum_color->items; i++)
    {
      spectrum_color->value[i] = 0.0;
    }
  }

  curve1->clear();
}


void UI_SpectrumDockWindow::update_curve()
{
  int i, j, k, n,
      dftblocksize,
      dftblocks,
      samplesleft,
      fft_outputbufsize;

  long long s, s2;

  char str[512];

  double dig_value=0.0,
         f_tmp=0.0;

  union {
          unsigned int one;
          signed int one_signed;
          unsigned short two[2];
          signed short two_signed[2];
          unsigned char four[4];
        } var;

  if(signalcomp == NULL)
  {
    return;
  }

  if(busy)
  {
    return;
  }

  viewbuf = mainwindow->viewbuf;

  if(viewbuf == NULL)
  {
    return;
  }

  busy = 1;

  curve1->setUpdatesEnabled(FALSE);

  samples = signalcomp->samples_on_screen;

  if(signalcomp->samples_on_screen > signalcomp->sample_stop)
  {
    samples = signalcomp->sample_stop;
  }

  samples -= signalcomp->sample_start;

  if((samples < 10) || (viewbuf == NULL))
  {
    curve1->setUpdatesEnabled(TRUE);

    busy = 0;

    if(spectrum_color != NULL)
    {
      for(i=0; i < spectrum_color->items; i++)
      {
        spectrum_color->value[i] = 0.0;
      }
    }

    curve1->clear();

    return;
  }

  if(buf1 != NULL)
  {
    free(buf1);
  }
  buf1 = (double *)malloc(sizeof(double) * signalcomp->samples_on_screen);
  if(buf1 == NULL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "The system was not able to provide enough resources (memory) to perform the requested action.\n"
                                  "Decrease the timescale and try again.");
    messagewindow.exec();
    return;
  }

  samples = 0;

  for(s=signalcomp->sample_start; s<signalcomp->samples_on_screen; s++)
  {
    if(s>signalcomp->sample_stop)  break;

    dig_value = 0.0;
    s2 = s + signalcomp->sample_timeoffset - signalcomp->sample_start;

    for(j=0; j<signalcomp->num_of_signals; j++)
    {
      if(signalcomp->edfhdr->bdf)
      {
        var.two[0] = *((unsigned short *)(
          viewbuf
          + signalcomp->viewbufoffset
          + (signalcomp->edfhdr->recordsize * (s2 / signalcomp->edfhdr->edfparam[signalcomp->edfsignal[j]].smp_per_record))
          + signalcomp->edfhdr->edfparam[signalcomp->edfsignal[j]].buf_offset
          + ((s2 % signalcomp->edfhdr->edfparam[signalcomp->edfsignal[j]].smp_per_record) * 3)));

        var.four[2] = *((unsigned char *)(
          viewbuf
          + signalcomp->viewbufoffset
          + (signalcomp->edfhdr->recordsize * (s2 / signalcomp->edfhdr->edfparam[signalcomp->edfsignal[j]].smp_per_record))
          + signalcomp->edfhdr->edfparam[signalcomp->edfsignal[j]].buf_offset
          + ((s2 % signalcomp->edfhdr->edfparam[signalcomp->edfsignal[j]].smp_per_record) * 3)
          + 2));

        if(var.four[2]&0x80)
        {
          var.four[3] = 0xff;
        }
        else
        {
          var.four[3] = 0x00;
        }

        f_tmp = var.one_signed;
      }

      if(signalcomp->edfhdr->edf)
      {
        f_tmp = *(((short *)(
          viewbuf
          + signalcomp->viewbufoffset
          + (signalcomp->edfhdr->recordsize * (s2 / signalcomp->edfhdr->edfparam[signalcomp->edfsignal[j]].smp_per_record))
          + signalcomp->edfhdr->edfparam[signalcomp->edfsignal[j]].buf_offset))
          + (s2 % signalcomp->edfhdr->edfparam[signalcomp->edfsignal[j]].smp_per_record));
      }

      f_tmp += signalcomp->edfhdr->edfparam[signalcomp->edfsignal[j]].offset;
      f_tmp *= signalcomp->factor[j];

      dig_value += f_tmp;
    }

    for(k=0; k<signalcomp->filter_cnt; k++)
    {
      dig_value = first_order_filter(dig_value, signalcomp->filter[k]);
    }

    for(k=0; k<signalcomp->ravg_filter_cnt; k++)
    {
      if(s==signalcomp->sample_start)
      {
        ravg_filter_restore_buf(signalcomp->ravg_filter[k]);
      }

      dig_value = run_ravg_filter(dig_value, signalcomp->ravg_filter[k]);
    }

    for(k=0; k<signalcomp->fidfilter_cnt; k++)
    {
      if(s==signalcomp->sample_start)
      {
        memcpy(signalcomp->fidbuf[k], signalcomp->fidbuf2[k], fid_run_bufsize(signalcomp->fid_run[k]));
      }

      dig_value = signalcomp->fidfuncp[k](signalcomp->fidbuf[k], dig_value);
    }

    if(signalcomp->ecg_filter != NULL)
    {
      if(s==signalcomp->sample_start)
      {
        ecg_filter_restore_buf(signalcomp->ecg_filter);
      }

      dig_value = run_ecg_filter(dig_value, signalcomp->ecg_filter);
    }

    if(s>=signalcomp->sample_start)
    {
      buf1[samples++] = dig_value * signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].bitvalue;
    }
  }

  samplefreq = (double)signalcomp->edfhdr->edfparam[signalcomp->edfsignal[0]].smp_per_record / ((double)signalcomp->edfhdr->long_data_record_duration / TIME_DIMENSION);

  dftblocksize = mainwindow->maxdftblocksize;

  if(dftblocksize & 1)
  {
    dftblocksize--;
  }

  dftblocks = 1;

  if(dftblocksize < samples)
  {
    dftblocks = samples / dftblocksize;
  }
  else
  {
    dftblocksize = samples;
  }

  if(dftblocksize & 1)
  {
    dftblocksize--;
  }

  samplesleft = samples % dftblocksize;

  if(samplesleft & 1)
  {
    samplesleft--;
  }

  freqstep = samplefreq / (double)dftblocksize;

  fft_outputbufsize = dftblocksize / 2;

  steps = fft_outputbufsize;

  if(buf2 != NULL)
  {
    free(buf2);
  }
  buf2 = (double *)calloc(1, sizeof(double) * fft_outputbufsize);
  if(buf2 == NULL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "The system was not able to provide enough resources (memory) to perform the requested action.");
    messagewindow.exec();
    free(buf1);
    buf1 = NULL;
    return;
  }

  if(buf3 != NULL)
  {
    free(buf3);
  }
  buf3 = (double *)malloc(sizeof(double) * fft_outputbufsize);
  if(buf3 == NULL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "The system was not able to provide enough resources (memory) to perform the requested action.");
    messagewindow.exec();
    free(buf1);
    free(buf2);
    buf1 = NULL;
    buf2 = NULL;
    return;
  }

  maxvalue = 0.000001;
  maxvalue_sqrt = 0.000001;

  kiss_fftr_cfg cfg;

  kiss_fft_cpx *kiss_fftbuf;

  kiss_fftbuf = (kiss_fft_cpx *)malloc((fft_outputbufsize + 1) * sizeof(kiss_fft_cpx));
  if(kiss_fftbuf == NULL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "The system was not able to provide enough resources (memory) to perform the requested action.");
    messagewindow.exec();
    free(buf1);
    free(buf2);
    free(buf3);
    buf1 = NULL;
    buf2 = NULL;
    buf3 = NULL;
    return;
  }

  cfg = kiss_fftr_alloc(dftblocksize, 0, NULL, NULL);

  for(j=0; j<dftblocks; j++)
  {
    kiss_fftr(cfg, buf1 + (j * dftblocksize), kiss_fftbuf);

    for(i=0; i<fft_outputbufsize; i++)
    {
      buf2[i] += (((kiss_fftbuf[i].r * kiss_fftbuf[i].r) + (kiss_fftbuf[i].i * kiss_fftbuf[i].i)) / fft_outputbufsize);
    }
  }

  if(samplesleft)
  {
    kiss_fftr(cfg, buf1 + (((j-1) * dftblocksize) + samplesleft), kiss_fftbuf);

    for(i=0; i<fft_outputbufsize; i++)
    {
      buf2[i] += (((kiss_fftbuf[i].r * kiss_fftbuf[i].r) + (kiss_fftbuf[i].i * kiss_fftbuf[i].i)) / fft_outputbufsize);

      buf2[i] /= (dftblocks + 1);
    }
  }
  else
  {
    for(i=0; i<fft_outputbufsize; i++)
    {
      buf2[i] /= dftblocks;
    }
  }

  if(signalcomp->ecg_filter == NULL)
  {
    buf2[0] /= 2.0;  // DC!
  }
  else
  {
    buf2[0] = 0.0;  // Remove DC because heart rate is always a positive value
  }


  free(cfg);

  free(kiss_fftbuf);

  for(i=0; i<fft_outputbufsize; i++)
  {
    buf2[i] /= samplefreq;

    buf3[i] = sqrt(buf2[i] * freqstep);

    if(i)  // don't use the dc-bin for the autogain of the screen
    {
      if(buf2[i] > maxvalue)
      {
        maxvalue = buf2[i];
      }

      if(buf3[i] > maxvalue_sqrt)
      {
        maxvalue_sqrt = buf3[i];
      }
    }
  }

  if(samplesleft)
  {
    dftblocks++;
  }

  if(buf1 != NULL)
  {
    free(buf1);

    buf1 = NULL;
  }

  sprintf(str, "FFT resolution: %f Hz   %i blocks of %i samples", freqstep, dftblocks, dftblocksize);

  remove_trailing_zeros(str);

  curve1->setUpperLabel1(str);

  curve1->setUpperLabel2(signallabel);

  if(spectrum_color != NULL)
  {
    if(spectrum_color->items > 0)
    {
      spectrum_color->value[0] = 0.0;

      n = 0;

      for(j=0; j<steps; j++)
      {
        if(((freqstep * j) + (freqstep * 0.5)) < spectrum_color->freq[0])
        {
          if(spectrum_color->method == 0)  // sum
          {
            spectrum_color->value[0] += buf2[j];
          }

          if(spectrum_color->method == 1)  // peak
          {
            if(spectrum_color->value[0] < buf2[j])
            {
              spectrum_color->value[0] = buf2[j];
            }
          }

          if(spectrum_color->method == 2)  // average
          {
            spectrum_color->value[0] += buf2[j];

            n++;
          }
        }
      }

      if(spectrum_color->method == 2)  // average
      {
        if(n)
        {
          spectrum_color->value[0] /= n;
        }
      }
    }

    for(i=1; i < spectrum_color->items; i++)
    {
      spectrum_color->value[i] = 0.0;

      n = 0;

      for(j=0; j<steps; j++)
      {
        if((((freqstep * j) + (freqstep * 0.5)) > spectrum_color->freq[i-1]) && (((freqstep * j) + (freqstep * 0.5)) < spectrum_color->freq[i]))
        {
          if(spectrum_color->method == 0)  // sum
          {
            spectrum_color->value[i] += buf2[j];
          }

          if(spectrum_color->method == 1)  // peak
          {
            if(spectrum_color->value[i] < buf2[j])
            {
              spectrum_color->value[i] = buf2[j];
            }
          }

          if(spectrum_color->method == 2)  // average
          {
            spectrum_color->value[i] += buf2[j];

            n++;
          }
        }
      }

      if(spectrum_color->method == 2)  // average
      {
        if(n)
        {
          spectrum_color->value[i] /= n;
        }
      }
    }
  }

  if(sqrt_powerspectrum)
  {
    snprintf(str, 512, "Amplitude Spectrum %s", signallabel);
  }
  else
  {
    snprintf(str, 512, "Power Spectral Density %s", signallabel);
  }

  dock->setWindowTitle(str);

  sliderMoved(0);

  curve1->setUpdatesEnabled(TRUE);

  busy = 0;
}



UI_SpectrumDockWindow::~UI_SpectrumDockWindow()
{
  if(buf2 != NULL)
  {
    free(buf2);
  }

  if(buf3 != NULL)
  {
    free(buf3);
  }

  delete SpectrumDialog;
}




















