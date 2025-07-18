#ifndef __FITACF2CBORLIB_H__
#define __FITACF2CBORLIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <cbor.h>
#include "fitacf2cbor.h"


// Function to write FitPrm to CBOR
void write_fit_prm(cbor_item_t *map, struct FitPrm *prm);
// Function to write FitRange to CBOR
void write_fit_range(cbor_item_t *map, struct FitRange *rng);
// Function to write FitElv to CBOR
void write_fit_elv(cbor_item_t *map, struct FitElv *elv);
// Function to write FitNoise to CBOR
void write_fit_noise(cbor_item_t *map, struct FitNoise *noise);
// Function to write all elements of RadarParm to CBOR
void write_radar_parm(cbor_item_t *map, struct RadarParm *parm);
void write_fit_data(cbor_item_t *map, struct FitData *fit, struct RadarParm *prm, struct FitModeRng *modes); 

#endif
