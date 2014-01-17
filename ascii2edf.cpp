/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2008, 2009, 2010, 2011, 2012, 2013, 2014 Teunis van Beelen
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



#include "ascii2edf.h"



#if defined(__APPLE__) || defined(__MACH__) || defined(__APPLE_CC__)

#define fopeno fopen

#else

#define fseeko fseeko64
#define ftello ftello64
#define fopeno fopen64

#endif





UI_ASCII2EDFapp::UI_ASCII2EDFapp(char *recent_dir, char *save_dir)
{
  recent_opendir = recent_dir;
  recent_savedir = save_dir;

  autoPhysicalMaximum = 1;

  edf_format = 1;

  ascii2edfDialog = new QDialog;

  ascii2edfDialog->setMinimumSize(QSize(600, 440));
  ascii2edfDialog->setMaximumSize(QSize(600, 440));
  ascii2edfDialog->setWindowTitle("ASCII to EDF/BDF converter");
  ascii2edfDialog->setModal(TRUE);
  ascii2edfDialog->setAttribute(Qt::WA_DeleteOnClose, TRUE);

  SeparatorLabel = new QLabel(ascii2edfDialog);
  SeparatorLabel->setGeometry(QRect(20, 20, 140, 16));
  SeparatorLabel->setText("Column separator");

  NumsignalsLabel = new QLabel(ascii2edfDialog);
  NumsignalsLabel->setGeometry(QRect(20, 50, 140, 16));
  NumsignalsLabel->setText("Number of columns");

  DatastartLabel = new QLabel(ascii2edfDialog);
  DatastartLabel->setGeometry(QRect(20, 80, 140, 16));
  DatastartLabel->setText("Data starts at line");

  SamplefreqLabel = new QLabel(ascii2edfDialog);
  SamplefreqLabel->setGeometry(QRect(20, 110, 140, 16));
  SamplefreqLabel->setText("Samplefrequency");

  PatientnameLabel = new QLabel(ascii2edfDialog);
  PatientnameLabel->setGeometry(QRect(20, 140, 140, 16));
  PatientnameLabel->setText("Subject name");

  RecordingLabel = new QLabel(ascii2edfDialog);
  RecordingLabel->setGeometry(QRect(20, 170, 140, 16));
  RecordingLabel->setText("Recording");

  DatetimeLabel = new QLabel(ascii2edfDialog);
  DatetimeLabel->setGeometry(QRect(20, 200, 140, 16));
  DatetimeLabel->setText("Startdate and time");

  autoPhysicalMaximumLabel = new QLabel(ascii2edfDialog);
  autoPhysicalMaximumLabel->setGeometry(QRect(360, 200, 180, 16));
  autoPhysicalMaximumLabel->setText("Auto detect physical maximum");

  SignalsLabel = new QLabel(ascii2edfDialog);
  SignalsLabel->setGeometry(QRect(20, 240, 140, 16));
  SignalsLabel->setText("Signals");

  groupbox1 = new QGroupBox(ascii2edfDialog);
  groupbox1->setGeometry(QRect(330, 20, 160, 90));
  groupbox1->setTitle("Output format");

  edfButton = new QRadioButton("EDF (16-bit)");
  bdfButton = new QRadioButton("BDF (24-bit)");
  edfButton->setChecked(TRUE);

  vbox1 = new QVBoxLayout;
  vbox1->addWidget(edfButton);
  vbox1->addWidget(bdfButton);
  vbox1->addStretch(1);

  groupbox1->setLayout(vbox1);

  SeparatorLineEdit = new QLineEdit(ascii2edfDialog);
  SeparatorLineEdit->setGeometry(QRect(160, 20, 30, 21));
  SeparatorLineEdit->setMaxLength(3);
  SeparatorLineEdit->setText("tab");

  NumcolumnsSpinbox = new QSpinBox(ascii2edfDialog);
  NumcolumnsSpinbox->setGeometry(QRect(160, 50, 70, 22));
  NumcolumnsSpinbox->setRange(1,256);
  NumcolumnsSpinbox->setValue(1);

  DatastartSpinbox = new QSpinBox(ascii2edfDialog);
  DatastartSpinbox->setGeometry(QRect(160, 80, 70, 22));
  DatastartSpinbox->setRange(1,100);
  DatastartSpinbox->setValue(1);

  SamplefreqSpinbox = new QDoubleSpinBox(ascii2edfDialog);
  SamplefreqSpinbox->setGeometry(QRect(160, 110, 140, 22));
  SamplefreqSpinbox->setDecimals(7);
  SamplefreqSpinbox->setRange(0.0000001,1000000.0);
  SamplefreqSpinbox->setSuffix(" Hz");
  SamplefreqSpinbox->setValue(1.0);

  PatientnameLineEdit = new QLineEdit(ascii2edfDialog);
  PatientnameLineEdit->setGeometry(QRect(160, 140, 420, 21));
  PatientnameLineEdit->setMaxLength(80);

  RecordingLineEdit = new QLineEdit(ascii2edfDialog);
  RecordingLineEdit->setGeometry(QRect(160, 170, 420, 21));
  RecordingLineEdit->setMaxLength(80);

  StartDatetimeedit = new QDateTimeEdit(ascii2edfDialog);
  StartDatetimeedit->setGeometry(QRect(160, 200, 140, 22));
  StartDatetimeedit->setDisplayFormat("dd/MM/yyyy hh:mm:ss");
  StartDatetimeedit->setDateTime(QDateTime::currentDateTime());

  autoPhysicalMaximumCheckbox = new QCheckBox(ascii2edfDialog);
  autoPhysicalMaximumCheckbox->setGeometry(QRect(560, 200, 20, 20));
  autoPhysicalMaximumCheckbox->setTristate(FALSE);
  autoPhysicalMaximumCheckbox->setCheckState(Qt::Checked);

  SignalsTablewidget = new QTableWidget(ascii2edfDialog);
  SignalsTablewidget->setGeometry(QRect(160, 230, 420, 140));
  SignalsTablewidget->setSelectionMode(QAbstractItemView::NoSelection);
  SignalsTablewidget->setColumnCount(5);
  SignalsTablewidget->setColumnWidth(0, 20);
  SignalsTablewidget->setColumnWidth(1, 120);
  SignalsTablewidget->setColumnWidth(2, 120);
  SignalsTablewidget->setColumnWidth(3, 120);
  SignalsTablewidget->setColumnWidth(4, 120);
  SignalsTablewidget->setRowCount(1);
  SignalsTablewidget->setRowHeight(0, 20);
  SignalsTablewidget->setCellWidget(0, 0, new QCheckBox);
  ((QCheckBox *)SignalsTablewidget->cellWidget(0, 0))->setCheckState(Qt::Checked);
  SignalsTablewidget->setCellWidget(0, 1, new QLineEdit);
  ((QLineEdit *)SignalsTablewidget->cellWidget(0, 1))->setMaxLength(16);
  SignalsTablewidget->setCellWidget(0, 2, new QLineEdit);
  ((QLineEdit *)SignalsTablewidget->cellWidget(0, 2))->setMaxLength(7);
  ((QLineEdit *)SignalsTablewidget->cellWidget(0, 2))->setEnabled(FALSE);
  SignalsTablewidget->setCellWidget(0, 3, new QLineEdit);
  ((QLineEdit *)SignalsTablewidget->cellWidget(0, 3))->setMaxLength(8);
  SignalsTablewidget->setCellWidget(0, 4, new QDoubleSpinBox);
  ((QDoubleSpinBox *)SignalsTablewidget->cellWidget(0, 4))->setDecimals(4);
  ((QDoubleSpinBox *)SignalsTablewidget->cellWidget(0, 4))->setRange(0.0001,1000000.0);
  ((QDoubleSpinBox *)SignalsTablewidget->cellWidget(0, 4))->setValue(1.0);

  QStringList horizontallabels;
  horizontallabels += "";
  horizontallabels += "Label";
  horizontallabels += "Physical maximum";
  horizontallabels += "Physical dimension";
  horizontallabels += "Multiplier";
  SignalsTablewidget->setHorizontalHeaderLabels(horizontallabels);

  GoButton = new QPushButton(ascii2edfDialog);
  GoButton->setGeometry(QRect(20, 400, 75, 25));
  GoButton->setText("Start");

  CloseButton = new QPushButton(ascii2edfDialog);
  CloseButton->setGeometry(QRect(160, 400, 75, 25));
  CloseButton->setText("Cancel");

  SaveButton = new QPushButton(ascii2edfDialog);
  SaveButton->setGeometry(QRect(400, 400, 75, 25));
  SaveButton->setText("Save");

  LoadButton = new QPushButton(ascii2edfDialog);
  LoadButton->setGeometry(QRect(510, 400, 75, 25));
  LoadButton->setText("Load");

  columns = 1;

  edfsignals = 1;

  QObject::connect(NumcolumnsSpinbox,           SIGNAL(valueChanged(int)), this,            SLOT(numofcolumnschanged(int)));
  QObject::connect(GoButton,                    SIGNAL(clicked()),         this,            SLOT(gobuttonpressed()));
  QObject::connect(CloseButton,                 SIGNAL(clicked()),         ascii2edfDialog, SLOT(close()));
  QObject::connect(SaveButton,                  SIGNAL(clicked()),         this,            SLOT(savebuttonpressed()));
  QObject::connect(LoadButton,                  SIGNAL(clicked()),         this,            SLOT(loadbuttonpressed()));
  QObject::connect(autoPhysicalMaximumCheckbox, SIGNAL(stateChanged(int)), this,            SLOT(autoPhysicalMaximumCheckboxChanged(int)));

  ascii2edfDialog->exec();
}



