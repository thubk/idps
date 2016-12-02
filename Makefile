CPP = g++ -std=gnu++11
CFLAGS		= -g
KERNEL_DIR	=	../libraries/kernel
LIBPCAP_DIR	=	/usr/local/lib
INCLUDE		=	-I$(KERNEL_DIR) -I$(LIBPCAP_DIR) -I/usr/local/include
LIBPFRING	=	libpfring.a
RANLIB		=	ranlib
OBJ			=	MurmurHash3.o BidirectionalCountSketch.o CountMinSketch.o BloomFilter.o pfringcpp.o Bucket.o BucketCMS.o master.o slave.o protocols.o
LIBS		=	/usr/local/lib/libpfring.so -lpthread
all: $(LIBPFRING) main

main: main.cpp $(LIBPFRING)
	$(CPP) $(INCLUDE) $< $(LIBPFRING) -o $@ $(LIBS) $(LIBPCAP_DIR)/libpcap.so -lrt
	
$(LIBPFRING):$(OBJ)
		@rm -f $@
		ar rc $@ $(OBJ) $(LIBPFRING)
		$(RANLIB) $@	

MurmurHash3.o: MurmurHash3.cpp MurmurHash3.h
	$(CPP) $(CFLAGS) -c $(INCLUDE) $<
	
CountMinSketch.o: CountMinSketch.cpp CountMinSketch.h
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<
	
BidirectionalCountSketch.o: BidirectionalCountSketch.cpp BidirectionalCountSketch.h
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<
	
BloomFilter.o: BloomFilter.cpp BloomFilter.h
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<

pfringcpp.o: pfringcpp.cpp
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<

Bucket.o: Bucket.cpp Bucket.h
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<
	
BucketCMS.o: BucketCMS.cpp BucketCMS.h
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<
	
master.o: master.cpp
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<
	
slave.o: slave.cpp
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<
	
protocols.o: protocols.cpp protocols.h
	$(CPP) $(CFLAGS) -c  $(INCLUDE) $<
	
clean:
		/bin/rm -f $(TARGET) *.o *~ $(LIBPFRING) main