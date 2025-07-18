/*
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <cbor.h>
#include "fitacf2cbor.h"
*/

#include "fitacf2cbor_lib.h"


// Function to write FitPrm to CBOR
void write_fit_prm(cbor_item_t *map, struct FitPrm *prm) {
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("channel")),
        .value = cbor_move(cbor_build_uint32(prm->channel))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("offset")),
        .value = cbor_move(cbor_build_uint32(prm->offset))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("cp")),
        .value = cbor_move(cbor_build_uint32(prm->cp))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("xcf")),
        .value = cbor_move(cbor_build_uint32(prm->xcf))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("tfreq")),
        .value = cbor_move(cbor_build_uint32(prm->tfreq))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("noise")),
        .value = cbor_move(cbor_build_uint32(prm->noise))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("nrang")),
        .value = cbor_move(cbor_build_uint32(prm->nrang))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("smsep")),
        .value = cbor_move(cbor_build_uint32(prm->smsep))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("nave")),
        .value = cbor_move(cbor_build_uint32(prm->nave))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("mplgs")),
        .value = cbor_move(cbor_build_uint32(prm->mplgs))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("mpinc")),
        .value = cbor_move(cbor_build_uint32(prm->mpinc))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("txpl")),
        .value = cbor_move(cbor_build_uint32(prm->txpl))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("lagfr")),
        .value = cbor_move(cbor_build_uint32(prm->lagfr))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("mppul")),
        .value = cbor_move(cbor_build_uint32(prm->mppul))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("bmnum")),
        .value = cbor_move(cbor_build_uint32(prm->bmnum))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("old")),
        .value = cbor_move(cbor_build_uint32(prm->old))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("maxbeam")),
        .value = cbor_move(cbor_build_uint32(prm->maxbeam))
    });
    
    // Add arrays
    cbor_item_t *lag0 = cbor_new_definite_array(prm->mplgs);
    cbor_item_t *lag1 = cbor_new_definite_array(prm->mplgs);
    for (int i = 0; i < prm->mplgs; i++) {
        cbor_array_push(lag0, cbor_move(cbor_build_uint32(prm->lag[0][i])));
        cbor_array_push(lag1, cbor_move(cbor_build_uint32(prm->lag[1][i])));
    }
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("lag0")),
        .value = cbor_move(lag0)
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("lag1")),
        .value = cbor_move(lag1)
    });
    
    cbor_item_t *pulse = cbor_new_definite_array(prm->mppul);
    for (int i = 0; i < prm->mppul; i++) {
        cbor_array_push(pulse, cbor_move(cbor_build_uint32(prm->pulse[i])));
    }
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("pulse")),
        .value = cbor_move(pulse)
    });
    
    
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("bmsep")),
        .value = cbor_move(cbor_build_float4(prm->bmsep))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("tdiff")),
        .value = cbor_move(cbor_build_float4(prm->tdiff))
    });
}

// Function to write FitRange to CBOR
void write_fit_range(cbor_item_t *map, struct FitRange *rng) {
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("v")),
        .value = cbor_move(cbor_build_float4(rng->v))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("v_err")),
        .value = cbor_move(cbor_build_float4(rng->v_err))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("p_0")),
        .value = cbor_move(cbor_build_float4(rng->p_0))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("p_l")),
        .value = cbor_move(cbor_build_float4(rng->p_l))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("p_l_err")),
        .value = cbor_move(cbor_build_float4(rng->p_l_err))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("p_s")),
        .value = cbor_move(cbor_build_float4(rng->p_s))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("p_s_err")),
        .value = cbor_move(cbor_build_float4(rng->p_s_err))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("w_l")),
        .value = cbor_move(cbor_build_float4(rng->w_l))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("w_l_err")),
        .value = cbor_move(cbor_build_float4(rng->w_l_err))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("w_s")),
        .value = cbor_move(cbor_build_float4(rng->w_s))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("w_s_err")),
        .value = cbor_move(cbor_build_float4(rng->w_s_err))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("phi0")),
        .value = cbor_move(cbor_build_float4(rng->phi0))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("phi0_err")),
        .value = cbor_move(cbor_build_float4(rng->phi0_err))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("sdev_l")),
        .value = cbor_move(cbor_build_float4(rng->sdev_l))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("sdev_s")),
        .value = cbor_move(cbor_build_float4(rng->sdev_s))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("sdev_phi")),
        .value = cbor_move(cbor_build_float4(rng->sdev_phi))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("qflg")),
        .value = cbor_move(cbor_build_uint32(rng->qflg))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("gsct")),
        .value = cbor_move(cbor_build_uint32(rng->gsct))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("nump")),
        .value = cbor_move(cbor_build_uint32(rng->nump))
    });
}

