FLAGS = -W -Wall -O3 -std=c++14 -pthread
DEFINE = -DW=4 -DH=5

PROGNAME = exe
HEADERS = Board.hpp Node.hpp
SOURCES = Board.cpp Node.cpp main.cpp

all: $(SOURCES) $(HEADERS)
	g++ -o $(PROGNAME) $(FLAGS) $(SOURCES) $(DEFINE)

run:
	@./exe

clean:
	@rm exe
