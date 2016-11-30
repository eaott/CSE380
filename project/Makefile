INCLUDES:=-Ithird_party -Iincludes


CFLAGS:= -lgsl -lgslcblas -lz -lm -lhdf5 -lhdf5_cpp
TESTFLAGS:=
host:=$(shell uname -s)
ifeq ($(host),Darwin)
	CC:=g++
	HDF_INSTALL=/Users/Evan/hpc_codes/installed/hdf5-1.8.18
	HDF5_LIB:=$(HDF_INSTALL)/lib
	HDF5_INC:=$(HDF_INSTALL)/include
	CFLAGS += -L$(HDF5_LIB) -I$(HDF5_INC) -L$(HDF_INSTALL)/lib/libhdf5.a
else
	CC:=icpc
	# FIXME look into this
	CFLAGS += -I$$TACC_GSL_INC -L$$TACC_GSL_LIB
	CFLAGS += -I$$TACC_HDF5_INC -L$$TACC_HDF5_LIB -L$$TACC_HDF5_LIB/libhdf5.a
endif

COMPILE=$(CC) $(INCLUDES) $(CFLAGS)

all: bin
all: runner

bin:
	mkdir $@

bin/rk.o: src/rk.cpp
	$(COMPILE) -c src/rk.cpp -o $@

runner: bin/rk.o src/pr2.cpp src/pr1.cpp src/runner.cpp
	$(COMPILE) -o $@ bin/rk.o src/pr2.cpp src/runner.cpp src/pr1.cpp

.PHONY=check,clean,debug,check-debug
debug: CFLAGS += -DDEBUG
debug: all

check-debug: CFLAGS += -DDEBUG
check-debug: TESTFLAGS += -s
check-debug: check

clean: bin
	@rm -f runner
	@rm -rf bin
	@rm -f file.h5

check: bin bin/rk.o test/*.cpp
	$(COMPILE) -Itest -o bin/tests test/*.cpp bin/rk.o
	@echo "running tests..."
	./bin/tests $(TESTFLAGS)
