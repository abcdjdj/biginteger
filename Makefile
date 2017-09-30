all:
	gcc main.c biginteger.c linkedlist.c -Wall -g -o main
clean: 
	rm -rf *.out
