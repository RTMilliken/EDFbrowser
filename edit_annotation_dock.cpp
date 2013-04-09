/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2009, 2010, 2011, 2012, 2013 Teunis van Beelen
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



#include "annotations_dock.h"



UI_AnnotationEditwindow::UI_AnnotationEditwindow(int file_number, QWidget *w_parent)
{
  mainwindow = (UI_Mainwindow *)w_parent;

  file_num = file_number;

  annotation = mainwindow->annotationlist[file_num];

  dockedit = new QDockWidget("Annotation editor", w_parent);
  dockedit->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
  dockedit->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);

  annot_edit_dialog = new QDialog;
  annot_edit_dialog->setMinimumSize(640, 45);

  dockedit->setWidget(annot_edit_dialog);

  descriptionLabel = new QLabel(annot_edit_dialog);
  descriptionLabel->setGeometry(10, 10, 78, 25);
  descriptionLabel->setText("Description");

  annot_descript_lineEdit = new QLineEdit(annot_edit_dialog);
  annot_descript_lineEdit->setGeometry(90, 10, 150, 25);

  onsetLabel = new QLabel(annot_edit_dialog);
  onsetLabel->setGeometry(250, 10, 48, 25);
  onsetLabel->setText("Onset");

  posNegTimebox = new QComboBox(annot_edit_dialog);
  posNegTimebox->setGeometry(300, 10, 35, 25);
  posNegTimebox->setEditable(FALSE);
  posNegTimebox->addItem("+");
  posNegTimebox->addItem("-");

  onset_daySpinbox = new QSpinBox(annot_edit_dialog);
  onset_daySpinbox->setGeometry(335, 10, 45, 25);
  onset_daySpinbox->setRange(0, 99);
  onset_daySpinbox->setSingleStep(1);
  onset_daySpinbox->setValue(0);

  onset_timeEdit = new QTimeEdit(annot_edit_dialog);
  onset_timeEdit->setGeometry(380, 10, 100, 25);
  onset_timeEdit->setDisplayFormat("hh:mm:ss.zzz");
  onset_timeEdit->setMinimumTime(QTime(-1, 0, 0, 0));

  durationLabel = new QLabel(annot_edit_dialog);
  durationLabel->setGeometry(490, 10, 58, 25);
  durationLabel->setText("Duration");

  duration_spinbox = new QDoubleSpinBox(annot_edit_dialog);
  duration_spinbox->setGeometry(550, 10, 120, 25);
  duration_spinbox->setRange(-1.0, 10000.0);
  duration_spinbox->setSingleStep(1.0);
  duration_spinbox->setDecimals(3);
  duration_spinbox->setSuffix(" sec");
  duration_spinbox->setValue(-1.0);

  modifybutton = new QPushButton(annot_edit_dialog);
  modifybutton->setGeometry(720, 10, 100, 25);
  modifybutton->setText("Modify");
  modifybutton->setEnabled(FALSE);

  deletebutton = new QPushButton(annot_edit_dialog);
  deletebutton->setGeometry(840, 10, 100, 25);
  deletebutton->setText("Delete");
  deletebutton->setEnabled(FALSE);

  createbutton = new QPushButton(annot_edit_dialog);
  createbutton->setGeometry(960, 10, 100, 25);
  createbutton->setText("Create");

  QObject::connect(modifybutton, SIGNAL(clicked()),               this, SLOT(modifyButtonClicked()));
  QObject::connect(deletebutton, SIGNAL(clicked()),               this, SLOT(deleteButtonClicked()));
  QObject::connect(createbutton, SIGNAL(clicked()),               this, SLOT(createButtonClicked()));
  QObject::connect(dockedit,     SIGNAL(visibilityChanged(bool)), this, SLOT(open_close_dock(bool)));
}



void UI_AnnotationEditwindow::open_close_dock(bool visible)
{
  if(visible==TRUE)
  {
    mainwindow->annot_editor_active = 1;

    mainwindow->show_annot_markers = 1;

    if(mainwindow->annotationlist_backup==NULL)
    {
      mainwindow->annotationlist_backup = edfplus_annotation_copy_list(&mainwindow->annotationlist[0]);
    }
  }
  else
  {
    modifybutton->setEnabled(FALSE);

    deletebutton->setEnabled(FALSE);

    mainwindow->annot_editor_active = 0;
  }
}



