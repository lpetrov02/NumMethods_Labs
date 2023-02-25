CXXFLAGS = -Wall -Wextra -g -std=c++17
OBJECTS = obj/lab1_2.o obj/lab1_3.o obj/lab1_4.o obj/lab1_5.o obj/lab1_6

.PHONY: all clean

all: lab1_2 lab1_3 lab1_4 lab1_5 lab1_6

obj:
	mkdir obj
	chmod a=rwx obj

obj/lab1_2.o: src/lab1_2.cpp
	g++ $(CXXFLAGS) -c $< -o $@

obj/lab1_3.o: src/lab1_3.cpp
	g++ $(CXXFLAGS) -O0 -c $< -o $@

obj/lab1_4.o: src/lab1_4.cpp
	g++ $(CXXFLAGS) -O3 -march=native -fopenmp -c $< -o $@

obj/lab1_5.o: src/lab1_5.cpp
	g++ $(CXXFLAGS) -O3 -march=native -fopenmp -c $< -o $@

obj/lab1_6.o: src/lab1_6.cpp
	g++ $(CXXFLAGS) -c $< -o $@

lab1_2: obj obj/lab1_2.o
	g++ $(CXXFLAGS) -o $@ obj/lab1_2.o

lab1_3: obj obj/lab1_3.o
	g++ $(CXXFLAGS) -O0 -o $@ obj/lab1_3.o

lab1_4: obj obj/lab1_4.o
	g++ $(CXXFLAGS) -march=native -fopenmp -o $@ obj/lab1_4.o

lab1_5: obj obj/lab1_5.o
	g++ $(CXXFLAGS) -march=native -fopenmp -o $@ obj/lab1_5.o

lab1_6: obj obj/lab1_6.o
	g++ $(CXXFLAGS) -o $@ obj/lab1_6.o

clean :
	$(RM) -f -r obj lab1_2 lab1_3 lab1_4 lab1_5 lab1_6
