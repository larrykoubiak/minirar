#ifndef _MINIRAR_
#define _MINIRAR_

#define  SIZEOF_MARKHEAD3        7 // Size of RAR 4.x archive mark header.
#define  SIZEOF_MAINHEAD14       7 // Size of RAR 1.4 main archive header.
#define  SIZEOF_MAINHEAD3       13 // Size of RAR 4.x main archive header.
#define  SIZEOF_FILEHEAD14      21 // Size of RAR 1.4 file header.
#define  SIZEOF_FILEHEAD3       32 // Size of RAR 3.0 file header.

typedef enum {
	RARFMT_NONE,RARFMT14,RARFMT15,RARFMT50,RARFMT_FUTURE
} RARFORMAT;

typedef enum {
  // RAR 5.0 header types.
  HEAD_MARK=0x00, HEAD_MAIN=0x01, HEAD_FILE=0x02, HEAD_SERVICE=0x03,
  HEAD_CRYPT=0x04, HEAD_ENDARC=0x05, HEAD_UNKNOWN=0xff,
  // RAR 1.5 - 4.x header types.
  HEAD3_MARK=0x72,HEAD3_MAIN=0x73,HEAD3_FILE=0x74,HEAD3_CMT=0x75,
  HEAD3_AV=0x76,HEAD3_OLDSERVICE=0x77,HEAD3_PROTECT=0x78,HEAD3_SIGN=0x79,
  HEAD3_SERVICE=0x7a,HEAD3_ENDARC=0x7b
} HEADER_TYPE;

static RARFORMAT IsSignature(char *D,size_t Size);

#endif