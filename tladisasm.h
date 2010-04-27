
#ifndef I2C_H
#define I2C_H

#include <stdint.h>

enum TLA_INFO {
        TLA_INFO_FIRST_SEQUENCE,
        TLA_INFO_LAST_SEQUENCE,
        TLA_INFO_DISPLAY_ATTRIBUTE,
        TLA_INFO_3,
        TLA_INFO_MNEMONICS_WIDTH=5
};

struct lactx;
struct businfo {
	int val0;
	int val4;
	int val8;
	int valc;
	int val10;
	int val14;
	void *val18;
	int val1c;
};

struct modeinfo {
	char *name;
	char **options;
	int val1;
	int val2;
};

struct sequence {
	struct sequence *next;
	char *textp;
	uint8_t flags;
        char field_9;
        char field_A;
        char field_B;
	char *text2;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	char text[128];
};

struct pctx_functable {
	int (*LAGroupValue)(struct lactx *lactx, int seqno, int group);
	void (*rda_free)(void *p);
        void *(*rda_calloc)(int memb, int size);
        int (*LAInfo)(struct lactx *, enum TLA_INFO, int16_t bus);
};
struct pctx {
	struct lactx *lactx;
	struct pctx_functable func;
};

struct groupinfo {
	char *name;
	char field_4;
	char field_5;
	char field_6;
	char field_7;
	uint16_t members;
	uint16_t len;
};

enum MODEINFO {
	MODEINFO_MAX_BUS=0,
	MODEINFO_MAX_GROUP=1,
	MODEINFO_MAX_MODE=2,
	MODEINFO_3=3,
	MODEINFO_GETNAME=4,
	MODEINFO_MAX,
};

struct lafunc {
	int unknown;
	char *support_path;
	char *support_sep;
	char *support_name;
	char *support_name2;
	char *support_ext;
	void *(*rda_malloc)(int size);
	void *(*rda_calloc)(int members, int size);
	void *(*rda_realloc)(void *p, int size);
	void (*rda_free)(void *p);
	void (*LABus)(struct lactx *, int seq);
	int  (*LAInfo)(struct lactx *, enum TLA_INFO, int16_t bus);
	void (*LAError)(struct lactx *, int, char *, ...);
	void (*LAFindSeq)(struct lactx *, int seq, int off, int bus);
	void (*LAFormatValue)(void);
	void (*LAGap)(struct lactx *, int seq);
	int (*LAGroupValue)(struct lactx *lactx, int seqno, int group);
	void (*LAInvalidate)(struct lactx *lactx, int firstseq, int lastseq);
	void (*LASeqToText)(struct lactx *, int seqno, int, int);
	int (*LAGroupWidth_)(struct lactx *, int seqno);
	void *(*LATimeStamp_ps_)(struct lactx *, int seqno, int arg);
	void (*LASysTrigTime_ps_)(void);
	void (*LABusModTrigTime_ps_)(void);
	void (*LABusModTimeOffset_ps_)(void);
	void (*LAGroupInvalidBitMask_)(void);
	void (*LAContigLongToSeq_)(void);
	void (*LALongToSeq_)(void);
	void (*LALongToValidSeq_)(void);
	void (*LASeqToContigLong_)(void);
	void (*LASeqToLong_)(void);
	void (*LASubDisasmLoad)(void);
	void (*LASubDisasmUnload)(void);
	void (*LASubDisasmFuncTablePtr_)(void);
	void (*LAWhichBusMod_)(void);
	void (*LASeqDisplayFormat_)(void);
	void (*LAInteractiveUI2_)(void);
	void (*LAProgAbort_)(char *, struct lactx *, int seqno);
	void (*LATimestamp_ps_ToText)(void);
	void (*LATimeStampDisplayFormat)(void);
	void (*LAReferenceTime_ps_)(void);
	void (*LABusModSysTrigTime_ps_)(void);
	void (*LABusModFrameOffset_ps_)(void);
	void (*LABusModTimeToUserAlignedTime_ps_)(void);
	void (*LABusModTrigSample)(void);
	void (*LABusModWallClockStart_)(void);
	void *field_B4;
	void (*LAReferenceTime_ps_2)(void);
	void (*LASampleStatusBits)(void);
	void (*LASampleStatusBitsType_)(void);
	void (*LAGroupViolationBitMask_)(void);
	void (*LAGroupViolationBitMaskType_)(void);
	void (*LABusModVariantName_)(void);
	void (*LASystemName_)(void);
	void (*LASystemPath_)(void);
	void *field_DC;
};
__declspec(dllexport) struct pctx *ParseReinit(struct pctx *pctx, struct lactx *lactx, struct lafunc *func);
__declspec(dllexport) int ParseFinish(struct pctx *pctx);
__declspec(dllexport) int ParseInfo(struct pctx *pctx, unsigned int request);
__declspec(dllexport) int ParseMarkMenu(struct pctx *, int, int, int, int);
__declspec(dllexport) int ParseMarkGet(struct pctx *pctx, int seq);
__declspec(dllexport) int ParseMarkSet(struct pctx *pctx, int seq, int);
__declspec(dllexport) int ParseMarkNext(struct pctx *pctx, int seq, int);
__declspec(dllexport) int ParseModeGetPut(struct pctx *pctx, int mode, int, int request);
__declspec(dllexport) struct sequence *ParseSeq(struct pctx *, int seq);
__declspec(dllexport) struct businfo *ParseBusInfo(struct pctx *, uint16_t bus);
__declspec(dllexport) struct modeinfo *ParseModeInfo(struct pctx *pctx, uint16_t mode);
__declspec(dllexport) struct groupinfo *ParseGroupInfo(struct pctx *pctx, uint16_t group);
__declspec(dllexport) int ParseDisasmReinit(struct pctx *, int request);

#endif
