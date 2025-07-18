#ifndef __READ_CNFIG_H__
#define __READ_CNFIG_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define CONFIG_FILE "./phases.dat"
//#define RADAR_CFG_FILE "radar_hdw.cfg"

#define WRONG_PHASE -999.9

typedef struct 
{
 int chan;
 int fmin,fmax;
 long long int dstart;
 double dphase;
 double D;
 double H;
}config_record_type;

typedef struct
{
 float lat;
 float lon;
 float mean_az;
 float az_step;
 float DX;
 float DY;
 long beams;
 float phase_sign;
}radarConfigType;


void read_radar_config(radarConfigType* config,char* fname);

long read_config_phase(config_record_type** config_record,long* config_record_len,char* fname);

double read_phase(long long date,int channel,int freq,
		    config_record_type* config_record,
		    long config_record_len,double* D,double* H);

#endif
