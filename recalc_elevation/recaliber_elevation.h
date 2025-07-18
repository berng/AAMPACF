#ifndef __RECAL_ELEV_H__
#define __RECAL_ELEV_H__

#include "main.h"

// #define LightSpeed 3e8
// #define PI 3.1415926
// #define MAX(x,y) ((x>y)?x:y)

// #define Pi 3.1415926
// #define RAD2DEG (180./Pi)
// #define DEG2RAD (Pi/180.)
// #include "read_config.h"
#include "read_config.h"
extern radarConfigType radar_config;
#include "radar_params.h"

#define  Re 6371
#define WRONG_RESULT -999.0

double get_expected_phase(double Az,double El,double Lambda,double D,double H);

double phi_to_elev(double phi,double Az,double Lambda,double D,double H);
long recaliber_elevation(struct RadarParm *prm, ///sounding parameters
			struct FitData *fit,	///fit array
			int i,			///element to fit in fit array
			float dphase,		///calibration coefficient to phase
			float* calibrated_elev_front, /// calibrated elevation in case it is front lobe
			float* calibrated_elev_back, /// calibrated elevation in case it is back lobe
			long* max_n,		 /// calibrated optimal number of cycles
			double* expected_elev,	 /// expected elevation in case it is meteors
			double* expected_phi,	/// expected phase in case it is meteors
			double* corrected_phi); /// corrected phase to use for calculating elevation
#endif