// Function to write FitElv to CBOR
void write_fit_elv(cbor_item_t *map, struct FitElv *elv) {
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("normal")),
        .value = cbor_move(cbor_build_float4(elv->normal))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("low")),
        .value = cbor_move(cbor_build_float4(elv->low))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("high")),
        .value = cbor_move(cbor_build_float4(elv->high))
    });
}

// Function to write FitNoise to CBOR
void write_fit_noise(cbor_item_t *map, struct FitNoise *noise) {
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("vel")),
        .value = cbor_move(cbor_build_float4(noise->vel))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("skynoise")),
        .value = cbor_move(cbor_build_float4(noise->skynoise))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("lag0")),
        .value = cbor_move(cbor_build_float4(noise->lag0))
    });
}

// Function to write all elements of RadarParm to CBOR
void write_radar_parm(cbor_item_t *map, struct RadarParm *parm) {
    // Add revision fields
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("revision.major")),
        .value = cbor_move(cbor_build_uint8(parm->revision.major))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("revision.minor")),
        .value = cbor_move(cbor_build_uint8(parm->revision.minor))
    });

    // Add origin fields
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("origin.code")),
        .value = cbor_move(cbor_build_uint8(parm->origin.code))
    });
    if (parm->origin.time) {
        cbor_map_add(map, (struct cbor_pair){
            .key = cbor_move(cbor_build_string("origin.time")),
            .value = cbor_move(cbor_build_string(parm->origin.time))
        });
    }
    if (parm->origin.command) {
        cbor_map_add(map, (struct cbor_pair){
            .key = cbor_move(cbor_build_string("origin.command")),
            .value = cbor_move(cbor_build_string(parm->origin.command))
        });
    }

    // Add simple fields
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("cp")),
        .value = cbor_move(cbor_build_uint16(parm->cp))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("stid")),
        .value = cbor_move(cbor_build_uint16(parm->stid))
    });

    // Add time fields
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("time.yr")),
        .value = cbor_move(cbor_build_uint16(parm->time.yr))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("time.mo")),
        .value = cbor_move(cbor_build_uint16(parm->time.mo))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("time.dy")),
        .value = cbor_move(cbor_build_uint16(parm->time.dy))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("time.hr")),
        .value = cbor_move(cbor_build_uint16(parm->time.hr))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("time.mt")),
        .value = cbor_move(cbor_build_uint16(parm->time.mt))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("time.sc")),
        .value = cbor_move(cbor_build_uint16(parm->time.sc))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("time.us")),
        .value = cbor_move(cbor_build_uint32(parm->time.us))
    });

    // Add more simple fields
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("txpow")),
        .value = cbor_move(cbor_build_uint16(parm->txpow))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("nave")),
        .value = cbor_move(cbor_build_uint16(parm->nave))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("atten")),
        .value = cbor_move(cbor_build_uint16(parm->atten))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("lagfr")),
        .value = cbor_move(cbor_build_uint16(parm->lagfr))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("smsep")),
        .value = cbor_move(cbor_build_uint16(parm->smsep))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("ercod")),
        .value = cbor_move(cbor_build_uint16(parm->ercod))
    });

    // Add stat fields
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("stat.agc")),
        .value = cbor_move(cbor_build_uint16(parm->stat.agc))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("stat.lopwr")),
        .value = cbor_move(cbor_build_uint16(parm->stat.lopwr))
    });

    // Add noise fields
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("noise.search")),
        .value = cbor_move(cbor_build_float4(parm->noise.search))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("noise.mean")),
        .value = cbor_move(cbor_build_float4(parm->noise.mean))
    });

    // Add more fields
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("channel")),
        .value = cbor_move(cbor_build_uint16(parm->channel))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("bmnum")),
        .value = cbor_move(cbor_build_uint16(parm->bmnum))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("bmazm")),
        .value = cbor_move(cbor_build_float4(parm->bmazm))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("scan")),
        .value = cbor_move(cbor_build_uint16(parm->scan))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("rxrise")),
        .value = cbor_move(cbor_build_uint16(parm->rxrise))
    });

    // Add intt fields
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("intt.sc")),
        .value = cbor_move(cbor_build_uint16(parm->intt.sc))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("intt.us")),
        .value = cbor_move(cbor_build_uint32(parm->intt.us))
    });

    // Add remaining fields
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("txpl")),
        .value = cbor_move(cbor_build_uint16(parm->txpl))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("mpinc")),
        .value = cbor_move(cbor_build_uint16(parm->mpinc))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("mppul")),
        .value = cbor_move(cbor_build_uint16(parm->mppul))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("mplgs")),
        .value = cbor_move(cbor_build_uint16(parm->mplgs))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("mplgexs")),
        .value = cbor_move(cbor_build_uint16(parm->mplgexs))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("nrang")),
        .value = cbor_move(cbor_build_uint16(parm->nrang))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("frang")),
        .value = cbor_move(cbor_build_uint16(parm->frang))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("rsep")),
        .value = cbor_move(cbor_build_uint16(parm->rsep))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("xcf")),
        .value = cbor_move(cbor_build_uint16(parm->xcf))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("tfreq")),
        .value = cbor_move(cbor_build_uint16(parm->tfreq))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("offset")),
        .value = cbor_move(cbor_build_uint16(parm->offset))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("ifmode")),
        .value = cbor_move(cbor_build_uint16(parm->ifmode))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("mxpwr")),
        .value = cbor_move(cbor_build_uint32(parm->mxpwr))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("lvmax")),
        .value = cbor_move(cbor_build_uint32(parm->lvmax))
    });

    // Add pulse array
    if (parm->pulse && parm->mppul > 0) {
        cbor_item_t *pulse = cbor_new_definite_array(parm->mppul);
        for (int i = 0; i < parm->mppul; i++) {
            cbor_array_push(pulse, cbor_move(cbor_build_uint16(parm->pulse[i])));
        }
        cbor_map_add(map, (struct cbor_pair){
            .key = cbor_move(cbor_build_string("pulse")),
            .value = cbor_move(pulse)
        });
    }

    // Add lag arrays
    if (parm->lag[0] && parm->lag[1] && parm->mplgs > 0) {
        cbor_item_t *lag0 = cbor_new_definite_array(parm->mplgs);
        cbor_item_t *lag1 = cbor_new_definite_array(parm->mplgs);
        for (int i = 0; i < parm->mplgs; i++) {
            cbor_array_push(lag0, cbor_move(cbor_build_uint16(parm->lag[0][i])));
            cbor_array_push(lag1, cbor_move(cbor_build_uint16(parm->lag[1][i])));
        }
        cbor_map_add(map, (struct cbor_pair){
            .key = cbor_move(cbor_build_string("lag0")),
            .value = cbor_move(lag0)
        });
        cbor_map_add(map, (struct cbor_pair){
            .key = cbor_move(cbor_build_string("lag1")),
            .value = cbor_move(lag1)
        });
    }

    // Add combf string if present
    if (parm->combf) {
        cbor_map_add(map, (struct cbor_pair){
            .key = cbor_move(cbor_build_string("combf")),
            .value = cbor_move(cbor_build_string(parm->combf))
        });
    }
}

