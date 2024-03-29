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





#include "utils.h"



  /* removes extension including the dot */
void remove_extension_from_filename(char *str)
{
  int i, len;

  len = strlen(str);

  if(len < 1)
  {
    return;
  }

  for(i=len-1; i>=0; i--)
  {
    if((str[i]=='/') || (str[i]=='\\'))
    {
      return;
    }

    if(str[i]=='.')
    {
      str[i] = 0;

      return;
    }
  }
}


  /* size is size of destination, returns length of filename */
int get_filename_from_path(char *dest, const char *src, int size)
{
  int i, len;

  if(size<1)
  {
    return(-1);
  }

  if(size<2)
  {
    dest[0] = 0;

    return(0);
  }

  len = strlen(src);

  if(len < 1)
  {
    dest[0] = 0;

    return(0);
  }

  for(i=len-1; i>=0; i--)
  {
    if((src[i]=='/') || (src[i]=='\\'))
    {
      break;
    }
  }

  i++;

  strncpy(dest, src + i, size);

  dest[size-1] = 0;

  return(strlen(dest));
}


  /* size is size of destination, returns length of directory */
  /* last character of destination is not a slash! */
int get_directory_from_path(char *dest, const char *src, int size)
{
  int i, len;

  if(size<1)
  {
    return(-1);
  }

  if(size<2)
  {
    dest[0] = 0;

    return(0);
  }

  len = strlen(src);

  if(len < 1)
  {
    dest[0] = 0;

    return(0);
  }

  for(i=len-1; i>=0; i--)
  {
    if((src[i]=='/') || (src[i]=='\\'))
    {
      break;
    }
  }

  strncpy(dest, src, size);

  if(i < size)
  {
    dest[i] = 0;
  }
  else
  {
    dest[size-1] = 0;
  }

  return(strlen(dest));
}



void convert_trailing_zeros_to_spaces(char *str)
{
  int i, j, len;

  len = strlen(str);

  for(i=0; i<len; i++)
  {
    if(str[i]=='.')
    {
      for(j=(len-1); j>=0; j--)
      {
        if((str[j]!='.')&&(str[j]!='0'))
        {
          break;
        }

        if(str[j]=='.')
        {
          str[j] = ' ';
          break;
        }

        str[j] = ' ';
      }
      break;
    }
  }
}



void remove_trailing_spaces(char *str)
{
  int i, len;

  len = strlen(str);

  for(i=(len-1); i>-1; i--)
  {
    if(str[i]!=' ')  break;
  }

  str[i+1] = 0;
}



void remove_leading_spaces(char *str)
{
  int i, diff, len;

  len = strlen(str);

  for(i=0; i<len; i++)
  {
    if(str[i] != ' ')
    {
      break;
    }
  }

  if(!i)
  {
    return;
  }

  diff = i;

  for(; i<=len; i++)
  {
    str[i - diff] = str[i];
  }
}



void remove_trailing_zeros(char *str)
{
  int i, j,
      len,
      numberfound,
      dotfound,
      decimalzerofound,
      trailingzerofound=1;

  while(trailingzerofound)
  {
    numberfound = 0;
    dotfound = 0;
    decimalzerofound = 0;
    trailingzerofound = 0;

    len = strlen(str);

    for(i=0; i<len; i++)
    {
      if((str[i] < '0') || (str[i] > '9'))
      {
        if(decimalzerofound)
        {
          if(str[i-decimalzerofound-1] == '.')
          {
            decimalzerofound++;
          }

          for(j=i; j<(len+1); j++)
          {
            str[j-decimalzerofound] = str[j];
          }

          trailingzerofound = 1;

          break;
        }

        if(str[i] != '.')
        {
          numberfound = 0;
          dotfound = 0;
          decimalzerofound = 0;
        }
      }
      else
      {
        numberfound = 1;

        if(str[i] > '0')
        {
          decimalzerofound = 0;
        }
      }

      if((str[i] == '.') && numberfound)
      {
        dotfound = 1;
      }

      if((str[i] == '0') && dotfound)
      {
        decimalzerofound++;
      }
    }
  }

  if(decimalzerofound)
  {
    if(str[i-decimalzerofound-1] == '.')
    {
      decimalzerofound++;
    }

    for(j=i; j<(len+1); j++)
    {
      str[j-decimalzerofound] = str[j];
    }
  }

  if(len > 1)
  {
    if(!((str[len - 2] < '0') || (str[i] > '9')))
    {
       if(str[len - 1] == '.')
       {
         str[len - 1] = 0;
       }
    }
  }
}