void UI_ASCII2EDFapp::autoPhysicalMaximumCheckboxChanged(int state)
{
  int i;

  if(state == Qt::Checked)
  {
    autoPhysicalMaximum = 1;

    for(i=0; i< columns; i++)
    {
      ((QLineEdit *)SignalsTablewidget->cellWidget(i, 2))->setEnabled(FALSE);
    }
  }
  else
  {
    autoPhysicalMaximum = 0;

    for(i=0; i< columns; i++)
    {
      ((QLineEdit *)SignalsTablewidget->cellWidget(i, 2))->setEnabled(TRUE);
    }
  }
}



void UI_ASCII2EDFapp::numofcolumnschanged(int cnt)
{
  int i;

  if(cnt>columns)
  {
    SignalsTablewidget->setRowCount(cnt);

    for(i=columns; i<cnt; i++)
    {
      SignalsTablewidget->setRowHeight(i, 20);

      SignalsTablewidget->setCellWidget(i, 0, new QCheckBox);
      ((QCheckBox *)SignalsTablewidget->cellWidget(i, 0))->setCheckState(Qt::Checked);
      SignalsTablewidget->setCellWidget(i, 1, new QLineEdit);
      ((QLineEdit *)SignalsTablewidget->cellWidget(i, 1))->setMaxLength(16);
      SignalsTablewidget->setCellWidget(i, 2, new QLineEdit);
      ((QLineEdit *)SignalsTablewidget->cellWidget(i, 2))->setMaxLength(7);
      if(autoPhysicalMaximum)
      {
        ((QLineEdit *)SignalsTablewidget->cellWidget(i, 2))->setEnabled(FALSE);
      }
      else
      {
        ((QLineEdit *)SignalsTablewidget->cellWidget(i, 2))->setEnabled(TRUE);
      }
      SignalsTablewidget->setCellWidget(i, 3, new QLineEdit);
      ((QLineEdit *)SignalsTablewidget->cellWidget(i, 3))->setMaxLength(8);
      SignalsTablewidget->setCellWidget(i, 4, new QDoubleSpinBox);
      ((QDoubleSpinBox *)SignalsTablewidget->cellWidget(i, 4))->setDecimals(4);
      ((QDoubleSpinBox *)SignalsTablewidget->cellWidget(i, 4))->setRange(0.0001,1000000.0);
      ((QDoubleSpinBox *)SignalsTablewidget->cellWidget(i, 4))->setValue(1.0);

      edfsignals++;
    }
  }

  if(cnt<columns)
  {
    for(i=columns-1; i>=cnt; i--)
    {
      delete SignalsTablewidget->cellWidget(i, 0);
      delete SignalsTablewidget->cellWidget(i, 1);
      delete SignalsTablewidget->cellWidget(i, 2);
      delete SignalsTablewidget->cellWidget(i, 3);
      delete SignalsTablewidget->cellWidget(i, 4);

      SignalsTablewidget->removeRow(i);

      edfsignals--;
    }
  }

  columns = cnt;
}



