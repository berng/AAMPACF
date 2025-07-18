#include "print_cbor.h"

// Helper function to read string from CBOR
char* read_cbor_string(cbor_item_t *item) {
    if (!cbor_isa_string(item)) return NULL;
    size_t len = cbor_string_length(item);
    char *str = malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, cbor_string_handle(item), len);
    str[len] = '\0';
    return str;
}


// Function to read RadarParm from CBOR
void read_radar_parm(cbor_item_t *map, struct RadarParm *parm) {
    // Initialize pointers to NULL
    parm->pulse = NULL;
    parm->lag[0] = NULL;
    parm->lag[1] = NULL;
    parm->combf = NULL;
    parm->origin.time = NULL;
    parm->origin.command = NULL;

    // Iterate through all key-value pairs in the map
    for (size_t i = 0; i < cbor_map_size(map); i++) {
        struct cbor_pair pair = cbor_map_handle(map)[i];
        char *key = read_cbor_string(pair.key);
        
        if (strcmp(key, "revision.major") == 0) {
            parm->revision.major = cbor_get_uint8(pair.value);
        } else if (strcmp(key, "revision.minor") == 0) {
            parm->revision.minor = cbor_get_uint8(pair.value);
        } else if (strcmp(key, "origin.code") == 0) {
            parm->origin.code = cbor_get_uint8(pair.value);
        } else if (strcmp(key, "origin.time") == 0) {
            parm->origin.time = read_cbor_string(pair.value);
        } else if (strcmp(key, "origin.command") == 0) {
            parm->origin.command = read_cbor_string(pair.value);
        } else if (strcmp(key, "cp") == 0) {
            parm->cp = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "stid") == 0) {
            parm->stid = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "time.yr") == 0) {
            parm->time.yr = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "time.mo") == 0) {
            parm->time.mo = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "time.dy") == 0) {
            parm->time.dy = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "time.hr") == 0) {
            parm->time.hr = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "time.mt") == 0) {
            parm->time.mt = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "time.sc") == 0) {
            parm->time.sc = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "time.us") == 0) {
            parm->time.us = cbor_get_uint32(pair.value);
        } else if (strcmp(key, "txpow") == 0) {
            parm->txpow = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "nave") == 0) {
            parm->nave = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "atten") == 0) {
            parm->atten = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "lagfr") == 0) {
            parm->lagfr = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "smsep") == 0) {
            parm->smsep = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "ercod") == 0) {
            parm->ercod = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "stat.agc") == 0) {
            parm->stat.agc = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "stat.lopwr") == 0) {
            parm->stat.lopwr = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "noise.search") == 0) {
            parm->noise.search = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "noise.mean") == 0) {
            parm->noise.mean = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "channel") == 0) {
            parm->channel = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "bmnum") == 0) {
            parm->bmnum = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "bmazm") == 0) {
            parm->bmazm = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "scan") == 0) {
            parm->scan = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "rxrise") == 0) {
            parm->rxrise = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "intt.sc") == 0) {
            parm->intt.sc = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "intt.us") == 0) {
            parm->intt.us = cbor_get_uint32(pair.value);
        } else if (strcmp(key, "txpl") == 0) {
            parm->txpl = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "mpinc") == 0) {
            parm->mpinc = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "mppul") == 0) {
            parm->mppul = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "mplgs") == 0) {
            parm->mplgs = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "mplgexs") == 0) {
            parm->mplgexs = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "nrang") == 0) {
            parm->nrang = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "frang") == 0) {
            parm->frang = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "rsep") == 0) {
            parm->rsep = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "xcf") == 0) {
            parm->xcf = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "tfreq") == 0) {
            parm->tfreq = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "offset") == 0) {
            parm->offset = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "ifmode") == 0) {
            parm->ifmode = cbor_get_uint16(pair.value);
        } else if (strcmp(key, "mxpwr") == 0) {
            parm->mxpwr = cbor_get_uint32(pair.value);
        } else if (strcmp(key, "lvmax") == 0) {
            parm->lvmax = cbor_get_uint32(pair.value);
        } else if (strcmp(key, "pulse") == 0) {
            // Read pulse array
            if (cbor_isa_array(pair.value)) {
                size_t len = cbor_array_size(pair.value);
                parm->pulse = malloc(len * sizeof(uint16_t));
                if (parm->pulse) {
                    for (size_t j = 0; j < len; j++) {
                        parm->pulse[j] = cbor_get_uint16(cbor_array_get(pair.value, j));
                    }
                }
            }
        } else if (strcmp(key, "lag0") == 0) {
            // Read lag0 array
            if (cbor_isa_array(pair.value)) {
                size_t len = cbor_array_size(pair.value);
                parm->lag[0] = malloc(len * sizeof(uint16_t));
                if (parm->lag[0]) {
                    for (size_t j = 0; j < len; j++) {
                        parm->lag[0][j] = cbor_get_uint16(cbor_array_get(pair.value, j));
                    }
                }
            }
        } else if (strcmp(key, "lag1") == 0) {
            // Read lag1 array
            if (cbor_isa_array(pair.value)) {
                size_t len = cbor_array_size(pair.value);
                parm->lag[1] = malloc(len * sizeof(uint16_t));
                if (parm->lag[1]) {
                    for (size_t j = 0; j < len; j++) {
                        parm->lag[1][j] = cbor_get_uint16(cbor_array_get(pair.value, j));
                    }
                }
            }
        } else if (strcmp(key, "combf") == 0) {
            parm->combf = read_cbor_string(pair.value);
        }
