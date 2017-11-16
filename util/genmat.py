import numpy as np
import sys

m = np.random.rand(int(sys.argv[1]), int(sys.argv[2]))

f = open('test.w', 'w')
f.write(str(m.shape[0]) + ' ' + str(m.shape[1]) + '\n')
f.close()

f = open('test.w', 'ab')
np.savetxt(f, m)
f.close()

