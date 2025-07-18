CC = gcc 
RST = $(RSTPATH)
CFLAGS = -g -O0 -I$(RST)/include/superdarn -I$(RST)/include/base -I$(RST)/include/general -Iusr/include/superdarn -Ifit2cbor -Irecalc_elevation
LDFLAGS = -Lfit2cbor -Lrecalc_elevation -L$(RST)/lib -lcbor -lm -Llib -lz -Llib -ldmap.1 -lrcnv.1 -lfit.1 -lrscan.1 -lcfit.1 -lcnvmap.1 -lrtime.1 -lradar.1 -lraw.1 -laacgm.1 -lmlt.1 -lastalg.1 -lraw.1 -lfitacfex2.1 -lrmath.1 -llmfit.1 -lmpfit -lfitacf.1 -lfitacfex2.1
LDFLAGSP = -lpython3.12 -lm -L/usr/lib/python3.12/config-3.12-x86_64-linux-gnu
CFLAGSP = -I/usr/include/python3.12
CFLAGS2P = -I/usr/include/python3.12 -I/usr/include/python3.12  -fno-strict-overflow -Wsign-compare  -DNDEBUG -g -O2 -Wall
LDFLAGS2P = -L/usr/lib/python3.12/config-3.12-x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu  -ldl  -lm 

all:	main_raw_refit printcbor/printcbor

fit2cbor/fitacf2cbor_lib.o:
	cd fit2cbor && make clean all

printcbor/printcbor:
	cd printcbor && make clean all

recalc_elevation/recaliber_elevation.o:
	cd recalc_elevation && make clean all
recalc_elevation/read_config.o:
	cd recalc_elevation && make clean all

main_raw_refit: 	main_raw_refit.c fit2cbor/fitacf2cbor_lib.o recalc_elevation/recaliber_elevation.o recalc_elevation/read_config.o
	$(CC) -o main_raw_refit main_raw_refit.c fit2cbor/fitacf2cbor_lib.o recalc_elevation/recaliber_elevation.o recalc_elevation/read_config.o  -Wl,--start-group  $(CFLAGS) $(LDFLAGS)  $(CFLAGSP) $(LDFLAGSP) $(CFLAGS2P) $(LDFLAGS2P) 


clean:
	rm -f main_raw_refit
	cd fit2cbor && make clean
	cd printcbor && make clean
	cd recalc_elevation && make clean
