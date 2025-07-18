#include "main.h"

#define LightSpeed 3e8
#define PI 3.1415926
#define MAX(x,y) ((x>y)?x:y)

#define Pi 3.1415926
#define RAD2DEG (180./Pi)
#define DEG2RAD (Pi/180.)
#include "read_config.h"
radarConfigType radar_config;

#include "radar_params.h"
#define  Re 6371
#include "recaliber_elevation.h"



int main(int argc,char *argv[]) {
  FILE *fp;
  struct RadarParm prm;
  struct RadarParm *PRM;
  struct FitData fit;
  struct FitData *FIT;
  char* radarname;
  double Xlat0=(double)56.5,Xlong0=(double)58.5;
  float DayNo=0;
  long SELECT_BEAM=0;


  config_record_type* config_record;
  long config_record_len;
  double phase=WRONG_PHASE;



  
  if(argc<2)
   {
      fprintf(stderr,"usage %s fname outfname radar_hdw_file radar_cal_file\n",argv[0]);
      exit(1);
   }
  fp=fopen(argv[1],"r");
  long CHANNEL=-1;//atol(argv[2]);
  long BEAM=-1;//atol(argv[3]);
  FILE* fpout;
  if(argv[2])
   fpout=fopen(argv[2],"wb");

  if (fp==NULL) 
   {
    fprintf(stderr,"File %s not found.\n",argv[1]);
    exit(-1);
   }

  read_radar_config(&radar_config,argv[3]);

  read_config_phase(&config_record,&config_record_len,argv[4]);


  PRM=RadarParmMake();
  FIT=FitMake();
  prm=*PRM;
  fit=*FIT;

  int init=0;

double range;
double phi0;
double TdiffNew=0.0;
double PrmIntX=0.0; 

int front=1;
double alias=0.0;
double elv_out[1000];
double j=0;
  while(FitFread(fp,&prm,&fit) !=-1) 
   {
     long long int curdate;

     curdate=(long long int)prm.time.yr;
     curdate=curdate*100+(long long int)prm.time.mo;
     curdate=curdate*100+(long long int)prm.time.dy;
     curdate=curdate*100+(long long int)prm.time.hr;
     curdate=curdate*100+(long long int)prm.time.mt;
//     fprintf(stderr,"%d ",prm.channel);
     double D,H;
     phase=read_phase(curdate,prm.channel,prm.tfreq,config_record,config_record_len,&D,&H);
     if(phase<=WRONG_PHASE)
        fprintf(stderr,"no phase error! cdate:%lld channel: %d freq: %d\n",curdate,prm.channel,prm.tfreq);
     if(fit.elv!=NULL)
       {
    
long i;

     for(i=0;i<prm.nrang;i++)
      {
       fit.elv[i].normal=
       fit.elv[i].high=
       fit.elv[i].low=0.;
       
       if(
        //berng fit.rng[i].p_l>3 && 
        fit.rng[i].qflg>0 && 
        fit.xrng[i].phi0_err>0 
//berng  && prm.channel==CHANNEL 
//       && (long)prm.bmnum==BEAM
       )
       {
        float cal_elev_front=-1,cal_elev_back=-1;
	long min_n=10;
	long max_n=-10;
	double expected_elev=-1;
	double expected_phi=-666;
	double corrected_phi=-666;
	min_n=recaliber_elevation(&prm,&fit,i,phase,&cal_elev_front,&cal_elev_back,&max_n,&expected_elev,&expected_phi,&corrected_phi);

	if(min_n<10)
	{
	    fit.elv[i].normal=cal_elev_front;
    	    fit.elv[i].high=cal_elev_back; /// elevation for backlobe
    	    fit.elv[i].low=(float)min_n;
    	}
	char substring[255];
	substring[0]=0;
        range=(double)prm.frang+(double)prm.rsep*i;
	if(min_n<10 
	 && range<350
//	 && (long)prm.bmnum >=5 && (long)prm.bmnum <=10
    		
	 )
         printf("%lf"
        	"\t%.0lf\t%.3lf"
        	"\t%ld %ld %ld"
        	"\t%.0lf %.0lf %.0lf"
        	"\t%ld"
        	"\t%lf %lf"
        	"\t%lf %lf %lf"
        	"\t%.0lf %ld"
        	"\t%lf %lf %lf"
        	"\t%lf"
        	"\t%s"
        	"\t%f"
        	"\t%lf %lf %ld"
        	"\t\n",
			(double)prm.time.hr+(double)prm.time.mt/60.+(double)prm.time.sc/3600.,

    			(double)prm.frang+(double)prm.rsep*i,
    			cal_elev_front,

    			(long)prm.channel,
    			(long)prm.tfreq,
    			(long)prm.bmnum,

    			(double)fit.rng[i].p_l,
    			(double)fit.rng[i].v,
    			(double)fit.rng[i].w_l,

    			(long)fit.rng[i].gsct,
    			
    			expected_phi,
    			corrected_phi,

    			fit.elv[i].high,
    			fit.elv[i].low,

    			fit.elv[i].normal,    			
    			sin(cal_elev_front*3.14/180.)*(range+(double)prm.rsep/2.)+(range*range)/(2.*Re),
    			min_n,
    			
    			fit.xrng[i].phi0,
    			fit.rng[i].phi0,
    			phase,
    			
    			phase,
    			substring,
			expected_elev*180./3.14,

			cal_elev_back,
    			sin(cal_elev_back*3.14/180.)*(range+(double)prm.rsep/2.)+(range*range)/(2.*Re),
    			max_n

    			);

	}
      }
     }
    if(fpout)
     FitFwrite(fpout,&prm,&fit);
   }
 
  fclose(fp);
  if(fpout)
   fclose(fpout);
  

}

