#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minirar.h"
#include "stringtable.h"

void ReadSignature(Archive *arc,FILE *fp)
{
	arc->format = RARFMT_NONE;
	fread(arc->signature,SIZEOF_SIGNATURE,1,fp);
	if (arc->signature[0]==0x52) {
		if (arc->signature[1]==0x61 && arc->signature[2]==0x72 && arc->signature[3]==0x21 && arc->signature[4]==0x1a && arc->signature[5]==0x07) 
		{
			switch(arc->signature[6]) {
				case 0:
					arc->format=RARFMT15;
					break;
				case 1:
					arc->format=RARFMT50;
					break;
				case 2:
					arc->format=RARFMT_FUTURE;
					break;
				default:
					arc->format=RARFMT_NONE;
			}
		}	  
	}
}

BaseBlock ReadBaseBlock(FILE *fp)
{
	BaseBlock new_block;
	unsigned short crc,flags,size;
	unsigned char type;
	fread(&crc,2,1,fp);
	fread(&type,1,1,fp);
	fread(&flags,2,1,fp);
	fread(&size,2,1,fp);
	new_block = (BaseBlock){crc,type,flags,size};	
	return new_block;
}

void ReadMainHeader(Archive *arc,FILE *fp)
{
	BaseBlock bb;
	unsigned short highav;
	unsigned int av;
	bool v,c,l,s,n,a,p,w,f;
	bb = ReadBaseBlock(fp);
	fread(&highav,2,1,fp);
	fread(&av,4,1,fp);
	v = ((bb.Flags & MHD_VOLUME			) !=0);
	c = ((bb.Flags & MHD_COMMENT		) !=0);
	l = ((bb.Flags & MHD_LOCK			) !=0);
	s = ((bb.Flags & MHD_SOLID			) !=0);
	n = ((bb.Flags & MHD_NEWNUMBERING	) !=0);
	a = ((bb.Flags & MHD_AV          	) !=0);
	p = ((bb.Flags & MHD_PROTECT		) !=0);
	w = ((bb.Flags & MHD_PASSWORD		) !=0);	
	f = ((bb.Flags & MHD_FIRSTVOLUME	) !=0);	
	arc->mainheader = (MainHeader){bb,highav,av,v,c,l,s,n,a,p,w,f};
}

void ReadFileHeader(Archive *arc,FILE *fp)
{
	BaseBlock bb;
	unsigned short namesize;
	bb = ReadBaseBlock(fp);
	fread(&(arc->fileheader.DataSize),4,1,fp);
	fread(&(arc->fileheader.LowUnpSize),4,1,fp);
	fread(&(arc->fileheader.HostOS),1,1,fp);
	fread(&(arc->fileheader.FileCRC),4,1,fp);
	fread(&(arc->fileheader.FileTime),4,1,fp);
	fread(&(arc->fileheader.UnpVer),1,1,fp);
	fread(&(arc->fileheader.Method),1,1,fp);
	fread(&namesize,2,1,fp);
	fread(&(arc->fileheader.FileAttr),4,1,fp);
	fread(&(arc->fileheader.FileName),namesize,1,fp);
	arc->fileheader.FileName[namesize] = 0;
	arc->fileheader.SplitBefore = ((bb.Flags & LHD_SPLIT_BEFORE) != 0);
	arc->fileheader.SplitAfter = ((bb.Flags & LHD_SPLIT_AFTER) != 0);
	arc->fileheader.Password = ((bb.Flags & LHD_PASSWORD) != 0);
	arc->fileheader.Comment = ((bb.Flags & LHD_COMMENT) != 0);
	arc->fileheader.Solid = ((bb.Flags & LHD_SOLID) != 0);
	arc->fileheader.Large = ((bb.Flags & LHD_LARGE) != 0);
	arc->fileheader.Unicode = ((bb.Flags & LHD_UNICODE) != 0);
	arc->fileheader.Salt = ((bb.Flags & LHD_SALT) != 0);
	arc->fileheader.Version = ((bb.Flags & LHD_VERSION) != 0);
	arc->fileheader.Dir = ((bb.Flags & LHD_WINDOWMASK) == LHD_DIRECTORY);
}
  
void print_archive(Archive *arc)
{
	printf("Archive: %s Format %s (%x)\n",arc->filename,GetFormatName(arc->format),arc->format);
	printf("Header: CRC 0x%04X Type %s (0x%02X) Flags %c%c%c%c%c%c%c%c%c (0x%04X) Length 0x%04X\n",
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
	printf("File: CRC 0x%04X Type %s (0x%02X) Flags %c%c%c%c%c%c%c%c%c%c (0x%04X) Length 0x%04X\n",
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
	printf("      Packed %i Unpacked %i Host 0x%02X CRC 0x%08X Time 0x%08X Ver 0x%02X Method 0x%02X FileAttr 0x%08X Filename %s",
		arc->fileheader.DataSize,
		arc->fileheader.LowUnpSize,
		arc->fileheader.HostOS,
		arc->fileheader.FileCRC,
		arc->fileheader.FileTime,
		arc->fileheader.UnpVer,
		arc->fileheader.Method,
		arc->fileheader.FileAttr,
		arc->fileheader.FileName);
}

int main(int argc, char**argv)
{
	Archive *arc;
	arc = malloc(sizeof(Archive));
	if(argc>1) 
	{
		arc->filename = argv[1];
		FILE *f = fopen(argv[1],"rb");
		ReadSignature(arc,f);
		if(arc->format!=RARFMT_NONE) 
		{
			ReadMainHeader(arc,f);
			ReadFileHeader(arc,f);
			print_archive(arc);
		}
		else 
		{
			printf("File %s is not a RAR Archive\n",argv[1]);
		}
		fclose(f);
	}
	free(arc);
}