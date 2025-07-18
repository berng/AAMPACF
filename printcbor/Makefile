RST = $(RSTPATH)
CFLAGS = -I$(RST)/include/superdarn -I$(RST)/include/base -I$(RST)/include/general -I../ -I../fit2cbor
LDFLAGS = -Wl,--start-group -L$(RST)/lib -lcbor -lm -Llib -lz -Llib -ldmap.1 -lrcnv.1 -lfit.1 -lrscan.1 -lcfit.1 -lcnvmap.1 -lrtime.1 -lradar.1 -lraw.1 -laacgm.1 -lmlt.1 -lastalg.1 -lraw.1 -lfitacfex2.1 -lrmath.1 -llmfit.1 -lmpfit -lfitacf.1 

all:	print_cbor

print_cbor: print_cbor.c  print_cbor.h
	gcc print_cbor.c -o print_cbor  $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o print_cbor
