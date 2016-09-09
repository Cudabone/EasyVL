FILES = ./src/lex.cpp 

all: $(FILES) 
	g++ src/*.cpp -Wall -O2 -lm -o myevl

debug: $(FILES) 
	g++ src/*.cpp -Wall -g -lm -o myevl
tests: $(FILES)
	@ ./myevl ./mytests/test.evl
	@ ./myevl ./mytests/test2.evl
	@ ./myevl ./mytests/test3.evl
	@ ./myevl ./mytests/test4.evl
	@ ./myevl ./mytests/test5.evl
	@ ./myevl ./mytests/test6.evl
 
clean: 
	rm myevl ./mytests/*.tokens
