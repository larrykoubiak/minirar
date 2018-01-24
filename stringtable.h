#ifndef __MINIRAR_STRINGTABLE__
#define __MINIRAR_STRINGTABLE__
#include "rar_headers.h"

char *GetFormatName(RARFORMAT fmt);
char *GetHeaderTypeName(HEADER_TYPE ht);
char *GetHostSystemName(HOST_SYSTEM ht);
void print_archive(Archive *arc);

#endif