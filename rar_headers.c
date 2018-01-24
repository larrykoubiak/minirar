#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rar_headers.h"
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
	v = ((bb.Flags & MHD_VOLUME			) !=0);
	c = ((bb.Flags & MHD_COMMENT		) !=0);
	l = ((bb.Flags & MHD_LOCK			) !=0);
	s = ((bb.Flags & MHD_SOLID			) !=0);
	n = ((bb.Flags & MHD_NEWNUMBERING	) !=0);
	a = ((bb.Flags & MHD_AV          	) !=0);
	p = ((bb.Flags & MHD_PROTECT		) !=0);
	w = ((bb.Flags & MHD_PASSWORD		) !=0);
	f = ((bb.Flags & MHD_FIRSTVOLUME	) !=0);
	fread(&highav,2,1,fp);
	fread(&av,4,1,fp);
	arc->mainheader = (MainHeader){bb,highav,av,v,c,l,s,n,a,p,w,f};
}

void ReadFileHeader(Archive *arc,FILE *fp)
{
	BaseBlock bb;
	unsigned char uv,m;
	char fn[2048];
	unsigned short ns;
	unsigned int ds,lus,fc,ft,fa;
	HOST_SYSTEM hs;
	bool b,a,p,c,s,l,u,t,v,d;
	bb = ReadBaseBlock(fp);
	arc->fileheader.base = bb;
	b = ((bb.Flags & LHD_SPLIT_BEFORE	) != 0);
	a = ((bb.Flags & LHD_SPLIT_AFTER	) != 0);
	p = ((bb.Flags & LHD_PASSWORD		) != 0);
	c = ((bb.Flags & LHD_COMMENT		) != 0);
	s = ((bb.Flags & LHD_SOLID			) != 0);
	l = ((bb.Flags & LHD_LARGE			) != 0);
	u = ((bb.Flags & LHD_UNICODE		) != 0);
	t = ((bb.Flags & LHD_SALT 			) != 0);
	v = ((bb.Flags & LHD_VERSION		) != 0);
	d = ((bb.Flags & LHD_WINDOWMASK		) == LHD_DIRECTORY);
	fread(&ds,4,1,fp);
	fread(&lus,4,1,fp);
	fread(&hs,1,1,fp);
	fread(&fc,4,1,fp);
	fread(&ft,4,1,fp);
	fread(&uv,1,1,fp);
	fread(&m,1,1,fp);
	fread(&ns,2,1,fp);
	fread(&fa,4,1,fp);
	fread(&fn,ns,1,fp);
	fn[ns] = 0;
	arc->fileheader = (FileHeader){bb,ds,lus,hs,fc,ft,uv,m,fa,fn,b,a,p,c,s,l,u,t,v,d};
}