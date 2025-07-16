all : program testing # builds all programs

program : 2048_main.o 2048_funcs.o # builds just the main program
	gcc -o program 2048_main.o 2048_funcs.o -g

2048_main.o : 2048_main.c 2048.h # builds binary file for main 
	gcc -c 2048_main.c

2048_funcs.o : 2048_funcs.c 2048.h # builds binary file for functions
	gcc -c 2048_funcs.c

testing : 2048_testing.o 2048_funcs.o # builds just the testing program
	gcc -o testing 2048_testing.o 2048_funcs.o -g
