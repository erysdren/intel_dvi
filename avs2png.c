
#include <SDL3/SDL.h>
#include "utils.h"

#define AVL_LAST_HDR 0x7fffffff

#define VSTD_HDR_ID 0x56445649
#define VSTD_HDR_VER 1

typedef struct StdFileHdr {
	uint32_t FileId;
	int16_t HdrSize, HdrVersion;
	uint32_t AnnOffset;
} StdFileHdr;

#define AVL_FILE_ID 0x41565353
#define AVL_FILE_VER 3

typedef struct AvLFile {
	uint32_t HdrID;
	int16_t HdrSize, HdrVer, StrmGrpCnt, StrmGrpSize;
	uint32_t StrmGrpOffset;
	int16_t StrmGrpVer, StrmSize, StrmVer, StrmCnt;
	uint32_t StrmOffset, HdrPoolOffset;
	int32_t LabelCnt;
	uint32_t LabelOffset;
	int16_t LabelSize, LabelVer;
	uint32_t VshOffset;
	uint16_t VshSize;
	int16_t FrmVer;
	int32_t FrmCnt, FrmSize;
	uint32_t FirstFrmOffset, EndOfFrmsOffset;
	int16_t FrmHdrSize, FrmDirSize;
	uint32_t FrmDirOffset;
	int16_t FrmDirVer, FrmsPerSec;
	uint32_t Flag;
	uint32_t FreeBlockOffset;
	uint8_t Patch[32];
} AvLFile;

#define AVL_STRM_ID 0x5354524d
#define AVL_STRM_VER 3

#define AVL_STRMGRP_VER 3
#define AVL_STRMGRP_END -1

enum {
	AVL_T_AUD = 2, /* Compressed audio stream */
	AVL_T_CIM = 3, /* Compressed image stream */
	AVL_T_ULAY = 5, /* Associated per-frame data */
	AVL_T_UIM = 6, /* Uncompressed Image Stream */
	AVL_T_PAD = 7 /* Pad Stream */
};

enum {
	AVL_ST_Y = 1, /* Y-channel image data */
	AVL_ST_U = 11, /* U-channel image data */
	AVL_ST_V = 12, /* V-channel image data */
	AVL_ST_YVU = 13, /* YVU image data */
	AVL_ST_YUV_S = 14 /* YUV image data (Industry Standard Order) */
};

static const char *avl_type_names[] = {
	[AVL_T_AUD] = "AVL_T_AUD",
	[AVL_T_CIM] = "AVL_T_CIM",
	[AVL_T_ULAY] = "AVL_T_ULAY",
	[AVL_T_UIM] = "AVL_T_UIM",
	[AVL_T_PAD] = "AVL_T_PAD"
};

static const char *avl_subtype_names[] = {
	[AVL_ST_Y] = "AVL_ST_Y",
	[AVL_ST_U] = "AVL_ST_U",
	[AVL_ST_V] = "AVL_ST_V",
	[AVL_ST_YVU] = "AVL_ST_YVU",
	[AVL_ST_YUV_S] = "AVL_ST_YUV_S"
};

typedef struct AvLStrm {
	uint32_t HdrID;
	uint16_t Type, SubType;
	int16_t HdrCnt, NextStrmNum, StrmGrpNum, Pad;
	uint32_t Flag;
	int32_t FrmSize;
	uint32_t FirstHdrOffset;
	uint8_t StrmName[16];
} AvLStrm;

#define AVL_AUD_ID 0x41554449
#define AVL_AUD_VER 5

typedef struct AvLAud {
	uint32_t HdrID;
	int16_t HdrSize, HdrVer;
	uint8_t OrigFile[80];
	int32_t OrigFrm;
	int16_t OrigStrm, Pad;
	int32_t FrmCnt;
	uint32_t NextHdrOffset;
	uint8_t Lib[16], Alg[16];
	int32_t Parm1;
	int16_t Parm2, Parm3, LeftVol, RightVol;
	int32_t LoopOffset, StartFrm;
	uint32_t Flag;
	int16_t Parm4, Pad2;
	int32_t DCFId;
} AvLAud;

#define AVL_CIM_ID 0x43494d47
#define AVL_CIM_VER 4
#define AVL_CIM_RANDOM_STILL -1

typedef struct AvLCim {
	uint32_t HdrID;
	int16_t HdrSize, HdrVer;
	uint8_t OrigFile[80];
	int32_t OrigFrm;
	int16_t OrigStrm, Pad;
	int32_t FrmCnt;
	uint32_t NextHdrOffset;
	int16_t XPos, YPos, XLen, YLen;
	int16_t XCrop, YCrop, DropFrm, DropPhase;
	int32_t StillPeriod;
	int16_t BufsMin, BufsMax, DeCodeAlg, Pad2;
	int32_t DCFId;
} AvLCim;

