first_program: first_program.o
	gcc first_program.o -o first_program -lncurses

first_program.o: first_program.c
	gcc -c first_program.c -o first_program.o -lncurses

clean:
	rm -rf *.o
