#ifndef __MAIN_RAW_REFIT_H__
#define __MAIN_RAW_REFIT_H__

#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "rtypes.h"
#include "dmap.h"

#include "radar.h"

//#include "limit.h"
#include "rprm.h"
#include "rawdata.h"
#include "rawread.h"

#include "fitdata.h"
//#include "fitacfex.h"
#include "badsmp.h"
#include "badlags.h"
#include "dofit.h"

// #include "FT.h"

#include "radar.h"
#include "fitacf.h"
#include "fitacfex2.h"
#include "lmfit.h"
//#include "fitacfex2.h"

#include <sys/types.h>
#include <dirent.h>

#define FFTLEN 256

#ifndef PI
#define PI 3.1415926
#endif

/*
struct RadarSite* site;
float time_from,time_to;

long outmode=3;

long AccLen=1;
int SELECT_BEAM=0;
int SELECT_CHANNEL=2;


static struct FitData fit;
static struct RawData raw_3[16];
static struct RawData raw_10[16];
*/

int printRaw(struct RadarParm *prm, 
	     struct RawData *raw, 
	     struct FitBlock *input,
	     struct FitData *fit,
	     char* stream
	    );

#include <cbor.h>
#include "fitacf2cbor_lib.h"

#endif
