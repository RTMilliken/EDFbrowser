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




#include "scp_ecg2edf.h"


#if defined(__APPLE__) || defined(__MACH__) || defined(__APPLE_CC__)

#define fopeno fopen

#else

#define fseeko fseeko64
#define ftello ftello64
#define fopeno fopen64

#endif


#define SPCECGBUFSIZE 1048576




UI_SCPECG2EDFwindow::UI_SCPECG2EDFwindow(char *recent_dir, char *save_dir)
{
  char txt_string[2048];

  recent_opendir = recent_dir;
  recent_savedir = save_dir;

  myobjectDialog = new QDialog;

  myobjectDialog->setMinimumSize(QSize(600, 480));
  myobjectDialog->setMaximumSize(QSize(600, 480));
  myobjectDialog->setWindowTitle("SCP ECG to EDF+ converter");
  myobjectDialog->setModal(TRUE);
  myobjectDialog->setAttribute(Qt::WA_DeleteOnClose, TRUE);

  pushButton1 = new QPushButton(myobjectDialog);
  pushButton1->setGeometry(QRect(20, 430, 100, 26));
  pushButton1->setText("Select File");

  pushButton2 = new QPushButton(myobjectDialog);
  pushButton2->setGeometry(QRect(480, 430, 100, 26));
  pushButton2->setText("Close");

  textEdit1 = new QTextEdit(myobjectDialog);
  textEdit1->setGeometry(QRect(20, 20, 560, 380));
  textEdit1->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  textEdit1->setReadOnly(TRUE);
  textEdit1->setLineWrapMode(QTextEdit::NoWrap);
  sprintf(txt_string, "SCP ECG to EDF+ converter.\n");
  textEdit1->append(txt_string);

  crc_ccitt_init();

  QObject::connect(pushButton1, SIGNAL(clicked()), this, SLOT(SelectFileButton()));
  QObject::connect(pushButton2, SIGNAL(clicked()), myobjectDialog, SLOT(close()));

  myobjectDialog->exec();
}


