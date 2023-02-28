build: main.o
	gcc -o sudoku -Wall main.c

main.o: main.c jsmn.h
	gcc -c -o main.o -Wall main.c

.PHONY: clean run

clean:
	rm main.o sudoku
	
run:
	./sudoku 