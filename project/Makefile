.PHONY=check,check_pr1,check_pr2,clean

INCLUDES=-Ithird_party -Iincludes

CFLAGS:=""
TESTFLAGS:=""
#CFLAGS:=-DDEBUG
#TESTFLAGS:=-s
COMPILE:=g++ $(INCLUDES) $(CFLAGS)

all: pr1 pr2

pr1:

pr2: rk4 src/pr2_rk4.cpp
	@$(COMPILE) -o bin/pr2 bin/rk4.o src/pr2_rk4.cpp
	@echo "./bin/pr2 is ready to be executed"

rk4: src/rk4.cpp
	@$(COMPILE) -c src/rk4.cpp -o bin/rk4.o

clean:
	@rm -f bin/*
	@rm -f test.csv

check: check_pr1 check_pr2

check_pr1: rk4 test/*.cpp
	@$(COMPILE) -Itest -o bin/testpr2 test/*.cpp bin/rk4.o
	@echo "running tests..."
	@./bin/testpr2 $(TESTFLAGS)

check_pr2:
