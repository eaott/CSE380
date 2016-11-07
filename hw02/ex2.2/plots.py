from matplotlib import pyplot as plt
import numpy as np

radii = []
energies = []

point_file = open('ex2.2.data.txt', 'r')
for line in point_file:
	vals = line.split(" ")
	r = float(vals[0])
	energy = float(vals[1])
	radii.append(r)
	energies.append(energy)

poly_coeff = []
poly_file = open('poly.txt', 'r')
for line in poly_file:
	poly_coeff.append(float(line))

exp_coeff = []
exp_file = open('exp.txt', 'r')
for line in exp_file:
	exp_coeff.append(float(line))

myrange = np.linspace(0.2, 5, 100)

poly_vals = poly_coeff[0] +\
	poly_coeff[1] * myrange +\
	poly_coeff[2] * myrange ** 2 +\
	poly_coeff[3] * myrange ** 3 +\
	poly_coeff[4] * myrange ** 4 +\
	poly_coeff[5] * myrange ** 5 +\
	poly_coeff[6] * myrange ** 6

exp_vals = exp_coeff[0] +\
	exp_coeff[1] * np.exp(-myrange) +\
	exp_coeff[2] * np.exp(-2 * myrange) +\
	exp_coeff[3] * np.exp(-3 * myrange) +\
	exp_coeff[4] * np.exp(-4 * myrange) +\
	exp_coeff[5] * np.exp(-5 * myrange) +\
	exp_coeff[6] * np.exp(-6 * myrange)

plt.subplot(2, 1, 1)
plt.plot(myrange, poly_vals)
plt.plot(radii, energies, 'ro')
plt.title('Problem 2.2 - Poly fit')
plt.ylabel('Energy (hartees)')

plt.subplot(2, 1, 2)
plt.plot(myrange, exp_vals)
plt.plot(radii, energies, 'ro')
plt.title('Problem 2.2 - Exp fit')
plt.ylabel('Energy (hartrees)')
plt.xlabel('Radius (angstroms)')
# plt.show()
plt.savefig('2_2.png')