void UI_AnnotationEditwindow::modifyButtonClicked()
{
  annotation = edfplus_annotation_item(&mainwindow->annotationlist[file_num], annot_num);

  annotation->onset =  onset_daySpinbox->value() * 86400 * TIME_DIMENSION;
  annotation->onset += onset_timeEdit->time().hour() * 3600 * TIME_DIMENSION;
  annotation->onset += onset_timeEdit->time().minute() * 60 * TIME_DIMENSION;
  annotation->onset += onset_timeEdit->time().second() * TIME_DIMENSION;
  annotation->onset += onset_timeEdit->time().msec() * (TIME_DIMENSION / 1000);

  if(posNegTimebox->currentIndex() == 1)
  {
    annotation->onset = -(annotation->onset);
  }

  annotation->onset += mainwindow->edfheaderlist[file_num]->starttime_offset;

  if(duration_spinbox->value()>0.0)
  {
    snprintf(annotation->duration, 16, "%f", duration_spinbox->value());
  }
  else
  {
    annotation->duration[0] = 0;
  }

  strncpy(annotation->annotation, annot_descript_lineEdit->text().toUtf8().data(), MAX_ANNOTATION_LEN);

  annotation->annotation[MAX_ANNOTATION_LEN] = 0;

  annotation->modified = 1;

  annotation->selected = 1;

  mainwindow->annotations_edited = 1;

  mainwindow->annotations_dock[file_num]->updateList();

  mainwindow->maincurve->update();
}



void UI_AnnotationEditwindow::deleteButtonClicked()
{
  edfplus_annotation_delete(&mainwindow->annotationlist[file_num], annot_num);

  annotation = edfplus_annotation_item(&mainwindow->annotationlist[file_num], annot_num);

  if(annotation!=NULL)
  {
    annotation->selected = 1;

    annotation->jump = 1;
  }

  modifybutton->setEnabled(FALSE);

  deletebutton->setEnabled(FALSE);

  mainwindow->annotations_edited = 1;

  mainwindow->save_act->setEnabled(TRUE);

  mainwindow->annotations_dock[file_num]->updateList();

  mainwindow->maincurve->update();
}



void UI_AnnotationEditwindow::createButtonClicked()
{
  annotation = (struct annotationblock *)calloc(1, sizeof(struct annotationblock));

  annotation->onset =  onset_daySpinbox->value() * 86400 * TIME_DIMENSION;
  annotation->onset += onset_timeEdit->time().hour() * 3600 * TIME_DIMENSION;
  annotation->onset += onset_timeEdit->time().minute() * 60 * TIME_DIMENSION;
  annotation->onset += onset_timeEdit->time().second() * TIME_DIMENSION;
  annotation->onset += onset_timeEdit->time().msec() * (TIME_DIMENSION / 1000);

  if(posNegTimebox->currentIndex() == 1)
  {
    annotation->onset = -(annotation->onset);
  }

  annotation->onset += mainwindow->edfheaderlist[file_num]->starttime_offset;

  annotation->file_num = file_num;

  if(duration_spinbox->value()>0.0)
  {
    snprintf(annotation->duration, 16, "%f", duration_spinbox->value());
  }
  else
  {
    annotation->duration[0] = 0;
  }

  strncpy(annotation->annotation, annot_descript_lineEdit->text().toUtf8().data(), MAX_ANNOTATION_LEN);

  annotation->annotation[MAX_ANNOTATION_LEN] = 0;

  annotation->modified = 1;

  edfplus_annotation_add_item(&mainwindow->annotationlist[file_num], annotation);

  mainwindow->annotations_edited = 1;

  mainwindow->annotations_dock[file_num]->updateList();

  mainwindow->maincurve->update();
}



void UI_AnnotationEditwindow::annotEditSetOnset(long long onset)
{
  QTime ta;

  if(onset < 0LL)
  {
    onset_daySpinbox->setValue((-(onset)) / TIME_DIMENSION / 86400);

    ta.setHMS(((-(onset)) / TIME_DIMENSION / 3600) % 24,
            (((-(onset)) / TIME_DIMENSION) % 3600) / 60,
            ((-(onset)) / TIME_DIMENSION) % 60,
            ((-(onset)) % TIME_DIMENSION) / 10000);

    posNegTimebox->setCurrentIndex(1);
  }
  else
  {
    onset_daySpinbox->setValue(onset / TIME_DIMENSION / 86400);

    ta.setHMS((onset / TIME_DIMENSION / 3600) % 24,
            ((onset / TIME_DIMENSION) % 3600) / 60,
            (onset / TIME_DIMENSION) % 60,
            (onset % TIME_DIMENSION) / 10000);

    posNegTimebox->setCurrentIndex(0);
  }

  onset_timeEdit->setTime(ta);
}



