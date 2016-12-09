/*
 * Class that handles Problem 1.
 * In particular, solves the simple ODE below according to configuration
 * options in an INIReader.
 */

#include <iostream>
#include <cmath>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>
#include "INIReader.h"

/*
 * The simple ODE: dy/dt = -0.1 * y = f(t, y)
 * df/dy = - 0.1
 * df/dt = 0 -- no *explicit* time dependence in the ODE
 * Solution: y(t) = y0 * exp(-0.1 (t - t0))
 * given that y(t0) = y0
 * Function below is formatted for GSL. Takes current state (t, y)
 * applies it to the ODE and stores the values in f.
 */
int func(double t, const double y[], double f[], void * params) {
  f[0] = - 0.1 * y[0];
  return GSL_SUCCESS;
}

/*
 * Needed for some GSL solvers, the jacobian of the system. Takes current
 * state (t, y), and stores the jacobian in dfdy (in this case, a 1x1 matrix)
 * and the explicit time dependence in dfdt (in this case, a 1-vector).
 */
int jacobian(double t, const double y[], double *dfdy, double dfdt[], void * params) {
  dfdy[0] = - 0.1;

  dfdt[0] = 0.0;
  return GSL_SUCCESS;
}

/*
 * The analytic solution to this problem given the initial condition
 * y(t0) = y0. Unlike the above, this returns the value of the solution, rather than
 * the GSL-style success/failure on return.
 */
double analytic(double t, double t0, double y0) {
  return y0 * exp(-0.1 * (t - t0));
}

int run_pr1(INIReader reader) {
  // Step size
  double h = reader.GetReal("problem1", "h", 1e-6);

  // Initial conditions
  double t = reader.GetReal("problem1", "t0", 0.0);
  double y0 = reader.GetReal("problem1", "y0", 10.0);

  // Time at the end of the integration
  double t_end = reader.GetReal("problem1", "t_end", 5.0);

  // GSL: how many steps per iteration (deltaT = h * nsteps)
  int nsteps = reader.GetInteger("problem1", "nsteps", 1);
  // GSL: absolute and relative epsilon for convergence
  double epsilon_abs = reader.GetReal("problem1", "epsilon_abs", 1e-6);
  double epsilon_rel = reader.GetReal("problem1", "epsilon_rel", 0.0);

  double y[1] = {y0};
  gsl_odeiv2_system sys = {func, jacobian, 1, NULL};

  // FIXME this is the backward Euler so should be comparable,
  // but might consider using a better step function -- maybe
  // gsl_odeiv2_step_rk8pd? or the GSL version of Runge-Kutta-Fehlberg
  // gsl_odeiv2_step_rkf45
  gsl_odeiv2_driver * d =
    gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk1imp,
                                  h, epsilon_abs, epsilon_rel);

  while (t < t_end)
  {
    // goes nsteps of size h
    int status = gsl_odeiv2_driver_apply_fixed_step (d, &t, h, nsteps, y);

    if (status != GSL_SUCCESS)
  	{
  	  printf ("error, return value=%d\n", status);
  	  break;
  	}

    printf ("%.5e %.5e\n", t, y[0]);
  }

  gsl_odeiv2_driver_free (d);
  return 0;
}
