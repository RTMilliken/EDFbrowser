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


#include "xml.h"




int next_tag(int, struct xml_handle *);
int process_tag(const char *, struct xml_handle *);
int xml_attribute(const char *, const char *, char *, int);


int xml_character_encoding_type(struct xml_handle *handle_p)
{
  if(handle_p==NULL)  return(-1);

  return(handle_p->encoding);
}


void xml_fwrite_encode_entity(FILE *file, const char *str)
{
  for(; *str; str++)
  {
    switch(*str)
    {
      case '<'  : fwrite("&lt;", 4, 1, file);
                  break;

      case '>'  : fwrite("&gt;", 4, 1, file);
                  break;

      case '&'  : fwrite("&amp;", 5, 1, file);
                  break;

      case '\'' : fwrite("&apos;", 6, 1, file);
                  break;

      case '\"' : fwrite("&quot;", 6, 1, file);
                  break;

      default   : fputc(*str, file);
                  break;
    }
  }
}


void xml_fnwrite_encode_entity(FILE *file, const char *str, int n)
{
  int i;


  for(i=0; i<n; i++)
  {
    switch(str[i])
    {
      case '<'  : fwrite("&lt;", 4, 1, file);
                  break;

      case '>'  : fwrite("&gt;", 4, 1, file);
                  break;

      case '&'  : fwrite("&amp;", 5, 1, file);
                  break;

      case '\'' : fwrite("&apos;", 6, 1, file);
                  break;

      case '\"' : fwrite("&quot;", 6, 1, file);
                  break;

      default   : fputc(str[i], file);
                  break;
    }
  }
}


void xml_strcpy_encode_entity(char *dest, const char *src)
{
  *dest = 0;

  for(; *src; src++)
  {
    switch(*src)
    {
      case '<'  : strcpy(dest, "&lt;");
                  dest += 4;
                  break;

      case '>'  : strcpy(dest, "&gt;");
                  dest += 4;
                  break;

      case '&'  : strcpy(dest, "&amp;");
                  dest += 5;
                  break;

      case '\'' : strcpy(dest, "&apos;");
                  dest += 6;
                  break;

      case '\"' : strcpy(dest, "&quot;");
                  dest += 6;
                  break;

      default   : *dest = *src;
                  *(++dest) = 0;
                  break;
    }
  }
}


int xml_strncpy_encode_entity(char *dest, const char *src, int n)
{
  int i=0;


  if(n < 1)
  {
    return(0);
  }

  *dest = 0;

  for(; *src; src++)
  {
    if((n - i) < 7)
    {
      break;
    }

    if(*src == '<')
    {
      strcpy(dest + i, "&lt;");
      i += 4;
      continue;
    }

    if(*src == '>')
    {
      strcpy(dest + i, "&gt;");
      i += 4;
      continue;
    }

    if(*src == '&')
    {
      strcpy(dest + i, "&amp;");
      i += 5;
      continue;
    }

    if(*src == '\'')
    {
      strcpy(dest + i, "&apos;");
      i += 6;
      continue;
    }

    if(*src == '\"')
    {
      strcpy(dest + i, "&quot;");
      i += 6;
      continue;
    }

    dest[i++] = *src;
  }

  dest[i] = 0;

  return(i);
}


void xml_strcpy_decode_entity(char *dest, const char *src)
{
  int i, len, p=0;

  len = strlen(src);

  for(i=0; i<len; i++)
  {
    if((i + p) < (len - 4))
    {
      if(!strncmp(src + i + p, "&lt;", 4))
      {
        dest[i] = '<';
        p += 3;
        continue;
      }

      if(!strncmp(src + i + p, "&gt;", 4))
      {
        dest[i] = '>';
        p += 3;
        continue;
      }
    }

    if((i + p) < (len - 5))
    {
      if(!strncmp(src + i + p, "&amp;", 5))
      {
        dest[i] = '&';
        p += 4;
        continue;
      }
    }

    if((i + p) < (len - 6))
    {
      if(!strncmp(src + i + p, "&apos;", 6))
      {
        dest[i] = '\'';
        p += 5;
        continue;
      }

      if(!strncmp(src + i + p, "&quot;", 6))
      {
        dest[i] = '\"';
        p += 5;
        continue;
      }
    }

    dest[i] = src[i + p];

    if(dest[i] == 0)  break;
  }
}