#define AVL_ULAY_ID 0x554e4452
#define AVL_ULAY_VER 4

typedef struct AvLUlay {
	uint32_t HdrID;
	int16_t HdrSize, HdrVer;
	uint8_t OrigFile[80];
	int32_t OrigFrm;
	int16_t OrigStrm, Pad;
	int32_t FrmCnt;
	uint32_t NextHdrOffset;
	int32_t DCFId;
} AvLUlay;

#define AVL_UIM_ID 0x55494d47
#define AVL_UIM_VER 4

typedef struct AvLUim {
	uint32_t HdrID;
	int16_t HdrSize, HdrVer;
	uint8_t OrigFile[80];
	int32_t OrigFrm;
	int16_t OrigStrm, Pad;
	int32_t FrmCnt;
	uint32_t NextHdrOffset;
	int16_t XPos, YPos, XLen, YLen, PixBits, Pad2;
	int32_t DCFId;
} AvLUim;

#define AVL_PAD_ID 0x50414421
#define AVL_PAD_VER 4

typedef struct AvLPad {
	uint32_t HdrID;
	int16_t HdrSize, HdrVer;
	uint8_t OrigFile[80];
	int16_t OrigStrm, Pad;
	int32_t FrmCnt;
	uint32_t NextHdrOffset;
	int32_t ImagesPer, Seconds, VidFast, VidVar, VidRev, VidStart;
	int16_t UlayFast, UlayVar, UlayRev, UlayStart;
	int16_t PipeDepth, PipeStart, MinSeek, MinPad;
	int32_t DCFId;
} AvLPad;

#define AVL_FRM_VER 3

typedef struct AvLFrm {
	int32_t FrmNum, RevOffset, ChkSum;
	int32_t StrmFrmSize[1];
} AvLFrm;

typedef struct AvLBsh {
	uint16_t AlgNum;
	uint16_t Flags;
	uint32_t NumBits;
	uint32_t AlgSpec;
	uint16_t YSize;
	uint16_t XSize;
} AvLBsh;

#define AVL_FRMDIR_VER 3

typedef struct AvLFrmDir {
	uint32_t FrmOffset;
} AvLFrmDir;

#define AVK_IM_FILE_ID 0x56494d20
#define AVK_IM_HDR_VER 5

typedef struct AvLImHdr {
	uint32_t ImIDCode;
	int16_t ImByteSize, ImVer;
	uint32_t ImAnnOffset;
	uint32_t ImPlaneFlag;
	uint16_t ImXLen, ImYLen, ImPixBits, ImCodeVer;
	uint32_t ImImageOff;
	uint16_t ImClutCnt, ImClutBits;
	uint32_t ImClutOff;
	uint32_t ImAppDataOff, ImAppDataSize;
	uint32_t ImImageSize;
	uint16_t ImColor, ImPlane;
	uint32_t pad2, pad3;
} AvLImHdr;

#define AVL_LABEL_VER 3

typedef struct AvLLabel {
	uint8_t unknown[20];
} AvLLabel;

static void read_StdFileHdr(SDL_IOStream *io, StdFileHdr *hdr)
{
	SDL_ReadU32LE(io, &hdr->FileId);
	SDL_ReadU16LE(io, &hdr->HdrSize);
	SDL_ReadU16LE(io, &hdr->HdrVersion);
	SDL_ReadU32LE(io, &hdr->AnnOffset);

	soft_assert(hdr->FileId == VSTD_HDR_ID);
	soft_assert(hdr->HdrSize == sizeof(StdFileHdr));
	soft_assert(hdr->HdrVersion == VSTD_HDR_VER);
}

