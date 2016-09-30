FILES = ./src/syn.cpp 

all: $(FILES) 
	g++ $(FILES) -Wall -O2 -lm -o myevl

debug: $(FILES) 
	g++ $(FILES) -Wall -g -lm -o myevl

tests: $(FILES)
	@ ./myevl ./mytests/test1.evl
	@ ./myevl ./mytests/test2.evl
	@ ./myevl ./mytests/test3.evl
	@ ./myevl ./mytests/test4.evl
	@ ./myevl ./mytests/test5.evl
 
clean: 
	rm myevl ./mytests/*.tokens ./mytests/*.syntax
