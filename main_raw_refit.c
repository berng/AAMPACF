/// Fast version - first convert all rawacfs, than process them, than join the results
#include "main_raw_refit.h"
#include <Python.h>
#include <stdio.h>
#include <string.h>
// #include <sys/stat.h>
        
#include "main.h"
#include "read_config.h"
#include "radar_params.h"
#define  Re 6371
#include "recaliber_elevation.h"

radarConfigType radar_config;





struct RadarSite* site;
float time_from,time_to;

long outmode=3;

long AccLen=1;
//int SELECT_BEAM=0;
//int SELECT_CHANNEL=2;


static struct FitData fit;
static struct RawData raw_3[16];
static struct RawData raw_10[16];


#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

float** create_float_array(long N, int M) {
    // Allocate memory for the row pointers
    float** array = (float**)malloc(N * sizeof(float*));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for each row
    for (int i = 0; i < N; i++) {
        array[i] = (float*)malloc(M * sizeof(float));
        if (array[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for row %d\n", i);
            
            // Free previously allocated memory before exiting
            for (int j = 0; j < i; j++) {
                free(array[j]);
            }
            free(array);
            exit(EXIT_FAILURE);
        }
    }
    return array;
}


int printRaw(struct RadarParm *prm, 
	     struct RawData *raw, 
	     struct FitBlock *input,
	     struct FitData *fit,
	     char* out
	    )
 {
	long i,j;
//	FILE* stream;
	char fname[256];
	double ACF_RE[1000];
	double ACF_IM[1000];
	int ACF_N[1000];

	FitACF(prm,raw,input,fit);
	sprintf(out,"");
	for(j=0;j<prm->nrang;j++)
	{
		 double max;
		 for(i=0;i<1000;i++)
			ACF_RE[i]=
			ACF_IM[i]=
			ACF_N[i]=0;
			
		for(i=0;i<prm->mplgs;i++)
		{
			ACF_RE[prm->lag[1][i] - prm->lag[0][i]]+=raw->acfd[0][i+j*prm->mplgs];
			ACF_IM[prm->lag[1][i] - prm->lag[0][i]]+=raw->acfd[1][i+j*prm->mplgs];
			ACF_N [prm->lag[1][i] - prm->lag[0][i]]+=1;
		}
//	FILE* out;
//	if (fit->rng[j].qflg==1)
/*	if (fit->rng[j].p_l>3)
	    out=stdout;
	else
	    out=stderr;
*/
//     if(fit->rng[j].p_0>3 && fit->rng[j].qflg==1)
     
     if(fit->rng[j].p_0>3 && fit->rng[j].qflg==1)
      {
//        char* outstart;
//	outstart=out+strlen(out);
	sprintf(out+strlen(out),"%d ",prm->bmnum);   
	sprintf(out+strlen(out),"%d ",prm->channel);   
	sprintf(out+strlen(out),"%d ",prm->tfreq);   

	float float_time=(double)prm->time.hr+(double)prm->time.mt/60.+(double)prm->time.sc/3600.;
	sprintf(out+strlen(out),"%lf ",float_time);
	sprintf(out+strlen(out),"%.0lf ",(double)prm->frang+j*(double)prm->rsep);
	sprintf(out+strlen(out),"%le ",fit->rng[j].p_0);
	sprintf(out+strlen(out),"%.0lf ",(double)fit->rng[j].v);
	sprintf(out+strlen(out),"%.0lf\t",(double)fit->rng[j].w_l);
	sprintf(out+strlen(out),"%d\t\t",fit->rng[j].qflg);   
//	printf("%s\n",outstart);


	for(i=0;i<60;i++)
		{
		 int mask=0;
		 if(ACF_N[i]>0)
		    mask=1;
		  sprintf(out+strlen(out),"%.1f %.3f %d\t",
					(mask==1)?ACF_RE[i]/ACF_N[i]:0, 
					(mask==1)?ACF_IM[i]/ACF_N[i]:0, 
					mask);
		}
		 sprintf(out+strlen(out),"\n");
	}
      }
}



int main(int argc,char *argv[]) 
{
 
 int prm_time_hr_old;
 FILE *fp;

 struct RadarParm *prm;
 struct RawData *raw;
 struct RawData *raw_acc;
 time_t start,stop;
 DIR* dp;
 struct dirent* fent;
 char fname[254];
 char CBOR_FILENAME[1024];

 long accumul=0;
 config_record_type* config_record;
 long config_record_len;

 if(argc<2)
   {
    fprintf(stderr,"usage: %s rawfilein hdw_datafile calibration_file \n",argv[0]);
    exit(1);
   }

/// read configuration files for radar geometry (hdw) and elevation calibration (cal)
 sprintf(fname,"%s",argv[1]);
 read_radar_config(&radar_config,argv[2]);
 read_config_phase(&config_record,&config_record_len,argv[3]);




 sprintf(CBOR_FILENAME,"%s.cbor",fname);
// fprintf(stderr,"beam selected: %d channel: %d\n",SELECT_BEAM,SELECT_CHANNEL);
 fprintf(stderr,"check %s ...\n",fname);
 fp=fopen(fname,"r");
 if (fp==NULL)
  fprintf(stderr,"File %s not found.\n",fent->d_name), exit(-1);

 prm=RadarParmMake();
 raw=RawMake();

 if(fit.rng==NULL)
   {
    fit.rng=(struct FitRange *)calloc(100000,sizeof(struct FitRange));
    fit.xrng=(struct FitRange *)calloc(100000,sizeof(struct FitRange));
    fit.elv=(struct FitElv *)calloc(100000,sizeof(struct FitElv));
   }

 long ss;
 struct FitBlock input;
 input.prm.lag[0]=NULL;
 input.prm.lag[1]=NULL;
 input.prm.vdir=1.;
 input.prm.pwr0=NULL;
 input.acfd=NULL;
 input.xcfd=NULL;
 input.prm.pulse=NULL;
 long i;
 FILE* out;
 FILE* output;


 output=fopen(CBOR_FILENAME,"wt");



/// Load python AAMPACF module
 PyObject *pName, *pModule, *pFunc;
 PyObject *pArgs, *pValue;
    
    // Initialize the Python interpreter
 Py_Initialize();
    
    // Add the current directory to Python path
 PyRun_SimpleString("import sys");
 PyRun_SimpleString("sys.path.append(\".\")");
    
    // Import the module
 pName = PyUnicode_DecodeFSDefault("aampacf");
 pModule = PyImport_Import(pName);
 Py_DECREF(pName);
    
 if (pModule != NULL) {
        // Get the function
        pFunc = PyObject_GetAttrString(pModule, "AAMPACF");
    }
 else
    {
	fprintf(stderr,"module not loaded, exiting\n");
        exit(1);
    }


 char* buf_ptr;
#define CACHING_SIZE 1000000000
 char* input_buffer=NULL;

 input_buffer=(char*)calloc(CACHING_SIZE,sizeof(char)); //1kB per measurement, 100 lags, twice for problems //1GB cache
 if(!input_buffer)
  {
	fprintf(stderr,"buffer allocation error\n");
        exit(1);

  }
 buf_ptr=input_buffer;
 int block_count = 0;

/// first itteration - prepare rawacfs and calculate AAMPACF - to speedup the process, it uses effective parallel calculations
 start=time(NULL);
 while(RawFread(fp,prm,raw)!=-1) 
   {
    if (buf_ptr-input_buffer>=CACHING_SIZE)
    {
     fprintf(stderr,"too small cache, increase CACHING_SIZE\n");
     exit(1);
    }
    block_count++;
//    printf("processed block: %d\n",block_count);
     if(raw->acfd[0]==NULL)
     {
      RawSetACF(raw,prm->nrang,prm->mplgs,NULL,0,NULL);
      RawSetXCF(raw,prm->nrang,prm->mplgs,NULL,0,NULL);
      for(i=0;i<prm->nrang*prm->mplgs;i++)
        raw->acfd[0][i]=
	raw->acfd[0][i]=
        raw->xcfd[0][i]=
	raw->xcfd[0][i]=0;
     }
     if(raw->pwr0==NULL)
      raw->pwr0=malloc(prm->nrang*sizeof(float));
     printRaw(prm,raw,&input,&fit,buf_ptr);
     buf_ptr+=strlen(buf_ptr);
//     if(block_count>20)
//      break;
    }
 stop=time(NULL);
 printf("total cache used %ld, %ld blocks time: %.2lf\n",strlen(input_buffer),block_count,(double)difftime(stop,start));
 fflush(stdout);
 const  char *result;

 start=time(NULL);


    if (pFunc && PyCallable_Check(pFunc)) 
     {
            // Prepare the argument
            pArgs = PyTuple_New(1);
            pValue = PyUnicode_FromString(input_buffer);
            if (!pValue) {
                Py_DECREF(pArgs);
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return 1;
            }
            
            // pValue reference stolen here
            PyTuple_SetItem(pArgs, 0, pValue);
            
            // Call the function
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            
            if (pValue != NULL) {
                // Convert the result to a C string
                result = PyUnicode_AsUTF8(pValue);
        //        printf("Result from Python:\n%s\n", result);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr, "Call failed\n");
                return 1;
            }
        }
    else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"hello\"\n");
        }
  stop=time(NULL);
 printf("aampacf processed, time: %.2lf\n",(double)difftime(stop,start));
 fflush(stdout);

