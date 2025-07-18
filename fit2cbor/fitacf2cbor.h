#ifndef __FITACF2HDF5_H__
#define __FITACF2HDF5_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cbor.h>
#include "main_fitread.h"

struct Revision {
    int major;
    int minor;
};

struct FitInOrigin{
    char code;
    char *time;
    char *command;
  };

struct FitInTime{
    int16 yr;
    int16 mo;
    int16 dy;
    int16 hr;
    int16 mt;
    int16 sc;
    int32 us;
  };
  
struct FitInStat{
    int16 agc;
    int16 lopwr;
  };
  
struct FitInNoise{
    float search;
    float mean;
  };

  
struct FitInIntt{
    int16 sc;
    int32 us;
  };


struct FitMode {  
  float v;
  float w;
  float p;
  float phi0;
  float elev_front;
  float elev_back;
  float p_rel;
};

#define MAX_GATES 100
#define MAX_MODES 10
struct FitModeRng {  
 struct FitMode modes[MAX_GATES][MAX_MODES];
 int modes_cnt[MAX_GATES];
};
      


#endif

