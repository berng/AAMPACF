#include "recaliber_elevation.h"
double get_expected_phase(double Az,double El,double Lambda,double D,double H)
 {
  double res;
/// according to Shepherd//RS,2017
  res=2.*3.1415*1.0/Lambda*(D*sqrt(cos(Az)*cos(Az)-sin(El)*sin(El))+H*sin(El));
  return res;
 }

double phi_to_elev(double phi,double Az,double Lambda,double D,double H)
{
 double M=phi/(2.*3.1415)*Lambda/D;
 double det=(cos(Az)*cos(Az)-M*M)*(1+H*H/(D*D))+M*M*H*H/(D*D);
 if(det>0.)
  {
   double res=(sqrt(det)+H/D)/(1+H*H/(D*D));
   if (fabs(res)<=1)
    return asin(res);
   else
    return WRONG_RESULT;
  }
 return WRONG_RESULT;
}

long recaliber_elevation(struct RadarParm *prm,
			struct FitData *fit,
			int i,
			float dphase,
			float* calibrated_elev_front, 
			float* calibrated_elev_back,
			long* max_n,
			double* expected_elev,
			double* expected_phi,
			double* corrected_phi)
{
/// returns min_n [-5,5) or 10 in case of not found
	*calibrated_elev_front=-1;
    	*calibrated_elev_back=-1;
	*max_n=-10;

	double range;
    	double A;
//    	double B,C,expected_phi;
    	double D=0;
    	double H=0;//-1.2;  ///RawACF reverse sign for interferometer, we must revers height from -3.8 to 3.8
	D=RADAR_DX; 
	H=RADAR_DY;
	long min_n=10;
	
	double D1,beam;
	double Lambda;
	Lambda=150000./(double)prm->tfreq*2.0;

	beam=(double)prm->bmnum;

	A=dphase; 
//    	B=0;
//    	C=0;
        double phi0=(RADAR_PHASE_SIGN<0)?-fit->xrng[i].phi0:fit->xrng[i].phi0;

	double BeamWidth=STEP_BEAM;
	double Az;

	Az=(beam-CENTRAL_BEAM)*BeamWidth*3.1415/180.;
	D1=D*cos(Az);

	range=prm->frang+prm->rsep*(double)i;

/// debug: expected phases for meteors
	double altitude=104.;
	*expected_elev=asin((((Re+altitude)*(Re+altitude)-(Re*Re+range*range))/(2.*Re*range)));  
/*
	double expected_elev_plain;
	expected_elev_plain=atan2(altitude,sqrt(range*range-altitude*altitude));
*/
	*expected_phi=get_expected_phase(Az,*expected_elev,Lambda,D,H);


	double NormD1Lambda;
	NormD1Lambda=D1/Lambda;//-floor(D1/Lambda); /// Fix according to Shepherd 2017
	
	
//	double corrected_phi;
	double fr_norm;
	fr_norm=(double)prm->tfreq/10000.;

	*corrected_phi=phi0-A;

	double calibrated_elev;
	double tmp_corr_phi;
	long n;
	double min_calibrated_elev=100;
	double min_tmp_corr_phi=100;

	double max_calibrated_elev=-100;
	double max_tmp_corr_phi=-100;
	for(n=5;n>-5;n--)
	{
	 tmp_corr_phi=*corrected_phi+(double)n*2.*3.1415;
	 calibrated_elev=phi_to_elev(tmp_corr_phi,Az,Lambda,D,H);

        if(
    	calibrated_elev > WRONG_RESULT
        && range>0. 
    	&& beam>=0 && beam<=MAX_BEAMS
        )
        {
         if(calibrated_elev<min_calibrated_elev)
          {
           min_calibrated_elev=calibrated_elev;
           min_tmp_corr_phi=tmp_corr_phi;
           min_n=n;
          }

         if(calibrated_elev>max_calibrated_elev)
          {
           max_calibrated_elev=calibrated_elev;
           max_tmp_corr_phi=tmp_corr_phi;
           *max_n=n;
          }

        }
        }

	if(min_n<10)
	{
	    *calibrated_elev_front=min_calibrated_elev*180./3.14;
    	    *calibrated_elev_back=90-max_calibrated_elev*180./3.14; /// elevation for backlobe
    	    return min_n;
    	}

    return 10;

}
