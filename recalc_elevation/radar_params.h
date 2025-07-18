/// almost any radar
#define RADAR_GLAT 	(radar_config.lat)
#define RADAR_GLON	(radar_config.lon)
#define AZIMUTH_CENTER 	(radar_config.mean_az)
#define CENTRAL_BEAM	(((float)(radar_config.beams-1))/2.)
#define MAX_BEAMS	(radar_config.beams)
#define STEP_BEAM	(radar_config.az_step)

#define RADAR_DX 	(radar_config.DX)
#define RADAR_DY 	(radar_config.DY)
#define RADAR_DZ 	0

#define RADAR_PHASE_SIGN 	(radar_config.phase_sign)