/*
	else if (strcmp(key, "rng") == 0) {
         cbor_item_t *record = cbor_array_get(pair.value, i);
	 struct FitData  fit={0};
         
	}        
*/        free(key);
    }
}

// Function to read struct FitRange  from CBOR
void read_fit_range(cbor_item_t *map, struct FitRange  *rng) {
    for (size_t i = 0; i < cbor_map_size(map); i++) {
        struct cbor_pair pair = cbor_map_handle(map)[i];
        char *key = read_cbor_string(pair.key);
        
        if (strcmp(key, "v") == 0) {
            rng->v = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "v_err") == 0) {
            rng->v_err = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "p_0") == 0) {
            rng->p_0 = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "p_l") == 0) {
            rng->p_l = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "p_l_err") == 0) {
            rng->p_l_err = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "p_s") == 0) {
            rng->p_s = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "p_s_err") == 0) {
            rng->p_s_err = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "w_l") == 0) {
            rng->w_l = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "w_l_err") == 0) {
            rng->w_l_err = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "w_s") == 0) {
            rng->w_s = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "w_s_err") == 0) {
            rng->w_s_err = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "phi0") == 0) {
            rng->phi0 = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "phi0_err") == 0) {
            rng->phi0_err = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "sdev_l") == 0) {
            rng->sdev_l = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "sdev_s") == 0) {
            rng->sdev_s = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "sdev_phi") == 0) {
            rng->sdev_phi = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "qflg") == 0) {
            rng->qflg = cbor_get_uint32(pair.value);
        } else if (strcmp(key, "gsct") == 0) {
            rng->gsct = cbor_get_uint32(pair.value);
        } else if (strcmp(key, "nump") == 0) {
            rng->nump = cbor_get_uint8(pair.value);
        }
        
        free(key);
    }
}

// Function to read struct FitElv  from CBOR
void read_fit_elv(cbor_item_t *map, struct FitElv  *elv) {
    for (size_t i = 0; i < cbor_map_size(map); i++) {
        struct cbor_pair pair = cbor_map_handle(map)[i];
        char *key = read_cbor_string(pair.key);
        
        if (strcmp(key, "normal") == 0) {
            elv->normal = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "low") == 0) {
            elv->low = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "high") == 0) {
            elv->high = cbor_float_get_float4(pair.value);
        }
        
        free(key);
    }
}

// Function to read FitNoise from CBOR
void read_fit_noise(cbor_item_t *map, struct FitNoise *noise) {
    for (size_t i = 0; i < cbor_map_size(map); i++) {
        struct cbor_pair pair = cbor_map_handle(map)[i];
        char *key = read_cbor_string(pair.key);
        
        if (strcmp(key, "vel") == 0) {
            noise->vel = cbor_float_get_float(pair.value);
        } else if (strcmp(key, "skynoise") == 0) {
            noise->skynoise = cbor_float_get_float(pair.value);
        } else if (strcmp(key, "lag0") == 0) {
            noise->lag0 = cbor_float_get_float(pair.value);
        }
        
        free(key);
    }
}

// Function to read struct FitMode from CBOR
void read_fit_mode(cbor_item_t *map, struct FitMode *mode) {
    for (size_t i = 0; i < cbor_map_size(map); i++) {
        struct cbor_pair pair = cbor_map_handle(map)[i];
        char *key = read_cbor_string(pair.key);
        
        if (strcmp(key, "v") == 0) {
            mode->v = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "w") == 0) {
            mode->w = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "p") == 0) {
            mode->p = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "phi0") == 0) {
            mode->phi0 = cbor_float_get_float4(pair.value);
        } else if (strcmp(key, "p_rel") == 0) {
            mode->p_rel = cbor_float_get_float4(pair.value);
        }
        
        free(key);
    }
}

