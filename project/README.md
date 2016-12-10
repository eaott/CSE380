# Evan Ott Final Project
- CSE 380, Fall 2016
- UT EID: eao466

# Compilation:
To compile on Stampede, run the following within the project directory:
```
module load hdf5 gsl
make
```

# Configuration and Execution
The project is run with the `runner` executable, but can be configured via
the `input.dat` file, which is in, essentially, the INI format. It allows
for selecting which problem to run, and the parameters for both problems. All options are listed there. Edit, then simply run
```
./runner
```
See below for post-processing and other scripts.

# Make Targets
The main targets for `make` are:
- `runner` - creates the executable `runner` for the entire project
- `debug` - creates the runner with debug options enabled
- `check` - runs a test suite
- `check-debug` - runs the test suite with debug options enabled
- `coverage` - requires the `gcc` module to be loaded (`module load gcc`), will use `gcov` and run tests to determine the code coverage of source files
- `clean` - remove binaries, `.gcov` files, etc.

# Project structure
- `includes` - header files needed for operation of main files, including the input parser
- `src` - main source code for each problem, and the custom versions of Euler and RK solvers
- `test` - code for tests, including the test framework itself
- `third_party` - really, just Eigen for all the vector / matrix operations

# Post-processing
Post-processing, in particular, with regard to performance and accuracy measures
was most easily accomplished by setting up some Python and bash scripts to create
the necessary input files to use the same standard `runner`. These scripts
work completely on my laptop, but the final results may not work on Stampede
because I'm using `matplotlib`. In that case, the `outputs` directory can easily be zipped,
copied to a machine with `matplotlib` and the final script can be run.

Both problems have exactly the same structure:
```
pr1_setup.sh
|_____________ pr1_create_input.py
|_____________ pr1_results.py
> pr1_performance.png
```

## Problem 2
The HDF5 data generated in problem 2 (the trajectory, and optionally the
corresponding analytical solution) can be processed in Python with `matplotlib`
in 3D using `post_problem2.py` -- this is separate from the processing above,
as it relies on using the `trajectory` and `verification` options in the
configuration file.

# Notes
This code can be found on GitHub at [github.com/eaott/CSE380](github.com/eaott/CSE380) in the `project` directory. In particular, this README file will be better viewed there, as it will be nicely formatted in your web browser.

# TODO
Ideally, things I would continue updating:
- creating more of a standard factory so that the GSL implementations can
be run with identical syntax as mine
- simplifying the code for convergence and performance analysis
