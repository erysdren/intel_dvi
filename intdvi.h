
#ifndef _INTDVI_H_
#define _INTDVI_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef int8_t I8;
typedef int16_t I16;
typedef int32_t I32;

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;

#define AVL_LAST_HDR 0x7fffffff

#define VSTD_HDR_ID 0x56445649
#define VSTD_HDR_VER 1

typedef struct StdFileHdr {
	U32 FileId;
	I16 HdrSize, HdrVersion;
	U32 AnnOffset;
} StdFileHdr;

#define AVL_FILE_ID 0x41565353
#define AVL_FILE_VER 3

typedef struct AvLFile {
	U32 HdrID;
	I16 HdrSize, HdrVer, StrmGrpCnt, StrmGrpSize;
	U32 StrmGrpOffset;
	I16 StrmGrpVer, StrmSize, StrmVer, StrmCnt;
	U32 StrmOffset, HdrPoolOffset;
	I32 LabelCnt;
	U32 LabelOffset;
	I16 LabelSize, LabelVer;
	U32 VshOffset;
	U16 VshSize;
	I16 FrmVer;
	I32 FrmCnt, FrmSize;
	U32 FirstFrmOffset, EndOfFrmsOffset;
	I16 FrmHdrSize, FrmDirSize;
	U32 FrmDirOffset;
	I16 FrmDirVer, FrmsPerSec;
	U32 Flag;
	U32 FreeBlockOffset;
	U8 Patch[32];
} AvLFile;

#define AVL_STRM_ID 0x5354524d
#define AVL_STRM_VER 3

#define AVL_STRMGRP_VER 3
#define AVL_STRMGRP_END -1

typedef struct AvLStrm {
	U32 HdrID;
	U16 Type, SubType;
	I16 HdrCnt, NextStrmNum, StrmGrpNum, Pad;
	U32 Flag;
	I32 FrmSize;
	U32 FirstHdrOffset;
	U8 StrmName[16];
} AvLStrm;

#define AVL_AUD_ID 0x41554449
#define AVL_AUD_VER 5

typedef struct AvLAud {
	U32 HdrID;
	I16 HdrSize, HdrVer;
	U8 OrigFile[80];
	I32 OrigFrm;
	I16 OrigStrm, Pad;
	I32 FrmCnt;
	U32 NextHdrOffset;
	U8 Lib[16], Alg[16];
	I32 Parm1;
	I16 Parm2, Parm3, LeftVol, RightVol;
	I32 LoopOffset, StartFrm;
	U32 Flag;
	I16 Parm4, Pad2;
	I32 DCFId;
} AvLAud;

#define AVL_CIM_ID 0x43494d47
#define AVL_CIM_VER 4
#define AVL_CIM_RANDOM_STILL -1

typedef struct AvLCim {
	U32 HdrID;
	I16 HdrSize, HdrVer;
	U8 OrigFile[80];
	I32 OrigFrm;
	I16 OrigStrm, Pad;
	I32 FrmCnt;
	U32 NextHdrOffset;
	I16 XPos, YPos, XLen, YLen;
	I16 XCrop, YCrop, DropFrm, DropPhase;
	I32 StillPeriod;
	I16 BufsMin, BufsMax, DeCodeAlg, Pad2;
	I32 DCFId;
} AvLCim;

#define AVL_ULAY_ID 0x554e4452
#define AVL_ULAY_VER 4

typedef struct AvLUlay {
	U32 HdrID;
	I16 HdrSize, HdrVer;
	U8 OrigFile[80];
	I32 OrigFrm;
	I16 OrigStrm, Pad;
	I32 FrmCnt;
	U32 NextHdrOffset;
	I32 DCFId;
} AvLUlay;

#define AVL_UIM_ID 0x55494d47
#define AVL_UIM_VER 4

typedef struct AvLUim {
	U32 HdrID;
	I16 HdrSize, HdrVer;
	U8 OrigFile[80];
	I32 OrigFrm;
	I16 OrigStrm, Pad;
	I32 FrmCnt;
	U32 NextHdrOffset;
	I16 XPos, YPos, XLen, YLen, PixBits, Pad2;
	I32 DCFId;
} AvLUim;

#define AVL_PAD_ID 0x50414421
#define AVL_PAD_VER 4

typedef struct AvLPad {
	U32 HdrID;
	I16 HdrSize, HdrVer;
	U8 OrigFile[80];
	I16 OrigStrm, Pad;
	I32 FrmCnt;
	U32 NextHdrOffset;
	I32 ImagesPer, Seconds, VidFast, VidVar, VidRev, VidStart;
	I16 UlayFast, UlayVar, UlayRev, UlayStart;
	I16 PipeDepth, PipeStart, MinSeek, MinPad;
	I32 DCFId;
} AvLPad;

typedef struct AvLFrm {
	I32 FrmNum, RevOffset, ChkSum;
	I32 StrmFrmSize[1];
} AvLFrm;

typedef struct AvLBsh {
	U16 AlgNum;
	U16 Flags;
	U32 NumBits;
	U32 AlgSpec;
	U16 YSize;
	U16 XSize;
} AvLBsh;

typedef struct AvLFrmDir {
	U32 FrmOffset;
} AvLFrmDir;

#define AVK_IM_FILE_ID 0x56494d20
#define AVK_IM_HDR_VER 5

typedef struct AvLImHdr {
	U32 ImIDCode;
	I16 ImByteSize, ImVer;
	U32 ImAnnOffset;
	U32 ImPlaneFlag;
	U16 ImXLen, ImYLen, ImPixBits, ImCodeVer;
	U32 ImImageOff;
	U16 ImClutCnt, ImClutBits;
	U32 ImClutOff;
	U32 ImAppDataOff, ImAppDataSize;
	U32 ImImageSize;
	U16 ImColor, ImPlane;
	U32 pad2, pad3;
} AvLImHdr;

#ifdef __cplusplus
}
#endif
#endif /* _INTDVI_H_ */
