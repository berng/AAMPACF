#include "main.h"

#define LightSpeed 3e8
#define PI 3.1415926
#define MAX(x,y) ((x>y)?x:y)

#define Pi 3.1415926
#define RAD2DEG (180./Pi)
#define DEG2RAD (Pi/180.)




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


  
  if(argc<2)
   {
      fprintf(stderr,"usage %s fname\n",argv[0]);
      exit(1);
   }
  fp=fopen(argv[1],"r");
  
  if (fp==NULL) 
   {
    fprintf(stderr,"File %s not found.\n",argv[1]);
    exit(-1);
   }

  PRM=RadarParmMake();
  FIT=FitMake();
  prm=*PRM;
  fit=*FIT;

  int init=0;

double range;
double phi0;
double TdiffNew=0.0;
double PrmIntX=0.0; 
// EKB
// double PrmIntY=-58.0; 
// double PrmIntZ=-8.0;

// MAGW
//double PrmIntY=56.18; 
//double PrmIntZ=-0.5;
double PrmIntY=89.6; 
double PrmIntZ=0;

int front=1;
double alias=0.0;
double elv_out[1000];
double j=0;
  while(FitFread(fp,&prm,&fit) !=-1) 
   {
//    fprintf(stderr,"reading.. %ld..\n",prm.nrang);
       if(fit.elv!=NULL)
       {

long i;

     for(i=0;i<prm.nrang;i++)
      {
         double r;
         r=(double)prm.frang+(double)prm.rsep*i+45./2.;

       if(
        //berng fit.rng[i].p_l>3 && 
        fit.rng[i].qflg>0 
        &&
        (double)fit.elv[i].normal>0
//        && r<225
        && r<410
	&& fabs((double)fit.rng[i].v) < 50
//        && (long)prm.channel == 1
//    	&& (double)fit.rng[i].p_l > 10
       )
       {
         double h;
         h=sin((double)fit.elv[i].normal*3.14/180.)*r+r*r/(2.*6371.);
         printf("%lf"
        	"\t%.0lf"
        	"\t%ld %ld %ld"
        	"\t%.0lf %.0lf %.0lf"
        	"\t%ld"
        	"\t%lf %lf %lf"
        	"\t%lf "
        	"\t%lf %lf "
        	"\n",
			(double)prm.time.hr+(double)prm.time.mt/60.+(double)prm.time.sc/3600.,

    			(double)prm.frang+(double)prm.rsep*i,

    			(long)prm.channel,
    			(long)prm.tfreq,
    			(long)prm.bmnum,

    			(double)fit.rng[i].p_l,
    			(double)fit.rng[i].v,
    			(double)fit.rng[i].w_l,

    			(long)fit.rng[i].gsct,

    			(double)fit.elv[i].normal,
    			(double)fit.elv[i].low,
    			(double)fit.elv[i].high,
    			h,
    			(double)fit.xrng[i].phi0,
    			(double)fit.rng[i].phi0


    			);

//       printf("%ld %lf\n",i,elv_out[i]);
	}
      }
   }
  } 
  fclose(fp);
  

}

