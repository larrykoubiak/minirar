#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stringtable.h"

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