void utf8_to_latin1(char *utf8_str)
{
  int i, j, len;

  unsigned char *str;


  str = (unsigned char *)utf8_str;

  len = strlen(utf8_str);

  if(!len)
  {
    return;
  }

  j = 0;

  for(i=0; i<len; i++)
  {
    if((str[i] < 32) || ((str[i] > 127) && (str[i] < 192)))
    {
      str[j++] = '.';

      continue;
    }

    if(str[i] > 223)
    {
      str[j++] = 0;

      return;  /* can only decode Latin-1 ! */
    }

    if((str[i] & 224) == 192)  /* found a two-byte sequence containing Latin-1, Greek, Cyrillic, Coptic, Armenian, Hebrew, etc. characters */
    {
      if((i + 1) == len)
      {
        str[j++] = 0;

        return;
      }

      if((str[i] & 252) != 192) /* it's not a Latin-1 character */
      {
        str[j++] = '.';

        i++;

        continue;
      }

      if((str[i + 1] & 192) != 128) /* UTF-8 violation error */
      {
        str[j++] = 0;

        return;
      }

      str[j] = str[i] << 6;
      str[j] += (str[i + 1] & 63);

      i++;
      j++;

      continue;
    }

    str[j++] = str[i];
  }

  if(j<len)
  {
    str[j] = 0;
  }
}



void latin1_to_utf8(char *latin1_str, int len)
{
  int i, j;

  unsigned char *str, tmp_str[1024];


  str = (unsigned char *)latin1_str;

  j = 0;

  for(i=0; i<len; i++)
  {
    tmp_str[j] = str[i];

    if(str[i]==0)  break;

    if(str[i]<32) tmp_str[j] = '.';

    if((str[i]>126)&&(str[i]<160))  tmp_str[j] = '.';

    if(str[i]>159)
    {
      if((len-j)<2)
      {
        tmp_str[j] = ' ';
      }
      else
      {
        tmp_str[j] = 192 + (str[i]>>6);
        j++;
        tmp_str[j] = 128 + (str[i]&63);
      }
    }

    j++;

    if(j>=len)  break;
  }

  for(i=0; i<len; i++)
  {
    str[i] = tmp_str[i];

    if(str[i]==0)  return;
  }
}



void latin1_to_ascii(char *str, int len)
{
  int i, value;

  for(i=0; i<len; i++)
  {
    value = *((unsigned char *)(str + i));

    if((value>31)&&(value<127))
    {
      continue;
    }

    switch(value)
    {
      case 128 : str[i] = 'E';  break;

      case 130 : str[i] = ',';  break;

      case 131 : str[i] = 'F';  break;

      case 132 : str[i] = '\"';  break;

      case 133 : str[i] = '.';  break;

      case 134 : str[i] = '+';  break;

      case 135 : str[i] = '+';  break;

      case 136 : str[i] = '^';  break;

      case 137 : str[i] = 'm';  break;

      case 138 : str[i] = 'S';  break;

      case 139 : str[i] = '<';  break;

      case 140 : str[i] = 'E';  break;

      case 142 : str[i] = 'Z';  break;

      case 145 : str[i] = '`';  break;

      case 146 : str[i] = '\'';  break;

      case 147 : str[i] = '\"';  break;

      case 148 : str[i] = '\"';  break;

      case 149 : str[i] = '.';  break;

      case 150 : str[i] = '-';  break;

      case 151 : str[i] = '-';  break;

      case 152 : str[i] = '~';  break;

      case 154 : str[i] = 's';  break;

      case 155 : str[i] = '>';  break;

      case 156 : str[i] = 'e';  break;

      case 158 : str[i] = 'z';  break;

      case 159 : str[i] = 'Y';  break;

      case 171 : str[i] = '<';  break;

      case 180 : str[i] = '\'';  break;

      case 181 : str[i] = 'u';  break;

      case 187 : str[i] = '>';  break;

      case 191 : str[i] = '\?';  break;

      case 192 : str[i] = 'A';  break;

      case 193 : str[i] = 'A';  break;

      case 194 : str[i] = 'A';  break;

      case 195 : str[i] = 'A';  break;

      case 196 : str[i] = 'A';  break;

      case 197 : str[i] = 'A';  break;

      case 198 : str[i] = 'E';  break;

      case 199 : str[i] = 'C';  break;

      case 200 : str[i] = 'E';  break;

      case 201 : str[i] = 'E';  break;

      case 202 : str[i] = 'E';  break;

      case 203 : str[i] = 'E';  break;

      case 204 : str[i] = 'I';  break;

      case 205 : str[i] = 'I';  break;

      case 206 : str[i] = 'I';  break;

      case 207 : str[i] = 'I';  break;

      case 208 : str[i] = 'D';  break;

      case 209 : str[i] = 'N';  break;

      case 210 : str[i] = 'O';  break;

      case 211 : str[i] = 'O';  break;

      case 212 : str[i] = 'O';  break;

      case 213 : str[i] = 'O';  break;

      case 214 : str[i] = 'O';  break;

      case 215 : str[i] = 'x';  break;

      case 216 : str[i] = 'O';  break;

      case 217 : str[i] = 'U';  break;

      case 218 : str[i] = 'U';  break;

      case 219 : str[i] = 'U';  break;

      case 220 : str[i] = 'U';  break;

      case 221 : str[i] = 'Y';  break;

      case 222 : str[i] = 'I';  break;

      case 223 : str[i] = 's';  break;

      case 224 : str[i] = 'a';  break;

      case 225 : str[i] = 'a';  break;

      case 226 : str[i] = 'a';  break;

      case 227 : str[i] = 'a';  break;

      case 228 : str[i] = 'a';  break;

      case 229 : str[i] = 'a';  break;

      case 230 : str[i] = 'e';  break;

      case 231 : str[i] = 'c';  break;

      case 232 : str[i] = 'e';  break;

      case 233 : str[i] = 'e';  break;

      case 234 : str[i] = 'e';  break;

      case 235 : str[i] = 'e';  break;

      case 236 : str[i] = 'i';  break;

      case 237 : str[i] = 'i';  break;

      case 238 : str[i] = 'i';  break;

      case 239 : str[i] = 'i';  break;

      case 240 : str[i] = 'd';  break;

      case 241 : str[i] = 'n';  break;

      case 242 : str[i] = 'o';  break;

      case 243 : str[i] = 'o';  break;

      case 244 : str[i] = 'o';  break;

      case 245 : str[i] = 'o';  break;

      case 246 : str[i] = 'o';  break;

      case 247 : str[i] = '-';  break;

      case 248 : str[i] = '0';  break;

      case 249 : str[i] = 'u';  break;

      case 250 : str[i] = 'u';  break;

      case 251 : str[i] = 'u';  break;

      case 252 : str[i] = 'u';  break;

      case 253 : str[i] = 'y';  break;

      case 254 : str[i] = 't';  break;

      case 255 : str[i] = 'y';  break;

      default  : str[i] = ' ';  break;
    }
  }
}