static void read_AvLFile(SDL_IOStream *io, AvLFile *avl)
{
	SDL_ReadU32LE(io, &avl->HdrID);
	SDL_ReadU16LE(io, &avl->HdrSize);
	SDL_ReadU16LE(io, &avl->HdrVer);
	SDL_ReadU16LE(io, &avl->StrmGrpCnt);
	SDL_ReadU16LE(io, &avl->StrmGrpSize);
	SDL_ReadU32LE(io, &avl->StrmGrpOffset);
	SDL_ReadU16LE(io, &avl->StrmGrpVer);
	SDL_ReadU16LE(io, &avl->StrmSize);
	SDL_ReadU16LE(io, &avl->StrmVer);
	SDL_ReadU16LE(io, &avl->StrmCnt);
	SDL_ReadU32LE(io, &avl->StrmOffset);
	SDL_ReadU32LE(io, &avl->HdrPoolOffset);
	SDL_ReadU32LE(io, &avl->LabelCnt);
	SDL_ReadU32LE(io, &avl->LabelOffset);
	SDL_ReadU16LE(io, &avl->LabelSize);
	SDL_ReadU16LE(io, &avl->LabelVer);
	SDL_ReadU32LE(io, &avl->VshOffset);
	SDL_ReadU16LE(io, &avl->VshSize);
	SDL_ReadU16LE(io, &avl->FrmVer);
	SDL_ReadU32LE(io, &avl->FrmCnt);
	SDL_ReadU32LE(io, &avl->FrmSize);
	SDL_ReadU32LE(io, &avl->FirstFrmOffset);
	SDL_ReadU32LE(io, &avl->EndOfFrmsOffset);
	SDL_ReadU16LE(io, &avl->FrmHdrSize);
	SDL_ReadU16LE(io, &avl->FrmDirSize);
	SDL_ReadU32LE(io, &avl->FrmDirOffset);
	SDL_ReadU16LE(io, &avl->FrmDirVer);
	SDL_ReadU16LE(io, &avl->FrmsPerSec);
	SDL_ReadU32LE(io, &avl->Flag);
	SDL_ReadU32LE(io, &avl->FreeBlockOffset);
	SDL_ReadIO(io, avl->Patch, sizeof(avl->Patch));

	soft_assert(avl->HdrID == AVL_FILE_ID);
	soft_assert(avl->HdrSize == sizeof(AvLFile));
	soft_assert(avl->HdrVer == AVL_FILE_VER);
	soft_assert(avl->StrmGrpVer == AVL_STRMGRP_VER);
	soft_assert(avl->StrmSize == sizeof(AvLStrm));
	soft_assert(avl->StrmVer == AVL_STRM_VER);
	soft_assert(avl->LabelSize == sizeof(AvLLabel));
	soft_assert(avl->LabelVer == AVL_LABEL_VER);
	soft_assert(avl->FrmVer == AVL_FRM_VER);
	soft_assert(avl->FrmDirSize == sizeof(AvLFrmDir));
	soft_assert(avl->FrmDirVer == AVL_FRMDIR_VER);
}

static void read_AvLStrm(SDL_IOStream *io, AvLStrm *stream)
{
	SDL_ReadU32LE(io, &stream->HdrID);
	SDL_ReadU16LE(io, &stream->Type);
	SDL_ReadU16LE(io, &stream->SubType);
	SDL_ReadU16LE(io, &stream->HdrCnt);
	SDL_ReadU16LE(io, &stream->NextStrmNum);
	SDL_ReadU16LE(io, &stream->StrmGrpNum);
	SDL_ReadU16LE(io, &stream->Pad);
	SDL_ReadU32LE(io, &stream->Flag);
	SDL_ReadU32LE(io, &stream->FrmSize);
	SDL_ReadU32LE(io, &stream->FirstHdrOffset);
	SDL_ReadIO(io, stream->StrmName, sizeof(stream->StrmName));

	soft_assert(stream->HdrID == AVL_STRM_ID);
}

static void read_AvLCim(SDL_IOStream *io, AvLCim *cim)
{
	SDL_ReadU32LE(io, &cim->HdrID);
	SDL_ReadU16LE(io, &cim->HdrSize);
	SDL_ReadU16LE(io, &cim->HdrVer);
	SDL_ReadIO(io, cim->OrigFile, sizeof(cim->OrigFile));
	SDL_ReadU32LE(io, &cim->OrigFrm);
	SDL_ReadU16LE(io, &cim->OrigStrm);
	SDL_ReadU16LE(io, &cim->Pad);
	SDL_ReadU32LE(io, &cim->FrmCnt);
	SDL_ReadU32LE(io, &cim->NextHdrOffset);
	SDL_ReadU16LE(io, &cim->XPos);
	SDL_ReadU16LE(io, &cim->YPos);
	SDL_ReadU16LE(io, &cim->XLen);
	SDL_ReadU16LE(io, &cim->YLen);
	SDL_ReadU16LE(io, &cim->XCrop);
	SDL_ReadU16LE(io, &cim->YCrop);
	SDL_ReadU16LE(io, &cim->DropFrm);
	SDL_ReadU16LE(io, &cim->DropPhase);
	SDL_ReadU32LE(io, &cim->StillPeriod);
	SDL_ReadU16LE(io, &cim->BufsMin);
	SDL_ReadU16LE(io, &cim->BufsMax);
	SDL_ReadU16LE(io, &cim->DeCodeAlg);
	SDL_ReadU16LE(io, &cim->Pad2);
	SDL_ReadU32LE(io, &cim->DCFId);

	soft_assert(cim->HdrID == AVL_CIM_ID);
	soft_assert(cim->HdrSize == sizeof(AvLCim));
	soft_assert(cim->HdrVer == AVL_CIM_VER);
}

