#include<stdio.h>
#include<stdlib.h>
#include<math.h>
double sterling(double n) {
	double result = n * log(n) - n;
	return exp(result);
}
double exact(double n) {
	if (n <= 1)
		return 1;
	double result = 1;
	while (n > 1) {
		result *= n;
		n--;
	}
	return result;
}
int main(int argc, char** argv) {
	double n, ex, st, diff, percent;
	for (n = 1; n <= 150; n++) {
		ex = exact(n);
		st = sterling(n);
		diff = st - ex;
		percent = fabs(diff / ex);
		printf("%.9e %.9e %.9e %.9f\n", ex, st, diff, percent);
	}
}
