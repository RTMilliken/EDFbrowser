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



#include "load_montage_dialog.h"



UI_LoadMontagewindow::UI_LoadMontagewindow(QWidget *w_parent, char *path)
{
  int i;

  mainwindow = (UI_Mainwindow *)w_parent;

  mtg_path[0] = 0;

  if(path!=NULL)
  {
    strcpy(mtg_path, path);
  }

  if(mainwindow->files_open==1)
  {
    LoadMontageDialog = NULL;
    LoadButtonClicked();
    return;
  }

  LoadMontageDialog = new QDialog;

  LoadMontageDialog->setMinimumSize(QSize(800, 180));
  LoadMontageDialog->setMaximumSize(QSize(800, 180));
  LoadMontageDialog->setWindowTitle("Load montage");
  LoadMontageDialog->setModal(TRUE);
  LoadMontageDialog->setAttribute(Qt::WA_DeleteOnClose, TRUE);

  label1 = new QLabel(LoadMontageDialog);
  label1->setGeometry(QRect(10, 10, 780, 25));
  label1->setText("Choose to which file you want to apply the montage:");

  filelist = new QListWidget(LoadMontageDialog);
  filelist->setGeometry(QRect(10, 35, 780, 75));
  filelist->setSelectionBehavior(QAbstractItemView::SelectRows);
  filelist->setSelectionMode(QAbstractItemView::SingleSelection);
  for(i=0; i<mainwindow->files_open; i++)
  {
    new QListWidgetItem(QString::fromLocal8Bit(mainwindow->edfheaderlist[i]->filename), filelist);
  }

  LoadButton = new QPushButton(LoadMontageDialog);
  LoadButton->setGeometry(QRect(10, 140, 100, 25));
  LoadButton->setText("Load");

  CloseButton = new QPushButton(LoadMontageDialog);
  CloseButton->setGeometry(QRect(690, 140, 100, 25));
  CloseButton->setText("Close");

  QObject::connect(CloseButton,  SIGNAL(clicked()), LoadMontageDialog, SLOT(close()));
  QObject::connect(LoadButton,   SIGNAL(clicked()), this,              SLOT(LoadButtonClicked()));

  filelist->setCurrentRow(mainwindow->files_open - 1);

  LoadMontageDialog->exec();
}




void UI_LoadMontagewindow::LoadButtonClicked()
{
  int i, k, n, p,
      tmp,
      skip,
      found,
      signalcomps_read=0,
      signals_read,
      signal_cnt,
      filters_read,
      filter_cnt=0,
      ravg_filter_cnt=0,
      fidfilter_cnt=0,
      len,
      order=1,
      type=0,
      model=0,
      size=0,
      amp_cat[3],
      f_ruler_cnt=0;

  char *result,
       scratchpad[2048],
       str[128],
       *err,
       *filter_spec,
       spec_str[256];

  double frequency=1.0,
         frequency2=2.0,
         ripple=1.0;


  struct xml_handle *xml_hdl;

  struct signalcompblock *newsignalcomp;


  if(mainwindow->files_open==1)  n = 0;
  else  n = filelist->currentRow();

  if(mtg_path[0]==0)
  {
    strcpy(mtg_path, QFileDialog::getOpenFileName(0, "Load montage", QString::fromLocal8Bit(mainwindow->recent_montagedir), "Montage files (*.mtg *.MTG)").toLocal8Bit().data());

    if(!strcmp(mtg_path, ""))
    {
      return;
    }

    get_directory_from_path(mainwindow->recent_montagedir, mtg_path, MAX_PATH_LENGTH);
  }

  xml_hdl = xml_get_handle(mtg_path);
  if(xml_hdl==NULL)
  {
    sprintf(scratchpad, "Can not open montage file:\n%s", mtg_path);
    QMessageBox messagewindow(QMessageBox::Critical, "Error", QString::fromLocal8Bit(scratchpad));
    messagewindow.exec();
    mainwindow->remove_recent_file_mtg_path(mtg_path);
    return;
  }

  if(strcmp(xml_hdl->elementname, PROGRAM_NAME "_montage"))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
    messagewindow.exec();
    xml_close(xml_hdl);
    return;
  }