// Function to read struct FitData  from CBOR
void read_fit_data(cbor_item_t *map, struct FitData  *fit, struct RadarParm *prm, struct FitModeRng* ModesStruct) {
    // Initialize pointers to NULL
    fit->rng = NULL;
    fit->xrng = NULL;
    fit->elv = NULL;
// Have static defined sizes
//    ModesStruct->modes = NULL;
//    ModesStruct->modes_cnt = NULL;

    for (size_t i = 0; i < cbor_map_size(map); i++) {
        struct cbor_pair pair = cbor_map_handle(map)[i];
        char *key = read_cbor_string(pair.key);
        
        if (strcmp(key, "revision.major") == 0) {
            fit->revision.major = cbor_get_uint32(pair.value);
        } else if (strcmp(key, "revision.minor") == 0) {
            fit->revision.minor = cbor_get_uint32(pair.value);
        } else if (strcmp(key, "noise") == 0) {
            read_fit_noise(pair.value, &fit->noise);
        } else if (strcmp(key, "rng") == 0) {
            // Read range data array
            if (cbor_isa_array(pair.value)) {
                size_t len = cbor_array_size(pair.value);
                fit->rng = malloc(len * sizeof(struct FitRange ));
// Have static sizes
//            	ModesStruct->modes = malloc(len * sizeof(struct FitMode*));
//                ModesStruct->modes_cnt = malloc(len * sizeof(uint8_t));
                
                if (fit->rng && ModesStruct->modes && ModesStruct->modes_cnt) {
                    for (size_t j = 0; j < len; j++) {
                        cbor_item_t *range_map = cbor_array_get(pair.value, j);
                        
                        // Read the main range data
                        read_fit_range(range_map, &fit->rng[j]);
                        
                        // Initialize modes for this range
// Has static size
//                        ModesStruct->modes[j] = NULL;
                        ModesStruct->modes_cnt[j] = 0;
                        
                        // Check if this range has modes data
                        for (size_t k = 0; k < cbor_map_size(range_map); k++) {
                            struct cbor_pair rpair = cbor_map_handle(range_map)[k];
                            char *rkey = read_cbor_string(rpair.key);
                            
                            if (strcmp(rkey, "modes") == 0 && cbor_isa_array(rpair.value)) {
                                size_t num_modes = cbor_array_size(rpair.value);
// Has static size
//                                ModesStruct->modes[j] = malloc(num_modes * sizeof(struct FitMode));
                                ModesStruct->modes_cnt[j] = num_modes;
                                
                                if (ModesStruct->modes[j]) {
                                    for (size_t m = 0; m < num_modes; m++) {
                                        cbor_item_t *mode_map = cbor_array_get(rpair.value, m);
                                        read_fit_mode(mode_map, &ModesStruct->modes[j][m]);
                                    }
                                }
                            }
                            
                            free(rkey);
                        }
                    }
                }
            }
        } else if (strcmp(key, "xrng") == 0) {
            // Read xrange data array (similar to rng)
            if (cbor_isa_array(pair.value)) {
                size_t len = cbor_array_size(pair.value);
                fit->xrng = malloc(len * sizeof(struct FitRange ));
                if (fit->xrng) {
                    for (size_t j = 0; j < len; j++) {
                        read_fit_range(cbor_array_get(pair.value, j), &fit->xrng[j]);
                    }
                }
            }
        } else if (strcmp(key, "elv") == 0) {
            // Read elevation data array
            if (cbor_isa_array(pair.value)) {
                size_t len = cbor_array_size(pair.value);
                fit->elv = malloc(len * sizeof(struct FitElv ));
                if (fit->elv) {
                    for (size_t j = 0; j < len; j++) {
                        read_fit_elv(cbor_array_get(pair.value, j), &fit->elv[j]);
                    }
                }
            }
        }
        
        free(key);
    }
}

// Function to free struct RadarParm memory
void free_radar_parm(struct RadarParm *parm) {
    if (parm->origin.time) free(parm->origin.time);
    if (parm->origin.command) free(parm->origin.command);
    if (parm->pulse) free(parm->pulse);
    if (parm->lag[0]) free(parm->lag[0]);
    if (parm->lag[1]) free(parm->lag[1]);
    if (parm->combf) free(parm->combf);
}

