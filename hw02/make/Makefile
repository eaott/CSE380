.PHONY: clean

EXEC:=tardis
CC:=gcc
CFLAGS=$$CFLAGS

ifndef TACC_GSL_INC
abort:
	@echo Variable TACC_GSL_INC is not defined && false
else
ifndef TACC_GSL_LIB
abort:
	@echo Variable TACC_GSL_LIB is not defined && false
endif
endif

GSL_INC:=$$TACC_GSL_INC
GSL_LIB:=$$TACC_GSL_LIB

SRC := $(wildcard *.c)
OBJ := $(patsubst %.c,%.o,$(SRC))

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -I$(GSL_INC) -lgsl -lgslcblas -L$(GSL_LIB) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -I$(GSL_INC) -c $<
clean:
	rm -f *.o tardis
