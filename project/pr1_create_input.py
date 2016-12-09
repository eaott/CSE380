hs = [1e-6, 1e-5, 1e-4, 1e-4, 1e-3]
methods = ["evan", "gsl"]
for method in methods:
    for i in xrange(len(hs)):
        filename = 'inputs/pr1_input_{1:s}_{0:d}.ini'.format(i, method)
        f = open(filename, 'w')
        f.write('[runner]\n')
        f.write('problem=problem1\n')
        f.write('[problem1]\n')
        f.write('h={0:f}\n'.format(hs[i]))
        f.write('method={0:s}\n'.format(method))
        f.write('verification=true\n')
        f.close()