void UI_ASCII2EDFapp::gobuttonpressed()
{
  int i, j, k, p,
      column,
      column_end,
      headersize,
      temp,
      datarecords,
      str_start,
      line_nr,
      smpls_per_block=0,
      bufsize=0,
      edf_signal,
      len;

  char path[MAX_PATH_LENGTH],
       txt_string[ASCII_MAX_LINE_LEN],
       str[256],
       line[ASCII_MAX_LINE_LEN],
       *buf,
       scratchpad[128],
       outputfilename[MAX_PATH_LENGTH];

  FILE *inputfile,
       *outputfile;



  ascii2edfDialog->setEnabled(FALSE);

  if(edfButton->isChecked() == TRUE)
  {
    edf_format = 1;
  }
  else
  {
    edf_format = 0;
  }

  if(check_input())
  {
    ascii2edfDialog->setEnabled(TRUE);
    return;
  }

  if(!(strlen(PatientnameLineEdit->text().toLatin1().data())))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", "Please enter a subjectname.");
    messagewindow.exec();
    ascii2edfDialog->setEnabled(TRUE);
    return;
  }

  if(!(strlen(RecordingLineEdit->text().toLatin1().data())))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", "Please enter a recording description.");
    messagewindow.exec();
    ascii2edfDialog->setEnabled(TRUE);
    return;
  }

  strcpy(path, QFileDialog::getOpenFileName(0, "Open ASCII file", QString::fromLocal8Bit(recent_opendir), "ASCII files (*.txt *.TXT *.csv *.CSV)").toLocal8Bit().data());

  if(!strcmp(path, ""))
  {
    ascii2edfDialog->setEnabled(TRUE);
    return;
  }

  get_directory_from_path(recent_opendir, path, MAX_PATH_LENGTH);

  inputfile = fopeno(path, "rb");
  if(inputfile==NULL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Can not open file for reading.");
    messagewindow.exec();
    ascii2edfDialog->setEnabled(TRUE);
    return;
  }

/********************** check file *************************/

  rewind(inputfile);

  temp = 0;

  for(i=0; i<(startline-1);)
  {
    temp = fgetc(inputfile);

    if(temp==EOF)
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "File does not contain enough lines.");
      messagewindow.exec();
      fclose(inputfile);
      ascii2edfDialog->setEnabled(TRUE);
      return;
    }

    if(temp=='\n')
    {
      i++;
    }
  }

  headersize = ftello(inputfile);

  columns = NumcolumnsSpinbox->value();

  column_end = 1;

  column = 0;

  for(i=0; i<(ASCII_MAX_LINE_LEN - 2); i++)
  {
    temp = fgetc(inputfile);

    if(temp==EOF)
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "File does not contain enough lines.");
      messagewindow.exec();
      fclose(inputfile);
      ascii2edfDialog->setEnabled(TRUE);
      return;
    }

    if(temp=='\r')
    {
      continue;
    }

    if(temp==separator)
    {
      if(!column_end)
      {
        column++;

        column_end = 1;
      }
    }
    else
    {
      if(temp=='\n')
      {
        if(!column_end)
        {
          column++;
        }

        if(column!=columns)
        {
          QMessageBox messagewindow(QMessageBox::Critical, "Error", "Number of columns does not match.");
          messagewindow.exec();
          fclose(inputfile);
          ascii2edfDialog->setEnabled(TRUE);
          return;
        }

        break;
      }

      column_end = 0;
    }
  }

  if(i>ASCII_MAX_LINE_LEN)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Too many characters in a line.");
    messagewindow.exec();
    fclose(inputfile);
    ascii2edfDialog->setEnabled(TRUE);
    return;
  }