int xml_strncpy_decode_entity(char *dest, const char *src, int n)
{
  int i, len, p=0;

  if(n < 1)
  {
    return(0);
  }

  len = strlen(src);

  for(i=0; i<len; i++)
  {
    if((i + p) < (len - 4))
    {
      if(!strncmp(src + i + p, "&lt;", 4))
      {
        dest[i] = '<';
        p += 3;
        continue;
      }

      if(!strncmp(src + i + p, "&gt;", 4))
      {
        dest[i] = '>';
        p += 3;
        continue;
      }
    }

    if((i + p) < (len - 5))
    {
      if(!strncmp(src + i + p, "&amp;", 5))
      {
        dest[i] = '&';
        p += 4;
        continue;
      }
    }

    if((i + p) < (len - 6))
    {
      if(!strncmp(src + i + p, "&apos;", 6))
      {
        dest[i] = '\'';
        p += 5;
        continue;
      }

      if(!strncmp(src + i + p, "&quot;", 6))
      {
        dest[i] = '\"';
        p += 5;
        continue;
      }
    }

    dest[i] = src[i + p];

    if(i >= (n - 1))
    {
      i++;

      break;
    }
  }

  return(i);
}


int xml_get_attribute_of_element(struct xml_handle *handle_p, const char *attr_name, char *str_buf, int content_len)
{
  if(handle_p == NULL)  return(1);

  while(handle_p->child_handle_p != NULL)
  {
    handle_p = handle_p->child_handle_p;
  }

  if(handle_p->attributes == NULL)  return(1);

  if(strlen(attr_name) < 1)  return(1);

  if(content_len < 1)  return(1);

  if(xml_attribute(handle_p->attributes, attr_name, str_buf, content_len) < 1)
  {
    return(1);
  }

  return(0);
}


int xml_attribute(const char *data, const char *item, char *result, int result_len)
{
  int i, j,
      data_len,
      item_len,
      quote=0;

  if(data == NULL)
  {
    return(-1);
  }

  data_len = strlen(data);
  item_len = strlen(item);

  if((data_len < 4) || (item_len >= (data_len - 4)))
  {
    return(-1);
  }

  for(i=0; i<(data_len - item_len); i++)
  {
    if(data[i] == '\"')
    {
      if(quote)
      {
        quote--;
      }
      else
      {
        quote++;
      }
    }

    if(quote)  continue;

    if(!strncmp(data + i, item, item_len))
    {
      i += item_len;

      if(i < (data_len - 3))
      {
        if(!strncmp(data + i, "=\"", 2))
        {
          i += 2;

          for(j=i; j<data_len; j++)
          {
            if(data[j] == '\"')
            {
              if((j - i) < result_len)
              {
                // strncpy(result, data + i, j - i);
                xml_strncpy_decode_entity(result, data + i, j - i);
                result[j - i] = 0;

                return(j - i);
              }
            }
          }
        }
      }

      return(-1);
    }
  }

  return(-1);
}


struct xml_handle * xml_get_handle(const char *filename)
{
  char scratchpad[512];

  struct xml_handle *handle_p;

  handle_p = (struct xml_handle *)calloc(1, sizeof(struct xml_handle));
  if(handle_p==NULL)  return(NULL);

  handle_p->file = fopen(filename, "rb");
  if(handle_p->file==NULL)
  {
    xml_close(handle_p);
    return(NULL);
  }

  handle_p->offset = next_tag(0, handle_p);
  if(handle_p->offset==-1)
  {
    xml_close(handle_p);
    return(NULL);
  }

  if(handle_p->tag_search_result==NULL)
  {
    xml_close(handle_p);
    return(NULL);
  }

  if(strlen(handle_p->tag_search_result) < 3)
  {
    xml_close(handle_p);
    return(NULL);
  }

