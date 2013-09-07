/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2013 Teunis van Beelen
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




#include "check_for_updates.h"





Check_for_updates::Check_for_updates()
{
  QNetworkRequest request;
  QNetworkReply *reply;

  manager = new QNetworkAccessManager(this);

  request.setUrl(QUrl("http://www.teuniz.net/edfbrowser/latest_version.txt"));
  request.setRawHeader("User-Agent", PROGRAM_NAME " " PROGRAM_VERSION " OS_UNKNOWN");
#ifdef Q_WS_X11
  request.setRawHeader("User-Agent", PROGRAM_NAME " " PROGRAM_VERSION " Q_WS_X11");
#endif
#ifdef Q_WS_MAC
  request.setRawHeader("User-Agent", PROGRAM_NAME " " PROGRAM_VERSION " Q_WS_MAC");
#endif
#ifdef Q_WS_WIN
  request.setRawHeader("User-Agent", PROGRAM_NAME " " PROGRAM_VERSION " Q_WS_WIN");
#endif
  request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);

  reply = manager->get(request);

  connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
}


void Check_for_updates::replyFinished()
{
  long long int n;

  char buf[128];

  int this_version,
      latest_version;

  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

  if(reply->error() != QNetworkReply::NoError)
  {
    reply->deleteLater();

    return;
  }

  if(reply->bytesAvailable() < 31)
  {
    reply->deleteLater();

    return;
  }

  n = reply->read(buf, 100);

  if(n < 31)
  {
    reply->deleteLater();

    return;
  }

  reply->deleteLater();

  buf[30] = 0;

  if(strncmp(buf, "EDFbrowser latest version: ", 27))
  {
    return;
  }

  if(is_integer_number(buf + 27))
  {
    return;
  }

  latest_version = atoi(buf + 27);

  if((latest_version < 1) || (latest_version > 1000000))
  {
    return;
  }

  sprintf(buf, PROGRAM_VERSION);

  buf[1] = buf[0];

  this_version = atoi(buf + 1);

  if(this_version >= latest_version)
  {
    return;
  }

  QMessageBox messagewindow(QMessageBox::Information,
                            "New version available",
                            "A newer version of EDFbrowser is available.\n"
                            "Do you want to download the new version now?",
                            QMessageBox::Yes | QMessageBox::No);

  if(messagewindow.exec() != QMessageBox::Yes)
  {
    return;
  }

  QDesktopServices::openUrl(QUrl("http://www.teuniz.net/edfbrowser/"));
}










