all: GenericHashTable.c GenericHashTable.h main.c
	gcc -g GenericHashTable.c main.c -o ex1 -Wall -Wvla

all-GDB: GenericHashTable.c GenericHashTable.h main.c
	gcc -g GenericHashTable.c main.c -o ex1 -Wall -Wvla
	