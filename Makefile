CFLAGS?=-std=c99 -D_GNU_SOURCE -Wall -O2 -g
CC		= gcc
LDLIBS= -lprotobuf-c

OBJS	= main.o server.o client.o
LIBS  = lib.o proto/amessage.pb-c.o
HTTP	= http_server.o http_client.o http_main.o http_header_parser.o

default: all

all: bin main http httpTest

bin:
	mkdir -p bin/

main: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) -o bin/app $^ $(LDLIBS)

clean:
	rm -rf bin/ *.o proto/*.o

proto:
	proto-c --c_out=. proto/amessage.proto

%.o: %.c  proto/%.c
	$(CC) $(CFLAGS) -c bin/$<

http: $(HTTP) $(LIBS)
	$(CC) $(CFLAGS) -o bin/http_server $^ $(LDLIBS)

httpTest: $(LIBS) http_test.o http_header_parser.o
	$(CC) $(CFLAGS) -o bin/http_test $^ $(LDLIBS)
