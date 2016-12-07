CPP = g++ -std=gnu++11
CFLAGS		= -g
KERNEL_DIR	=	../libraries/kernel
LIBPCAP_DIR	=	../libraries/libpcap
HTTP		= 	../libraries/http/libchilkat-9.5.0.a -lresolv -lpthread
INCLUDE		=	-I$(KERNEL_DIR) -I$(LIBPCAP_DIR) -I/usr/local/include
LIBPFRING	=	libpfring.a
RANLIB		=	ranlib
OBJ			=	MurmurHash3.o BiCountSketch.o CountMinSketch.o Masters.o BloomFilter.o pfringcpp.o Bucket.o master.o slave.o protocols.o
LIBS		=	../libraries/lib/libpfring.a -lpthread
all: $(LIBPFRING) main

main: main.cpp $(LIBPFRING)
	$(CPP) $(INCLUDE) $< $(LIBPFRING) -o $@ $(LIBS) $(HTTP) $(LIBPCAP_DIR)/libpcap.a -lrt
	
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