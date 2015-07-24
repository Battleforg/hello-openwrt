all: listener.c
	gcc -g -Wall -o listener listener.c -lpcap
clean:
	rm -rf *.o listener *.xml