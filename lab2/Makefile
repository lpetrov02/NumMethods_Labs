CXXFLAGS = -Wall -Wextra -g -std=c++17
OBJECTS = obj/det.o obj/multidim.o

.PHONY: all clean

all: det multidim
obj:
	mkdir obj
	chmod a=rwx obj

obj/det.o: src/det.cpp
	g++ $(CXXFLAGS) -c $< -o $@

obj/multidim.o: src/multidim.cpp
	g++ $(CXXFLAGS) -c $< -o $@

det: obj obj/det.o
	g++ $(CXXFLAGS) -o $@ obj/det.o
	
multidim: obj obj/multidim.o
	g++ $(CXXFLAGS) -o $@ obj/multidim.o

clean :
	$(RM) -f -r obj multidim
