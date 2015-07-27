all: listener.c parser.c
	gcc -g -Wall -o listener listener.c parser.c -lpcap
clean:
	rm -rf *.o listener *.xml