////////////////////////////////////////////

  for(k=0; k<mainwindow->signalcomps; )
  {
    if(mainwindow->signalcomp[k]->filenum==n)
    {
      if(mainwindow->spectrumdock->signalcomp == mainwindow->signalcomp[k])
      {
        mainwindow->spectrumdock->clear();
        mainwindow->spectrumdock->dock->hide();
      }

      for(i=0; i<MAXSPECTRUMDIALOGS; i++)
      {
        p = mainwindow->signalcomp[k]->spectr_dialog[i];

        if(p != 0)
        {
          delete mainwindow->spectrumdialog[p - 1];

          mainwindow->spectrumdialog[p - 1] = NULL;
        }
      }

      for(i=0; i<MAXAVERAGECURVEDIALOGS; i++)
      {
        p = mainwindow->signalcomp[k]->avg_dialog[i];

        if(p != 0)
        {
          delete mainwindow->averagecurvedialog[p - 1];

          mainwindow->averagecurvedialog[p - 1] = NULL;
        }
      }

      if(mainwindow->signalcomp[k]->hascursor2)
      {
/*        crosshair_2_active = 0;
        crosshair_2_moving = 0;*/
      }

      if(mainwindow->signalcomp[k]->hascursor1)
      {
//         crosshair_1_active = 0;
//         crosshair_2_active = 0;
//         crosshair_1_moving = 0;
//         crosshair_2_moving = 0;

        for(i=0; i<mainwindow->signalcomps; i++)
        {
          mainwindow->signalcomp[i]->hascursor2 = 0;
        }
      }

      for(i=0; i<mainwindow->signalcomp[k]->filter_cnt; i++)
      {
        free(mainwindow->signalcomp[k]->filter[i]);
      }

      mainwindow->signalcomp[k]->filter_cnt = 0;

      for(i=0; i<mainwindow->signalcomp[k]->ravg_filter_cnt; i++)
      {
        free_ravg_filter(mainwindow->signalcomp[k]->ravg_filter[i]);
      }

      mainwindow->signalcomp[k]->ravg_filter_cnt = 0;

      for(i=0; i<mainwindow->signalcomp[k]->fidfilter_cnt; i++)
      {
        free(mainwindow->signalcomp[k]->fidfilter[i]);
        fid_run_free(mainwindow->signalcomp[k]->fid_run[i]);
        fid_run_freebuf(mainwindow->signalcomp[k]->fidbuf[i]);
        fid_run_freebuf(mainwindow->signalcomp[k]->fidbuf2[i]);
      }

      mainwindow->signalcomp[k]->fidfilter_cnt = 0;


      if(mainwindow->signalcomp[k]->ecg_filter != NULL)
      {
        free_ecg_filter(mainwindow->signalcomp[k]->ecg_filter);

        mainwindow->signalcomp[k]->ecg_filter = NULL;

        strcpy(mainwindow->signalcomp[k]->signallabel, mainwindow->signalcomp[k]->signallabel_bu);
        mainwindow->signalcomp[k]->signallabellen = mainwindow->signalcomp[k]->signallabellen_bu;
      }

      free(mainwindow->signalcomp[k]);

      for(i=k; i<mainwindow->signalcomps - 1; i++)
      {
        mainwindow->signalcomp[i] = mainwindow->signalcomp[i + 1];
      }

      mainwindow->signalcomps--;

      k = 0;
    }
    else
    {
      k++;
    }
  }

