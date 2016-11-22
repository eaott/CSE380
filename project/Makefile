.PHONY=check,check_pr1,clean

INCLUDES=-Ithird_party -Iincludes

all: pr1 pr2

pr1:

pr2: rk4 src/pr2_rk4.cpp
	g++ $(INCLUDES) -o bin/pr2 bin/rk4.o src/pr2_rk4.cpp

rk4: src/rk4.cpp
	g++ $(INCLUDES) -c src/rk4.cpp -o bin/rk4.o

clean:
	rm -f bin/*
	rm -f test.csv

check: check_pr1 check_pr2

check_pr1: rk4 test/pr2_rk4_test.cpp
	g++ $(INCLUDES) -o bin/testpr2 bin/rk4.o test/pr2_rk4_test.cpp
	./bin/testpr2

check_pr2:
