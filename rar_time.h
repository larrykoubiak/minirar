#ifndef _RAR_TIME_
#define _RAR_TIME_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
  unsigned int Year;
  unsigned int Month;
  unsigned int Day;
  unsigned int Hour;
  unsigned int Minute;
  unsigned int Second;
  unsigned int Reminder; // Part of time smaller than 1 second, represented in 100-nanosecond intervals.
  unsigned int wDay;
  unsigned int yDay;
}  RarLocalTime;

void ReadExtTime(FILE *fp);
#endif