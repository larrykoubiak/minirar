#include "stringtable.h"

char *GetFormatName(RARFORMAT fmt)
{
	char *type = NULL;
	switch(fmt) {
		case RARFMT14:
			type="RARFMT14";
			break;
		case RARFMT15:
			type="RARFMT15";
			break;
		case RARFMT50:
			type="RARFMT50";
			break;
		case RARFMT_FUTURE:
			type="RARFMT_FUTURE";
		default:
			type="RARFMT_NONE";
	}
	return type;
}

char *GetHeaderTypeName(HEADER_TYPE ht)
{
	char *type = NULL;
	switch(ht) {
		case HEAD_MARK:
			type="HEAD_MARK";
			break;
		case HEAD_MAIN:
			type="HEAD_MAIN";
			break;
		case HEAD_FILE:
			type="HEAD_FILE";
			break;
		case HEAD_SERVICE:
			type="HEAD_SERVICE";
			break;
		case HEAD_CRYPT:
			type="HEAD_CRYPT";
			break;
		case HEAD_ENDARC:
			type="HEAD_ENDARC";
			break;
		case HEAD_UNKNOWN:
			type="HEAD_UNKNOWN";
			break;
		case HEAD3_MARK:
			type="HEAD3_MARK";
			break;
		case HEAD3_MAIN:
			type="HEAD3_MAIN";
			break;
		case HEAD3_FILE:
			type="HEAD3_FILE";
			break;
		case HEAD3_CMT:
			type="HEAD3_CMT";
			break;
		case HEAD3_AV:
			type="HEAD3_AV";
			break;
		case HEAD3_OLDSERVICE:
			type="HEAD3_OLDSERVICE";
			break;
		case HEAD3_PROTECT:
			type="HEAD3_PROTECT";
			break;
		case HEAD3_SIGN:
			type="HEAD3_SIGN";
			break;
		case HEAD3_SERVICE:
			type="HEAD3_SERVICE";
			break;
		case HEAD3_ENDARC:
			type="HEAD3_ENDARC";
			break;
		default:
			type="INVALID_HEADER";
			break;
	}
	return type;
}

char *GetHostSystemName(HOST_SYSTEM hs)
{
	char *type = NULL;
	switch(hs) {
		case HOST_MSDOS:
			type="HOST_MSDOS";
			break;
		case HOST_OS2:
			type="HOST_OS2";
			break;
		case HOST_WIN32:
			type="HOST_WIN32";
			break;
		case HOST_UNIX:
			type="HOST_UNIX";
			break;
		case HOST_MACOS:
			type="HOST_MACOS";
			break;
		case HOST_BEOS:
			type="HOST_BEOS";
			break;
		default:
			type="INVALID_HOST";
			break;
	}
	return type;
}
void print_archive(Archive *arc)
{
	int i;
	printf("_____________\n");
	printf("Archive: %s\nFormat %s (%x)\n",arc->filename,GetFormatName(arc->format),arc->format);
	printf("_____________\n");
	printf("Header:\n CRC 0x%04X\n Type %s (0x%02X)\n Flags %c%c%c%c%c%c%c%c%c (0x%04X)\n Length 0x%04X\n",
		arc->mainheader.base.HeadCRC,
		GetHeaderTypeName(arc->mainheader.base.HeaderType),
		arc->mainheader.base.HeaderType,
		arc->mainheader.FirstVolume ? 'F' : '-',
		arc->mainheader.Password ? 'W' : '-',
		arc->mainheader.Protect ? 'P' : '-',
		arc->mainheader.Signed ? 'A' : '-',
		arc->mainheader.NewNumbering ? 'N' : '-',
		arc->mainheader.Solid ? 'S' : '-',
		arc->mainheader.Lock ? 'L' : '-',
		arc->mainheader.Comment ? 'C' : '-',
		arc->mainheader.Volume ? 'V' : '-',
		arc->mainheader.base.Flags,
		arc->mainheader.base.HeadSize);
	for(i=0;i<arc->nbFiles;i++)
	{
		struct tm *ModifiedTime;
		char timestr[20];
		ModifiedTime = RLT2Tm(arc->fileheaders[i].ModifiedTime);
		strftime(timestr,20,"%d/%m/%Y %H:%M:%S",ModifiedTime);
		printf("_____________\n");
		printf("File %i:\n CRC 0x%04X\n Type %s (0x%02X)\n Flags %c%c%c%c%c%c%c%c%c%c (0x%04X)\n Length 0x%04X\n",i,
			arc->fileheaders[i].base.HeadCRC,
			GetHeaderTypeName(arc->fileheaders[i].base.HeaderType),
			arc->fileheaders[i].base.HeaderType,
			arc->fileheaders[i].SplitBefore ? 'B' : '-',
			arc->fileheaders[i].SplitAfter ? 'A' : '-',
			arc->fileheaders[i].Password ? 'P' : '-',
			arc->fileheaders[i].Comment ? 'C' : '-',
			arc->fileheaders[i].Solid ? 'S' : '-',
			arc->fileheaders[i].Large ? 'L' : '-',
			arc->fileheaders[i].Unicode ? 'U' : '-',
			arc->fileheaders[i].Salt ? 'T' : '-',
			arc->fileheaders[i].Version ? 'V' : '-',
			arc->fileheaders[i].Dir ? 'D' : '-',
			arc->fileheaders[i].base.Flags,
			arc->fileheaders[i].base.HeadSize);
		printf(" Packed %i\n Unpacked %i\n Host %s (0x%02X)\n CRC 0x%08X\n Time %s (0x%08X)\n Ver 0x%02X\n Method 0x%02X\n FileAttr 0x%08X\n WinSize 0x%08X\n Filename \"%s\"\n",
			arc->fileheaders[i].DataSize,
			arc->fileheaders[i].LowUnpSize,
			GetHostSystemName(arc->fileheaders[i].HostOS),
			arc->fileheaders[i].HostOS,
			arc->fileheaders[i].FileCRC,
			timestr,
			arc->fileheaders[i].FileTime,
			arc->fileheaders[i].UnpVer,
			arc->fileheaders[i].Method,
			arc->fileheaders[i].FileAttr,
			arc->fileheaders[i].WinSize,
			arc->fileheaders[i].FileName);
		printf("_____________\n");
	}

}