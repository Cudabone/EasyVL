#Compiler
CXX  = g++

CXXFLAGS = -g -Wall

# Source files
SRCS = ./src/main.cpp ./src/parser.cpp ./src/netlist.cpp ./src/net.cpp ./src/gate.cpp ./src/pin.cpp

# define any directories containing header files other than the default
INCLUDES = -Isrc

# define library paths in addition to the default
LFLAGS = 

# libraries to link into executable:
LIBS = 

# The object files
OBJS = $(SRCS:.cpp=.o)

# The executable
MAIN = ./myevl

all: $(MAIN) 
	@echo build complete

$(MAIN): $(OBJS) 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<  -o $@

.PHONY: tests clean debug

debug: $(FILES) 
	g++ $(FILES) -Wall -g -lm -o myevl

tests: $(FILES)
	@ ./myevl ./mytests/test1.evl
	@ ./myevl ./mytests/test2.evl
	@ ./myevl ./mytests/test3.evl
	@ ./myevl ./mytests/test4.evl
	@ ./myevl ./mytests/test5.evl
 
clean: 
	@rm -f ./myevl ./mytests/*.tokens ./mytests/*.syntax ./src/*.o
	@rm -rf ./myevl.dSYM