  if((handle_p->tag_search_result[0] == '?')  && (handle_p->tag_search_result[strlen(handle_p->tag_search_result) - 1] == '?'))
  {
    if(xml_attribute(handle_p->tag_search_result, "encoding", scratchpad, 512) < 0)
    {
      handle_p->encoding = 0;  // attribute encoding not present
    }
    else
    {
      if((!strcmp(scratchpad, "ISO-8859-1")) || (!strcmp(scratchpad, "iso-8859-1")))
      {
        handle_p->encoding = 1;
      }
      else if((!strcmp(scratchpad, "UTF-8"))  || (!strcmp(scratchpad, "utf-8")))
        {
          handle_p->encoding = 2;
        }
        else
        {
          handle_p->encoding = 99999;  // unknown encoding
        }
    }
  }

  while((handle_p->tag_search_result[0]=='!') || (handle_p->tag_search_result[0]=='?'))
  {
    handle_p->offset = next_tag(handle_p->offset, handle_p);
    if(handle_p->offset==-1)
    {
      xml_close(handle_p);
      return(NULL);
    }

    if(handle_p->tag_search_result==NULL)
    {
      xml_close(handle_p);
      return(NULL);
    }
  }

  if(process_tag(handle_p->tag_search_result , handle_p))
  {
    xml_close(handle_p);
    return(NULL);
  }

  return(handle_p);
}


struct xml_handle * xml_create_handle(const char *filename, char *rootname)
{
  int len;

  struct xml_handle *handle_p;

  len = strlen(rootname);

  if(!len)  return(NULL);

  handle_p = (struct xml_handle *)calloc(1, sizeof(struct xml_handle));
  if(handle_p==NULL)  return(NULL);

  handle_p->file = fopen(filename, "wb+");
  if(handle_p->file==NULL)
  {
    free(handle_p);
    return(NULL);
  }

  fprintf(handle_p->file, "<?xml version=\"1.0\"?>\n<%s>\n</%s>\n", rootname, rootname);

  handle_p->offset = (ftell(handle_p->file) - len) - 4;

  handle_p->elementname = (char *)calloc(1, len + 1);
  if(handle_p->elementname==NULL)
  {
    fclose(handle_p->file);
    free(handle_p);
    return(NULL);
  }
  strncpy(handle_p->elementname, rootname, len);
  handle_p->elementname[len] = 0;

  return(handle_p);
}




char * xml_get_content_of_element(struct xml_handle *handle_p)
{
  int i, j, offset, len, deep=0, cdata=0;

  char *content;

  if(handle_p==NULL)  return(NULL);

  while(handle_p->child_handle_p!=NULL)
  {
    handle_p = handle_p->child_handle_p;
  }

  if(handle_p->elementname==NULL)  return(NULL);

  offset = handle_p->offset;

  len = strlen(handle_p->elementname);

  while(1)
  {
    offset = next_tag(offset, handle_p);
    if(offset == -1)
    {
      return(NULL);
    }

    if(handle_p->tag_search_result==NULL)
    {
      return(NULL);
    }

    if((handle_p->tag_search_result[0]=='!') || (handle_p->tag_search_result[0]=='?'))
    {
      continue;
    }

    if(handle_p->tag_search_result[0]=='/')
    {
      if(deep)  deep--;
      else  break;
    }
    else
    {
      deep++;
    }
  }

  offset -= (len + 3);

  if(offset<handle_p->offset)  return(NULL);

  fseek(handle_p->file, handle_p->offset, SEEK_SET);

  content = (char *)calloc(1, offset - handle_p->offset + 1);
  if(content==NULL)
  {
    return(NULL);
  }

  if(offset>handle_p->offset)
  {
    if(fread(content, offset - handle_p->offset, 1, handle_p->file) != 1)
    {
      free(content);
      return(NULL);
    }
  }

  len = offset - handle_p->offset;

  content[len] = 0;

  if(len > 11)  // cdata?
  {
    if(!(strncmp(content, "<![CDATA[", 9)))  // cdata
    {
      cdata = 1;

      len -= 9;

      for(j=0; j<len; j++)
      {
        content[j] = content[j+9];
      }

      len -= 3;

      content[len] = 0;
    }
  }

  if(cdata == 0)
  {
    for(i=0; content[i]!=0; i++)
    {
      if(content[i]=='&')
      {
        if(i <= (len - 3))
        {
          if(!strncmp(content + i + 1, "lt;", 3))
          {
            content[i] = '<';

            for(j=i+1; content[j-1]!=0; j++)
            {
              content[j] = content[j+3];
            }
          }

          if(!strncmp(content + i + 1, "gt;", 3))
          {
            content[i] = '>';

            for(j=i+1; content[j-1]!=0; j++)
            {
              content[j] = content[j+3];
            }
          }
        }

        if(i <= (len - 5))
        {
          if(!strncmp(content + i + 1, "apos;", 5))
          {
            content[i] = '\'';

            for(j=i+1; content[j-1]!=0; j++)
            {
              content[j] = content[j+5];
            }
          }

          if(!strncmp(content + i + 1, "quot;", 5))
          {
            content[i] = '\"';

            for(j=i+1; content[j-1]!=0; j++)
            {
              content[j] = content[j+5];
            }
          }
        }

        if(i <= (len - 4))
        {
          if(!strncmp(content + i + 1, "amp;", 4))
          {
            for(j=i+1; content[j-1]!=0; j++)
            {
              content[j] = content[j+4];
            }
          }
        }
      }
    }
  }

  return(content);
}




