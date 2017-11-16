import numpy as np

m = np.random.rand(3, 5)

f = open('test.w', 'w')
f.write(str(m.shape[0]) + ' ' + str(m.shape[1]) + '\n')
f.close()

f = open('test.w', 'ab')
np.savetxt(f, m)
f.close()

