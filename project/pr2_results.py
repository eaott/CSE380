import ConfigParser
import os

import matplotlib
from matplotlib import pyplot as plt

files = os.listdir("outputs")
evan_rkf_hs = []
evan_rkf_errors = []
evan_rkf_times = []

evan_rk4_hs = []
evan_rk4_errors = []
evan_rk4_times = []

evan_rk38_hs = []
evan_rk38_errors = []
evan_rk38_times = []

gsl_rkf_hs = []
gsl_rkf_errors = []
gsl_rkf_times = []

gsl_rk4_hs = []
gsl_rk4_errors = []
gsl_rk4_times = []

gsl_rk8pd_hs = []
gsl_rk8pd_errors = []
gsl_rk8pd_times = []
for file in files:
    config = ConfigParser.RawConfigParser()
    config.read("outputs/" + file)

    h = config.getfloat("pr2_results", "step")
    error = config.getfloat("pr2_results", "error")
    method = config.get("pr2_results", "method")
    execution_time = config.get("pr2_results", "time")

    if method == "evan-rk4":
        evan_rk4_hs.append(h)
        evan_rk4_errors.append(error)
        evan_rk4_times.append(execution_time)
    elif method == "evan-rk38":
        evan_rk38_hs.append(h)
        evan_rk38_errors.append(error)
        evan_rk38_times.append(execution_time)
    elif method == "evan-rkf":
        evan_rkf_hs.append(h)
        evan_rkf_errors.append(error)
        evan_rkf_times.append(execution_time)
    elif method == "rk4":
        gsl_rk4_hs.append(h)
        gsl_rk4_errors.append(error)
        gsl_rk4_times.append(execution_time)
    elif method == "rk8pd":
        gsl_rk8pd_hs.append(h)
        gsl_rk8pd_errors.append(error)
        gsl_rk8pd_times.append(execution_time)
    elif method == "rkf":
        gsl_rkf_hs.append(h)
        gsl_rkf_errors.append(error)
        gsl_rkf_times.append(execution_time)

fig = plt.figure(dpi=300)
ax1 = fig.add_subplot(211)
ax2 = fig.add_subplot(212)
ax1.loglog(evan_rk4_hs, evan_rk4_errors, "-bo", label="custom rk4")
ax1.loglog(evan_rkf_hs, evan_rkf_errors, '-go', label="custom rkf")
#ax1.loglog(evan_rk38_hs, evan_rk38_errors, '-mo', label="custom rk38")
ax1.loglog(gsl_rk4_hs, gsl_rk4_errors, '-ro', label="GSL rk4")
ax1.loglog(gsl_rkf_hs, gsl_rkf_errors, '-co', label="GSL rkf")
ax1.loglog(gsl_rk8pd_hs, gsl_rk8pd_errors, '-ko', label="GSL rk8pd")
ax1.set_ylabel('Error')
ax1.legend(bbox_to_anchor=(0.6, 1), loc=1, borderaxespad=0., fontsize='x-small')

ax2.loglog(evan_rk4_hs, evan_rk4_times, "-bo", label="custom rk4")
ax2.loglog(evan_rkf_hs, evan_rkf_times, '-go', label="custom rkf")
ax2.loglog(evan_rk38_hs, evan_rk38_times, '-mo', label="custom rk38")
ax2.loglog(gsl_rk4_hs, gsl_rk4_times, '-ro', label="GSL rk4")
ax2.loglog(gsl_rkf_hs, gsl_rkf_times, '-co', label="GSL rkf")
ax2.loglog(gsl_rk8pd_hs, gsl_rk8pd_times, '-ko', label="GSL rk8pd")
ax2.set_ylabel('Executition time (s)')
ax2.set_xlabel('Step size')
ax2.legend(bbox_to_anchor=(1, 1), loc=1, borderaxespad=0., fontsize='x-small')

#ax2.legend()

# plt.show()
fig.savefig('pr2_performance.png')
