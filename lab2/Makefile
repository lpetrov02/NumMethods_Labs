CXXFLAGS = -Wall -Wextra -g -std=c++17
OBJECTS = obj/det.o

.PHONY: all clean

all: det
obj:
	mkdir obj
	chmod a=rwx obj

obj/det.o: src/det.cpp
	g++ $(CXXFLAGS) -c $< -o $@

det: obj obj/det.o
	g++ $(CXXFLAGS) -o $@ obj/det.o

clean :
	$(RM) -f -r obj det