////////////////////////////////////////////

  while(1)
  {
    skip = 0;

    xml_goto_root(xml_hdl);

    signals_read = 0;

    if(xml_goto_nth_element_inside(xml_hdl, "signalcomposition", signalcomps_read))
    {
      break;
    }

    newsignalcomp = (struct signalcompblock *)calloc(1, sizeof(struct signalcompblock));
    if(newsignalcomp==NULL)
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "Internal error: Memory allocation error:\n\"new signal composition\"");
      messagewindow.exec();
      xml_close(xml_hdl);
      return;
    }

    if(xml_goto_nth_element_inside(xml_hdl, "num_of_signals", 0))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
      messagewindow.exec();
      free(newsignalcomp);
      xml_close(xml_hdl);
      return;
    }
    result = xml_get_content_of_element(xml_hdl);
    signal_cnt = atoi(result);
    free(result);
    if((signal_cnt<1)||(signal_cnt>MAXSIGNALS))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
      messagewindow.exec();
      free(newsignalcomp);
      xml_close(xml_hdl);
      return;
    }
    newsignalcomp->edfhdr = mainwindow->edfheaderlist[n];
    newsignalcomp->filenum = n;
    newsignalcomp->num_of_signals = signal_cnt;
    newsignalcomp->hascursor1 = 0;
    newsignalcomp->hascursor2 = 0;
    newsignalcomp->hasoffsettracking = 0;
    newsignalcomp->hasgaintracking = 0;
    newsignalcomp->screen_offset = 0;
    newsignalcomp->filter_cnt = 0;
    newsignalcomp->ravg_filter_cnt = 0;
    newsignalcomp->ecg_filter = NULL;
    newsignalcomp->fidfilter_cnt = 0;
    newsignalcomp->hasruler = 0;
    newsignalcomp->polarity = 1;

    xml_go_up(xml_hdl);
    if(!(xml_goto_nth_element_inside(xml_hdl, "alias", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      if(result[0] != 0)
      {
        strncpy(newsignalcomp->alias, result, 16);
        newsignalcomp->alias[16] = 0;
        latin1_to_ascii(newsignalcomp->alias, 16);
        remove_trailing_spaces(newsignalcomp->alias);
        remove_leading_spaces(newsignalcomp->alias);
      }
      free(result);
      xml_go_up(xml_hdl);
    }

    if(xml_goto_nth_element_inside(xml_hdl, "voltpercm", 0))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
      messagewindow.exec();
      free(newsignalcomp);
      xml_close(xml_hdl);
      return;
    }
    result = xml_get_content_of_element(xml_hdl);
    newsignalcomp->voltpercm = atof(result);
    if(newsignalcomp->voltpercm==0.0)  newsignalcomp->voltpercm = 0.000000001;
    free(result);
    xml_go_up(xml_hdl);
    if(xml_goto_nth_element_inside(xml_hdl, "screen_offset", 0))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
      messagewindow.exec();
      free(newsignalcomp);
      xml_close(xml_hdl);
      return;
    }
    result = xml_get_content_of_element(xml_hdl);
    newsignalcomp->screen_offset = atof(result);
    free(result);
    xml_go_up(xml_hdl);

    if(!(xml_goto_nth_element_inside(xml_hdl, "polarity", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      newsignalcomp->polarity = atoi(result);
      if(newsignalcomp->polarity != -1)
      {
        newsignalcomp->polarity = 1;
      }
      free(result);
      xml_go_up(xml_hdl);
    }

    if(xml_goto_nth_element_inside(xml_hdl, "color", 0))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
      messagewindow.exec();
      free(newsignalcomp);
      xml_close(xml_hdl);
      return;
    }
    result = xml_get_content_of_element(xml_hdl);
    newsignalcomp->color = atoi(result);
    if((newsignalcomp->color < 2) || (newsignalcomp->color > 18))
    {
      newsignalcomp->color = 2;
    }
    free(result);

    xml_go_up(xml_hdl);

    if(!(xml_goto_nth_element_inside(xml_hdl, "filter_cnt", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      filter_cnt = atoi(result);
      if(filter_cnt < 0)  filter_cnt = 0;
      if(filter_cnt > MAXFILTERS)  filter_cnt = MAXFILTERS;
      free(result);

      xml_go_up(xml_hdl);
    }

    if(!(xml_goto_nth_element_inside(xml_hdl, "ravg_filter_cnt", 0)))
    {
      result = xml_get_content_of_element(xml_hdl);
      ravg_filter_cnt = atoi(result);
      if(ravg_filter_cnt < 0)  filter_cnt = 0;
      if(ravg_filter_cnt > MAXFILTERS)  ravg_filter_cnt = MAXFILTERS;
      free(result);

      xml_go_up(xml_hdl);
    }

    if(filter_cnt)
    {
      fidfilter_cnt = 0;
    }
    else
    {
      if(!(xml_goto_nth_element_inside(xml_hdl, "fidfilter_cnt", 0)))
      {
        result = xml_get_content_of_element(xml_hdl);
        fidfilter_cnt = atoi(result);
        if(fidfilter_cnt < 0)  fidfilter_cnt = 0;
        if(fidfilter_cnt > MAXFILTERS)  fidfilter_cnt = MAXFILTERS;
        free(result);

        xml_go_up(xml_hdl);
      }
    }

    for(signals_read=0; signals_read<signal_cnt; signals_read++)
    {
      if(xml_goto_nth_element_inside(xml_hdl, "signal", signals_read))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }

      if(xml_goto_nth_element_inside(xml_hdl, "factor", 0))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }
      result = xml_get_content_of_element(xml_hdl);
      newsignalcomp->factor[signals_read] = atoi(result);
      if(newsignalcomp->factor[signals_read] < -128)
      {
        newsignalcomp->factor[signals_read] = -128;
      }
      if(newsignalcomp->factor[signals_read] > 128)
      {
        newsignalcomp->factor[signals_read] = 128;
      }
      if(newsignalcomp->factor[signals_read] == 0)
      {
        newsignalcomp->factor[signals_read] = 1;
      }
      free(result);

      xml_go_up(xml_hdl);

      if(xml_goto_nth_element_inside(xml_hdl, "edfindex", 0))
      {
        if(xml_goto_nth_element_inside(xml_hdl, "label", 0))
        {
          QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
          messagewindow.exec();
          free(newsignalcomp);
          xml_close(xml_hdl);
          return;
        }
        result = xml_get_content_of_element(xml_hdl);

        remove_trailing_spaces(result);

        found = 0;

        for(i=0; i<newsignalcomp->edfhdr->edfsignals; i++)
        {
          strcpy(scratchpad, newsignalcomp->edfhdr->edfparam[i].label);

          remove_trailing_spaces(scratchpad);

          if(!strcmp(scratchpad, result))
          {
            newsignalcomp->edfsignal[signals_read] = i;

            if(newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[signals_read]].annotation)
            {
              found = 0;
            }
            else
            {
              found = 1;
            }
            break;
          }
        }
      }
      else
      {
        result = xml_get_content_of_element(xml_hdl);

        newsignalcomp->edfsignal[signals_read] = atoi(result);

        if((newsignalcomp->edfsignal[signals_read] < 0) || (newsignalcomp->edfsignal[signals_read] >= newsignalcomp->edfhdr->edfsignals))
        {
          found = 0;
        }
        else
        {
          if(newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[signals_read]].annotation)
          {
            found = 0;
          }
          else
          {
            found = 1;
          }
        }
      }

      free(result);

      if(!found)
      {
        free(newsignalcomp);
        skip = 1;
        signalcomps_read++;
        xml_go_up(xml_hdl);
        xml_go_up(xml_hdl);
        break;
      }

      if(signals_read)
      {
        if(newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[signals_read]].smp_per_record
          != newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[0]].smp_per_record)
        {
          free(newsignalcomp);
          skip = 1;
          signalcomps_read++;
          xml_go_up(xml_hdl);
          xml_go_up(xml_hdl);
          break;
        }
      }

      newsignalcomp->sensitivity[signals_read] = newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[signals_read]].bitvalue / (newsignalcomp->voltpercm * mainwindow->pixelsizefactor);

      if(!signals_read)
      {
        newsignalcomp->signallabel[0] = 0;
      }

      if(signal_cnt>1)
      {
        if(newsignalcomp->factor[signals_read]<0)
        {
          strcat(newsignalcomp->signallabel, "- ");
        }
        else
        {
          if(signals_read)
          {
            strcat(newsignalcomp->signallabel, "+ ");
          }
        }
      }
      strcpy(str, newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[signals_read]].label);
      strip_types_from_label(str);
      strcat(newsignalcomp->signallabel, str);
      strcat(newsignalcomp->signallabel, " ");
      len = strlen(newsignalcomp->signallabel);
      for(k=(len-1); k>0; k--)
      {
        if(newsignalcomp->signallabel[k]!=' ')  break;
      }
      newsignalcomp->signallabel[k+2] = 0;

      newsignalcomp->file_duration = newsignalcomp->edfhdr->long_data_record_duration * newsignalcomp->edfhdr->datarecords;

      newsignalcomp->signallabellen = strlen(newsignalcomp->signallabel);

      xml_go_up(xml_hdl);
      xml_go_up(xml_hdl);
    }

    if(skip)  continue;

    strcpy(newsignalcomp->physdimension, newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[0]].physdimension);
    remove_trailing_spaces(newsignalcomp->physdimension);

    for(filters_read=0; filters_read<filter_cnt; filters_read++)
    {
      if(xml_goto_nth_element_inside(xml_hdl, "filter", filters_read))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }

      if(xml_goto_nth_element_inside(xml_hdl, "LPF", 0))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }
      result = xml_get_content_of_element(xml_hdl);
      type = atoi(result);
      free(result);

      xml_go_up(xml_hdl);
      if(xml_goto_nth_element_inside(xml_hdl, "frequency", 0))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }
      result = xml_get_content_of_element(xml_hdl);
      frequency = atof(result);
      free(result);

      if((type   < 0) || (type   >   1) || (frequency < 0.0001))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file. (filter values)");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }

      if(frequency >= ((newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[0]].smp_per_record
                      / newsignalcomp->edfhdr->data_record_duration)
                      / 2.0))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "The frequency of the filter(s) must be less than: samplerate / 2");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }

      if(type == 0)
      {
        sprintf(spec_str, "HpBu1/%f", frequency);
      }

      if(type == 1)
      {
        sprintf(spec_str, "LpBu1/%f", frequency);
      }

      filter_spec = spec_str;

      err = fid_parse(((double)(newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[0]].smp_per_record)) / newsignalcomp->edfhdr->data_record_duration,
                      &filter_spec,
                      &newsignalcomp->fidfilter[filters_read]);

      if(err != NULL)
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", err);
        messagewindow.exec();
        free(err);
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }

      newsignalcomp->fid_run[filters_read] = fid_run_new(newsignalcomp->fidfilter[filters_read],
                                                                         &newsignalcomp->fidfuncp[filters_read]);

      newsignalcomp->fidbuf[filters_read] = fid_run_newbuf(newsignalcomp->fid_run[filters_read]);
      newsignalcomp->fidbuf2[filters_read] = fid_run_newbuf(newsignalcomp->fid_run[filters_read]);

      newsignalcomp->fidfilter_freq[filters_read] = frequency;

      newsignalcomp->fidfilter_freq2[filters_read] = frequency * 1.12;

      newsignalcomp->fidfilter_ripple[filters_read] = -1.0;

      newsignalcomp->fidfilter_order[filters_read] = 1;

      newsignalcomp->fidfilter_type[filters_read] = type;

      newsignalcomp->fidfilter_model[filters_read] = 0;

      newsignalcomp->fidfilter_setup[filters_read] = 1;

      newsignalcomp->fidfilter_cnt = filters_read + 1;

      xml_go_up(xml_hdl);
      xml_go_up(xml_hdl);
    }

    for(filters_read=0; filters_read<ravg_filter_cnt; filters_read++)
    {
      if(xml_goto_nth_element_inside(xml_hdl, "ravg_filter", filters_read))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }

      if(xml_goto_nth_element_inside(xml_hdl, "type", 0))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }
      result = xml_get_content_of_element(xml_hdl);
      type = atoi(result);
      free(result);

      xml_go_up(xml_hdl);
      if(xml_goto_nth_element_inside(xml_hdl, "size", 0))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }
      result = xml_get_content_of_element(xml_hdl);
      size = atoi(result);
      free(result);

      if((type   < 0) || (type   >   1) || (size < 2) || (size > 10000))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file. (ravg_filter values)");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }

      newsignalcomp->ravg_filter[filters_read] = create_ravg_filter(type, size);
      if(newsignalcomp->ravg_filter[filters_read] == NULL)
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "A memory allocation error occurred when creating an ravg filter.");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }

      newsignalcomp->ravg_filter_size[filters_read] = size;

      newsignalcomp->ravg_filter_type[filters_read] = type;

      newsignalcomp->ravg_filter_cnt = filters_read + 1;

      xml_go_up(xml_hdl);
      xml_go_up(xml_hdl);
    }

    for(filters_read=0; filters_read<fidfilter_cnt; filters_read++)
    {
      if(xml_goto_nth_element_inside(xml_hdl, "fidfilter", filters_read))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file. (fidfilter)");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }

      if(xml_goto_nth_element_inside(xml_hdl, "type", 0))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file. (type)");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }
      result = xml_get_content_of_element(xml_hdl);
      type = atoi(result);
      free(result);

      xml_go_up(xml_hdl);
      if(xml_goto_nth_element_inside(xml_hdl, "model", 0))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file. (model)");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }
      result = xml_get_content_of_element(xml_hdl);
      model = atoi(result);
      free(result);

      xml_go_up(xml_hdl);
      if(xml_goto_nth_element_inside(xml_hdl, "frequency", 0))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file. (frequency)");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }
      result = xml_get_content_of_element(xml_hdl);
      frequency = atof(result);
      free(result);

      xml_go_up(xml_hdl);
      if(xml_goto_nth_element_inside(xml_hdl, "frequency2", 0))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file. (frequency2)");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }
      result = xml_get_content_of_element(xml_hdl);
      frequency2 = atof(result);
      free(result);

      xml_go_up(xml_hdl);
      if(xml_goto_nth_element_inside(xml_hdl, "ripple", 0))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file. (ripple)");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }
      result = xml_get_content_of_element(xml_hdl);
      ripple = atof(result);
      free(result);

      xml_go_up(xml_hdl);
      if(xml_goto_nth_element_inside(xml_hdl, "order", 0))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file. (order)");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }
      result = xml_get_content_of_element(xml_hdl);
      order = atoi(result);
      free(result);

      if((model  < 0)         || (model  >   2)       ||
         (order  < 1)         || (order  > 100)       ||
         (type   < 0)         || (type   >   4)       ||
         (ripple < (-6.0))    || (ripple > (-0.1))    ||
         (frequency < 0.0001) || (frequency2 < 0.0001))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file. (fidfilter values)");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }

      if(frequency >= ((newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[0]].smp_per_record
                      / newsignalcomp->edfhdr->data_record_duration)
                      / 2.0))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "The frequency of the filter(s) must be less than: samplerate / 2");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }

      if(type > 2)
      {
        if(frequency2 >= ((newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[0]].smp_per_record
                        / newsignalcomp->edfhdr->data_record_duration)
                        / 2.0))
        {
          QMessageBox messagewindow(QMessageBox::Critical, "Error", "The frequency of the filter(s) must be less than: samplerate / 2");
          messagewindow.exec();
          free(newsignalcomp);
          xml_close(xml_hdl);
          return;
        }
      }

      if((type == 0) || (type == 1))
      {
        if(order > 8)
        {
          order = 8;
        }
      }

      if(type == 0)
      {
        if(model == 0)
        {
          sprintf(spec_str, "HpBu%i/%f", order, frequency);
        }

        if(model == 1)
        {
          sprintf(spec_str, "HpCh%i/%f/%f", order, ripple, frequency);
        }

        if(model == 2)
        {
          sprintf(spec_str, "HpBe%i/%f", order, frequency);
        }
      }

      if(type == 1)
      {
        if(model == 0)
        {
          sprintf(spec_str, "LpBu%i/%f", order, frequency);
        }

        if(model == 1)
        {
          sprintf(spec_str, "LpCh%i/%f/%f", order, ripple, frequency);
        }

        if(model == 2)
        {
          sprintf(spec_str, "LpBe%i/%f", order, frequency);
        }
      }

      if(type == 2)
      {
        if(order > 100)
        {
          order = 100;
        }

        if(order < 3)
        {
          order = 3;
        }

        if(model == 0)
        {
          sprintf(spec_str, "BsRe/%i/%f", order, frequency);
        }
      }

      if((type == 3) || (type == 4))
      {
        if(order < 2)
        {
          order = 2;
        }

        if(order % 2)
        {
          order++;
        }

        if(order > 16)
        {
          order = 16;
        }
      }

      if(type == 3)
      {
        if(model == 0)
        {
          sprintf(spec_str, "BpBu%i/%f-%f", order, frequency, frequency2);
        }

        if(model == 1)
        {
          sprintf(spec_str, "BpCh%i/%f/%f-%f", order, ripple, frequency, frequency2);
        }

        if(model == 2)
        {
          sprintf(spec_str, "BpBe%i/%f-%f", order, frequency, frequency2);
        }
      }

      if(type == 4)
      {
        if(model == 0)
        {
          sprintf(spec_str, "BsBu%i/%f-%f", order, frequency, frequency2);
        }

        if(model == 1)
        {
          sprintf(spec_str, "BsCh%i/%f/%f-%f", order, ripple, frequency, frequency2);
        }

        if(model == 2)
        {
          sprintf(spec_str, "BsBe%i/%f-%f", order, frequency, frequency2);
        }
      }

      filter_spec = spec_str;

      err = fid_parse(((double)(newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[0]].smp_per_record)) / newsignalcomp->edfhdr->data_record_duration,
                      &filter_spec,
                      &newsignalcomp->fidfilter[filters_read]);

      if(err != NULL)
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", err);
        messagewindow.exec();
        free(err);
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }

      newsignalcomp->fid_run[filters_read] = fid_run_new(newsignalcomp->fidfilter[filters_read],
                                                                         &newsignalcomp->fidfuncp[filters_read]);

      newsignalcomp->fidbuf[filters_read] = fid_run_newbuf(newsignalcomp->fid_run[filters_read]);
      newsignalcomp->fidbuf2[filters_read] = fid_run_newbuf(newsignalcomp->fid_run[filters_read]);

      newsignalcomp->fidfilter_freq[filters_read] = frequency;

      newsignalcomp->fidfilter_freq2[filters_read] = frequency2;

      newsignalcomp->fidfilter_ripple[filters_read] = ripple;

      newsignalcomp->fidfilter_order[filters_read] = order;

      newsignalcomp->fidfilter_type[filters_read] = type;

      newsignalcomp->fidfilter_model[filters_read] = model;

      newsignalcomp->fidfilter_setup[filters_read] = 1;

      newsignalcomp->fidfilter_cnt = filters_read + 1;

      xml_go_up(xml_hdl);
      xml_go_up(xml_hdl);
    }

    if(!xml_goto_nth_element_inside(xml_hdl, "ecg_filter", 0))
    {
      if(xml_goto_nth_element_inside(xml_hdl, "type", 0))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
        messagewindow.exec();
        free(newsignalcomp);
        xml_close(xml_hdl);
        return;
      }
      result = xml_get_content_of_element(xml_hdl);
      type = atoi(result);
      free(result);

      if(type == 1)
      {
        newsignalcomp->ecg_filter = create_ecg_filter(newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[0]].smp_per_record /
                                                      newsignalcomp->edfhdr->data_record_duration,
                                                      newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[0]].bitvalue,
                                                      mainwindow->powerlinefreq);
        if(newsignalcomp->ecg_filter == NULL)
        {
          QMessageBox messagewindow(QMessageBox::Critical, "Error", "A memory allocation error occurred when creating an ECG filter.");
          messagewindow.exec();
          free(newsignalcomp);
          xml_close(xml_hdl);
          return;
        }

        strcpy(newsignalcomp->signallabel_bu, newsignalcomp->signallabel);
        newsignalcomp->signallabellen_bu = newsignalcomp->signallabellen;
        strcpy(newsignalcomp->signallabel, "HR");
        newsignalcomp->signallabellen = strlen(newsignalcomp->signallabel);
        strcpy(newsignalcomp->physdimension_bu, newsignalcomp->physdimension);
        strcpy(newsignalcomp->physdimension, "bpm");
      }

      xml_go_up(xml_hdl);
      xml_go_up(xml_hdl);
    }

    if(newsignalcomp->ecg_filter == NULL)
    {
      if(!xml_goto_nth_element_inside(xml_hdl, "zratio_filter", 0))
      {
        if(xml_goto_nth_element_inside(xml_hdl, "type", 0))
        {
          QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
          messagewindow.exec();
          free(newsignalcomp);
          xml_close(xml_hdl);
          return;
        }
        result = xml_get_content_of_element(xml_hdl);
        type = atoi(result);
        free(result);

        xml_go_up(xml_hdl);

        if(type == 1)
        {
          if(xml_goto_nth_element_inside(xml_hdl, "crossoverfreq", 0))
          {
            QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
            messagewindow.exec();
            free(newsignalcomp);
            xml_close(xml_hdl);
            return;
          }
          result = xml_get_content_of_element(xml_hdl);
          newsignalcomp->zratio_crossoverfreq = atof(result);
          free(result);

          if((newsignalcomp->zratio_crossoverfreq < 5.0) || (newsignalcomp->zratio_crossoverfreq > 9.5))
          {
            newsignalcomp->zratio_crossoverfreq = 7.5;
          }

          newsignalcomp->zratio_filter = create_zratio_filter(newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[0]].smp_per_record,
                                                              newsignalcomp->edfhdr->long_data_record_duration,
                                                              newsignalcomp->zratio_crossoverfreq,
            newsignalcomp->edfhdr->edfparam[newsignalcomp->edfsignal[0]].bitvalue);

          if(newsignalcomp->zratio_filter == NULL)
          {
            QMessageBox messagewindow(QMessageBox::Critical, "Error", "A memory allocation error occurred when creating a Z-ratio filter.");
            messagewindow.exec();
            free(newsignalcomp);
            xml_close(xml_hdl);
            return;
          }

          strcpy(newsignalcomp->signallabel_bu, newsignalcomp->signallabel);
          newsignalcomp->signallabellen_bu = newsignalcomp->signallabellen;
          strcpy(newsignalcomp->signallabel, "Z-ratio ");
          strcat(newsignalcomp->signallabel, newsignalcomp->signallabel_bu);
          newsignalcomp->signallabellen = strlen(newsignalcomp->signallabel);
          strcpy(newsignalcomp->physdimension_bu, newsignalcomp->physdimension);
          strcpy(newsignalcomp->physdimension, "");

          xml_go_up(xml_hdl);
        }

        xml_go_up(xml_hdl);
      }
    }

    if(f_ruler_cnt == 0)
    {
      if(!xml_goto_nth_element_inside(xml_hdl, "floating_ruler", 0))
      {
        if(xml_goto_nth_element_inside(xml_hdl, "hasruler", 0))
        {
          QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this montage file.");
          messagewindow.exec();
          free(newsignalcomp);
          xml_close(xml_hdl);
          return;
        }
        result = xml_get_content_of_element(xml_hdl);
        tmp = atoi(result);
        free(result);

        xml_go_up(xml_hdl);

        if(tmp == 1)
        {
          f_ruler_cnt = 1;

          mainwindow->maincurve->ruler_x_position = 200;
          mainwindow->maincurve->ruler_y_position = 200;
          mainwindow->maincurve->floating_ruler_value = 0;

          if(!xml_goto_nth_element_inside(xml_hdl, "ruler_x_position", 0))
          {
            result = xml_get_content_of_element(xml_hdl);
            tmp = atoi(result);
            free(result);

            if((tmp >= 0) && (tmp < 5000))
            {
              mainwindow->maincurve->ruler_x_position = tmp;
            }

            xml_go_up(xml_hdl);
          }

          if(!xml_goto_nth_element_inside(xml_hdl, "ruler_y_position", 0))
          {
            result = xml_get_content_of_element(xml_hdl);
            tmp = atoi(result);
            free(result);

            if((tmp >= 0) && (tmp < 5000))
            {
              mainwindow->maincurve->ruler_y_position = tmp;
            }

            xml_go_up(xml_hdl);
          }

          if(!xml_goto_nth_element_inside(xml_hdl, "floating_ruler_value", 0))
          {
            result = xml_get_content_of_element(xml_hdl);
            tmp = atoi(result);
            free(result);

            if((tmp >= 0) && (tmp < 2))
            {
              mainwindow->maincurve->floating_ruler_value = tmp;
            }

            xml_go_up(xml_hdl);
          }

          newsignalcomp->hasruler = 1;
        }

        xml_go_up(xml_hdl);
      }
    }

    mainwindow->signalcomp[mainwindow->signalcomps] = newsignalcomp;

    mainwindow->signalcomps++;

    signalcomps_read++;
  }

  xml_goto_root(xml_hdl);

  if(!(xml_goto_nth_element_inside(xml_hdl, "pagetime", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    mainwindow->pagetime = atoll(result);
    if(mainwindow->pagetime < 10000LL)
    {
      mainwindow->pagetime = 10000LL;
    }
    free(result);
  }

  xml_close(xml_hdl);

  if(LoadMontageDialog!=NULL) LoadMontageDialog->close();

  mainwindow->setMainwindowTitle(mainwindow->edfheaderlist[mainwindow->sel_viewtime]);

  if(mainwindow->files_open == 1)
  {
    strcpy(&mainwindow->recent_file_mtg_path[0][0], mtg_path);
  }

  mainwindow->timescale_doubler = round_125_cat(mainwindow->pagetime);

  for(i=0; i<3; i++)
  {
    amp_cat[i] = 0;
  }

  for(i=0; i<mainwindow->signalcomps; i++)
  {
    tmp = round_125_cat(mainwindow->signalcomp[i]->voltpercm);

    switch(tmp)
    {
      case 10 : amp_cat[0]++;
                break;
      case 20 : amp_cat[1]++;
                break;
      case 50 : amp_cat[2]++;
                break;
    }
  }

  mainwindow->amplitude_doubler = 10;

  if((amp_cat[1] > amp_cat[0]) && (amp_cat[1] >= amp_cat[2]))
  {
    mainwindow->amplitude_doubler = 20;
  }

  if((amp_cat[2] > amp_cat[0]) && (amp_cat[2] > amp_cat[1]))
  {
    mainwindow->amplitude_doubler = 50;
  }

  if(f_ruler_cnt == 1)
  {
    mainwindow->maincurve->ruler_active = 1;
  }

  mainwindow->setup_viewbuf();
}


void UI_LoadMontagewindow::strip_types_from_label(char *label)
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








