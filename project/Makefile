.PHONY=check,check_pr1,check_pr2,clean,debug,check-debug

INCLUDES:=-Ithird_party -Iincludes

CFLAGS:=
TESTFLAGS:=
host:=$(shell uname -s)
ifeq ($(host),Darwin)
	CC:=g++
else
	CC:=icpc
endif

COMPILE=$(CC) $(INCLUDES) $(CFLAGS)

all: | bin pr1 pr2

bin:
	mkdir bin

debug: CFLAGS += -DDEBUG
debug: all

check-debug: CFLAGS += -DDEBUG
check-debug: TESTFLAGS += -s
check-debug: check

pr1:

pr2: rk4 src/pr2_rk4.cpp
	$(COMPILE) -o bin/pr2 bin/rk4.o src/pr2_rk4.cpp
	@echo "./bin/pr2 is ready to be executed"

rk4: src/rk4.cpp
	$(COMPILE) -c src/rk4.cpp -o bin/rk4.o

clean: | bin
	rm -f bin/*
	rm -f test.csv

check: | bin check_pr1 check_pr2

check_pr1: rk4 test/*.cpp
	$(COMPILE) -Itest -o bin/testpr2 test/*.cpp bin/rk4.o
	@echo "running tests..."
	./bin/testpr2 $(TESTFLAGS)

check_pr2:
