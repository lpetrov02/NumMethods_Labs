CXXFLAGS = -Wall -Wextra -g -std=c++17
OBJECTS = obj/1.o obj/2.0 obj/3.o

.PHONY: all clean

all: 1 2 3
obj:
	mkdir obj
	chmod a=rwx obj

obj/1.o: src/1.cpp
	g++ $(CXXFLAGS) -c $< -o $@

obj/2.o: src/2.cpp
	g++ $(CXXFLAGS) -c $< -o $@

obj/3.o: src/3.cpp
	g++ $(CXXFLAGS) -c $< -o $@

1: obj obj/1.o
	g++ $(CXXFLAGS) -o $@ obj/1.o

2: obj obj/2.o
	g++ $(CXXFLAGS) -o $@ obj/2.o

3: obj obj/3.o
	g++ $(CXXFLAGS) -o $@ obj/3.o

clean :
	$(RM) -f -r 1 2 3