/***************** find highest physical maximums ***********************/

  if(autoPhysicalMaximum)
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    for(j=0; j<10; j++)  qApp->processEvents();

    for(i=0; i<ASCII_MAX_EDF_SIGNALS; i++)
    {
      physmax[i] = 0.00001;
    }

    fseeko(inputfile, (long long)headersize, SEEK_SET);

    i = 0;

    column = 0;

    column_end = 1;

    str_start = 0;

    edf_signal = 0;

    line_nr = startline;

    while(1)
    {
      temp = fgetc(inputfile);

      if(temp==EOF)
      {
        break;
      }

      line[i] = temp;

      if(line[i]=='\r')
      {
        continue;
      }

      if(separator!=',')
      {
        if(line[i]==',')
        {
          line[i] = '.';
        }
      }

      if(line[i]==separator)
      {
        if(!column_end)
        {
          if(column_enabled[column])
          {
            value[edf_signal] = atof(line + str_start);

            edf_signal++;
          }

          column_end = 1;

          column++;
        }
      }
      else
      {
        if(line[i]!='\n')
        {
          if(column_end)
          {
            str_start = i;

            column_end = 0;
          }
        }
      }

      if(line[i]=='\n')
      {
        if(!column_end)
        {
          if(column_enabled[column])
          {
            value[edf_signal] = atof(line + str_start);

            edf_signal++;
          }

          column++;

          column_end = 1;
        }

        if(column!=columns)
        {
          for(j=0; j<ASCII_MAX_LINE_LEN; j++)
          {
            if(fgetc(inputfile) == EOF)
            {
              break;  /* ignore error because we reached the end of the file */
            }         /* added this code because some ascii-files stop abruptly in */
          }           /* the middle of a row but they do put a newline-character at the end */

          if(j < ASCII_MAX_LINE_LEN)
          {
            break;
          }

          QApplication::restoreOverrideCursor();
          snprintf(txt_string, ASCII_MAX_LINE_LEN, "Error, number of columns in line %i is wrong.\n", line_nr);
          QMessageBox messagewindow(QMessageBox::Critical, "Error", txt_string);
          messagewindow.exec();
          fclose(inputfile);
          ascii2edfDialog->setEnabled(TRUE);
          return;
        }

        line_nr++;

        for(j=0; j<edfsignals; j++)
        {
          if(value[j] < 0.0)
          {
            value[j] *= -1.0;
          }

          if(physmax[j] < value[j])
          {
            physmax[j] = value[j];
          }
        }

        str_start = 0;

        i = 0;

        column = 0;

        column_end = 1;

        edf_signal = 0;

        qApp->processEvents();

        continue;
      }

      i++;

      if(i>(ASCII_MAX_LINE_LEN - 2))
      {
        QApplication::restoreOverrideCursor();
        snprintf(txt_string, ASCII_MAX_LINE_LEN, "Error, line %i is too long.\n", line_nr);
        QMessageBox messagewindow(QMessageBox::Critical, "Error", txt_string);
        messagewindow.exec();
        fclose(inputfile);
        ascii2edfDialog->setEnabled(TRUE);
        return;
      }
    }

    columns = NumcolumnsSpinbox->value();

    edf_signal = 0;

    for(i=0; i<columns; i++)
    {
      if(column_enabled[i])
      {
        physmax[edf_signal] *= ((QDoubleSpinBox *)SignalsTablewidget->cellWidget(i, 4))->value();

        if(physmax[edf_signal] > 9999999.0)
        {
          physmax[edf_signal] = 9999999.0;
        }

        if(edf_format)
        {
          sensitivity[edf_signal] = 32767.0 / physmax[edf_signal];
        }
        else
        {
          sensitivity[edf_signal] = 8388607.0 / physmax[edf_signal];
        }

        sensitivity[edf_signal++] *= ((QDoubleSpinBox *)SignalsTablewidget->cellWidget(i, 4))->value();
      }
    }

    QApplication::restoreOverrideCursor();
  }

/***************** write header *****************************************/

  outputfilename[0] = 0;
  if(recent_savedir[0]!=0)
  {
    strcpy(outputfilename, recent_savedir);
    strcat(outputfilename, "/");
  }
  len = strlen(outputfilename);
  get_filename_from_path(outputfilename + len, path, MAX_PATH_LENGTH - len);
  remove_extension_from_filename(outputfilename);
  if(edf_format)
  {
    strcat(outputfilename, ".edf");

    strcpy(outputfilename, QFileDialog::getSaveFileName(0, "Output file", QString::fromLocal8Bit(outputfilename), "EDF files (*.edf *.EDF)").toLocal8Bit().data());
  }
  else
  {
    strcat(outputfilename, ".bdf");

    strcpy(outputfilename, QFileDialog::getSaveFileName(0, "Output file", QString::fromLocal8Bit(outputfilename), "BDF files (*.bdf *.BDF)").toLocal8Bit().data());
  }

  if(!strcmp(outputfilename, ""))
  {
    fclose(inputfile);
    ascii2edfDialog->setEnabled(TRUE);
    return;
  }

  get_directory_from_path(recent_savedir, outputfilename, MAX_PATH_LENGTH);

  outputfile = fopeno(outputfilename, "wb");
  if(outputfile==NULL)
  {
    snprintf(txt_string, ASCII_MAX_LINE_LEN, "Can not open file %s for writing.", outputfilename);
    QMessageBox messagewindow(QMessageBox::Critical, "Error", txt_string);
    messagewindow.exec();
    fclose(inputfile);
    ascii2edfDialog->setEnabled(TRUE);
    return;
  }

  if(edf_format)
  {
    fprintf(outputfile, "0       ");
  }
  else
  {
    fputc(255, outputfile);
    fprintf(outputfile, "BIOSEMI");
  }

  p = snprintf(scratchpad, 128, "%s", PatientnameLineEdit->text().toLatin1().data());
  for( ; p<80; p++)
  {
    scratchpad[p] = ' ';
  }
  latin1_to_ascii(scratchpad, 80);
  scratchpad[80] = 0;
  fprintf(outputfile, "%s", scratchpad);

  p = snprintf(scratchpad, 128, "%s", RecordingLineEdit->text().toLatin1().data());
  for( ; p<80; p++)
  {
    scratchpad[p] = ' ';
  }
  latin1_to_ascii(scratchpad, 80);
  scratchpad[80] = 0;
  fprintf(outputfile, "%s", scratchpad);

  fprintf(outputfile,
          "%02i.%02i.%02i%02i.%02i.%02i",
          StartDatetimeedit->date().day(),
          StartDatetimeedit->date().month(),
          StartDatetimeedit->date().year() % 100,
          StartDatetimeedit->time().hour(),
          StartDatetimeedit->time().minute(),
          StartDatetimeedit->time().second());
  fprintf(outputfile, "%-8i", 256 * edfsignals + 256);
  fprintf(outputfile, "                                            ");
  fprintf(outputfile, "-1      ");
  if(samplefrequency<1.0)
  {
    datrecduration = 1.0 / samplefrequency;
    snprintf(str, 256, "%.8f", datrecduration);
    if(fwrite(str, 8, 1, outputfile)!=1)
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "A write error occurred.");
      messagewindow.exec();
      fclose(inputfile);
      fclose(outputfile);
      ascii2edfDialog->setEnabled(TRUE);
      return;
    }
  }
  else
  {
    if(((int)samplefrequency)%10)
    {
      datrecduration = 1.0;
      fprintf(outputfile, "1       ");
    }
    else
    {
      datrecduration = 0.1;
      fprintf(outputfile, "0.1     ");
    }
  }
  fprintf(outputfile, "%-4i", edfsignals);

  for(i=0; i<columns; i++)
  {
    if(column_enabled[i])
    {
      p = fprintf(outputfile, "%s", ((QLineEdit *)SignalsTablewidget->cellWidget(i, 1))->text().toLatin1().data());
      for(j=p; j<16; j++)
      {
        fputc(' ', outputfile);
      }
    }
  }

  for(i=0; i<(80*edfsignals); i++)
  {
    fputc(' ', outputfile);
  }

  for(i=0; i<columns; i++)
  {
    if(column_enabled[i])
    {
      p = fprintf(outputfile, "%s", ((QLineEdit *)SignalsTablewidget->cellWidget(i, 3))->text().toLatin1().data());
      for(j=p; j<8; j++)
      {
        fputc(' ', outputfile);
      }
    }
  }

  edf_signal = 0;

  for(i=0; i<columns; i++)
  {
    if(column_enabled[i])
    {
      if(autoPhysicalMaximum)
      {
        sprintf(str, "%.8f", physmax[edf_signal++] * -1.0);
        strcat(str, "        ");
        str[8] = 0;
        fprintf(outputfile, "%s", str);
      }
      else
      {
        fputc('-', outputfile);
        p = fprintf(outputfile, "%s", ((QLineEdit *)SignalsTablewidget->cellWidget(i, 2))->text().toLatin1().data());
        for(j=p; j<7; j++)
        {
          fputc(' ', outputfile);
        }
      }
    }
  }

  edf_signal = 0;

  for(i=0; i<columns; i++)
  {
    if(column_enabled[i])
    {
      if(autoPhysicalMaximum)
      {
        sprintf(str, "%.8f", physmax[edf_signal++]);
        strcat(str, "        ");
        str[8] = 0;
        fprintf(outputfile, "%s", str);
      }
      else
      {
        p = fprintf(outputfile, "%s", ((QLineEdit *)SignalsTablewidget->cellWidget(i, 2))->text().toLatin1().data());
        for(j=p; j<8; j++)
        {
          fputc(' ', outputfile);
        }

        if(edf_format)
        {
          sensitivity[edf_signal] = 32767.0 / atof(((QLineEdit *)SignalsTablewidget->cellWidget(i, 2))->text().toLatin1().data());
        }
        else
        {
          sensitivity[edf_signal] = 8388607.0 / atof(((QLineEdit *)SignalsTablewidget->cellWidget(i, 2))->text().toLatin1().data());
        }

        sensitivity[edf_signal++] *= ((QDoubleSpinBox *)SignalsTablewidget->cellWidget(i, 4))->value();
      }
    }
  }

  for(i=0; i<edfsignals; i++)
  {
    if(edf_format)
    {
      fprintf(outputfile, "-32768  ");
    }
    else
    {
      fprintf(outputfile, "-8388608");
    }
  }

  for(i=0; i<edfsignals; i++)
  {
    if(edf_format)
    {
      fprintf(outputfile, "32767   ");
    }
    else
    {
      fprintf(outputfile, "8388607 ");
    }
  }

  for(i=0; i<(80*edfsignals); i++)
  {
    fputc(' ', outputfile);
  }

  if(samplefrequency<1.0)
  {
    for(i=0; i<edfsignals; i++)
    {
      fprintf(outputfile, "1       ");
      smpls_per_block = 1;
    }
  }
  else
  {
    if(((int)samplefrequency)%10)
    {
      for(i=0; i<edfsignals; i++)
      {
        fprintf(outputfile, "%-8i", (int)samplefrequency);
        smpls_per_block = (int)samplefrequency;
      }
    }
    else
    {
      for(i=0; i<edfsignals; i++)
      {
        fprintf(outputfile, "%-8i", ((int)samplefrequency) / 10);
        smpls_per_block = ((int)samplefrequency) / 10;
      }
    }
  }

  for(i=0; i<(32*edfsignals); i++)
  {
    fputc(' ', outputfile);
  }