void UI_AnnotationEditwindow::annotEditSetDuration(long long duration)
{
  double tmp;

  tmp = duration;
  tmp /= TIME_DIMENSION;

  duration_spinbox->setValue(tmp);
}



void UI_AnnotationEditwindow::set_selected_annotation(int file_nr, int annot_nr)
{
  long long l_tmp;

  QTime ta;


  file_num = file_nr;

  annot_num = annot_nr;

  annotation = edfplus_annotation_item(&mainwindow->annotationlist[file_num], annot_num);

  annot_descript_lineEdit->setText(QString::fromUtf8(annotation->annotation));

  l_tmp = annotation->onset - mainwindow->edfheaderlist[annotation->file_num]->starttime_offset;

  if(l_tmp < 0LL)
  {
    onset_daySpinbox->setValue((-(l_tmp)) / TIME_DIMENSION / 86400);

    ta.setHMS(((-(l_tmp)) / TIME_DIMENSION / 3600) % 24,
            (((-(l_tmp)) / TIME_DIMENSION) % 3600) / 60,
            ((-(l_tmp)) / TIME_DIMENSION) % 60,
            ((-(l_tmp)) % TIME_DIMENSION) / 10000);

    posNegTimebox->setCurrentIndex(1);
  }
  else
  {
    onset_daySpinbox->setValue(l_tmp / TIME_DIMENSION / 86400);

    ta.setHMS((l_tmp / TIME_DIMENSION / 3600) % 24,
            ((l_tmp / TIME_DIMENSION) % 3600) / 60,
            (l_tmp / TIME_DIMENSION) % 60,
            (l_tmp % TIME_DIMENSION) / 10000);

    posNegTimebox->setCurrentIndex(0);
  }
  onset_timeEdit->setTime(ta);

  if(strlen(annotation->duration))
  {
    duration_spinbox->setValue(atof(annotation->duration));
  }
  else
  {
    duration_spinbox->setValue(-1);
  }

  modifybutton->setEnabled(TRUE);

  deletebutton->setEnabled(TRUE);
}




void UI_AnnotationEditwindow::set_selected_annotation(struct annotationblock *annot)
{
  long long l_tmp;

  QTime ta;

  annotation = annot;

  file_num = annot->file_num;

  annot_descript_lineEdit->setText(QString::fromUtf8(annotation->annotation));

  l_tmp = annotation->onset - mainwindow->edfheaderlist[annotation->file_num]->starttime_offset;

  if(l_tmp < 0LL)
  {
    onset_daySpinbox->setValue((-(l_tmp)) / TIME_DIMENSION / 86400);

    ta.setHMS(((-(l_tmp)) / TIME_DIMENSION / 3600) % 24,
            (((-(l_tmp)) / TIME_DIMENSION) % 3600) / 60,
            ((-(l_tmp)) / TIME_DIMENSION) % 60,
            ((-(l_tmp)) % TIME_DIMENSION) / 10000);

    posNegTimebox->setCurrentIndex(1);
  }
  else
  {
    onset_daySpinbox->setValue(l_tmp / TIME_DIMENSION / 86400);

    ta.setHMS((l_tmp / TIME_DIMENSION / 3600) % 24,
            ((l_tmp / TIME_DIMENSION) % 3600) / 60,
            (l_tmp / TIME_DIMENSION) % 60,
            (l_tmp % TIME_DIMENSION) / 10000);

    posNegTimebox->setCurrentIndex(0);
  }
  onset_timeEdit->setTime(ta);

  if(strlen(annotation->duration))
  {
    duration_spinbox->setValue(atof(annotation->duration));
  }
  else
  {
    duration_spinbox->setValue(-1);
  }

  modifybutton->setEnabled(FALSE);

  deletebutton->setEnabled(FALSE);
}