int antoi(const char *input_str, int len)
{
  char str[1024];

  strncpy(str, input_str, len);

  str[len] = 0;

  return(atoi_nonlocalized(str));
}


/* minimum is the minimum digits that will be printed (minus sign not included), leading zero's will be added if necessary */
/* if sign is zero, only negative numbers will have the sign '-' character */
/* if sign is one, the sign '+' or '-' character will always be printed */
/* returns the amount of characters printed */
int fprint_int_number_nonlocalized(FILE *file, int q, int minimum, int sign)
{
  int flag=0, z, i, j=0, base = 1000000000;

  if(minimum < 0)
  {
    minimum = 0;
  }

  if(minimum > 9)
  {
    flag = 1;
  }

  if(q < 0)
  {
    fputc('-', file);

    j++;

    q = -q;
  }
  else
  {
    if(sign)
    {
      fputc('+', file);

      j++;
    }
  }

  for(i=10; i; i--)
  {
    if(minimum == i)
    {
      flag = 1;
    }

    z = q / base;

    q %= base;

    if(z || flag)
    {
      fputc('0' + z, file);

      j++;

      flag = 1;
    }

    base /= 10;
  }

  if(!flag)
  {
    fputc('0', file);

    j++;
  }

  return(j);
}


/* minimum is the minimum digits that will be printed (minus sign not included), leading zero's will be added if necessary */
/* if sign is zero, only negative numbers will have the sign '-' character */
/* if sign is one, the sign '+' or '-' character will always be printed */
/* returns the amount of characters printed */
int fprint_ll_number_nonlocalized(FILE *file, long long q, int minimum, int sign)
{
  int flag=0, z, i, j=0;

  long long base = 1000000000000000000LL;

  if(minimum < 0)
  {
    minimum = 0;
  }

  if(minimum > 18)
  {
    flag = 1;
  }

  if(q < 0LL)
  {
    fputc('-', file);

    j++;

    q = -q;
  }
  else
  {
    if(sign)
    {
      fputc('+', file);

      j++;
    }
  }

  for(i=19; i; i--)
  {
    if(minimum == i)
    {
      flag = 1;
    }

    z = q / base;

    q %= base;

    if(z || flag)
    {
      fputc('0' + z, file);

      j++;

      flag = 1;
    }

    base /= 10LL;
  }

  if(!flag)
  {
    fputc('0', file);

    j++;
  }

  return(j);
}


