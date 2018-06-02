tsort: tsort.c
	gcc -o tsort  tsort.c -l pthread

all: tsort

clean:
	-rm -f *.o  tsort
