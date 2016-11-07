#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_multifit.h>
int main() {
	std::ifstream file("ex2.2.data.txt");
	std::string str;
	gsl_matrix * X = gsl_matrix_alloc (10, 7);
	gsl_vector * y = gsl_vector_alloc (10);
	gsl_vector * beta = gsl_vector_alloc (7);
	gsl_matrix * cov = gsl_matrix_alloc(7, 7);
	gsl_multifit_linear_workspace * work = gsl_multifit_linear_alloc(10, 7);
	double chisq = 0;

	int line = 0;
	while (std::getline(file, str)) {
		std::string r_str = str.substr(0, 17);
		std::string e_str = str.substr(18, std::string::npos);
		float r = atof(r_str.c_str());
		gsl_matrix_set(X, line, 0, 1);
		gsl_matrix_set(X, line, 1, exp(-r));
		gsl_matrix_set(X, line, 2, exp(-2 * r));
		gsl_matrix_set(X, line, 3, exp(-3 * r));
		gsl_matrix_set(X, line, 4, exp(-4 * r));
		gsl_matrix_set(X, line, 5, exp(-5 * r));
		gsl_matrix_set(X, line, 6, exp(-6 * r));

		gsl_vector_set(y, line, atof(e_str.c_str()));
		line++;
	}

	gsl_multifit_linear(X, y, beta, cov, &chisq, work);
	for (int i = 0; i < 7; i++) {
		printf("%g\n", gsl_vector_get(beta, i));
	}
	return 0;
}
