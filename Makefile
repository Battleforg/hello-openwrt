OBJS = parser.o listener.o saveXML.o upload.o delete.o
CC = gcc
CFLAGS = -Wall -g
INCLUDE = -I/usr/local/include
LDFLAGS = -L/usr/local/lib

listener : $(OBJS)
	$(CC) $(OBJS) -o listener $(INCLUDE) $(CFLAGS) $(LDFLAGS) -lpcap -lcurl
listener.o : listener.c listener.h
	$(CC) $(CFLAGS) -c listener.c -o listener.o
parser.o : parser.c listener.h
	$(CC) $(CFLAGS) -c parser.c -o parser.o
saveXML.o : saveXML.c saveXML.h
	$(CC) $(CFLAGS) -c saveXML.c -o saveXML.o
upload.o : upload.c upload.h
	$(CC) $(CFLAGS) -c upload.c -o upload.o
delete.o : delete.c delete.h
	$(CC) $(CFLAGS) -c delete.c -o delete.o

clean:
	rm -rf *.o listener data/hotspot/*.xml data/station/*.xml