/***************** start conversion **************************************/

  QApplication::setOverrideCursor(Qt::WaitCursor);

  for(j=0; j<10; j++)  qApp->processEvents();

  if(edf_format)
  {
    bufsize = smpls_per_block * 2 * edfsignals;
  }
  else
  {
    bufsize = smpls_per_block * 3 * edfsignals;
  }

  buf = (char *)calloc(1, bufsize);
  if(buf==NULL)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Malloc error (buf)");
    messagewindow.exec();
    fclose(inputfile);
    fclose(outputfile);
    ascii2edfDialog->setEnabled(TRUE);
    return;
  }

  fseeko(inputfile, (long long)headersize, SEEK_SET);

  i = 0;

  k = 0;

  column = 0;

  column_end = 1;

  datarecords = 0;

  str_start = 0;

  edf_signal = 0;

  line_nr = startline;

  while(1)
  {
    temp = fgetc(inputfile);

    if(temp==EOF)
    {
      break;
    }

    line[i] = temp;

    if(line[i]=='\r')
    {
      continue;
    }

    if(separator!=',')
    {
      if(line[i]==',')
      {
        line[i] = '.';
      }
    }

    if(line[i]==separator)
    {
      if(!column_end)
      {
        if(column_enabled[column])
        {
          value[edf_signal] = atof(line + str_start);

          edf_signal++;
        }

        column_end = 1;

        column++;
      }
    }
    else
    {
      if(line[i]!='\n')
      {
        if(column_end)
        {
          str_start = i;

          column_end = 0;
        }
      }
    }

    if(line[i]=='\n')
    {
      if(!column_end)
      {
        if(column_enabled[column])
        {
          value[edf_signal] = atof(line + str_start);

          edf_signal++;
        }

        column++;

        column_end = 1;
      }

      if(column!=columns)
      {
        for(j=0; j<ASCII_MAX_LINE_LEN; j++)
        {
          if(fgetc(inputfile) == EOF)
          {
            break;  /* ignore error because we reached the end of the file */
          }         /* added this code because some ascii-files stop abruptly in */
        }           /* the middle of a row but they do put a newline-character at the end */

        if(j < ASCII_MAX_LINE_LEN)
        {
          break;
        }

        QApplication::restoreOverrideCursor();
        snprintf(txt_string, ASCII_MAX_LINE_LEN, "Error, number of columns in line %i is wrong.\n", line_nr);
        QMessageBox messagewindow(QMessageBox::Critical, "Error", txt_string);
        messagewindow.exec();
        fclose(inputfile);
        fclose(outputfile);
        free(buf);
        ascii2edfDialog->setEnabled(TRUE);
        return;
      }

      line_nr++;

      for(j=0; j<edfsignals; j++)
      {
        temp = (int)(value[j] * sensitivity[j]);

        if(edf_format)
        {
          if(temp>32767)  temp = 32767;

          if(temp<-32768)  temp = -32768;

          *(((short *)buf) + k + (j * smpls_per_block)) = (short)temp;
        }
        else
        {
          if(temp>8388607)  temp = 8388607;

          if(temp<-8388608)  temp = -8388608;

          p = (k + (j * smpls_per_block)) * 3;

          buf[p++] = temp & 0xff;
          buf[p++] = (temp>>8) & 0xff;
          buf[p] = (temp>>16) & 0xff;
        }
      }

      k++;

      if(k>=smpls_per_block)
      {
        if(fwrite(buf, bufsize, 1, outputfile)!=1)
        {
          QApplication::restoreOverrideCursor();
          QMessageBox messagewindow(QMessageBox::Critical, "Error", "Write error during conversion.");
          messagewindow.exec();
          fclose(inputfile);
          fclose(outputfile);
          free(buf);
          ascii2edfDialog->setEnabled(TRUE);
          return;
        }

        datarecords++;

        k = 0;
      }

      str_start = 0;

      i = 0;

      column = 0;

      column_end = 1;

      edf_signal = 0;

      qApp->processEvents();

      continue;
    }

    i++;

    if(i>(ASCII_MAX_LINE_LEN - 2))
    {
      QApplication::restoreOverrideCursor();
      snprintf(txt_string, ASCII_MAX_LINE_LEN, "Error, line %i is too long.\n", line_nr);
      QMessageBox messagewindow(QMessageBox::Critical, "Error", txt_string);
      messagewindow.exec();
      fclose(inputfile);
      fclose(outputfile);
      free(buf);
      ascii2edfDialog->setEnabled(TRUE);
      return;
    }
  }

  QApplication::restoreOverrideCursor();

  fseeko(outputfile, 236LL, SEEK_SET);

  fprintf(outputfile, "%-8i", datarecords);

  free(buf);

  if(fclose(outputfile))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "An error occurred when closing outputfile.");
    messagewindow.exec();
    fclose(inputfile);
    ascii2edfDialog->setEnabled(TRUE);
    return;
  }

  if(fclose(inputfile))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "An error occurred when closing inputfile.");
    messagewindow.exec();
    ascii2edfDialog->setEnabled(TRUE);
    return;
  }

  snprintf(txt_string, ASCII_MAX_LINE_LEN, "Done. EDF file is located at %s\n", outputfilename);
  QMessageBox messagewindow(QMessageBox::Information, "Ready", txt_string);
  messagewindow.setIconPixmap(QPixmap(":/images/ok.png"));
  messagewindow.exec();

  ascii2edfDialog->setEnabled(TRUE);
}



