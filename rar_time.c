#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rar_time.h"

void ReadExtTime(FILE *fp)
{
	unsigned short Flags;
	unsigned int rmode;
	int i,j;
	unsigned int reminder;
	char count,cb;
	fread(&Flags,2,1,fp);
	for(i=0;i<4;i++)
	{
		reminder = 0;
		rmode = Flags >> ((3-i)*4);
		if((rmode & 8) == 0)
			continue;
		count = rmode & 3;
		for(j=0;j<count;j++)
		{
			fread(&cb,1,1,fp);
			reminder |= (((unsigned int)cb) << ((j+3-count)*8));
		}
	}
}

RarLocalTime Dos2RLT(unsigned int dt)
{
	RarLocalTime lt;
	lt.Second=(dt & 0x1f)*2;
	lt.Minute=(dt>>5) & 0x3f;
	lt.Hour=(dt>>11) & 0x1f;
	lt.Day=(dt>>16) & 0x1f;
	lt.Month=(dt>>21) & 0x0f;
	lt.Year=(dt>>25)+1980;
	lt.Reminder=0;
	return lt;
}
