FILES = ./src/lex.cpp 

all: $(FILES) 
	g++ src/*.cpp -Wall -O2 -lm -o myevl

alldebug: $(FILES) 
	g++ src/*.cpp -Wall -g -lm -o myevl
 
clean: 
	rm myevl ./mytests/*.tokens
