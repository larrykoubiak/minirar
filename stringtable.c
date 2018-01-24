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
	printf("______________________________________________________________________________________\n");
	printf("Archive: %s Format %s (%x)\n",arc->filename,GetFormatName(arc->format),arc->format);
	printf("______________________________________________________________________________________\n");
	printf("Header:  CRC 0x%04X Type %s (0x%02X) Flags %c%c%c%c%c%c%c%c%c (0x%04X) Length 0x%04X\n",
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
	printf("______________________________________________________________________________________\n");
	printf("File:    CRC 0x%04X Type %s (0x%02X) Flags %c%c%c%c%c%c%c%c%c%c (0x%04X) Length 0x%04X\n",
		arc->fileheader.base.HeadCRC,
		GetHeaderTypeName(arc->fileheader.base.HeaderType),
		arc->fileheader.base.HeaderType,
		arc->fileheader.SplitBefore ? 'B' : '-',
		arc->fileheader.SplitAfter ? 'A' : '-',
		arc->fileheader.Password ? 'P' : '-',
		arc->fileheader.Comment ? 'C' : '-',
		arc->fileheader.Solid ? 'S' : '-',
		arc->fileheader.Large ? 'L' : '-',
		arc->fileheader.Unicode ? 'U' : '-',
		arc->fileheader.Salt ? 'T' : '-',
		arc->fileheader.Version ? 'V' : '-',
		arc->fileheader.Dir ? 'D' : '-',
		arc->fileheader.base.Flags,
		arc->fileheader.base.HeadSize);
	printf("         Packed %i Unpacked %i Host %s (0x%02X) CRC 0x%08X Time 0x%08X\nVer 0x%02X Method 0x%02X FileAttr 0x%08X Filename \"%s\"\n",
		arc->fileheader.DataSize,
		arc->fileheader.LowUnpSize,
		GetHostSystemName(arc->fileheader.HostOS),
		arc->fileheader.HostOS,
		arc->fileheader.FileCRC,
		arc->fileheader.FileTime,
		arc->fileheader.UnpVer,
		arc->fileheader.Method,
		arc->fileheader.FileAttr,
		arc->fileheader.FileName);
	printf("______________________________________________________________________________________\n");

}