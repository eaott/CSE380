import numpy as np
import h5py
import matplotlib
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


f = h5py.File("/Users/Evan/GitProjects/cse380/project/file.h5", "r")
dset = f['dataset']
dset.attrs['Rows']
data = dset.value
x = data[0]
y = data[1]
z = data[2]


fig = plt.figure()
ax = fig.gca(projection='3d')
ax.plot(x, y, z, label = 'basic trajectory')
ax.legend()
plt.title('Trajectories')
plt.show()
