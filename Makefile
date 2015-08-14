OBJS = parser.o listener.o saveXML.o upload.o delete.o threads.o
CC = gcc
CFLAGS = -Wall -g
INCLUDE = -I/usr/local/include
LDFLAGS = -L/usr/local/lib

listener : $(OBJS)
	$(CC) $(OBJS) -o listener $(INCLUDE) $(CFLAGS) $(LDFLAGS) -lpcap -lcurl -lpthread -lm
listener.o : listener.c listener.h
	$(CC) $(CFLAGS) -c listener.c -o listener.o
parser.o : parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c -o parser.o
saveXML.o : saveXML.c saveXML.h
	$(CC) $(CFLAGS) -c saveXML.c -o saveXML.o
upload.o : upload.c upload.h
	$(CC) $(CFLAGS) -c upload.c -o upload.o
delete.o : delete.c delete.h
	$(CC) $(CFLAGS) -c delete.c -o delete.o
threads.o : threads.c parser.h
	$(CC) $(CFLAGS) -c threads.c -o threads.o
clean:
	rm -rf *.o listener /tmp/group2/data/hotspot/*.xml /tmp/group2/data/station/*.xml /tmp/group2/data/*.xml /tmp/group2/zip/*.zip