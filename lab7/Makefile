CXXFLAGS = -Wall -Wextra -g -std=c++17
OBJECTS = obj/1.o

.PHONY: all clean

all: 1
obj:
	mkdir obj
	chmod a=rwx obj

obj/1.o: src/1.cpp
	g++ $(CXXFLAGS) -c $< -o $@

1: obj obj/1.o
	g++ $(CXXFLAGS) -o $@ obj/1.o

clean :
	$(RM) -f -r 1
