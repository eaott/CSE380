from matplotlib import pyplot as plt
import numpy as np
f = open('1.2.out.txt', 'r')
diffs = []
errors = []
ns = range(1, 101)
for line in f:
	vals = line.split(" ")
	diff = float(vals[2])
	err = float(vals[3])
	errors.append(err)
	diffs.append(diff)

diffarray = np.array(diffs)
plt.subplot(2, 1, 1)
plt.plot(ns, -diffarray)
plt.title('Problem 1.2')
plt.gca().set_yscale('log')
plt.ylabel('Difference (log scale)')

plt.subplot(2, 1, 2)
plt.plot(ns, errors)
plt.ylabel('Percent error')
plt.xlabel('n')
# plt.show()
plt.savefig('1.2.png')
