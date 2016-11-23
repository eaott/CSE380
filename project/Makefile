.PHONY=check,check_pr1,check_pr2,clean

INCLUDES=-Ithird_party -Iincludes

# THIS DOESN'T WORK --- try to use --debug in `make` instead...
ifdef DEBUG
CFLAGS:=-DDEBUG
else
CFLAGS:=
endif
#CFLAGS:=-DDEBUG
COMPILE:=g++ $(INCLUDES) $(CFLAGS)

all: pr1 pr2

pr1:

pr2: rk4 src/pr2_rk4.cpp
	@$(COMPILE) -o bin/pr2 bin/rk4.o src/pr2_rk4.cpp

rk4: src/rk4.cpp
	@$(COMPILE) -c src/rk4.cpp -o bin/rk4.o

clean:
	@rm -f bin/*
	@rm -f test.csv

check: check_pr1 check_pr2

check_pr1: rk4 test/rk4_test.cpp
	@$(COMPILE) -Itest -o bin/testpr2 test/main_test.cpp bin/rk4.o test/rk4_test.cpp
	@./bin/testpr2 -s

check_pr2:
