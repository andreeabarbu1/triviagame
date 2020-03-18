build: trivia.c
	gcc trivia.c -Wall -lncurses -o trivia 
run: build
	./trivia 4 fisier1 fisier2 fisier3
clean:
	rm trivia
