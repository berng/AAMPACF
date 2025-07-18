
/// Prepare data for calibration
#include "main.h"

#define LightSpeed 3e8
#define PI 3.1415926
#define MAX(x,y) ((x>y)?x:y)

#define Pi 3.1415926
#define RAD2DEG (180./Pi)
#define DEG2RAD (Pi/180.)

/*
double SQR(double x)
{ 
 return ((x)*(x));
}
*/


int main(int argc,char *argv[]) {
  FILE *fp;
  struct RadarParm prm;
  struct RadarParm *PRM;
  struct FitData fit;
  struct FitData *FIT;
  char* radarname;
//  double Xlat0=(double)56.5,Xlong0=(double)58.5;
  float DayNo=0;
  long SELECT_BEAM=0;


  
  if(argc<2)
   {
      fprintf(stderr,"usage %s fname\n",argv[0]);
      exit(1);
   }
  fp=fopen(argv[1],"r");
  char* date;
  date=argv[2];

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
        && r<410
//        && r<350
	&& fabs((double)fit.rng[i].v) < 50
       )
       {
         double h;
         h=sin((double)fit.elv[i].normal*3.14/180.)*r+r*r/(2.*6371.);

double myelev;
double R,Re;
double altitude,altitude_corrected;
R=(double)prm.frang+(double)prm.rsep*i+(double)prm.rsep/2.;
Re=6731.;
double A0,A1,A2;
//first approximation
A0=0.; 
A1=0.;
A2=0.;
//second approximation
/*
A0=-5.92e-5; 
A1=3.86e-3;
A2=-5.07;
*/

altitude=-17*log(((float)prm.tfreq)/1000.)/log(10.)+124;   // Model height - gives 104-106km
//altitude=110.;
//altitude=90.;
#define SQR(x) ((x)*(x))
altitude_corrected=altitude-(A0*SQR(R-405.)+A1*(R-405.)+A2);
//fprintf(stderr,"alt: %f alt corrected: %f\n",altitude,altitude_corrected);
myelev=asin((SQR(Re+altitude_corrected)-(SQR(Re)+SQR(R)))/(2.*Re*R));
double el2;
el2=atan2(altitude_corrected,sqrt(R*R-altitude_corrected*altitude_corrected));
	printf(	"%f %f %ld "
		"%f %ld "
		"1.0 1.0 0.0 "
		"%ld %s %f "
		"%f "
		"%d "
//		"  e: %f %f %f %f "
		"\n",
    			(double)fit.xrng[i].phi0,
			R,
    			(long)prm.bmnum,

			myelev,
    			(long)prm.tfreq,

			(long)altitude_corrected,
			date,
			el2,
    			(float)prm.bmazm,
    			(int)prm.channel
		);
	    
//       printf("%ld %lf\n",i,elv_out[i]);
	}
      }
   }
  } 
  fclose(fp);
  

}

