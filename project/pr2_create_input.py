hs = [5e-7, 1e-6, 1e-5, 1e-4, 1e-3]
iters = [2000000, 1000000,  100000,  10000,  1000]
methods = ["rk4", "rk8pd", "rkf", "evan-rk4", "evan-rk38", "evan-rkf"]
for method in methods:
    for i in xrange(len(hs)):
        filename = 'inputs/pr2_input_{1:s}_{0:d}.ini'.format(i, method)
        f = open(filename, 'w')
        f.write('[runner]\n')
        f.write('problem=problem2\n')
        f.write('[problem2]\n')
        f.write('step={0:.8f}\n'.format(hs[i]))
        f.write('iter={0:d}\n'.format(iters[i]))
        f.write('method={0:s}\n'.format(method))
        f.write('trajectory=false\n')
        f.write('verification=true\n')
        f.close()
