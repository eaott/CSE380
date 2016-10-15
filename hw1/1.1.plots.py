from matplotlib import pyplot as plt
f = open('1.1.out.txt', 'r')
errors = []
times = []
nps = []
for line in f:
	vals = line.split(" ")
	error = float(vals[0])
	time = float(vals[1])
	np = float(vals[2])
	errors.append(error)
	times.append(time)
	nps.append(np)

plt.subplot(2, 1, 1)
plt.plot(nps, times)
plt.title('Problem 1.1')
plt.ylabel('Wallclock time')
plt.subplot(2, 1, 2)
plt.plot(nps, errors)
plt.ylabel('Absolute error')
plt.xlabel('np')
plt.savefig('1.1.png')
