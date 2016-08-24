#ifndef _MINIRAR_
#define _MINIRAR_

#define  SIZEOF_SIGNATURE		 7
#define  SIZEOF_MAINHEAD3       13 // Size of RAR 4.x main archive header.
#define  SIZEOF_FILEHEAD3       32 // Size of RAR 3.0 file header.
// Main Header flags
#define  MHD_VOLUME         0x0001U
#define  MHD_COMMENT        0x0002U
#define  MHD_LOCK           0x0004U
#define  MHD_SOLID          0x0008U
#define  MHD_NEWNUMBERING   0x0010U
#define  MHD_AV             0x0020U
#define  MHD_PROTECT        0x0040U
#define  MHD_PASSWORD       0x0080U
#define  MHD_FIRSTVOLUME    0x0100U
//File Header flags
#define  LHD_SPLIT_BEFORE   0x0001U
#define  LHD_SPLIT_AFTER    0x0002U
#define  LHD_PASSWORD       0x0004U
#define  LHD_COMMENT        0x0008U
#define  LHD_SOLID          0x0010U
#define  LHD_LARGE          0x0100U
#define  LHD_UNICODE        0x0200U
#define  LHD_SALT           0x0400U
#define  LHD_VERSION        0x0800U
#define  LHD_EXTTIME        0x1000U
//File Header masks
#define  LHD_WINDOWMASK     0x00e0U
#define  LHD_WINDOW64       0x0000U
#define  LHD_WINDOW128      0x0020U
#define  LHD_WINDOW256      0x0040U
#define  LHD_WINDOW512      0x0060U
#define  LHD_WINDOW1024     0x0080U
#define  LHD_WINDOW2048     0x00a0U
#define  LHD_WINDOW4096     0x00c0U
#define  LHD_DIRECTORY      0x00e0U

#include <stdio.h>
#include <stdbool.h>

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

enum HOST_SYSTEM {
  HOST_MSDOS=0,HOST_OS2=1,HOST_WIN32=2,HOST_UNIX=3,HOST_MACOS=4,
  HOST_BEOS=5,HOST_MAX
};

typedef struct 
{
  unsigned short HeadCRC;  // 'ushort' for RAR 1.5.
  HEADER_TYPE HeaderType; // 1 byte for RAR 1.5.
  unsigned short Flags;    // 'ushort' for RAR 1.5.
  unsigned short HeadSize; // 'ushort' for RAR 1.5, up to 2 MB for RAR 5.0.
} BaseBlock;

typedef struct
{
	BaseBlock base;
	unsigned short HighPosAV;
	unsigned int PosAV;
	bool Volume;		//V
	bool Comment;		//C
	bool Lock;			//L
	bool Solid;			//S
	bool NewNumbering;	//N
	bool Signed;		//A
	bool Protect;		//P
	bool Password;		//W
	bool FirstVolume ;	//F
} MainHeader;

typedef struct
{
	BaseBlock base;
	unsigned int DataSize;
	unsigned int LowUnpSize;
	char HostOS;
	unsigned int FileCRC;
	unsigned int FileTime;
	char UnpVer;
	char Method;
	unsigned int FileAttr;
	char FileName[2048];
	bool SplitBefore;	//B
	bool SplitAfter;	//A
	bool Password;     	//P
	bool Comment;      	//C
	bool Solid;        	//S
	bool Large;        	//L
	bool Unicode;      	//U
	bool Salt;         	//T
	bool Version;      	//V
	bool Dir;			//D
	
} FileHeader;

typedef struct
{
	char *filename;
	char signature[SIZEOF_SIGNATURE];
	RARFORMAT format;
	MainHeader mainheader;
	FileHeader fileheader;
} Archive;

BaseBlock ReadBaseBlock(FILE *fp);
void ReadSignature(Archive *arc,FILE *fp);
void ReadMainHeader(Archive *arc,FILE *fp);
void ReadFileHeader(Archive *arc,FILE *fp);
void print_archive(Archive *arc);
#endif