// get parameters array
#define MAX_GATES 100
#define MAX_MODES 10

     int aamp_cnt=0;
     long totalReturnedResults;

     totalReturnedResults=(long)block_count*(long)MAX_GATES;
     float** params;


     params=create_float_array(totalReturnedResults*(long)MAX_MODES, 14);

     char* curpos;
     curpos=result;
//     printf("RES:%s\n",result);
//     exit(1);
     for(int ii=0,pos=0;ii<totalReturnedResults;ii++)
     {
      char* nextpos;
      int fnd=-1;
      nextpos=strstr(curpos,"\n");
      if(nextpos!=NULL)
      {
      fnd=sscanf(curpos,"%f%f%f%f%f%f%f%f%f%f%f%f%f%f",
	&(params[ii][0]),
	&(params[ii][1]),
	&(params[ii][2]),
	&(params[ii][3]),
	&(params[ii][4]),
	&(params[ii][5]),
	&(params[ii][6]),
	&(params[ii][7]),
	&(params[ii][8]),
	&(params[ii][9]),
	&(params[ii][10]),
	&(params[ii][11]),
	&(params[ii][12]),
	&(params[ii][13])
	);
//       printf("%d %p %p\n",fnd,curpos,nextpos);
       if (fnd<14)
        break;
       curpos=nextpos+1;
      } 
      if (fnd<14)
        break;
      aamp_cnt=ii;
//      printf("aampcnt: %ld\n",aamp_cnt);
     }

 printf("bcnt %ld\n",block_count);
 cbor_item_t *root = cbor_new_definite_map(block_count);
 printf("root %p\n",root);
 printf("aamp cnt %ld\n",aamp_cnt);
 printf("results %ld\n",strlen(result));

 if (pValue)
     Py_DECREF(pValue);

