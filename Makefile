all:
	gcc mainadd.c biginteger.c linkedlist.c -Wall -g -O3 -o mainadd
	gcc mainmul.c biginteger.c linkedlist.c -Wall -g -O3 -o mainmul
	gcc mainsub.c biginteger.c linkedlist.c -Wall -g -O3 -o mainsub
	gcc maindiv.c biginteger.c linkedlist.c -Wall -g -O3 -o maindiv
clean: 
	rm -rf *.out
