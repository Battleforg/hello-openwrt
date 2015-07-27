OBJS = parser.o listener.o saveXML.o
CC = gcc
CFLAGS = -Wall -O -g

listener : $(OBJS)
	$(CC) $(OBJS) -o listener -lpcap
listener.o : listener.c listener.h
	$(CC) $(CFLAGS) -c listener.c -o listener.o -lpcap
parser.o : parser.c listener.h
	$(CC) $(CFLAGS) -c parser.c -o parser.o -lpcap
saveXML.o : saveXML.c saveXML.h
	$(CC) $(CFLAGS) -c saveXML.c -o saveXML.o
clean:
	rm -rf *.o *.xml rssi 