// exit(1);







/// Second itteration - calibrate phases, calculate elevations, join AAMPACF with FITACF and write to CBOR
 rewind(fp);
 int blocks_total;
 blocks_total=block_count;

 start=time(NULL);


 long long int curdate;
 double D,H;

 while(RawFread(fp,prm,raw)!=-1) 
   {


    /// Get calibration phase difference 
    curdate=(long long int)prm->time.yr;
    curdate=curdate*100+(long long int)prm->time.mo;
    curdate=curdate*100+(long long int)prm->time.dy;
    curdate=curdate*100+(long long int)prm->time.hr;
    curdate=curdate*100+(long long int)prm->time.mt;
//     fprintf(stderr,"%d ",prm.channel);
    double phase=WRONG_PHASE;

    phase=read_phase(curdate,prm->channel,prm->tfreq,config_record,config_record_len,&D,&H);

    block_count--;
//    printf("processed blocks rest: %.2f",((float)blocks_total-(float)block_count)/(float)blocks_total);
//    fflush(stdout);
     if(raw->acfd[0]==NULL)
     {
      RawSetACF(raw,prm->nrang,prm->mplgs,NULL,0,NULL);
      RawSetXCF(raw,prm->nrang,prm->mplgs,NULL,0,NULL);
      for(i=0;i<prm->nrang*prm->mplgs;i++)
        raw->acfd[0][i]=
	raw->acfd[0][i]=
        raw->xcfd[0][i]=
	raw->xcfd[0][i]=0;
     }
     if(raw->pwr0==NULL)
      raw->pwr0=malloc(prm->nrang*sizeof(float));

    FitACF(prm,raw,&input,&fit);

// Add RadarParm to CBOR
    cbor_item_t *parm_map = cbor_new_definite_map(MAX_GATES); // Adjust based on actual field count
    write_radar_parm(parm_map, prm);

//    fprintf(stderr,"NRNG2: %d",prm.nrang);
    struct FitModeRng modes;
    float float_time=(double)prm->time.hr+(double)prm->time.mt/60.+(double)prm->time.sc/3600.;

    for(int i=0;i<prm->nrang;i++)
      {

	/// recaliber phase and calculate elevation for the data
        float cal_elev_front=-1,cal_elev_back=-1;
	long min_n=10;
	long max_n=-10;
	double expected_elev=-1;
	double expected_phi=-666;
	double corrected_phi=-666;

	//process only well fitted by FITACF data - save time only
	if(fit.xrng && fit.rng[i].qflg>0 &&  fit.xrng[i].phi0_err>0 )
        {
	 min_n=recaliber_elevation(prm,&fit,i,phase,&cal_elev_front,&cal_elev_back,&max_n,&expected_elev,&expected_phi,&corrected_phi);
        }
        //set fit elv array
	if(fit.elv)
          {
           fit.elv[i].normal=cal_elev_front;
           fit.elv[i].high=cal_elev_back;
           fit.elv[i].low=(float)min_n;
          }



       modes.modes_cnt[i]=0;
       for(int ii=0;ii<aamp_cnt;ii++)
        if((long)(params[ii][3])==prm->frang+i*prm->rsep 
		&& params[ii][4]>0.3 /// limit mode amplitudes - use only high-power modes >0.3 from main 
		&& params[ii][2]==float_time 
		&& (int)(params[ii][1])==prm->channel 
		&& (int)(params[ii][0])==prm->bmnum 
		)
         {
/*
	    printf("t: %f %f c: %d %d b: %d %d r:%ld %ld q:%d\n",
		params[ii][2],float_time,
		(int)(params[ii][1]),prm->channel,
		(int)(params[ii][0]),prm->bmnum,
		(long)(params[ii][3]),prm->frang+i*prm->rsep,
		fit.rng[i].qflg
		);
*/
	  if(fit.elv)
          {
          fit.elv[i].normal=cal_elev_front;
          fit.elv[i].high=cal_elev_back;
          fit.elv[i].low=(float)min_n;
          }
	  modes.modes[i][modes.modes_cnt[i]].v=params[ii][6];
          modes.modes[i][modes.modes_cnt[i]].w=params[ii][7];
          modes.modes[i][modes.modes_cnt[i]].p=params[ii][5];
          modes.modes[i][modes.modes_cnt[i]].p_rel=params[ii][4];
          modes.modes[i][modes.modes_cnt[i]].phi0=corrected_phi;
          modes.modes[i][modes.modes_cnt[i]].elev_front=cal_elev_front;
          modes.modes[i][modes.modes_cnt[i]].elev_back=cal_elev_back;
          modes.modes_cnt[i]++;
         }
      }
    write_fit_data(parm_map, &fit, prm, &modes);
    char recname[100];
//    sprintf(recname,"rprm%d",block_count);
    sprintf(recname,"%.4d-%.2d-%.2d %.2d:%.2d:%.2d c%d",
             prm->time.yr,prm->time.mo,prm->time.dy,
             prm->time.hr,prm->time.mt,prm->time.sc,prm->channel);
    cbor_map_add(root, (struct cbor_pair){
            .key = cbor_move(cbor_build_string(recname)),
            .value = cbor_move(parm_map)
        });
    if (block_count<=0)
     break;
   }
 stop=time(NULL);
 printf("results processed, time: %.2lf\n",(double)difftime(stop,start));
 fflush(stdout);

printf("DONE!\n");
//exit(1);
fflush(stdout);
    // Write CBOR to output
    unsigned char *buffer;
    size_t buffer_size;
    cbor_serialize_alloc(root, &buffer, &buffer_size);
    fwrite(buffer, 1, buffer_size, output);
    free(buffer);
    cbor_decref(&root);
  fclose(output);

  Py_XDECREF(pFunc);
  Py_DECREF(pModule);
  Py_Finalize();

  exit(1);

  fclose(fp);

  return 0;
}


 







       

