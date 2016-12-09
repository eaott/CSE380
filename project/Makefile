INCLUDES:=-Ithird_party -Iincludes

RUNNER:=src/runner.cpp
SOURCES:=src/rk.cpp src/pr1.cpp src/pr2.cpp
.PHONY=check,clean,debug,check-debug

CFLAGS:= -lgsl -lgslcblas -lz -lm -lhdf5 -lhdf5_cpp
TESTFLAGS:=
host:=$(shell uname -s)
ifeq ($(host),Darwin)
	CC:=g++
	HDF_INSTALL=/Users/Evan/hpc_codes/installed/hdf5-1.8.18
	HDF5_LIB:=$(HDF_INSTALL)/lib
	HDF5_INC:=$(HDF_INSTALL)/include
	CFLAGS += -L$(HDF5_LIB) -I$(HDF5_INC)
	COV_FLAGS:=-o=$(PWD)
else
	CC:=icpc
	# FIXME look into this
	CFLAGS += -I$$TACC_GSL_INC -L$$TACC_GSL_LIB
	CFLAGS += -I$$TACC_HDF5_INC -L$$TACC_HDF5_LIB -L$$TACC_HDF5_LIB/libhdf5.a
	COV_FLAGS:=-o$(PWD)
endif

COMPILE=$(CC) $(INCLUDES) $(CFLAGS)

runner: $(RUNNER) $(SOURCES)
	$(COMPILE) -o $@ $^

debug: CFLAGS += -DDEBUG
debug: clean | runner

check-debug: CFLAGS += -DDEBUG
check-debug: TESTFLAGS += -as
check-debug: check

clean:
	@rm -f runner
	@rm -f check
	@rm -rf check.dSYM
	@rm -f file.h5
	@rm -f *.gcov *.gcno *.gcda
	@rm -f *.bbl *.blg *.nav *.snm *.toc *.aux *.fdb_latexmk *.fls *log *.out *.synctex.gz

check: test/*.cpp
	$(COMPILE) -Itest -o check test/*.cpp $(SOURCES)
	@echo "running tests..."
	./check $(TESTFLAGS)

coverage:	CC:=g++
coverage:	CFLAGS += -g -coverage
coverage: clean | check
coverage:
	@echo "Coverage for the source files:"
	gcov $(COV_FLAGS) -f $(SOURCES) | grep -A2 "src/.*\.cpp'"
	@echo "Unless you still need the .gcov files, recommend running \`make clean\` now..."