int xml_goto_next_element_with_same_name(struct xml_handle *handle_p)
{
  int len, offset, deep=0;

  if(handle_p==NULL)  return(1);

  while(handle_p->child_handle_p!=NULL)
  {
    handle_p = handle_p->child_handle_p;
  }

  if(handle_p->elementname==NULL)  return(1);

  len = strlen(handle_p->elementname);
  offset = handle_p->offset;

  while(1)
  {
    offset = next_tag(offset, handle_p);
    if(offset == -1)
    {
      return(1);
    }

    if(handle_p->tag_search_result==NULL)
    {
      return(1);
    }

    if((handle_p->tag_search_result[0]=='!') || (handle_p->tag_search_result[0]=='?'))
    {
      continue;
    }

    if(handle_p->tag_search_result[0]=='/')
    {
      if(deep)  deep--;
      else  break;
    }
    else
    {
      deep++;
    }
  }

  while(1)
  {
    offset = next_tag(offset, handle_p);
    if(offset == -1)
    {
      return(1);
    }

    if(handle_p->tag_search_result==NULL)
    {
      return(1);
    }

    if((handle_p->tag_search_result[0]=='!') || (handle_p->tag_search_result[0]=='?'))
    {
      continue;
    }

    if(handle_p->tag_search_result[0]=='/')
    {
      if(deep)  deep--;
      else  return(1);
    }
    else
    {
      if((int)strlen(handle_p->tag_search_result)>=len)
      {
        if((!strncmp(handle_p->tag_search_result, handle_p->elementname, len))&&(!deep))
        {
          if(process_tag(handle_p->tag_search_result, handle_p))
          {
            return(1);
          }

          handle_p->offset = offset;

          return(0);
        }
        else
        {
          deep++;
        }
      }
      else
      {
        deep++;
      }
    }
  }
}




int xml_goto_next_element_at_same_level(struct xml_handle *handle_p)
{
  int offset, deep=0, ts_len;

  if(handle_p==NULL)  return(1);

  while(handle_p->child_handle_p!=NULL)
  {
    handle_p = handle_p->child_handle_p;
  }

  offset = handle_p->offset;

  while(1)
  {
    offset = next_tag(offset, handle_p);
    if(offset == -1)
    {
      return(1);
    }

    if(handle_p->tag_search_result==NULL)
    {
      return(1);
    }

    if((handle_p->tag_search_result[0]=='!') || (handle_p->tag_search_result[0]=='?'))
    {
      continue;
    }

    if(handle_p->tag_search_result[0]=='/')
    {
      if(deep)  deep--;
      else  break;
    }
    else
    {
      ts_len = strlen(handle_p->tag_search_result);

      if(handle_p->tag_search_result[ts_len - 1]!='/')
      {
        deep++;
      }
    }
  }

  offset = next_tag(offset, handle_p);
  if(offset == -1)
  {
    return(1);
  }

  if(handle_p->tag_search_result==NULL)
  {
    return(1);
  }

  while((handle_p->tag_search_result[0]=='!') || (handle_p->tag_search_result[0]=='?'))
  {
    offset = next_tag(offset, handle_p);
    if(offset == -1)
    {
      return(1);
    }

    if(handle_p->tag_search_result==NULL)
    {
      return(1);
    }
  }

  if(handle_p->tag_search_result[0]=='/')
  {
    return(1);
  }

  if(process_tag(handle_p->tag_search_result, handle_p))
  {
    return(1);
  }

  handle_p->offset = offset;

  return(0);
}




