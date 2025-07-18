CC = gcc -DKERFIX
RST=$(RSTPATH)
CFLAGS = -I$(RST)/include/superdarn -I$(RST)/include/base -I$(RST)/include/general -I/usr/include -Iusr/include/superdarn 
# -I./src
LDFLAGS = -lm -L$(RST)/lib -lz -lfit.1 -ldmap.1 -laacgm.1 -lradar.1 -lrcnv.1 -lcfit.1 -lraw.1 -lrscan.1 -lrtime.1 -lgfortran
FFLAGS = -freal-4-real-8
#LDFLAGS = -lm -Llib -lz

all:	preparer viewer recalc_elevation_any_radar_hdw  # get_meteors_distribution
# recalc_elevation_DAVIT_linear

read_config.o:	read_config.c read_config.h
	$(CC) -c -o read_config.o read_config.c $(CFLAGS) $(LDFLAGS) 

recaliber_elevation.o:	recaliber_elevation.c recaliber_elevation.h
	$(CC) -c -o recaliber_elevation.o recaliber_elevation.c $(CFLAGS) $(LDFLAGS) 

recalc_elevation_any_radar_hdw:	recalc_elevation_any_radar_hdw.c read_config.o recaliber_elevation.o
	$(CC) -o recalc_elevation_any_radar_hdw recalc_elevation_any_radar_hdw.c read_config.o recaliber_elevation.o $(CFLAGS) $(LDFLAGS) 
#	rm -f *.o


viewer:	viewer.c
	$(CC) -o viewer viewer.c  $(CFLAGS) $(LDFLAGS)
#	rm -f *.o

preparer:	preparer.c
	$(CC) -o preparer preparer.c  $(CFLAGS) $(LDFLAGS)

get_meteors_distribution:	get_meteors_distribution.c
	$(CC) -o get_meteors_distribution get_meteors_distribution.c  $(CFLAGS) $(LDFLAGS)



distclean:
	rm -f *.o

clean:
	rm -f *.o recalc_elevation_any_radar_hdw viewer preparer get_meteors_distribution