static void make_output_filename(const char *input, char *output, size_t output_size)
{
	size_t inputLen = SDL_strlen(input);

	if (string_endswith(input, ".avs") || string_endswith(input, ".AVS"))
	{
		SDL_snprintf(output, output_size, "%s", input);
		SDL_snprintf(output + inputLen - 4, output_size - inputLen - 4, ".avi", input);
	}
	else
	{
		SDL_snprintf(output, output_size, "%s_converted.avi", input);
	}
}

int main(int argc, char **argv)
{
	for (int arg = 1; arg < argc; arg++)
	{
		log_info("Processing \"%s\"", argv[arg]);

		/* open input file */
		SDL_IOStream *inputIo = SDL_IOFromFile(argv[arg], "rb");
		if (!inputIo)
		{
			log_warning("Failed to open \"%s\" for reading", argv[arg]);
			goto cleanup;
		}

		/* open temporary buffer for writing */
		SDL_IOStream *outputIo = SDL_IOFromDynamicMem();
		if (!outputIo)
		{
			log_warning("Failed to create output buffer");
			goto cleanup;
		}

		/* read file header */
		StdFileHdr hdr;
		AvLFile avl;
		read_StdFileHdr(inputIo, &hdr);
		read_AvLFile(inputIo, &avl);

		log_info("FrmsPerSec: %d", avl.FrmsPerSec);

		/* read streams */
		SDL_SeekIO(inputIo, avl.StrmOffset, SDL_IO_SEEK_SET);
		for (int i = 0; i < avl.StrmCnt; i++)
		{
			AvLStrm stream;
			read_AvLStrm(inputIo, &stream);
			log_info("Stream %d:", i);
			log_info("\tName: %.*s", sizeof(stream.StrmName), stream.StrmName);
			log_info("\tType: %s", avl_type_names[stream.Type]);

			/* save position of next stream header */
			Sint64 nextStrmOffset = SDL_TellIO(inputIo);

			/* read substreams */
			SDL_SeekIO(inputIo, stream.FirstHdrOffset, SDL_IO_SEEK_SET);
			for (int j = 0; j < stream.HdrCnt; j++)
			{
				log_info("\tSubStream %d:", j);
				log_info("\t\tSubType: %s", avl_subtype_names[stream.SubType]);

				if (stream.Type == AVL_T_CIM)
				{
					AvLCim cim;
					read_AvLCim(inputIo, &cim);
					log_info("\t\tDeCodeAlg: %d", cim.DeCodeAlg);
					log_info("\t\tXPos: %d", cim.XPos);
					log_info("\t\tYPos: %d", cim.YPos);
					log_info("\t\tXLen: %d", cim.XLen);
					log_info("\t\tYLen: %d", cim.YLen);
					log_info("\t\tXCrop: %d", cim.XCrop);
					log_info("\t\tYCrop: %d", cim.YCrop);
				}
			}

			/* seek to next stream header */
			SDL_SeekIO(inputIo, nextStrmOffset, SDL_IO_SEEK_SET);
		}

		/* get output filename */
		char outputFilename[1024];
		make_output_filename(argv[arg], outputFilename, sizeof(outputFilename));

		/* get pointer to the buffer we wrote */
		Sint64 outputSize = SDL_GetIOSize(outputIo);
		SDL_PropertiesID outputProps = SDL_GetIOProperties(outputIo);
		void *outputData = SDL_GetPointerProperty(outputProps, SDL_PROP_IOSTREAM_DYNAMIC_MEMORY_POINTER, NULL);

		/* save output file */
		if (!SDL_SaveFile(outputFilename, outputData, outputSize))
			log_warning("Failed to save \"%s\"", outputFilename);
		else
			log_info("Successfully Saved \"%s\"", outputFilename);

		/* clean up */
cleanup:
		if (inputIo) SDL_CloseIO(inputIo);
		if (outputIo) SDL_CloseIO(outputIo);
	}

	SDL_Quit();

	return 0;
}
