RST = $(RSTPATH)
CFLAGS = -I$(RST)/include/superdarn -I$(RST)/include/base -I$(RST)/include/general 
LDFLAGS = -Wl,--start-group -L$(RST)/lib -lcbor -lm -Llib -lz -Llib -ldmap.1 -lrcnv.1 -lfit.1 -lrscan.1 -lcfit.1 -lcnvmap.1 -lrtime.1 -lradar.1 -lraw.1 -laacgm.1 -lmlt.1 -lastalg.1 -lraw.1 -lfitacfex2.1 -lrmath.1 -llmfit.1 -lmpfit -lfitacf.1 

all:	fitacf2cbor

fitacf2cbor: fitacf2cbor.c  fitacf2cbor.h fitacf2cbor_lib.o
	gcc fitacf2cbor.c fitacf2cbor_lib.o -o fitacf2cbor  $(CFLAGS) $(LDFLAGS)

fitacf2cbor_lib.o: fitacf2cbor_lib.c  fitacf2cbor.h
	gcc fitacf2cbor_lib.c -c -o fitacf2cbor_lib.o  $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o fitacf2cbor
