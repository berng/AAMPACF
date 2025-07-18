#ifndef __PRINT_CBOR_H__
#define __PRINT_CBOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cbor.h>
#include <stdint.h>
#include "main_fitread.h"
#include "fitacf2cbor.h"

// Helper function to read string from CBOR
char* read_cbor_string(cbor_item_t *item);

// Function to read RadarParm from CBOR
void read_radar_parm(cbor_item_t *map, struct RadarParm *parm);
// Function to read FitRange from CBOR
void read_fit_range(cbor_item_t *map, struct FitRange *rng);
// Function to read FitElv from CBOR
void read_fit_elv(cbor_item_t *map, struct FitElv *elv);
// Function to read FitNoise from CBOR
void read_fit_noise(cbor_item_t *map, struct FitNoise *noise);
// Function to read FitMode from CBOR
void read_fit_mode(cbor_item_t *map, struct FitMode *mode);
// Function to read FitData from CBOR
void read_fit_data(cbor_item_t *map, struct FitData *fit, struct RadarParm *prm, struct FitModeRng *ModesStruct);

// Function to free RadarParm memory
void free_radar_parm(struct RadarParm *parm);
// Function to free FitData memory
void free_fit_data(struct FitData *fit);
// Main function to read CBOR file
int read_cbor_file(const char *filename, struct RadarParm *prm, struct FitData *fit, struct FitModeRng* ModesStruct);

#endif

