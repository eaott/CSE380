.PHONY=check,check_pr1,check_pr2,clean,debug,check-debug

INCLUDES:=-Ithird_party -Iincludes


CFLAGS:= -lgsl -lgslcblas
TESTFLAGS:=
host:=$(shell uname -s)
ifeq ($(host),Darwin)
	CC:=g++
	HDF_INSTALL=/Users/Evan/hpc_codes/installed/hdf5-1.8.18
	HDF5_LIB:=-L$(HDF_INSTALL)/lib
	HDF5_INC:=-I$(HDF_INSTALL)/include
	CFLAGS += $(HDF5_LIB) $(HDF5_INC) $(HDF_INSTALL)/lib/libhdf5.a -lz -lm
else
	CC:=icpc
	# FIXME look into this
	CFLAGS += -I$$TACC_GSL_INC -L$$TACC_GSL_LIB
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
	$(COMPILE) -o bin/pr1 src/gsl_euler.cpp

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
