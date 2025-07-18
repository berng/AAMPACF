#include "fitacf2cbor.h"

int main(int argc,char *argv[]) {
 
  FILE *fp;

  struct RadarParm prm;
  struct RadarParm *PRM;
  
  struct FitData fit;
  struct FitData *FIT;

  if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file.h5>\n", argv[0]);
        return 1;
    }

    // Initialize HDF5
  FILE *output = fopen(argv[2], "wb");
  if (output < 0) {
        fprintf(stderr, "Error creating CBOR file\n");
        return 1;
    }

    // Open input file
  FILE *infile = fopen(argv[1], "rb");
  if (!infile) {
        perror("Error opening input file");
        fclose(output);
        return 1;
    }




  PRM=RadarParmMake();
  FIT=FitMake();
  prm=*PRM;
  fit=*FIT;

  int block_count = 1;
  cbor_item_t *root = cbor_new_definite_map(block_count);
 
  while(FitFread(infile,&prm,&fit) !=-1) {
//    cbor_item_t *root = cbor_new_definite_map(1);
    if(block_count--<0) break;
//    print_radar_parm(&prm);
//    printFitData(prm,fit);
    int nrang = prm.nrang;
//    print_fit_data(&fit, nrang);
//    block_count++;

    fprintf(stdout,"%.4d-%.2d-%.2d %.2d:%.2d:%.2d\n",
             prm.time.yr,prm.time.mo,prm.time.dy,
             prm.time.hr,prm.time.mt,prm.time.sc);

//  }
//    char group_name[256];
//    snprintf(group_name, sizeof(group_name), "Block_%d", block_count++);
//    snprintf(group_name, sizeof(group_name), "%s c%d", prm.origin.time+3,prm.channel);
//    hid_t group_id=0;

        // Add RadarParm to CBOR
    cbor_item_t *parm_map = cbor_new_definite_map(10000); // Adjust based on actual field count
//    fprintf(stderr,"NRNG: %d",prm.nrang);
    write_radar_parm(parm_map, &prm);
//    fprintf(stderr,"NRNG2: %d",prm.nrang);
    struct FitModeRng modes;
    for(int i=0;i<prm.nrang;i++)
      {
       modes.modes_cnt[i]=2;
       modes.modes[i][0].v=i;
       modes.modes[i][0].w=i*i;
       modes.modes[i][0].p=i*i*i;
       modes.modes[i][0].p_rel=(float)i*0.01;
       modes.modes[i][0].phi0=(float)i*0.02;
       modes.modes[i][0].elev_front=(float)i*0.12;
       modes.modes[i][0].elev_back=(float)i*0.13;
      }
    write_fit_data(parm_map, &fit, &prm, &modes);
    char recname[100];
//    sprintf(recname,"rprm%d",block_count);
    sprintf(recname,"%.4d-%.2d-%.2d %.2d:%.2d:%.2d c%d",
             prm.time.yr,prm.time.mo,prm.time.dy,
             prm.time.hr,prm.time.mt,prm.time.sc,prm.channel);
    cbor_map_add(root, (struct cbor_pair){
            .key = cbor_move(cbor_build_string(recname)),
            .value = cbor_move(parm_map)
        });
        


    

    }
    // Cleanup
    // Write CBOR to output
    unsigned char *buffer;
    size_t buffer_size;
    cbor_serialize_alloc(root, &buffer, &buffer_size);
    fwrite(buffer, 1, buffer_size, output);
    free(buffer);
    cbor_decref(&root);
  fclose(infile);
  fclose(output);

  fprintf(stdout,"stop...\n");


  return 0;
}
