target = offChip sender receiver ./driver/driver.o

# CC = g++
# LD_LIBRARY = -Wl,--whole-archive -ldpdk -Wl,--no-whole-archive -mssse3 -lmlx4 -libverbs -lpthread -ldl -lm -lnuma
# LD_INCLUDE = -I/usr/local/include/dpdk/
# CFLAGS = -O3 #-DDEBUG

CC = g++
CFLAGS=$(shell pkg-config --cflags libdpdk librdmacm libibverbs) -O3 -g -fpermissive
LD_LIBRARYS=$(shell pkg-config --libs libdpdk librdmacm libibverbs) -L/
LD_INCLUDE = -I../

all: offChip sender receiver

../driver/driver.o: ./driver/driver.cc
	$(CC) $(CFLAGS) $(LD_INCLUDE) $^ -c -o $@  

offChip: offChip.cc ./driver/driver.o
	$(CC) $(CFLAGS) $(LD_INCLUDE) $^ -o $@ $(LD_LIBRARYS) 

sender: sender.cc ./driver/driver.o
	$(CC) $(CFLAGS) $(LD_INCLUDE) $^ -o $@ $(LD_LIBRARYS) 

receiver: receiver.cc ./driver/driver.o
	$(CC) $(CFLAGS) $(LD_INCLUDE) $^ -o $@ $(LD_LIBRARYS) 

.PHONY: clean
clean:
	-rm $(target)