void write_fit_data(cbor_item_t *map, struct FitData *fit, struct RadarParm *prm, struct FitModeRng *modes) 
{
    // Add revision fields
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("revision.major")),
        .value = cbor_move(cbor_build_uint32(fit->revision.major))
    });
    cbor_map_add(map, (struct cbor_pair){
        .key = cbor_move(cbor_build_string("revision.minor")),
        .value = cbor_move(cbor_build_uint32(fit->revision.minor))
    });

    // Add noise fields
    if (&fit->noise) {
        cbor_item_t *noise_map = cbor_new_definite_map(3);
        cbor_map_add(noise_map, (struct cbor_pair){
            .key = cbor_move(cbor_build_string("vel")),
            .value = cbor_move(cbor_build_float4(fit->noise.vel))
        });
        cbor_map_add(noise_map, (struct cbor_pair){
            .key = cbor_move(cbor_build_string("skynoise")),
            .value = cbor_move(cbor_build_float4(fit->noise.skynoise))
        });
        cbor_map_add(noise_map, (struct cbor_pair){
            .key = cbor_move(cbor_build_string("lag0")),
            .value = cbor_move(cbor_build_float4(fit->noise.lag0))
        });
        cbor_map_add(map, (struct cbor_pair){
            .key = cbor_move(cbor_build_string("noise")),
            .value = cbor_move(noise_map)
        });
    }

    // Add range data arrays if present
    if (fit->rng) {
//        fprintf(stderr,"nrng:%d\n",(int)prm->nrang);
        cbor_item_t *rng_array = cbor_new_definite_array(prm->nrang);
        for (int i = 0; i < prm->nrang; i++) {
//            cbor_item_t *range_map = cbor_new_definite_map(modes->modes_cnt[i]>0?20:19); // Number of fields in FitRange
            cbor_item_t *range_map = cbor_new_definite_map(22); // Number of fields in FitRange

            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("g")),
                .value = cbor_move(cbor_build_uint16(i))
            });
            
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("v")),
                .value = cbor_move(cbor_build_float4(fit->rng[i].v))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("v_err")),
                .value = cbor_move(cbor_build_float4(fit->rng[i].v_err))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("p_0")),
                .value = cbor_move(cbor_build_float4(fit->rng[i].p_0))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("p_l")),
                .value = cbor_move(cbor_build_float4(fit->rng[i].p_l))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("p_l_err")),
                .value = cbor_move(cbor_build_float4(fit->rng[i].p_l_err))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("p_s")),
                .value = cbor_move(cbor_build_float4(fit->rng[i].p_s))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("p_s_err")),
                .value = cbor_move(cbor_build_float4(fit->rng[i].p_s_err))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("w_l")),
                .value = cbor_move(cbor_build_float4(fit->rng[i].w_l))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("w_l_err")),
                .value = cbor_move(cbor_build_float4(fit->rng[i].w_l_err))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("w_s")),
                .value = cbor_move(cbor_build_float4(fit->rng[i].w_s))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("w_s_err")),
                .value = cbor_move(cbor_build_float4(fit->rng[i].w_s_err))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("phi0")),
                .value = cbor_move(cbor_build_float4(fit->rng[i].phi0))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("phi0_err")),
                .value = cbor_move(cbor_build_float4(fit->rng[i].phi0_err))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("sdev_l")),
                .value = cbor_move(cbor_build_float4(fit->rng[i].sdev_l))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("sdev_s")),
                .value = cbor_move(cbor_build_float4(fit->rng[i].sdev_s))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("sdev_phi")),
                .value = cbor_move(cbor_build_float4(fit->rng[i].sdev_phi))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("qflg")),
                .value = cbor_move(cbor_build_uint32(fit->rng[i].qflg))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("gsct")),
                .value = cbor_move(cbor_build_uint32(fit->rng[i].gsct))
            });
            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("nump")),
                .value = cbor_move(cbor_build_uint8(fit->rng[i].nump))
            });

            cbor_map_add(range_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("num_modes")),
                .value = cbor_move(cbor_build_uint8(modes->modes_cnt[i]))
            });


            if(modes->modes_cnt[i]>0)
            {

//             printf("mc!\n");

             cbor_item_t *range_modes = cbor_new_definite_array(modes->modes_cnt[i]); // Number of fields in FitRange
             for (int k = 0; k < modes->modes_cnt[i]; k++) 
             {
              cbor_item_t *mode = cbor_new_definite_map(8); // Number of fields in FitRange
              cbor_map_add(mode, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("m")),
                .value = cbor_move(cbor_build_uint16(k))
              });
              cbor_map_add(mode, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("v")),
                .value = cbor_move(cbor_build_float4(modes->modes[i][k].v))
              });
              cbor_map_add(mode, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("w")),
                .value = cbor_move(cbor_build_float4(modes->modes[i][k].w))
              });
              cbor_map_add(mode, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("p")),
                .value = cbor_move(cbor_build_float4(modes->modes[i][k].p))
              });
              cbor_map_add(mode, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("phi0")),
                .value = cbor_move(cbor_build_float4(modes->modes[i][k].phi0))
              });
              cbor_map_add(mode, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("elev_front")),
                .value = cbor_move(cbor_build_float4(modes->modes[i][k].elev_front))
              });
              cbor_map_add(mode, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("elev_back")),
                .value = cbor_move(cbor_build_float4(modes->modes[i][k].elev_back))
              });
              cbor_map_add(mode, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("p_rel")),
                .value = cbor_move(cbor_build_float4(modes->modes[i][k].p_rel))
              });

             cbor_array_push(range_modes, cbor_move(mode));
             }

    	     cbor_map_add(range_map, (struct cbor_pair){
        	    .key = cbor_move(cbor_build_string("modes")),
        	    .value = cbor_move(range_modes)
    		});

            }
            cbor_array_push(rng_array, cbor_move(range_map));
        }
        cbor_map_add(map, (struct cbor_pair){
            .key = cbor_move(cbor_build_string("rng")),
            .value = cbor_move(rng_array)
        });
    }

    // Add xrng data (same structure as rng)
    if (fit->xrng) {
        cbor_item_t *xrng_array = cbor_new_definite_array(prm->nrang);
        for (int i = 0; i < prm->nrang; i++) {
            cbor_item_t *xrange_map = cbor_new_definite_map(18); // Same as rng
            
            // Add all xrng fields same as rng...
            cbor_map_add(xrange_map, (struct cbor_pair){
                .key = cbor_move(cbor_build_string("v")),
                .value = cbor_move(cbor_build_float4(fit->xrng[i].v))
            });
            // ... (add all other fields)
            
            cbor_array_push(xrng_array, cbor_move(xrange_map));
        }
        cbor_map_add(map, (struct cbor_pair){
            .key = cbor_move(cbor_build_string("xrng")),
            .value = cbor_move(xrng_array)
        });
    }

    // Add elevation data if present
    if (fit->elv) {
        cbor_item_t *elv_array = cbor_new_definite_array(prm->nrang);
        for (int i = 0; i < prm->nrang; i++) {
    	    cbor_item_t *elv_map = cbor_new_definite_map(4);
    	    cbor_map_add(elv_map, (struct cbor_pair){
    	        .key = cbor_move(cbor_build_string("g")),
    	        .value = cbor_move(cbor_build_uint16(i))
    	    });
    	    cbor_map_add(elv_map, (struct cbor_pair){
    	        .key = cbor_move(cbor_build_string("normal")),
    	        .value = cbor_move(cbor_build_float4(fit->elv[i].normal))
    	    });
    	    cbor_map_add(elv_map, (struct cbor_pair){
        	.key = cbor_move(cbor_build_string("low")),
        	.value = cbor_move(cbor_build_float4(fit->elv[i].low))
    	    });
    	    cbor_map_add(elv_map, (struct cbor_pair){
        	.key = cbor_move(cbor_build_string("high")),
        	.value = cbor_move(cbor_build_float4(fit->elv[i].high))
    	    });
            cbor_array_push(elv_array, cbor_move(elv_map));
	}
        cbor_map_add(map, (struct cbor_pair){
            .key = cbor_move(cbor_build_string("elv")),
            .value = cbor_move(elv_array)
        });
    }


}