void UI_SCPECG2EDFwindow::SelectFileButton()
{
  FILE *inputfile=NULL;

  int i, j, k, n, hdl, chns, sf, avm, blocks, *buf, encoding, compression, offset,
      abs_val_a[256],
      abs_val_b[256],
      var_tmp;

  unsigned short sh_tmp;

  long long filesize, ll_tmp, bits;

  char input_filename[MAX_PATH_LENGTH],
       txt_string[2048],
       edf_filename[MAX_PATH_LENGTH],
       scratchpad[MAX_PATH_LENGTH],
       *block,
       ch_tmp;

  union{
         unsigned long long ll_int;
         unsigned int one[2];
         unsigned short two[4];
         unsigned char four[8];
       } var;

  pushButton1->setEnabled(FALSE);

  for(i=0; i<256; i++)
  {
    abs_val_a[i] = 0;
    abs_val_b[i] = 0;
  }

  strcpy(input_filename, QFileDialog::getOpenFileName(0, "Select inputfile", QString::fromLocal8Bit(recent_opendir), "SCP files (*.scp *.SCP)").toLocal8Bit().data());

  if(!strcmp(input_filename, ""))
  {
    pushButton1->setEnabled(TRUE);
    return;
  }

  get_directory_from_path(recent_opendir, input_filename, MAX_PATH_LENGTH);

  inputfile = fopeno(input_filename, "rb");
  if(inputfile==NULL)
  {
    snprintf(txt_string, 2048, "Can not open file %s for reading.\n", input_filename);
    textEdit1->append(txt_string);
    pushButton1->setEnabled(TRUE);
    return;
  }

  get_filename_from_path(scratchpad, input_filename, MAX_PATH_LENGTH);

  snprintf(txt_string, 2048, "Read file: %s", scratchpad);
  textEdit1->append(txt_string);

  fseeko(inputfile, 0LL, SEEK_END);
  filesize = ftello(inputfile);
  if(filesize<126)
  {
    textEdit1->append("Error, filesize is too small.\n");
    fclose(inputfile);
    pushButton1->setEnabled(TRUE);
    return;
  }

  ll_tmp = 0LL;

  fseeko(inputfile, 2LL, SEEK_SET);

  if(fread(&ll_tmp, 4, 1, inputfile) != 1)
  {
    textEdit1->append("A read-error occurred (1)\n");
    fclose(inputfile);
    pushButton1->setEnabled(TRUE);
    return;
  }

  if(ll_tmp != filesize)
  {
    textEdit1->append("Error, filesize does not match with header.\n");
    fclose(inputfile);
    pushButton1->setEnabled(TRUE);
    return;
  }

  block = (char *)malloc(SPCECGBUFSIZE);
  if(block == NULL)
  {
    textEdit1->append("Malloc error (block 1)\n");
    fclose(inputfile);
    pushButton1->setEnabled(TRUE);
    return;
  }

  rewind(inputfile);

  if(fread(&sh_tmp, 2, 1, inputfile) != 1)
  {
    textEdit1->append("A read-error occurred (2)\n");
    fclose(inputfile);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

  if(check_crc(inputfile, 2LL, filesize - 2LL, sh_tmp, block))
  {
    fclose(inputfile);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

  memset(&sp, 0, sizeof(struct section_prop_struct[12]));

  if(read_section_header(0, inputfile, 6LL, block))
  {
    fclose(inputfile);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

  if(strncmp(sp[0].reserved, "SCPECG", 6))
  {
    textEdit1->append("Error, reserved field of section header 0 does not contain string \"SCPECG\".\n");
    fclose(inputfile);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

  sp[0].file_offset = 6LL;

// printf("\nsection ID is %i\n"
//        "section file offset is %lli\n"
//        "section CRC is 0x%04X\n"
//        "section length is %i\n"
//        "section version is %i\n"
//        "section protocol version is %i\n",
//        sp[0].section_id,
//        sp[0].file_offset,
//        (int)sp[0].crc,
//        sp[0].section_length,
//        sp[0].section_version,
//        sp[0].section_protocol_version);

  if(read_data_section_zero(inputfile, block, filesize))
  {
    fclose(inputfile);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

  fseeko(inputfile, sp[6].file_offset + 16LL, SEEK_SET);

  if(fread(block, 6, 1, inputfile) != 1)
  {
    textEdit1->append("A read-error occurred\n");
    fclose(inputfile);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

  avm = *((unsigned short *)block);

  sf = 1000000 / *((unsigned short *)(block + 2));

  encoding = *((unsigned char *)(block + 4));

  compression = *((unsigned char *)(block + 5));

  if(compression != 0)
  {
    textEdit1->append("File contains bimodal compressed data which is not supported by this converter.\n ");
    fclose(inputfile);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

  fseeko(inputfile, sp[3].file_offset + 16LL, SEEK_SET);

  if(fread(scratchpad, 2, 1, inputfile) != 1)
  {
    textEdit1->append("A read-error occurred (40)\n");
    fclose(inputfile);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

  chns = *((unsigned char *)scratchpad);

  if(chns < 1)
  {
    textEdit1->append("Error, number of signals is less than one.\n ");
    fclose(inputfile);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

  if(chns > 256)
  {
    textEdit1->append("Error, number of signals is more than 256.\n ");
    fclose(inputfile);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

  if(scratchpad[1] & 1)
  {
    textEdit1->append("Error, reference beat subtraction used for compression.\n ");
    fclose(inputfile);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

  if(!(scratchpad[1] & 4))
  {
    textEdit1->append("Error, leads are not simultaneously recorded. (1)\n ");
    fclose(inputfile);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

// printf("chns is %u   AVM is %u   sf is %u   encoding is %u   compression is %u\n",
//        chns, avm, sf, encoding, compression);
//
// for(i=0; i<12; i++)
// {
//   printf("section %i is present: %i    file offset is %lli   length is %i\n", i, sp[i].present, sp[i].file_offset, sp[i].section_length);
// }

  memset(&lp, 0, sizeof(struct lead_prop_struct[256]));

  fseeko(inputfile, sp[3].file_offset + 18LL, SEEK_SET);

  for(i=0; i<chns; i++)
  {
    if(fread(&(lp[i].start), 4, 1, inputfile) != 1)
    {
      textEdit1->append("A read-error occurred (30)\n");
      fclose(inputfile);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }

    if(fread(&(lp[i].end), 4, 1, inputfile) != 1)
    {
      textEdit1->append("A read-error occurred (31)\n");
      fclose(inputfile);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }

    if(fread(&(lp[i].label), 1, 1, inputfile) != 1)
    {
      textEdit1->append("A read-error occurred (32)\n");
      fclose(inputfile);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }
  }

// for(i=0; i<chns; i++)
// {
//   printf("lp[i].start is %i   lp[i].end is %i\n", lp[i].start, lp[i].end);
// }

  if(chns > 1)
  {
    for(i=1; i<chns; i++)
    {
      if(lp[i].start != lp[0].start)
      {
        textEdit1->append("Error, leads are not simultaneously recorded. (2)\n");
        fclose(inputfile);
        free(block);
        pushButton1->setEnabled(TRUE);
        return;
      }

      if(lp[i].end != lp[0].end)
      {
        textEdit1->append("Error, leads are not simultaneously recorded. (3)\n");
        fclose(inputfile);
        free(block);
        pushButton1->setEnabled(TRUE);
        return;
      }
    }
  }

  for(i=0; i<chns; i++)
  {
    if(lp[i].start < 1)
    {
      textEdit1->append("Error (55) (lp[i].start < 1)\n");
      fclose(inputfile);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }

    if(lp[i].end <= lp[i].start)
    {
      textEdit1->append("Error (56) (lp[i].end <= lp[i].start)\n");
      fclose(inputfile);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }

    if(lp[i].start != 1)
    {
      textEdit1->append("Error (57) (lp[i].start != 1)\n");
      fclose(inputfile);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }

    lp[i].start--;

    if((lp[i].end - lp[i].start) < sf)
    {
      textEdit1->append("Error, recording length is less than one second.\n");
      fclose(inputfile);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }

    lp[i].samples = lp[i].end - lp[i].start;
  }

  fseeko(inputfile, sp[6].file_offset + 22LL, SEEK_SET);

  n = 0;

  for(i=0; i<chns; i++)
  {
    if(fread(block, 2, 1, inputfile) != 1)
    {
      textEdit1->append("A read-error occurred (41)\n");
      fclose(inputfile);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }

    lp[i].bytes = *((unsigned short *)block);

    n += lp[i].bytes;

// printf("lead samples is %i   bytes is %i\n", lp[i].samples, lp[i].bytes);

    if(sp[2].present != 1)  // huffmantable
    {
      if(lp[i].bytes < (lp[i].samples * 2))
      {
        textEdit1->append("Error, lead samples is less than lead bytes.\n");
        fclose(inputfile);
        free(block);
        pushButton1->setEnabled(TRUE);
        return;
      }
    }
  }

  if(n > (sp[6].section_length - 22 - (chns * 2)))
  {
    textEdit1->append("Error, total databytes is more than section size.\n");
    fclose(inputfile);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

//////////////////////////////// Huffman tables ////////////////////

  if(sp[2].present == 1)  // huffmantable
  {
    fseeko(inputfile, sp[2].file_offset + 16LL, SEEK_SET);

    if(fread(block, 13, 1, inputfile) != 1)
    {
      textEdit1->append("A read-error occurred (50)\n");
      fclose(inputfile);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }

    ht.h_tables_cnt = *((unsigned short *)block);
    ht.code_structs_cnt = *((unsigned short *)(block + 2));
    ht.prefix_bits = *((unsigned char *)(block + 4));
    ht.total_bits = *((unsigned char *)(block + 5));
    ht.table_mode_switch = *((unsigned char *)(block + 6));
    ht.base_value = *((unsigned short *)(block + 7));
    ht.base_code = *((unsigned int *)(block + 9));

//   printf("ht.h_tables_cnt is %i   ht.code_structs_cnt is %i   ht.prefix_bits is %i   ht.total_bits is %i\n",
//         ht.h_tables_cnt, ht.code_structs_cnt, ht.prefix_bits, ht.total_bits);

    if(ht.h_tables_cnt != 19999)
    {
      textEdit1->append("Aborted, this converter does not support customized Huffmantables.\n");
      fclose(inputfile);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }
  }

//////////////////////////////// patient data ////////////////////

  memset(&pat_dat, 0, sizeof(struct patient_data_struct));

  if(get_patient_data(inputfile))
  {
    fclose(inputfile);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

// printf("patient ID:          %s\n"
//        "patient lastname:    %s\n"
//        "patient firstname:   %s\n"
//        "patient sex:         %i\n"
//        "startdate year:      %i\n"
//        "startdate month:     %i\n"
//        "startdate day:       %i\n"
//        "starttime hour:      %i\n"
//        "starttime minute:    %i\n"
//        "starttime second:    %i\n"
//        "birthdate year:      %i\n"
//        "birthdate month:     %i\n"
//        "birthdate day:       %i\n"
//        "device model:        %s\n"
//        "language code:       %u\n"
//        "manufacturer:        %s\n",
//        pat_dat.pat_id,
//        pat_dat.last_name,
//        pat_dat.first_name,
//        pat_dat.sex,
//        pat_dat.startdate_year,
//        pat_dat.startdate_month,
//        pat_dat.startdate_day,
//        pat_dat.starttime_hour,
//        pat_dat.starttime_minute,
//        pat_dat.starttime_second,
//        pat_dat.birthdate_year,
//        pat_dat.birthdate_month,
//        pat_dat.birthdate_day,
//        pat_dat.device_model,
//        pat_dat.lang_code,
//        pat_dat.manufacturer);

////////////////////////// start conversion ///////////////////////////////

  free(block);

  buf = (int *)malloc(((lp[0].samples / sf) + 1) * chns * sf * sizeof(int));
  if(buf == NULL)
  {
    textEdit1->append("Malloc error (buf)\n");
    fclose(inputfile);
    pushButton1->setEnabled(TRUE);
    return;
  }

  n = 0;

  for(i=0; i<chns; i++)
  {
    n += lp[i].bytes;
  }

  block = (char *)calloc(1, n + 64);
  if(block == NULL)
  {
    textEdit1->append("Malloc error (block 3)\n");
    fclose(inputfile);
    free(buf);
    pushButton1->setEnabled(TRUE);
    return;
  }

  fseeko(inputfile, sp[6].file_offset + 22LL + (chns * 2LL), SEEK_SET);  // rhythm data

  if(fread(block, n, 1, inputfile) != 1)
  {
    textEdit1->append("A read-error occurred during conversion (70)\n");
    fclose(inputfile);
    free(buf);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

// msb  lsb
// 11111111 10000011 01111111 11100000 11100110 11110001 01010011 01100101 01011001 10110110 01011011 10010110 01001011 10010110 0
// FF       83       7F       E0       E6       F5       53       65       59       B6       5B       96       4B       96
// 255      131      127      224      230      245      83       101      89       182      91       150      75       150

//   *((unsigned char *)block) = 255;
//   *((unsigned char *)(block + 1)) = 131;
//   *((unsigned char *)(block + 2)) = 127;
//   *((unsigned char *)(block + 3)) = 224;
//   *((unsigned char *)(block + 4)) = 230;
//   *((unsigned char *)(block + 5)) = 245;
//   *((unsigned char *)(block + 6)) = 83;
//   *((unsigned char *)(block + 7)) = 101;
//   *((unsigned char *)(block + 8)) = 89;
//   *((unsigned char *)(block + 9)) = 182;
//   *((unsigned char *)(block + 10)) = 91;
//   *((unsigned char *)(block + 11)) = 150;
//   *((unsigned char *)(block + 12)) = 75;
//   *((unsigned char *)(block + 13)) = 150;
//   *((unsigned char *)(block + 13)) = 0;


  offset = 0;

  for(j=0; j<chns; j++)
//  for(j=0; j<1; j++)
  {
    if(j > 0)
    {
      offset += lp[j - 1].bytes;
    }

    bits = 0LL;

    for(i=0; i<lp[j].samples; i++)
//    for(i=0; i<28; i++)
    {
      if(sp[2].present == 1)  // huffmantable present
      {
//        if((bits / 8LL) > (lp[j].bytes + 20))
        if((bits / 8LL) > lp[j].bytes)
        {
          textEdit1->append("Error, (bits / 8) >= lp[j].bytes (71)\n");
// printf("\nbits / 8 is %i    lp[j].bytes is %i\n", (int)(bits / 8LL), lp[j].bytes);
          fclose(inputfile);
          free(buf);
          free(block);
          pushButton1->setEnabled(TRUE);
          return;
        }

        memcpy(&var, block + offset + ((int)(bits / 8LL)), 5);

        for(k=0; k<5; k++)
        {
          var.four[k] = reverse_bitorder(var.four[k]);
        }

        var.ll_int >>= (int)(bits % 8LL);

        if((var.four[0] & 1) == 0)  // b00000001
        {
          buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = 0;
          bits++;
        } else
        if((var.four[0] & 7) == 1)  // b00000111
        {
          buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = 1;
          bits += 3LL;
        } else
        if((var.four[0] & 7) == 5)  // b00000111
        {
          buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = -1;
          bits += 3LL;
        } else
        if((var.four[0] & 15) == 3)  // b00001111
        {
          buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = 2;
          bits += 4LL;
        } else
        if((var.four[0] & 15) == 11)  // b00001111
        {
          buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = -2;
          bits += 4LL;
        } else
        if((var.four[0] & 31) == 7)  // b00011111
        {
          buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = 3;
          bits += 5LL;
        } else
        if((var.four[0] & 31) == 23)  // b00011111
        {
          buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = -3;
          bits += 5LL;
        } else
        if((var.four[0] & 63) == 15)  // b00111111
        {
          buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = 4;
          bits += 6LL;
        } else
        if((var.four[0] & 63) == 47)  // b00111111
        {
          buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = -4;
          bits += 6LL;
        } else
        if((var.four[0] & 127) == 31)  // b01111111
        {
          buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = 5;
          bits += 7LL;
        } else
        if((var.four[0] & 127) == 95)  // b01111111
        {
          buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = -5;
          bits += 7LL;
        } else
        if(var.four[0] == 63)  // b11111111
        {
          buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = 6;
          bits += 8LL;
        } else
        if(var.four[0] == 191)  // b11111111
        {
          buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = -6;
          bits += 8LL;
        } else
        if(var.four[0] == 127)  // b11111111
        {
          if((var.four[1] & 1) == 0)  // b00000001
          {
            buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = 7;
            bits += 9LL;
//printf(" 7:i: %i  ", i);
          }
          else
          {
            buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = -7;
            bits += 9LL;
//printf(" -7:i: %i  ", i);
          }
        } else
        if(var.four[0] == 255)  // b11111111
        {
          if((var.four[1] & 3) == 0)  // b00000011
//          if((var.four[1] & 192) == 0)
          {
            buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = 8;
            bits += 10LL;
//printf(" 8:i: %i  ", i);
          } else
          if((var.four[1] & 3) == 2)  // b00000011
//          if((var.four[1] & 192) == 64)
          {
            buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = -8;
            bits += 10LL;
//printf(" -8:i: %i  ", i);
          } else
          if((var.four[1] & 3) == 1)  // b00000011
//          if((var.four[1] & 192) == 128)
          {
//            var.four[0] = var.four[2];
            var.ll_int >>= 2;
            var.four[1] = reverse_bitorder(var.four[1]);
            buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = *((signed char *)&(var.four[1]));  // 8-bit original
            bits += 18LL;
//printf(" 8-bit:i: %i  ", i);
          } else
          if((var.four[1] & 3) == 3)  // b00000011
//          if((var.four[1] & 192) == 192)
          {
//            var.four[0] = var.four[2];
//            var.four[-1] = var.four[3];
            var.ll_int >>= 10;
            ch_tmp = reverse_bitorder(var.four[0]);
            var.four[0] = reverse_bitorder(var.four[1]);
            var.four[1] = ch_tmp;
            buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = *((signed short *)&(var.two[0]));  // 16-bit original
            bits += 26LL;
//printf(" 16-bit:i: %i  ", i);
          }
        }

// 11111111 11001111 11111010 10


// msb  lsb
// 11111111 10000011 01
// 11111111 10000011 01111111 11100000 11100110 11110001 01010011 01100101 01011001 10110110 01011011 10010110 01001011 10010110 0
// FF       83       7F       E0       E6       F5       53       65       59       B6       5B       96       4B       96
// 255      131      127      224      230      245      83       101      89       182      91       150      75       150

//   if(i == (lp[j].samples - 1))  printf("bits is %lli   bits / 8 is %lli   bits %% 8 is %lli   lp[j].bytes is %i\n",
//                                       bits, bits / 8LL, bits % 8LL, lp[j].bytes);

      }
      else  // huffmantable not present
      {
        buf[((i / sf) * (sf * chns)) + (j * sf) + (i % sf)] = ((signed short *)(block + offset))[i];
      }
    }
  }

// for(i=0; i<28; i++)
// {
//   printf(" %i", buf[i]);
// }
// putchar('\n');

  strcpy(edf_filename, input_filename);

  remove_extension_from_filename(edf_filename);

  strcat(edf_filename, ".edf");

  hdl = edfopen_file_writeonly(edf_filename, EDFLIB_FILETYPE_EDFPLUS, chns);

  if(hdl<0)
  {
    snprintf(txt_string, 2048, "Can not open file %s for writing.\n", edf_filename);
    textEdit1->append(txt_string);
    fclose(inputfile);
    free(buf);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

  for(i=0; i<chns; i++)
  {
    if(edf_set_samplefrequency(hdl, i, sf))
    {
      textEdit1->append("Error: edf_set_samplefrequency()\n");
      fclose(inputfile);
      edfclose_file(hdl);
      free(buf);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }

    if(edf_set_digital_minimum(hdl, i, -32768))
    {
      textEdit1->append("Error: edf_set_digital_minimum()\n");
      fclose(inputfile);
      edfclose_file(hdl);
      free(buf);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }

    if(edf_set_digital_maximum(hdl, i, 32767))
    {
      textEdit1->append("Error: edf_set_digital_maximum()\n");
      fclose(inputfile);
      edfclose_file(hdl);
      free(buf);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }

    lead_label_lookup(lp[i].label, scratchpad);

    if(edf_set_label(hdl, i, scratchpad))
    {
      textEdit1->append("Error: edf_set_label()\n");
      fclose(inputfile);
      edfclose_file(hdl);
      free(buf);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }

    if(edf_set_physical_dimension(hdl, i, "uV"))
    {
      textEdit1->append("Error: edf_set_physical_dimension()\n");
      fclose(inputfile);
      edfclose_file(hdl);
      free(buf);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }

    if(edf_set_physical_maximum(hdl, i, 32.767 * (double)avm))
    {
      textEdit1->append("Error: edf_set_physical_maximum()\n");
      fclose(inputfile);
      edfclose_file(hdl);
      free(buf);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }

    if(edf_set_physical_minimum(hdl, i, -32.768 * (double)avm))
    {
      textEdit1->append("Error: edf_set_physical_minimum()\n");
      fclose(inputfile);
      edfclose_file(hdl);
      free(buf);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }
  }

  strcpy(scratchpad, pat_dat.first_name);
  strcat(scratchpad, " ");
  strcat(scratchpad, pat_dat.last_name);
  remove_trailing_spaces(scratchpad);

  if(edf_set_patientname(hdl, scratchpad))
  {
    textEdit1->append("Error: edf_set_patientname()\n");
    fclose(inputfile);
    edfclose_file(hdl);
    free(buf);
    free(block);
    pushButton1->setEnabled(TRUE);
    return;
  }

  edf_set_startdatetime(hdl,
                        pat_dat.startdate_year,
                        pat_dat.startdate_month,
                        pat_dat.startdate_day,
                        pat_dat.starttime_hour,
                        pat_dat.starttime_minute,
                        pat_dat.starttime_second);

  edf_set_birthdate(hdl,
                    pat_dat.birthdate_year,
                    pat_dat.birthdate_month,
                    pat_dat.birthdate_day);

  if(pat_dat.sex == 1)
  {
    edf_set_gender(hdl, 1);
  }

  if(pat_dat.sex == 2)
  {
    edf_set_gender(hdl, 0);
  }

  edf_set_patientcode(hdl, pat_dat.pat_id);

  strcpy(scratchpad, pat_dat.device_model);
  strcat(scratchpad, " ");
  strcat(scratchpad, pat_dat.device_ident);

  edf_set_equipment(hdl, scratchpad);

  strcpy(scratchpad, pat_dat.manufacturer);
  strcat(scratchpad, " ");
  strcat(scratchpad, pat_dat.device_serial);

  edf_set_recording_additional(hdl, scratchpad);

  blocks = lp[0].samples / sf;

  for(i=0; i<blocks; i++)
  {
    if(encoding == 1)  // reconstitution of the data from the first differences
    {
      for(j=0; j<chns; j++)
      {
        for(k=0; k<sf; k++)
        {
          if((i == 0) && (k == 0))
          {
            abs_val_a[j] = buf[j * sf];
          }
          else
          {
            abs_val_a[j] += buf[(i * chns * sf) + (j * sf) + k];

            buf[(i * chns * sf) + (j * sf) + k] = abs_val_a[j];
          }
        }
      }
    }

    if(encoding == 2)  // reconstitution of the data from the second differences
    {
      for(j=0; j<chns; j++)
      {
        for(k=0; k<sf; k++)
        {
          if((k < 2) && (i == 0))
          {
            if(k == 0)
            {
              abs_val_a[j] = buf[j * sf];
            }
            else  // k == 1
            {
              abs_val_b[j] = buf[(j * sf) + 1];
            }
          }
          else  // Decoding of the 2nd difference data is performed using the following formula:
          {     // X(n) = D2(n) + 2*X(n-1) - X(n-2)
            var_tmp = buf[(i * chns * sf) + (j * sf) + k] + (abs_val_b[j] * 2) - abs_val_a[j];

            buf[(i * chns * sf) + (j * sf) + k] = var_tmp;

            abs_val_a[j] = abs_val_b[j];

            abs_val_b[j] = var_tmp;
          }
        }
      }
    }

    if(edf_blockwrite_digital_samples(hdl, buf + (sf * chns * i)))
    {
      textEdit1->append("A write error occurred during conversion (EDFlib).\n");
      fclose(inputfile);
      edfclose_file(hdl);
      free(buf);
      free(block);
      pushButton1->setEnabled(TRUE);
      return;
    }
  }

  edfclose_file(hdl);

  free(buf);
  free(block);

  fclose(inputfile);

  textEdit1->append("Done.\n");

  pushButton1->setEnabled(TRUE);
}


int UI_SCPECG2EDFwindow::read_data_section_zero(FILE *inputfile, char *block, long long filesize)
{
  int i;

  for(i=1; i<12; i++)
  {
    fseeko(inputfile, sp[0].file_offset + 16LL + (i * 10LL), SEEK_SET);

    if(fread(&(sp[i].section_id), 2, 1, inputfile) != 1)
    {
      textEdit1->append("A read-error occurred (20)\n");
      return(-1);
    }

    if(sp[i].section_id != i)
    {
      textEdit1->append("Error, section ID nr does not match\n");
      return(-1);
    }

    if(fread(&(sp[i].section_length), 4, 1, inputfile) != 1)
    {
      textEdit1->append("A read-error occurred (21)\n");
      return(-1);
    }

    if(fread(&(sp[i].file_offset), 4, 1, inputfile) != 1)
    {
      textEdit1->append("A read-error occurred (22)\n");
      return(-1);
    }

    if((sp[i].section_length > 0) && (sp[i].file_offset > 0LL))
    {
      sp[i].present = 1;

      sp[i].file_offset--;
    }
    else
    {
      if((sp[i].section_id == 1) || (sp[i].section_id == 3) || (sp[i].section_id == 6))
      {
        textEdit1->append("Error, a required section is missing\n");
        return(-1);
      }
    }

    if((sp[i].file_offset + sp[i].section_length) > filesize)
    {
      textEdit1->append("Error, index + length of section is > filesize\n");
      return(-1);
    }

    if(sp[i].present == 1)
    {
      if(read_section_header(i, inputfile, sp[i].file_offset, block))
      {
        return(-1);
      }
    }
  }

  return(0);
}


int UI_SCPECG2EDFwindow::check_crc(FILE *inputfile, long long offset, long long len, unsigned short crc2, char *block)
{
  int i,
      blocks;

  unsigned short crc = 0xFFFF;

  long long ll_tmp;


  blocks = len / SPCECGBUFSIZE;

  fseeko(inputfile, offset, SEEK_SET);

  for(i=0; i<blocks; i++)
  {
    if(fread(block, SPCECGBUFSIZE, 1, inputfile) != 1)
    {
      textEdit1->append("A read-error occurred (3)\n");
      return(-1);
    }

    crc = crc_ccitt((unsigned char *)block, SPCECGBUFSIZE, crc);
  }

  ll_tmp = len % SPCECGBUFSIZE;

  if(ll_tmp > 0LL)
  {
    if(fread(block, ll_tmp, 1, inputfile) != 1)
    {
      textEdit1->append("A read-error occurred (4)\n");
      return(-1);
    }

    crc = crc_ccitt((unsigned char *)block, ll_tmp, crc);
  }

  if(crc != crc2)
  {
    textEdit1->append("CRC check failed!\n");
    return(-1);
  }

  return(0);
}


int UI_SCPECG2EDFwindow::read_section_header(int n, FILE *inputfile, long long offset, char *block)
{
  char str[256];

  fseeko(inputfile, offset, SEEK_SET);

  if(fread(&(sp[n].crc), 2, 1, inputfile) != 1)
  {
    textEdit1->append("A read-error occurred (5)\n");
    return(-1);
  }

  if(fread(&(sp[n].section_id), 2, 1, inputfile) != 1)
  {
    textEdit1->append("A read-error occurred (6)\n");
    return(-1);
  }

  if(sp[n].section_id != n)
  {
    textEdit1->append("Error, section ID does not match");
    return(-1);
  }

  if(fread(&(sp[n].section_length), 4, 1, inputfile) != 1)
  {
    textEdit1->append("A read-error occurred (7)\n");
    return(-1);
  }

  if(fread(&(sp[n].section_version), 1, 1, inputfile) != 1)
  {
    textEdit1->append("A read-error occurred (8)\n");
    return(-1);
  }

  if(fread(&(sp[n].section_protocol_version), 1, 1, inputfile) != 1)
  {
    textEdit1->append("A read-error occurred (9)\n");
    return(-1);
  }

  if(fread(&(sp[n].reserved), 6, 1, inputfile) != 1)
  {
    textEdit1->append("A read-error occurred (10)\n");
    return(-1);
  }

  if(check_crc(inputfile, offset + 2, sp[n].section_length - 2LL, sp[n].crc, block))
  {
    sprintf(str, "CRC-error in section %i\n", n);
    textEdit1->append(str);
    return(-1);
  }

  sp[n].present = 1;

  return(0);
}


// unsigned short UI_SCPECG2EDFwindow::crc_ccitt(unsigned char const message[], int nbytes, unsigned short remainder)

unsigned short UI_SCPECG2EDFwindow::crc_ccitt(const unsigned char *message, int nbytes, unsigned short remainder)
{
  int byte;

  unsigned char data;

  for(byte=0; byte<nbytes; byte++)  /* Divide the message by the polynomial, a byte at a time. */
  {
    data = message[byte] ^ (remainder >> 8);

    remainder = crc_ccitt_table[data] ^ (remainder << 8);
  }

  return(remainder);  /* The final remainder is the CRC. */
}


void UI_SCPECG2EDFwindow::crc_ccitt_init(void)
{
  int dividend;

  unsigned short remainder;

  unsigned char bit;

  for(dividend=0; dividend<256; dividend++)  /* Compute the remainder of each possible dividend. */
  {
    remainder = dividend << 8;  /* Start with the dividend followed by zeros. */

    for(bit=8; bit>0; bit--)  /* Perform modulo-2 division, a bit at a time. */
    {
      if(remainder & 32768)  /* Try to divide the current data bit. */
      {
        remainder = (remainder << 1) ^ 0x1021;  /* polynomial */
      }
      else
      {
        remainder = (remainder << 1);
      }
    }

    crc_ccitt_table[dividend] = remainder;  /* Store the result into the table. */
  }
}


inline unsigned char UI_SCPECG2EDFwindow::reverse_bitorder(unsigned char byte)
{
  byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
  byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
  byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;

  return byte;
}


void UI_SCPECG2EDFwindow::lead_label_lookup(unsigned char idx, char *label)
{
  char scp_labels[256][9]={"unspec.", "I", "II", "V1", "V2", "V3", "V4", "V5", "V6", "V7",
                           "V2R", "V3R", "V4R", "V5R", "V6R", "V7R", "X", "Y", "Z", "CC5",
                           "CM5", "LA", "RA", "LL", "fl", "fE", "fC", "fA", "fM", "fF",
                           "fH", "dl", "dll", "dV1", "dV2", "dV3", "dV4", "dV5", "dV6", "dV7",
                           "dV2R", "dV3R", "dV4R", "dV5R", "dV6R", "dV7R", "dX", "dY", "dZ", "dCC5",
                           "dCM5", "dLA", "dRA", "dLL", "dfl", "dfE", "dfC", "dfA", "dfM", "dfF",
                           "dfH", "III", "aVR", "aVL", "aVF", "aVRneg", "V8", "V9", "V8R", "V9R",
                           "D", "A", "J", "Defib", "Extern", "A1", "A2", "A3", "A4", "dV8",
                           "dV9", "dV8R", "dV9R", "dD", "dA", "dj", "Chest", "V", "VR", "VL",
                           "VF", "MCL", "MCL1", "MCL2", "MCL3", "MCL4", "MCL5", "MCL6", "CC", "CC1",
                           "CC2", "CC3", "CC4", "CC6", "CC7", "CM", "CM1", "CM2", "CM3", "CM4",
                           "CM6", "dIII", "daVR", "daVL", "daVF", "daVRneg", "dChest", "dV", "dVR", "dVL",
                           "dVF", "CM7", "CH5", "CS5", "CB5", "CR5", "ML", "AB1", "AB2", "AB3",
                           "AB4", "ES", "AS", "AI", "S", "dDefib", "dExtern", "da1", "da2", "da3",
                           "da4", "dMCL1", "dMCL2", "dMCL3", "dMCL4", "dMCL5", "dMCL6", "RL", "CV5RL", "CV6LL",
                           "CV6LU", "V10", "dMCL", "dCC", "dCC1", "dCC2", "dCC3", "dCC4", "dCC5", "dCC6",
                           "dCM", "dCM1", "dCM2", "dCM3", "dCM4", "dCM6", "dCM7", "dCH5", "dCS5", "dCB5",
                           "dCR5", "dML", "dAB1", "dAB2", "dAB3", "dAB4", "dES", "dAS", "dAI", "dS",
                           "dRL", "dCV5RL", "dCV6LL", "dCV6LU", "dV10", "", "", "", "", "",
                           "", "", "", "", "", "", "", "", "", "",
                           "", "", "", "", "", "", "", "", "", "",
                           "", "", "", "", "", "", "", "", "", "",
                           "", "", "", "", "", "", "", "", "", "",
                           "", "", "", "", "", "", "", "", "", "",
                           "", "", "", "", "", "", "", "", "", "",
                           "", "", "", "", "", ""};

  if(idx > 184)
  {
    strcpy(label, "unknown");
  }
  else
  {
    strcpy(label, scp_labels[idx]);
  }
}


int UI_SCPECG2EDFwindow::get_patient_data(FILE *inputfile)
{
  char str[2048];

  int i, j, n, len, tag;

  long long offset;

  void *pntr=NULL;


  if(inputfile == NULL)
  {
    return(-1);
  }

  offset = sp[1].file_offset + 16LL;

for(int k=0; ; k++)
//  while(1)
  {
    fseeko(inputfile, offset, SEEK_SET);

    if(fread(str, 3, 1, inputfile) != 1)
    {
      textEdit1->append("A read-error occurred (80)\n");
      return(-1);
    }

    len = *((unsigned short *)(str + 1));

    if((offset + len + 3 - sp[1].file_offset) > sp[1].section_length)
    {
// printf("Found an error in section 1 (81)\n"
//                   "Conversion aborted\n"
//       "k is %i   offset is %lli   len is %i   sp[1].file_offset is %lli   sp[1].section_length is %i\n",
//       k, offset, len, sp[1].file_offset, sp[1].section_length);

      textEdit1->append("Found an error in section 1 (81)\n"
                        "Conversion aborted\n");
      return(-1);
    }

    offset += (len + 3);

    tag = *((unsigned char *)str);

    if(tag == 255)
    {
      return(0);
    }

    if(len)
    {
      n = len;

      if(n > 2046)
      {
        n = 2046;
      }

      if(fread(str, n, 1, inputfile) != 1)
      {
        textEdit1->append("A read-error occurred (82)\n");
        return(-1);
      }

      str[n] = 0;

// printf("patient data: tag is: %i     text is ->%s<-\n", tag, str);

      if(tag == 2)  // Patient ID
      {
        strncpy(pat_dat.pat_id, str, 20);

        pat_dat.pat_id[20] = 0;
        remove_trailing_spaces(pat_dat.pat_id);
        remove_leading_spaces(pat_dat.pat_id);
      }

      if(tag == 0)  // Patient last name
      {
        strncpy(pat_dat.last_name, str, 20);

        pat_dat.last_name[20] = 0;
        remove_trailing_spaces(pat_dat.last_name);
        remove_leading_spaces(pat_dat.last_name);
      }

      if(tag == 1)  // Patient first name
      {
        strncpy(pat_dat.first_name, str, 20);

        pat_dat.first_name[20] = 0;
        remove_trailing_spaces(pat_dat.first_name);
        remove_leading_spaces(pat_dat.first_name);
      }

      if(tag == 8)  // Patient sex
      {
        pat_dat.sex = str[0];
      }

      if(tag == 25)  // startdate
      {
        pntr = str;
        pat_dat.startdate_year = *((unsigned short *)pntr);
        pat_dat.startdate_month = str[2];
        pat_dat.startdate_day = str[3];
      }

      if(tag == 26)  // starttime
      {
        pat_dat.starttime_hour = str[0];
        pat_dat.starttime_minute = str[1];
        pat_dat.starttime_second = str[2];
      }

      if(tag == 5)  // birthdate
      {
        pntr = str;
        pat_dat.startdate_year = *((unsigned short *)pntr);
        pat_dat.startdate_month = str[2];
        pat_dat.startdate_day = str[3];
      }

      if(tag == 14)  // machine ID acquiring device
      {
        pat_dat.lang_code = str[16];
        strncpy(pat_dat.device_model, str + 8, 5);
        pat_dat.device_model[5] = 0;

        i = str[35];
        i += 36;

        if(i < 1500)
        {
          strncpy(pat_dat.device_serial, str + i + 1, 48);
          pat_dat.device_serial[48] = 0;
          latin1_to_ascii(pat_dat.device_serial, 48);
          remove_trailing_spaces(pat_dat.device_serial);
          remove_leading_spaces(pat_dat.device_serial);
        }

        j = 0;

        for( ; i<1500; i++)
        {
          if(str[i] == 0)
          {
            j++;

            if(j == 1)
            {
              strncpy(pat_dat.device_ident, str + i + 1, 48);
              pat_dat.device_ident[48] = 0;
              latin1_to_ascii(pat_dat.device_ident, 48);
              remove_trailing_spaces(pat_dat.device_ident);
              remove_leading_spaces(pat_dat.device_ident);
            }

            if(j == 3)
            {
              strncpy(pat_dat.manufacturer, str + i + 1, 48);
              pat_dat.manufacturer[48] = 0;
              latin1_to_ascii(pat_dat.manufacturer, 48);
              remove_trailing_spaces(pat_dat.manufacturer);
              remove_leading_spaces(pat_dat.manufacturer);

              break;
            }
          }
        }
      }
    }
  }

  return(0);
}








