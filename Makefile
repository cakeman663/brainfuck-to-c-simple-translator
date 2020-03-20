all:
	gcc -Wall -c -g *.c
	gcc -o example *.o -lm
	rm *.o
