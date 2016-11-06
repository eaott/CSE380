#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_multifit.h>
int main() {
	std::ifstream file("ex2.2.data.txt");
	std::string str;
	gsl_matrix * m = gsl_matrix_alloc (10, 7);
	gsl_vector * v = gsl_vector_alloc (10);

	int line = 0;
	while (std::getline(file, str)) {
		std::string r_str = str.substr(0, 17);
		std::string e_str = str.substr(18, std::string::npos);
		float r = atof(r_str.c_str());
		gsl_matrix_set(m, line, 0, 1);
		gsl_matrix_set(m, line, 1, r);
		gsl_matrix_set(m, line, 2, r * r);
		gsl_matrix_set(m, line, 3, r * r * r);
		gsl_matrix_set(m, line, 4, r * r * r * r);
		gsl_matrix_set(m, line, 5, r * r * r * r * r);
		gsl_matrix_set(m, line, 6, r * r * r * r * r * r);
		gsl_vector_set(v, line, atof(e_str.c_str()));
		line++;
	}
	gsl_vector * b = gsl_vector_alloc (7);
	gsl_matrix * cov = gsl_matrix_alloc(7, 7);
	double * chisq = 0;
	gsl_multifit_linear_workspace * work = gsl_multifit_linear_alloc(10, 7);
	gsl_multifit_linear(m, v, b, cov, chisq, work);

	return 0;
}
