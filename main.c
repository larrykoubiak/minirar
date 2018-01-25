#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stringtable.h"

Archive *read_archive(char *path)
{
	Archive *arc;
	int nbFiles;
	arc = malloc(sizeof(Archive));
	arc->filename = path;
	FILE *f = fopen(path,"rb");
	ReadSignature(arc,f);
	if(arc->format!=RARFMT_NONE) 
	{
		arc->fileheaders = malloc(sizeof(FileHeader) * 256);
		nbFiles = 0;
		BaseBlock bb;
		bb = ReadBaseBlock(f);
		while(bb.HeaderType != HEAD3_ENDARC)
		{
			switch(bb.HeaderType)
			{
				case HEAD3_MAIN:
					ReadMainHeader(arc,f,bb);
					break;
				case HEAD3_FILE:
					ReadFileHeader(arc,f,bb,nbFiles);
					nbFiles++;
					break;
				default:
					break;
			}
			bb = ReadBaseBlock(f);
		}
		arc->nbFiles = nbFiles;
	}
	else
	{
		arc = NULL;
	}
	fclose(f);
	return arc;
}

void free_archive(Archive *arc)
{
	int i;
	if(arc)
	{
		if(arc->fileheaders)
		{
			for(i=0;i<arc->nbFiles;i++)
			{
				if(arc->fileheaders[i].FileName)
					free(arc->fileheaders[i].FileName);
				if(arc->fileheaders[i].FileData)
					free(arc->fileheaders[i].FileData);
			}
			free(arc->fileheaders);
		}
		free(arc);
	}
}

int main(int argc, char**argv)
{
	Archive *arc = NULL;
	if(argc>1) 
	{
		arc = read_archive(argv[1]);
		if(arc)
			print_archive(arc);
		else 
			printf("File %s is not a RAR Archive\n",argv[1]);
		free_archive(arc);
	}
}