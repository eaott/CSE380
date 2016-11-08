# Note: verify gsl module is loaded first as well as gnu
# 
# This does work on stampede

gcc -I$TACC_GSL_INC main.c mytools.c -lgsl -lgslcblas -L$TACC_GSL_LIB 
