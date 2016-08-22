#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minirar.h"

RARFORMAT IsSignature(char *D,size_t Size)
{
  RARFORMAT Type=RARFMT_NONE;
  if (Size>=1 && D[0]==0x52)
    if (Size>=4 && D[1]==0x45 && D[2]==0x7e && D[3]==0x5e)
      Type=RARFMT14;
    else
      if (Size>=7 && D[1]==0x61 && D[2]==0x72 && D[3]==0x21 && D[4]==0x1a && D[5]==0x07)
      {
        if (D[6]==0)
          Type=RARFMT15;
        else
          if (D[6]==1)
            Type=RARFMT50;
          else
            if (D[6]==2)
              Type=RARFMT_FUTURE;
      }
  return Type;
}

int main(int argc, char**argv)
{
	char sig[7];
	char *type;
	RARFORMAT fmt;
	if(argc>1)
	{
		FILE *f = fopen(argv[1],"rb");
		fread(sig,7,1,f);
		fmt = IsSignature(sig,7);
		if(fmt!=RARFMT_NONE)
		{
			switch(fmt)
			{
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
			printf("File %s is a RAR Archive using the format %s\n",argv[1],type);
		}
		else
		{
			printf("File %s is not a RAR Archive\n",argv[1]);
		}
		fclose(f);		
	}
}