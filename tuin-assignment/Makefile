CPP=g++
CPPFLAGS=-std=c++14

src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

LDFLAGS = -lncurses

all: clean build run 

build: gameoflife

gameoflife: $(obj)
	$(CPP) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) gameoflife

run:
	./gameoflife