void UI_ASCII2EDFapp::savebuttonpressed()
{
  int i, j, len;

  char path[MAX_PATH_LENGTH],
       str[128];

  FILE *outputfile;


  ascii2edfDialog->setEnabled(FALSE);

  if(check_input())
  {
    ascii2edfDialog->setEnabled(TRUE);
    return;
  }

  path[0] = 0;
  if(recent_savedir[0]!=0)
  {
    strcpy(path, recent_savedir);
    strcat(path, "/");
  }
  strcat(path, "ascii_to_edf.template");

  strcpy(path, QFileDialog::getSaveFileName(0, "Save parameters", QString::fromLocal8Bit(path), "Template files (*.template *.TEMPLATE)").toLocal8Bit().data());

  if(!strcmp(path, ""))
  {
    ascii2edfDialog->setEnabled(TRUE);
    return;
  }

  get_directory_from_path(recent_savedir, path, MAX_PATH_LENGTH);

  outputfile = fopeno(path, "wb");
  if(outputfile==NULL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Can not open file for writing.");
    messagewindow.exec();
    ascii2edfDialog->setEnabled(TRUE);
    return;
  }

  fprintf(outputfile, "<?xml version=\"1.0\"?>\n<" PROGRAM_NAME "_ascii2edf_template>\n");

  fprintf(outputfile, "  <separator>%s</separator>\n", SeparatorLineEdit->text().toLatin1().data());

  fprintf(outputfile, "  <columns>%i</columns>\n", columns);

  fprintf(outputfile, "  <startline>%i</startline>\n", startline);

  fprintf(outputfile, "  <samplefrequency>%.10f</samplefrequency>\n", samplefrequency);

  fprintf(outputfile, "  <autophysicalmaximum>%i</autophysicalmaximum>\n", autoPhysicalMaximum);

  if(edfButton->isChecked() == TRUE)
  {
    fprintf(outputfile, "  <edf_format>1</edf_format>\n");
  }
  else
  {
    fprintf(outputfile, "  <edf_format>0</edf_format>\n");
  }

  for(i=0; i<columns; i++)
  {
    fprintf(outputfile, "  <signalparams>\n");

    if(((QCheckBox *)SignalsTablewidget->cellWidget(i, 0))->checkState()==Qt::Checked)
    {
      fprintf(outputfile, "    <checked>1</checked>\n");
    }
    else
    {
      fprintf(outputfile, "    <checked>0</checked>\n");
    }

    strcpy(str, ((QLineEdit *)SignalsTablewidget->cellWidget(i, 1))->text().toLatin1().data());

    len = strlen(str);
    for(j=len-1; j>0; j--)
    {
      if(str[j]!=' ')  break;
    }
    str[j+1] = 0;

    fprintf(outputfile, "    <label>%s</label>\n", str);

    strcpy(str, ((QLineEdit *)SignalsTablewidget->cellWidget(i, 2))->text().toLatin1().data());

    len = strlen(str);
    for(j=len-1; j>0; j--)
    {
      if(str[j]!=' ')  break;
    }
    str[j+1] = 0;

    fprintf(outputfile, "    <physical_maximum>%s</physical_maximum>\n", str);

    strcpy(str, ((QLineEdit *)SignalsTablewidget->cellWidget(i, 3))->text().toLatin1().data());

    len = strlen(str);
    for(j=len-1; j>0; j--)
    {
      if(str[j]!=' ')  break;
    }
    str[j+1] = 0;

    fprintf(outputfile, "    <physical_dimension>%s</physical_dimension>\n", str);

    fprintf(outputfile, "    <multiplier>%f</multiplier>\n", ((QDoubleSpinBox *)SignalsTablewidget->cellWidget(i, 4))->value());

    fprintf(outputfile, "  </signalparams>\n");
  }

  fprintf(outputfile, "</" PROGRAM_NAME "_ascii2edf_template>\n");

  fclose(outputfile);

  ascii2edfDialog->setEnabled(TRUE);
}


