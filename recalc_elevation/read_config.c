#include "read_config.h"

void read_radar_config(radarConfigType* config,char* RADAR_CFG_FILE)
{
 FILE* stream;
 stream=fopen(RADAR_CFG_FILE,"rt");
 if (!stream)
  {
   fprintf(stderr,"can not open file %s\n",RADAR_CFG_FILE);
   exit(1);
  }
 fscanf(stream,"%f%f%f%f%f%f%ld%f",
	&(config->lat),&(config->lon),
	&(config->mean_az),&(config->az_step),
	&(config->DX),&(config->DY),
	&(config->beams),&(config->phase_sign));
 fclose(stream);
}

long read_config_phase(config_record_type** config_record,long* config_record_len,char* CONFIG_FILE)
{
   char tmp[255];

 FILE* fstream;
 fstream=fopen(CONFIG_FILE,"rt");
 if(!fstream)
  return 0;
// ========== read reader =======
 fscanf(fstream,"%s%s%s%s%s%s%s%s",tmp,tmp,tmp,tmp,tmp,tmp,tmp,tmp);
 (*config_record_len)=0;
 double D,H;
 char rad[256];

 for(;!feof(fstream);)
  {
   int chan,fmin,fmax;
   long long dstart;
   double dphase;
   int n=0;
   n=fscanf(fstream,"%d%d%d%lld%lf%lf%lf%s",&chan,&fmin,&fmax,&dstart,&dphase,&D,&H,rad);
//   printf("n: %d\n",n);
   if(n<5)
     break;
   (*config_record_len)++;
  }
//printf("CRL: %ld",*config_record_len);
 *config_record=(config_record_type*)calloc((*config_record_len)+1,sizeof(config_record_type));
 if(!config_record)
    {
     fprintf(stderr,"cannot allocate configrecord\n");
     return 0;
    }
 rewind(fstream);
 fscanf(fstream,"%s%s%s%s%s%s%s%s",tmp,tmp,tmp,tmp,tmp,tmp,tmp,tmp);
 int config_record_pos=0;
// ========== read lines =======
 for(;!feof(fstream);)
  {
   int chan,fmin,fmax;
   long long dstart;
   double dphase;
   int n=0;
   n=fscanf(fstream,"%d%d%d%lld%lf%lf%lf%s",&chan,&fmin,&fmax,&dstart,&dphase,&D,&H,rad);
//   printf("n: %d\n",n);
   if(n<5)
     break;
   (*config_record)[config_record_pos].chan=chan;
   (*config_record)[config_record_pos].fmin=fmin;
   (*config_record)[config_record_pos].fmax=fmax;
   (*config_record)[config_record_pos].dstart=dstart;
   (*config_record)[config_record_pos].dphase=dphase;
   (*config_record)[config_record_pos].D=D;
   (*config_record)[config_record_pos].H=H;
   config_record_pos++;
  }
 fclose(fstream);
// printf("CRP: %d\n",config_record_pos);

 *config_record_len=config_record_pos;
 return *config_record_len;
}

double read_phase(long long date,int channel,int freq,
		    config_record_type* config_record,
		    long config_record_len,double* D,double *H)
{
   char tmp[255];

  double phase=WRONG_PHASE;

// ========== read lines =======
 int i;
 for(i=0;i<config_record_len;i++)
  {
//   printf("i: %d\n",i);
   if(config_record[i].chan==channel 
	&& freq>=config_record[i].fmin 
	&& freq<=config_record[i].fmax)
   {
    if(date<config_record[i].dstart)
     break;
    phase=config_record[i].dphase;
/*
    printf("fnd c:%d f:%d-%d d:%lld p:%lf\n",
		config_record[i].chan,
		config_record[i].fmin,
		config_record[i].fmax,
		config_record[i].dstart,
		config_record[i].dphase);
*/
   }
  }
 if(phase<=WRONG_PHASE)
  {
   for(i=0;i<config_record_len;i++)
    {
     fprintf(stderr,"i: %d ch:%d fr: %d check: [%d,%d,%d] \n",
    		i,channel,freq,
    		config_record[i].chan,config_record[i].fmin,config_record[i].fmax);
     if(config_record[i].chan==channel 
	&& freq>=config_record[i].fmin 
	&& freq<=config_record[i].fmax)
     {
      if(date<config_record[i].dstart)
      {
       fprintf(stderr,"date:%lld< config dstart:%lld, break\n",date,config_record[i].dstart);
       break;
      }
      phase=config_record[i].dphase;
      *D=config_record[i].D;
      *H=config_record[i].H;
      
      fprintf(stderr,"cur phase :%f ...",phase);
      fprintf(stderr,"fnd c:%d f:%d-%d d:%lld p:%lf\n",
		config_record[i].chan,
		config_record[i].fmin,
		config_record[i].fmax,
		config_record[i].dstart,
		config_record[i].dphase);
     }
    }
  }
 return phase;
}



/*
int main()
{
config_record_type* config_record;
long config_record_len;


 double phase=WRONG_PHASE;
 read_config_phase(&config_record,&config_record_len);
// read_config_phase(&config_record_len);
 phase=read_phase(202101022359,1,10200,config_record,config_record_len);
 printf("phase res: %lf\n",phase);
 phase=read_phase(202101032359,1,10200,config_record,config_record_len);
 printf("phase res: %lf\n",phase);
 phase=read_phase(202101012359,1,10200,config_record,config_record_len);
 printf("phase res: %lf\n",phase);
 free(config_record);
}

*/