#include "tladisasm.h"
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <stdarg.h>
#include <windows.h>
#include <time.h>
#include <errno.h>

#define ARRAY_SIZE(_x) (sizeof(_x)/sizeof(_x[0]))

static FILE *logfile = NULL;

const char *modeinfo_names[MODEINFO_MAX] = {
	"MAX_BUS",
	"MAX_GROUP",
	"MAX_MODE",
	"3",
	"GETNAME"
};

int bustable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

struct groupinfo groupinfo[] = { { "TLA-DISASM", 0, 0, 0, 0, 2, 0 },
                                 { "Traffic" , 0, 0, 2, 1, 0x80, sizeof("Traffic")-1 } };

struct businfo businfo[] = { { 0, 0, 0, 0, 0, 0x20000, NULL, 0 } };

char *onoff[] = { "On", "Off", NULL, NULL };

struct modeinfo modeinfo[] = { { "Testmenu1", onoff, 1, 0 },
                               { "Testmenu2", onoff, 1, 0 },
                               { "Testmenu3", onoff, 1, 0 },
                               { "Testmenu4", onoff, 1, 0 } };

static void LogDebug(struct pctx *pctx, int level, const char *fmt, ...)
{
	va_list ap;

	if (logfile) {
		va_start(ap, fmt);
		vfprintf(logfile, fmt, ap);
		va_end(ap);
		fprintf(logfile, "\n");
		fflush(logfile);
	}
}

struct pctx *ParseReinit(struct pctx *pctx, struct lactx *lactx, struct lafunc *func)
{
	struct pctx *ret;

	if (pctx)
			return pctx;

	if ((logfile = fopen("C:\\tladisasm.log", "wb")) == NULL) {
			func->LAError(lactx, 9, "Failed to open logfile: %s", strerror(errno));
			return NULL;
		}

	if (!(ret = func->rda_calloc(1, sizeof(struct pctx)))) {
		func->LAError(0, 9, "Out of Memory");
		return NULL;
	}

	ret->lactx = lactx;
	ret->func.LAGroupValue = func->LAGroupValue;
	ret->func.rda_calloc = func->rda_calloc;
	ret->func.rda_free = func->rda_free;
        ret->func.LAInfo = func->LAInfo;

	LogDebug(ret, 9, "Initialization finished");
	return ret;
}

int ParseFinish(struct pctx *pctx)
{
	LogDebug(pctx, 9, "%s", __FUNCTION__);
	if (logfile)
		fclose(logfile);
	logfile = NULL;
	pctx->func.rda_free(pctx);
	return 0;
}

struct sequence *ParseSeq(struct pctx *pctx, int initseq)
{
        struct sequence *seqinfo;

	LogDebug(pctx, 9, "%s: sequence %d", __FUNCTION__, initseq);

	if (!pctx) {
		LogDebug(pctx, 9, "pctx NULL");
		return NULL;
	}

        if (!(seqinfo = pctx->func.rda_calloc(1, sizeof(struct sequence)))) {
                LogDebug(pctx, 9, "calloc failed");
                return NULL;
        }

        sprintf(seqinfo->text, "insert text here");
        seqinfo->textp = seqinfo->text;
        seqinfo->text2 = calloc(1, 8);
        seqinfo->flags = 15;
	return seqinfo;
}

int ParseMarkNext(struct pctx *pctx, int seq, int a3)
{
	LogDebug(pctx, 9, "%s: sequence %d, a3 %d", __FUNCTION__, seq, a3);
	return 0;
}

int ParseMarkSet(struct pctx *pctx, int seq, int a3)
{
	LogDebug(pctx, 9, "%s", __FUNCTION__);
	return 0;
}

int ParseMarkGet(struct pctx *pctx, int seq)
{
	LogDebug(pctx, 9, "%s: sequence %d", __FUNCTION__, seq);
	return 0;
}


int ParseMarkMenu(struct pctx *pctx, int seq, int a3, int a4, int a5)
{
	LogDebug(pctx, 0, "%s: sequence %d, a3: %08x, a4: %08x, a5: %08x", __FUNCTION__, seq, a3, a4, a5);
	return 0;
}

int ParseInfo(struct pctx *pctx, unsigned int request)
{
	LogDebug(pctx, 6, "%s: %s", __FUNCTION__,
		request > ARRAY_SIZE(modeinfo_names) ? "invalid" : modeinfo_names[request]);

	switch(request) {
		case MODEINFO_MAX_BUS:
			return ARRAY_SIZE(businfo);
		case MODEINFO_MAX_GROUP:
			return ARRAY_SIZE(groupinfo);
		case MODEINFO_GETNAME:
			return (int)"TLADISASM";
		case 3:
			return 1;
		case MODEINFO_MAX_MODE:
			return ARRAY_SIZE(modeinfo);
		default:
			LogDebug(pctx, 9, "%s: invalid request: %d", __FUNCTION__, request);
			return 0;
	}	
	return 0;
}

struct businfo *ParseBusInfo(struct pctx *pctx, uint16_t bus)
{
	LogDebug(pctx, 6, "%s: %08x", __FUNCTION__, bus);

	if (bus >= ARRAY_SIZE(businfo))
		return NULL;
	return businfo+bus;
}

struct groupinfo *ParseGroupInfo(struct pctx *pctx, uint16_t group)
{
	LogDebug(pctx, 9, "%s: %08x", __FUNCTION__, group);

	if (group > ARRAY_SIZE(groupinfo))
			return NULL;
	return groupinfo+group;
}

struct modeinfo *ParseModeInfo(struct pctx *pctx, uint16_t mode)
{
	LogDebug(pctx, 9, "%s: %d", __FUNCTION__, mode);
	if (mode > ARRAY_SIZE(modeinfo))
			return NULL;
	return modeinfo+mode;
}

int ParseModeGetPut(struct pctx *pctx, int mode, int value, int request)
{
	LogDebug(pctx, 9, "%s: %d (%s), %d (%s)", __FUNCTION__,
			request, modeinfo[mode].name, value, onoff[value]);
	return value;
}

int ParseDisasmReinit(struct pctx *pctx, int request)
{
	LogDebug(pctx, 9, "%s: pctx %08x, request=%d", __FUNCTION__, pctx, request);
	return 1;
}