void UI_ASCII2EDFapp::loadbuttonpressed()
{
  int i,
      temp;

  char path[MAX_PATH_LENGTH],
       *content;

  double f_temp;

  struct xml_handle *xml_hdl;

  strcpy(path, QFileDialog::getOpenFileName(0, "Load parameters", QString::fromLocal8Bit(recent_opendir), "Template files (*.template *.TEMPLATE)").toLocal8Bit().data());

  if(!strcmp(path, ""))
  {
    return;
  }

  get_directory_from_path(recent_opendir, path, MAX_PATH_LENGTH);

  xml_hdl = xml_get_handle(path);
  if(xml_hdl==NULL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Can not open file for reading.");
    messagewindow.exec();
    return;
  }

  if(strcmp(xml_hdl->elementname, PROGRAM_NAME "_ascii2edf_template"))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
    messagewindow.exec();
    xml_close(xml_hdl);
    return;
  }

  if(xml_goto_nth_element_inside(xml_hdl, "separator", 0))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
    messagewindow.exec();
    xml_close(xml_hdl);
    return;
  }
  content = xml_get_content_of_element(xml_hdl);
  if(!strcmp(content, "tab"))
  {
    SeparatorLineEdit->setText("tab");
    free(content);
  }
  else
  {
    if(strlen(content)!=1)
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
      messagewindow.exec();
      free(content);
      xml_close(xml_hdl);
      return;
    }
    else
    {
      if((content[0]<32)||(content[0]>126))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
        messagewindow.exec();
        free(content);
        xml_close(xml_hdl);
        return;
      }
      SeparatorLineEdit->setText(content);
      free(content);
    }
  }

  xml_go_up(xml_hdl);

  if(xml_goto_nth_element_inside(xml_hdl, "columns", 0))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
    messagewindow.exec();
    xml_close(xml_hdl);
    return;
  }
  content = xml_get_content_of_element(xml_hdl);
  temp = atoi(content);
  free(content);
  if((temp<1)||(temp>256))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
    messagewindow.exec();
    xml_close(xml_hdl);
    return;
  }
  NumcolumnsSpinbox->setValue(temp);
  columns = temp;
  SignalsTablewidget->setRowCount(columns);

  xml_go_up(xml_hdl);

  if(xml_goto_nth_element_inside(xml_hdl, "startline", 0))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
    messagewindow.exec();
    xml_close(xml_hdl);
    return;
  }
  content = xml_get_content_of_element(xml_hdl);
  temp = atoi(content);
  free(content);
  if((temp<1)||(temp>100))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
    messagewindow.exec();
    xml_close(xml_hdl);
    return;
  }
  DatastartSpinbox->setValue(temp);

  xml_go_up(xml_hdl);

  if(xml_goto_nth_element_inside(xml_hdl, "samplefrequency", 0))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
    messagewindow.exec();
    xml_close(xml_hdl);
    return;
  }
  content = xml_get_content_of_element(xml_hdl);
  f_temp = atof(content);
  free(content);
  if((f_temp<0.0000001)||(f_temp>1000000.0))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
    messagewindow.exec();
    xml_close(xml_hdl);
    return;
  }
  SamplefreqSpinbox->setValue(f_temp);

  xml_go_up(xml_hdl);

  if(!(xml_goto_nth_element_inside(xml_hdl, "autophysicalmaximum", 0)))
  {
    content = xml_get_content_of_element(xml_hdl);
    autoPhysicalMaximum = atoi(content);
    free(content);
    if((autoPhysicalMaximum<0)||(autoPhysicalMaximum>1))
    {
      autoPhysicalMaximum = 1;
    }
    if(autoPhysicalMaximum)
    {
      autoPhysicalMaximumCheckbox->setCheckState(Qt::Checked);
    }
    else
    {
      autoPhysicalMaximumCheckbox->setCheckState(Qt::Unchecked);
    }
    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "edf_format", 0)))
  {
    content = xml_get_content_of_element(xml_hdl);
    edf_format = atoi(content);
    free(content);
    if((edf_format<0)||(edf_format>1))
    {
      edf_format = 0;
    }
    if(edf_format)
    {
      edfButton->setChecked(TRUE);
    }
    else
    {
      bdfButton->setChecked(TRUE);
    }
    xml_go_up(xml_hdl);
  }

  for(i=0; i<columns; i++)
  {
    if(xml_goto_nth_element_inside(xml_hdl, "signalparams", i))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
      messagewindow.exec();
      xml_close(xml_hdl);
      return;
    }

    if(xml_goto_nth_element_inside(xml_hdl, "checked", 0))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
      messagewindow.exec();
      xml_close(xml_hdl);
      return;
    }
    content = xml_get_content_of_element(xml_hdl);
    if(!strcmp(content, "0"))
    {
      ((QCheckBox *)SignalsTablewidget->cellWidget(i, 0))->setCheckState(Qt::Unchecked);
    }
    free(content);

    xml_go_up(xml_hdl);

    if(xml_goto_nth_element_inside(xml_hdl, "label", 0))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
      messagewindow.exec();
      xml_close(xml_hdl);
      return;
    }
    content = xml_get_content_of_element(xml_hdl);
    ((QLineEdit *)SignalsTablewidget->cellWidget(i, 1))->setText(content);
    free(content);

    xml_go_up(xml_hdl);

    if(xml_goto_nth_element_inside(xml_hdl, "physical_maximum", 0))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
      messagewindow.exec();
      xml_close(xml_hdl);
      return;
    }
    content = xml_get_content_of_element(xml_hdl);
    ((QLineEdit *)SignalsTablewidget->cellWidget(i, 2))->setText(content);
    free(content);

    xml_go_up(xml_hdl);

    if(xml_goto_nth_element_inside(xml_hdl, "physical_dimension", 0))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
      messagewindow.exec();
      xml_close(xml_hdl);
      return;
    }
    content = xml_get_content_of_element(xml_hdl);
    ((QLineEdit *)SignalsTablewidget->cellWidget(i, 3))->setText(content);
    free(content);

    xml_go_up(xml_hdl);

    if(xml_goto_nth_element_inside(xml_hdl, "multiplier", 0))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
      messagewindow.exec();
      xml_close(xml_hdl);
      return;
    }
    content = xml_get_content_of_element(xml_hdl);
    ((QDoubleSpinBox *)SignalsTablewidget->cellWidget(i, 4))->setValue(atof(content));
    free(content);

    xml_go_up(xml_hdl);
    xml_go_up(xml_hdl);
  }

  xml_close(xml_hdl);
}


