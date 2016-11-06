.PHONY: clean

EXEC:=tardis
CC:=gcc
CFLAGS=$$CFLAGS
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
