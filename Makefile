CFLAGS?=-std=c99 -D_GNU_SOURCE -Wall -O2
CC		=gcc
LDLIBS= -lprotobuf-c

OBJS	= main.o proto/amessage.pb-c.o server.o lib.o client.o

default: all

all: bin main

bin:
	mkdir -p bin/

main: $(OBJS)
	$(CC) $(CFLAGS) -o bin/app $^ $(LDLIBS)

clean:
	rm -rf bin/ *.o proto/*.o

proto:
	proto-c --c_out=. proto/amessage.proto

%.o: %.c  proto/%.c
	$(CC) $(CFLAGS) -c bin/$<

