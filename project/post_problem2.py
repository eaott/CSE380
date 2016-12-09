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


fig = plt.figure(figsize = plt.figaspect(0.5))
for i in [1, 2, 3]:
    ax = fig.add_subplot(1, 3, i, projection = '3d')
    ax.plot(x, y, z)#, label = 'basic trajectory')
    ax.set_xticks([])
    ax.set_yticks([])
    ax.set_zticks([])

    ax.w_xaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
    ax.w_yaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
    ax.w_zaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))

    ax.w_xaxis.line.set_color((1.0, 1.0, 1.0, 0.0))
    ax.w_yaxis.line.set_color((1.0, 1.0, 1.0, 0.0))
    ax.w_zaxis.line.set_color((1.0, 1.0, 1.0, 0.0))


    if 'analytical' in f.keys():
        adset = f['analytical']
        anax = adset.value[0]
        anay = adset.value[1]
        anaz = adset.value[2]
        ax.plot(anax, anay, anaz)#, label = 'analytical trajectory')
    ax.legend()
#plt.title('Trajectories')
#plt.tight_layout()

plt.show()
