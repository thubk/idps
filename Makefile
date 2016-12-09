CPP = g++ -std=gnu++11
CFLAGS		= -g
KERNEL_DIR	=	
HTTP		= 	../libraries/http/libchilkat-9.5.0.a -lresolv -lpthread
LIBS		=	/usr/local/pfring/lib/libpfring.so -lpthread
LIBPCAP_DIR	=	/usr/local/pfring/lib//libpcap.so -lrt
INCLUDE		=	-I$(KERNEL_DIR) -I$(LIBPCAP_DIR) -I/usr/local/include
LIBPFRING	=	libpfring.a
RANLIB		=	ranlib
OBJ			=	MurmurHash3.o BiCountSketch.o CountMinSketch.o Masters.o BloomFilter.o pfringcpp.o Bucket.o master.o slave.o protocols.o
all: $(LIBPFRING) main

main: main.cpp $(LIBPFRING)
	$(CPP) $(INCLUDE) $< $(LIBPFRING) -o $@ $(LIBS) $(HTTP) $(LIBPCAP_DIR)
	
$(LIBPFRING):$(OBJ)
		@rm -f $@
		ar rc $@ $(OBJ) $(LIBPFRING)
		$(RANLIB) $@	

MurmurHash3.o: MurmurHash3.cpp MurmurHash3.h
	$(CPP) $(CFLAGS) -c $(INCLUDE) $<
	
BiCountSketch.o: BiCountSketch.cpp BiCountSketch.h
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<
	
BloomFilter.o: BloomFilter.cpp BloomFilter.h
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<
	
CountMinSketch.o: CountMinSketch.cpp CountMinSketch.h
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<

pfringcpp.o: pfringcpp.cpp
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<

Bucket.o: Bucket.cpp Bucket.h
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<
	
Masters.o: Masters.cpp Masters.h
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<
	
master.o: master.cpp
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<
	
slave.o: slave.cpp
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<
	
protocols.o: protocols.cpp protocols.h
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<
	
clean:
		/bin/rm -f $(TARGET) *.o *~ $(LIBPFRING) main