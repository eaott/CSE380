import numpy as np
import h5py
import matplotlib
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


f = h5py.File("/Users/Evan/GitProjects/cse380/project/file.h5", "r")

dset = f['dataset']
x = dset.value[0]
y = dset.value[1]
z = dset.value[2]

adset = f['analytical']
anax = adset.value[0]
anay = adset.value[1]
anaz = adset.value[2]


fig = plt.figure()
ax = fig.gca(projection='3d')
ax.plot(x, y, z, label = 'basic trajectory')
ax.plot(anax, anay, anaz, label = 'analytical trajectory')
ax.legend()
plt.title('Trajectories')
plt.show()
