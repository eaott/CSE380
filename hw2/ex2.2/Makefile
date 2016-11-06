.PHONY: clean

CC:=g++
CFLAGS=$$CFLAGS

GSL_INC:=$$TACC_GSL_INC
GSL_LIB:=$$TACC_GSL_LIB
host:=$(shell uname -s)
ifneq ($(host),Darwin)
ifndef TACC_GSL_INC
abort:
	@echo Variable TACC_GSL_INC is not defined && false
else
ifndef TACC_GSL_LIB
abort:
	@echo Variable TACC_GSL_LIB is not defined && false
endif
endif
COMPILE:=$(CC) $(CFLAGS) -I$(GSL_INC) -lgsl -lgslcblas -L$(GSL_LIB)
else
COMPILE:=$(CC) $(CFLAGS) -lgsl -lgslcblas
endif


EXEC_POLY:=poly
EXEC_EXP:=exp
both: $(EXEC_POLY) $(EXEC_EXP)
	
$(EXEC_POLY): ex2.2.poly.cpp
	$(COMPILE) ex2.2.poly.cpp -o $(EXEC_POLY)
	./$(EXEC_POLY) > poly.txt
$(EXEC_EXP): ex2.2.exp.cpp
	$(COMPILE) ex2.2.exp.cpp -o $(EXEC_EXP)
	./$(EXEC_EXP) > exp.txt
clean:
	rm -f $(EXEC_POLY) $(EXEC_EXP) poly.txt exp.txt