/* minimum is the minimum digits that will be printed (minus sign not included), leading zero's will be added if necessary */
/* if sign is zero, only negative numbers will have the sign '-' character */
/* if sign is one, the sign '+' or '-' character will always be printed */
/* returns the amount of characters printed */
int sprint_int_number_nonlocalized(char *str, int q, int minimum, int sign)
{
  int flag=0, z, i, j=0, base = 1000000000;

  if(minimum < 0)
  {
    minimum = 0;
  }

  if(minimum > 9)
  {
    flag = 1;
  }

  if(q < 0)
  {
    str[j++] = '-';

    q = -q;
  }
  else
  {
    if(sign)
    {
      str[j++] = '+';
    }
  }

  for(i=10; i; i--)
  {
    if(minimum == i)
    {
      flag = 1;
    }

    z = q / base;

    q %= base;

    if(z || flag)
    {
      str[j++] = '0' + z;

      flag = 1;
    }

    base /= 10;
  }

  if(!flag)
  {
    str[j++] = '0';
  }

  str[j] = 0;

  return(j);
}


/* minimum is the minimum digits that will be printed (minus sign not included), leading zero's will be added if necessary */
/* if sign is zero, only negative numbers will have the sign '-' character */
/* if sign is one, the sign '+' or '-' character will always be printed */
/* returns the amount of characters printed */
int sprint_ll_number_nonlocalized(char *str, long long q, int minimum, int sign)
{
  int flag=0, z, i, j=0;

  long long base = 1000000000000000000LL;

  if(minimum < 0)
  {
    minimum = 0;
  }

  if(minimum > 18)
  {
    flag = 1;
  }

  if(q < 0LL)
  {
    str[j++] = '-';

    q = -q;
  }
  else
  {
    if(sign)
    {
      str[j++] = '+';
    }
  }

  for(i=19; i; i--)
  {
    if(minimum == i)
    {
      flag = 1;
    }

    z = q / base;

    q %= base;

    if(z || flag)
    {
      str[j++] = '0' + z;

      flag = 1;
    }

    base /= 10LL;
  }

  if(!flag)
  {
    str[j++] = '0';
  }

  str[j] = 0;

  return(j);
}


int sprint_number_nonlocalized(char *str, double nr)
{
  int flag=0, z, i, j=0, q, base = 1000000000;

  double var;

  q = (int)nr;

  var = nr - q;

  if(nr < 0.0)
  {
    str[j++] = '-';

    if(q < 0)
    {
      q = -q;
    }
  }

  for(i=10; i; i--)
  {
    z = q / base;

    q %= base;

    if(z || flag)
    {
      str[j++] = '0' + z;

      flag = 1;
    }

    base /= 10;
  }

  if(!flag)
  {
    str[j++] = '0';
  }

  base = 100000000;

  var *= (base * 10);

  q = (int)var;

  if(q < 0)
  {
    q = -q;
  }

  if(!q)
  {
    str[j] = 0;

    return(j);
  }

  str[j++] = '.';

  for(i=9; i; i--)
  {
    z = q / base;

    q %= base;

    str[j++] = '0' + z;

    base /= 10;
  }

  str[j] = 0;

  j--;

  for(; j>0; j--)
  {
    if(str[j] == '0')
    {
      str[j] = 0;
    }
    else
    {
      j++;

      break;
    }
  }

  return(j);
}


double atof_nonlocalized(const char *str)
{
  int i=0, dot_pos=-1, decimals=0, sign=1;

  double value, value2=0.0;


  value = atoi_nonlocalized(str);

  while(str[i] == ' ')
  {
    i++;
  }

  if((str[i] == '+') || (str[i] == '-'))
  {
    if(str[i] == '-')
    {
      sign = -1;
    }

    i++;
  }

  for(; ; i++)
  {
    if(str[i] == 0)
    {
      break;
    }

    if(((str[i] < '0') || (str[i] > '9')) && (str[i] != '.'))
    {
      break;
    }

    if(dot_pos >= 0)
    {
      if((str[i] >= '0') && (str[i] <= '9'))
      {
        decimals++;
      }
      else
      {
        break;
      }
    }

    if(str[i] == '.')
    {
      if(dot_pos < 0)
      {
        dot_pos = i;
      }
    }
  }

  if(decimals)
  {
    value2 = atoi_nonlocalized(str + dot_pos + 1) * sign;

    i = 1;

    while(decimals--)
    {
      i *= 10;
    }

    value2 /= i;
  }

  return(value + value2);
}