// Function to free struct FitData  memory
void free_fit_data(struct FitData  *fit) {
    if (fit->rng) free(fit->rng);
    if (fit->xrng) free(fit->xrng);
    if (fit->elv) free(fit->elv);
    
/* 
// have static sizes
    if (fit->modes && fit->modes_cnt) {
        for (size_t i = 0; i < fit->modes_cnt[i]; i++) {
            if (fit->modes[i]) free(fit->modes[i]);
        }
        free(fit->modes);
        free(fit->modes_cnt);
    }
*/
}


// Function to process a single RadarParm record
void process_radar_parm(struct RadarParm *parm) {
    // Print basic information about the record
    printf("Radar Parameters Record:\n");
    printf("  Station ID: %d\n", parm->stid);
    printf("  Time: %04d-%02d-%02d %02d:%02d:%02d.%06d\n", 
           parm->time.yr, parm->time.mo, parm->time.dy,
           parm->time.hr, parm->time.mt, parm->time.sc, parm->time.us);
    printf("  Beam: %d, Channel: %d\n", parm->bmnum, parm->channel);
    printf("  Pulses: %d, Lags: %d, Ranges: %d\n", 
           parm->mppul, parm->mplgs, parm->nrang);
    printf("\n");
}

// Function to process a single FitData record
void process_fitdata(struct FitData *fit) {
    // Print basic information about the record
    printf("FitData Parameters Record:\n");
    printf("  Version: %d\n", fit->revision.major);
    printf("  Noise %f\n", fit->noise.skynoise);
    printf("  P0 %f\n", fit->rng[0].p_0);
    printf("\n");
}
// Function to process a single FitData record
void process_modes(struct FitModeRng *modes) {
    // Print basic information about the record
    for(int i=0;i<MAX_GATES;i++)
    {
     if(modes->modes_cnt[i]>0)
     {
      printf("%d gt. modes:\n",i);
      for(int j=0;j<modes->modes_cnt[i];j++)
       printf("v %.2f ", modes->modes[i][j].v);
      printf("\n");
     }
    }
    printf("\n");
}


// Main function to read CBOR file
int read_cbor_file(const char *filename, struct RadarParm *prm, struct FitData  *fit, struct FitModeRng *ModesStruct) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Failed to open file");
        return -1;
    }
    
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    unsigned char *buffer = malloc(size);
    if (!buffer) {
        fclose(f);
        return -1;
    }
    
    if (fread(buffer, size, 1, f) != 1) {
        free(buffer);
        fclose(f);
        return -1;
    }
    fclose(f);
    
    struct cbor_load_result result;
    cbor_item_t *root = cbor_load(buffer, size, &result);
    free(buffer);
    
    if (result.error.code != CBOR_ERR_NONE) {
        fprintf(stderr, "CBOR parsing failed: %s\n", result.error.code == CBOR_ERR_MALFORMATED ? "malformed data" : "not enough data");
        return -1;
    }
    
/*
      if (!cbor_isa_map(root)) {
        fprintf(stderr, "Expected CBOR map as root item\n");
        cbor_decref(&root);
        return -1;
    }
*/ 
    fprintf(stderr,"processing...\n");

    if (cbor_isa_map(root)) {
        // Single record case
//        fprintf(stderr,"single record - error\n");
	for (size_t i = 0; i < cbor_map_size(root)*2; i++) 
        {
/*
         struct cbor_pair pair = cbor_map_handle(root)[i];
         char *key = read_cbor_string(pair.key);
         printf("key %s\n",key);
*/
         cbor_item_t *record = cbor_array_get(root, i);
         if (cbor_isa_map(record)) {
    	    struct RadarParm parm = {0};
    	    struct FitData fit = {0};
    	    struct FitModeRng ModeStruct = {0};
    	    read_radar_parm(record, &parm);
    	    read_fit_data(record, &fit, &parm, &ModeStruct);
    	    process_radar_parm(&parm);
    	    process_fitdata(&fit);
    	    process_modes(&ModeStruct);
    	    free_radar_parm(&parm);
    	    free_fit_data(&fit);
//    	    cbor_decref(&record);
	 }
	}
    } else {
        fprintf(stderr, "Unexpected CBOR root item type\n");
        cbor_decref(&root);
        return -1;
    }
    cbor_decref(&root);
    return 0;


}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <cbor_file>\n", argv[0]);
        return 1;
    }
    
    struct RadarParm prm = {0};
    struct FitData  fit = {0};
    struct FitModeRng ModesStruct={0};

    if (read_cbor_file(argv[1], &prm, &fit, &ModesStruct) != 0) {
        return 1;
    }
    
    return 0;
}