int xml_goto_nth_element_inside(struct xml_handle *handle_p, const char *name, int n)
{
  int len, offset, deep=0, cnt=0, ts_len;

  struct xml_handle *new_handle_p;

  if(handle_p==NULL)  return(1);

  while(handle_p->child_handle_p!=NULL)
  {
    handle_p = handle_p->child_handle_p;
  }

  len = strlen(name);

  offset = handle_p->offset;

  while(1)
  {
    while(1)
    {
      offset = next_tag(offset, handle_p);
      if(offset == -1)
      {
        return(1);
      }

      if(handle_p->tag_search_result==NULL)
      {
        return(1);
      }

      if((handle_p->tag_search_result[0]=='!') || (handle_p->tag_search_result[0]=='?'))
      {
        continue;
      }

      if(handle_p->tag_search_result[0]=='/')
      {
        if(deep)  deep--;
        else  return(1);
      }
      else
      {
        ts_len = strlen(handle_p->tag_search_result);

        if(handle_p->tag_search_result[ts_len - 1]!='/')
        {
          deep++;
        }

        break;
      }
    }

    if((int)strlen(handle_p->tag_search_result)>=len)
    {
      if(!strncmp(handle_p->tag_search_result, name, len))
      {
        if((handle_p->tag_search_result[len]==' ')||(handle_p->tag_search_result[len]==0))
        {
          if(cnt==n)
          {
            new_handle_p = (struct xml_handle *)calloc(1, sizeof(struct xml_handle));
            if(new_handle_p==NULL)
            {
              return(1);
            }

            handle_p->child_handle_p = new_handle_p;

            new_handle_p->file = handle_p->file;
            new_handle_p->level = handle_p->level + 1;
            new_handle_p->offset = offset;
            new_handle_p->len = 0;
            new_handle_p->elementname = NULL;
            new_handle_p->attributes = NULL;
            new_handle_p->parent_handle_p = handle_p;
            new_handle_p->child_handle_p = NULL;
            new_handle_p->tag_search_result = NULL;
            new_handle_p->encoding = handle_p->encoding;

            if(process_tag(handle_p->tag_search_result, new_handle_p))
            {
              return(1);
            }

            return(0);
          }

          cnt++;
        }
      }
    }
  }

  return(1);
}



int process_tag(const char *str, struct xml_handle *handle_p)
{
  int len, i, p;

  if(handle_p==NULL)  return(1);

  len = strlen(str);
  if(!len)  return(1);

  if((str[0]==' ')||(str[0]=='>'))  return(1);

  for(i=0; i<len; i++)
  {
    if((str[i]==' ')||(str[i]=='>'))  break;
  }

  if(handle_p->elementname != NULL)  free(handle_p->elementname);

  handle_p->elementname = (char *)calloc(1, i + 1);
  if(handle_p->elementname==NULL)  return(1);
  strncpy(handle_p->elementname, str, i);
  handle_p->elementname[i] = 0;

  if(handle_p->attributes != NULL)  free(handle_p->attributes);

  if(str[i]!=' ')  return(0);

  i++;

  p = i;

  for(; i<len; i++)
  {
    if(str[i]=='>')  break;
  }

  len = i - p;

  if(len < 2)  return(0);

  handle_p->attributes = (char *)calloc(1, len + 1);
  if(handle_p->attributes==NULL)  return(1);
  strncpy(handle_p->attributes, str + p, len);
  handle_p->attributes[len] = 0;

  return(0);
}



void xml_close(struct xml_handle *handle_p)  /* delete everything and close the file */
{
  if(handle_p!=NULL)
  {
    xml_goto_root(handle_p);

    if(handle_p->file != NULL)  fclose(handle_p->file);
    if(handle_p->elementname)  free(handle_p->elementname);
    if(handle_p->attributes)  free(handle_p->attributes);
    if(handle_p->tag_search_result)  free(handle_p->tag_search_result);
    free(handle_p);
    handle_p = NULL;
  }
}