int UI_ASCII2EDFapp::check_input(void)
{
  int i, j, k,
      len,
      dot;

  char str[128],
       big_str[ASCII_MAX_LINE_LEN];

  const char *columnname[]={"", "Label", "Physical maximum", "Physical dimension"};


  strcpy(str, SeparatorLineEdit->text().toLatin1().data());

  if(!strcmp(str, "tab"))
  {
    separator = '\t';
  }
  else
  {
    if(strlen(str)!=1)
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", "Separator must be one character or tab.");
      messagewindow.exec();

      return(1);
    }

    if((str[0]<32)||(str[0]>126))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", "Separator character is not a valid ASCII character.");
      messagewindow.exec();

      return(1);
    }

    if(str[0]=='.')
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", "Separator character can not be a dot.");
      messagewindow.exec();

      return(1);
    }

    if((str[0]>47)&&(str[0]<58))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", "Separator character can not be a number.");
      messagewindow.exec();

      return(1);
    }

    separator = str[0];
  }

  startline = DatastartSpinbox->value();

  samplefrequency = SamplefreqSpinbox->value();

  if(samplefrequency>=1.0)
  {
    if(samplefrequency>((double)((int)samplefrequency)))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", "Only samplefrequencies below 1 Hz can have a decimal fraction.");
      messagewindow.exec();

      return(1);
    }
  }

  edfsignals = 0;

  for(i=0; i<columns; i++)
  {
    if(((QCheckBox *)SignalsTablewidget->cellWidget(i, 0))->checkState()==Qt::Checked)
    {
      for(j=1; j<4; j++)
      {
        if((j == 2) && autoPhysicalMaximum)
        {
          continue;
        }

        dot = 0;

        strcpy(str, ((QLineEdit *)SignalsTablewidget->cellWidget(i, j))->text().toLatin1().data());

        len = strlen(str);

        if(!len)
        {
          snprintf(big_str, ASCII_MAX_LINE_LEN, "%s field of row %i is empty.", columnname[j], i + 1);

          QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", big_str);
          messagewindow.exec();

          return(1);
        }

        for(k=0; k<len; k++)
        {
          if((str[k]<32)||(str[k]>126))
          {
            snprintf(big_str, ASCII_MAX_LINE_LEN,
            "Character %i in %s field of row %i is not a valid ASCII character.",
            k + 1, columnname[j], i + 1);

            QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", big_str);
            messagewindow.exec();

            return(1);
          }

          if(str[k]==' ')
          {
            if(k==0)
            {
              snprintf(big_str, ASCII_MAX_LINE_LEN,
              "Text in %s field of row %i is not valid.\nField must not be empty and left-aligned (no spaces in front of the text).",
              columnname[j], i + 1);

              QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", big_str);
              messagewindow.exec();

              return(1);
            }
          }

          if(j==2)
          {
            if(str[len-1]=='.')
            {
              snprintf(big_str, ASCII_MAX_LINE_LEN,
              "Text in %s field of row %i is not valid.\n"
              "Last character can not be a dot.",
              columnname[j], i + 1);

              QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", big_str);
              messagewindow.exec();

              return(1);
            }

            if((str[k]<48)||(str[k]>57))
            {
              if(str[k]=='.')
              {
                if(dot)
                {
                  snprintf(big_str, ASCII_MAX_LINE_LEN,
                  "Text in %s field of row %i is not valid.\n"
                  "Only one dot is allowed as a decimal separator.",
                  columnname[j], i + 1);

                  QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", big_str);
                  messagewindow.exec();

                  return(1);
                }
                else
                {
                  if(k==0)
                  {
                    snprintf(big_str, ASCII_MAX_LINE_LEN,
                    "Text in %s field of row %i is not valid.\n"
                    "First character can not be a dot.",
                    columnname[j], i + 1);

                    QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", big_str);
                    messagewindow.exec();

                    return(1);
                  }

                  dot = 1;
                }
              }
              else
              {
                snprintf(big_str, ASCII_MAX_LINE_LEN,
                "Text in %s field of row %i is not valid.\n"
                "Field must contain a number and no spaces.",
                columnname[j], i + 1);

                QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", big_str);
                messagewindow.exec();

                return(1);
              }
            }
          }
        }

        if(j==2)
        {
          if(atof(str)<1.0)
          {
            snprintf(big_str, ASCII_MAX_LINE_LEN,
            "Value in %s field of row %i is not valid.\n"
            "Value must be 1 or more.",
            columnname[j], i + 1);

            QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", big_str);
            messagewindow.exec();

            return(1);
          }
        }
      }

      edfsignals++;

      column_enabled[i] = 1;
    }
    else
    {
      column_enabled[i] = 0;
    }
  }

  if(edfsignals<1)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", "At least one row needs to be checked.");
    messagewindow.exec();

    return(1);
  }

  return(0);
}











