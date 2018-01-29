#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rar_time.h"

RarLocalTime *ReadExtTime(FILE *fp,unsigned int filetime,bool exttime)
{
	RarLocalTime *tbl;
	unsigned short Flags;
	unsigned int rmode;
	unsigned int dostime;
	int i,j;
	char count,cb;
	fread(&Flags,2,1,fp);
	tbl=malloc(sizeof(RarLocalTime)*4);
	tbl[0] = Dos2RLT(filetime);
	if(exttime)
	{
		for(i=0;i<4;i++)
		{
			tbl[i].Reminder = 0;
			rmode = Flags >> ((3-i)*4);
			if((rmode & 8) == 0)
				continue;
			if(i!=0)
			{
				fread(&dostime,4,1,fp);
				tbl[i] = Dos2RLT(dostime);
			}
			if(rmode & 4)
				tbl[i].Second++;
			count = rmode & 3;
			for(j=0;j<count;j++)
			{
				fread(&cb,1,1,fp);
				tbl[i].Reminder |= (((unsigned int)cb) << ((j+3-count)*8));
			}
		}
	}
	return tbl;
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

struct tm *RLT2Tm(RarLocalTime rlt)
{
	struct tm t;
	time_t lt;
	t.tm_sec=rlt.Second;
	t.tm_min=rlt.Minute;
	t.tm_hour=rlt.Hour;
	t.tm_mday=rlt.Day;
	t.tm_mon=rlt.Month-1;
	t.tm_year=rlt.Year-1900;
	t.tm_isdst=1;
	lt = mktime(&t);
	return localtime(&lt);
}