void xml_goto_root(struct xml_handle *handle_p) /* go to rootlevel and delete everything beneath */
{
  if(handle_p==NULL)  return;

  while(handle_p->child_handle_p!=NULL)
  {
    handle_p = handle_p->child_handle_p;
  }

  while(handle_p->level!=0)
  {
    if(handle_p->elementname != NULL)  free(handle_p->elementname);
    if(handle_p->attributes != NULL)  free(handle_p->attributes);
    if(handle_p->tag_search_result != NULL)  free(handle_p->tag_search_result);

    handle_p = handle_p->parent_handle_p;

    free(handle_p->child_handle_p);
  }

  handle_p->child_handle_p = NULL;
}



void xml_go_up(struct xml_handle *handle_p) /* go one level up and delete everything beneath */
{
  int level;

  if(handle_p==NULL)  return;

  while(handle_p->child_handle_p!=NULL)
  {
    handle_p = handle_p->child_handle_p;
  }

  level = handle_p->level;

  if(level==0)  return;

  level--;

  while(handle_p->level!=level)
  {
    if(handle_p->elementname != NULL)  free(handle_p->elementname);
    if(handle_p->attributes != NULL)  free(handle_p->attributes);
    if(handle_p->tag_search_result != NULL)  free(handle_p->tag_search_result);

    handle_p = handle_p->parent_handle_p;

    free(handle_p->child_handle_p);
  }

  handle_p->child_handle_p = NULL;
}




int next_tag(int offset, struct xml_handle *handle_p) /* returns offset after '>' */
{
  int temp, fp1=0, fp2=0, tagstart=0,
      tag_char_cnt=0,
      comment=0,
      cdata=0;

  char circ_buf[16];


  if(handle_p->tag_search_result!=NULL)
  {
    free(handle_p->tag_search_result);
  }

  handle_p->tag_search_result = NULL;

  fseek(handle_p->file, offset, SEEK_SET);

  while(1)
  {
    temp = fgetc(handle_p->file);
    if(temp==EOF)
    {
      return(-1);
    }

    if(tagstart)
    {
      circ_buf[tag_char_cnt % 16] = temp;

      tag_char_cnt++;

      if(tag_char_cnt == 3)
      {
        if(!strncmp(circ_buf, "!--", 3))    // start of comment
        {
          comment = 1;
        }
      }

      if(tag_char_cnt == 8)
      {
        if(!strncmp(circ_buf, "![CDATA[", 8))    // start of cdata
        {
          cdata = 1;
        }
      }
    }

    if(!comment)
    {
      if(!cdata)
      {
        if(temp=='<')
        {
          if(tagstart)
          {
            return(-1);
          }

          tagstart = 1;
          fp1 = ftell(handle_p->file);
        }

        if(temp=='>')
        {
          if(!tagstart)
          {
            return(-1);
          }

          offset = ftell(handle_p->file);
          fp2 = offset - 1;

          break;
        }
      }
      else  // cdata
      {
        if((temp=='>') && (tag_char_cnt > 10))
        {
          if((circ_buf[(tag_char_cnt - 2) % 16] == ']') && (circ_buf[(tag_char_cnt - 3) % 16] == ']'))
          {  // end of cdata
            cdata = 0;
            tagstart = 0;
            fp1 = 0;
            fp2 = 0;
            tag_char_cnt = 0;
          }
        }
      }
    }
    else  // comment
    {
      if((temp=='>') && (tag_char_cnt > 5))
      {
        if((circ_buf[(tag_char_cnt - 2) % 16] == '-') && (circ_buf[(tag_char_cnt - 3) % 16] == '-'))
        {  // end of comment
          comment = 0;
          tagstart = 0;
          fp1 = 0;
          fp2 = 0;
          tag_char_cnt = 0;
        }
      }
    }
  }

  if(!fp2)
  {
    return(-1);
  }

  fseek(handle_p->file, fp1, SEEK_SET);

  handle_p->tag_search_result = (char *)calloc(1, fp2 - fp1 + 1);
  if(handle_p->tag_search_result==NULL)
  {
    return(-1);
  }

  if(fread(handle_p->tag_search_result, fp2 - fp1, 1, handle_p->file) != 1)
  {
    free(handle_p->tag_search_result);
    handle_p->tag_search_result = NULL;
    return(-1);
  }

  handle_p->tag_search_result[fp2 - fp1] = 0;

  return(offset);
}