int atoi_nonlocalized(const char *str)
{
  int i=0, value=0, sign=1;

  while(str[i] == ' ')
  {
    i++;
  }

  if((str[i] == '+') || (str[i] == '-'))
  {
    if(str[i] == '-')
    {
      sign = -1;
    }

    i++;
  }

  for( ; ; i++)
  {
    if(str[i] == 0)
    {
      break;
    }

    if((str[i] < '0') || (str[i] > '9'))
    {
      break;
    }

    value *= 10;

    value += (str[i] - '0');
  }

  return(value * sign);
}


long long atoll_x(const char *str, int dimension)
{
  int i,
      radix,
      negative=0;

  long long value=0LL;

  while(*str==' ')
  {
    str++;
  }

  if(*str=='-')
  {
    negative = 1;
    str++;
  }
  else
  {
    if(*str=='+')
    {
      str++;
    }
  }

  for(i=0; ; i++)
  {
    if(str[i]=='.')
    {
      str += (i + 1);

      break;
    }

    if((str[i]<'0') || (str[i]>'9'))
    {
      if(negative)
      {
        return(value * dimension * -1LL);
      }
      else
      {
        return(value * dimension);
      }
    }

    value *= 10LL;

    value += str[i] - '0';
  }

  radix = 1;

  for(i=0; radix<dimension; i++)
  {
    if((str[i]<'0') || (str[i]>'9'))
    {
      break;
    }

    radix *= 10;

    value *= 10LL;

    value += str[i] - '0';
  }

  if(negative)
  {
    return(value * (dimension / radix) * -1LL);
  }
  else
  {
    return(value * (dimension / radix));
  }
}



int is_integer_number(char *str)
{
  int i=0, l, hasspace = 0, hassign=0, digit=0;

  l = strlen(str);

  if(!l)  return(1);

  if((str[0]=='+')||(str[0]=='-'))
  {
    hassign++;
    i++;
  }

  for(; i<l; i++)
  {
    if(str[i]==' ')
    {
      if(!digit)
      {
        return(1);
      }
      hasspace++;
    }
    else
    {
      if((str[i]<48)||(str[i]>57))
      {
        return(1);
      }
      else
      {
        if(hasspace)
        {
          return(1);
        }
        digit++;
      }
    }
  }

  if(digit)  return(0);
  else  return(1);
}





int is_number(char *str)
{
  int i=0, l, hasspace = 0, hassign=0, digit=0, hasdot=0, hasexp=0;

  l = strlen(str);

  if(!l)  return(1);

  if((str[0]=='+')||(str[0]=='-'))
  {
    hassign++;
    i++;
  }

  for(; i<l; i++)
  {
    if((str[i]=='e')||(str[i]=='E'))
    {
      if((!digit)||hasexp)
      {
        return(1);
      }
      hasexp++;
      hassign = 0;
      digit = 0;

      break;
    }

    if(str[i]==' ')
    {
      if(!digit)
      {
        return(1);
      }
      hasspace++;
    }
    else
    {
      if(((str[i]<48)||(str[i]>57))&&str[i]!='.')
      {
        return(1);
      }
      else
      {
        if(hasspace)
        {
          return(1);
        }
        if(str[i]=='.')
        {
          if(hasdot)  return(1);
          hasdot++;
        }
        else
        {
          digit++;
        }
      }
    }
  }

  if(hasexp)
  {
    if(++i==l)
    {
      return(1);
    }

    if((str[i]=='+')||(str[i]=='-'))
    {
      hassign++;
      i++;
    }

    for(; i<l; i++)
    {
      if(str[i]==' ')
      {
        if(!digit)
        {
          return(1);
        }
        hasspace++;
      }
      else
      {
        if((str[i]<48)||(str[i]>57))
        {
          return(1);
        }
        else
        {
          if(hasspace)
          {
            return(1);
          }

          digit++;
        }
      }
    }
  }

  if(digit)  return(0);
  else  return(1);
}


void strntolower(char *s, int n)
{
  int i;

  for(i=0; i<n; i++)
  {
    s[i] = tolower(s[i]);
  }
}


int round_125_cat(double value)
{
  if(value < 0)  value *= -1;

  if(value < 0.000001)  return(10);

  while(value > 1000)  value /=10;

  while(value < 100)  value *=10;

  if(value > 670)
  {
    return(10);
  }
  else if(value > 300)
    {
      return(50);
    }
    else if(value > 135)
      {
        return(20);
      }
      else
      {
        return(10);
      }

  return(10);
}










