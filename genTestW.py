import numpy as np

m = np.array([[1,2,3],[-4,23,-5],[-1,-2,-3]])

f = open('test.w', 'w')
f.write(str(m.shape[0]) + ' ' + str(m.shape[1]) + '\n')
f.close()

f = open('test.w', 'ab')
np.savetxt(f, m)
f.close()

