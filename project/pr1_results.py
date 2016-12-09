import ConfigParser
import os

import matplotlib
from matplotlib import pyplot as plt

files = os.listdir("outputs")
evan_hs = []
evan_errors = []
evan_times = []

gsl_hs = []
gsl_errors = []
gsl_times = []

for file in files:
    config = ConfigParser.RawConfigParser()
    config.read("outputs/" + file)

    h = config.getfloat("pr1_results", "h")
    error = config.getfloat("pr1_results", "error")
    method = config.get("pr1_results", "method")
    execution_time = config.get("pr1_results", "time")

    if method == "evan":
        evan_hs.append(h)
        evan_errors.append(error)
        evan_times.append(execution_time)
    else:
        gsl_hs.append(h)
        gsl_errors.append(error)
        gsl_times.append(execution_time)

plt.subplot(2, 1, 1)
plt.loglog(evan_hs, evan_errors, "-bo", label="custom")
plt.loglog(gsl_hs, gsl_errors, '-ro', label="GSL")
plt.ylabel('Error')
plt.legend()

plt.subplot(2, 1, 2)
plt.loglog(evan_hs, evan_times, '-bo', label="custom")
plt.loglog(gsl_hs, gsl_times, '-ro', label="GSL")
plt.ylabel('Executition time (s)')
plt.xlabel('Step size')
plt.legend()
# plt.show()
plt.savefig('pr2_performance.png')
