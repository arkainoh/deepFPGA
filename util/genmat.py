import numpy as np
import sys

def save(mat, filename):

	f = open(filename, 'w')
	f.write(str(mat.shape[0]) + ' ' + str(mat.shape[1]) + '\n')
	f.close()

	f = open(filename, 'ab')
	np.savetxt(f, mat)
	f.close()

if __name__ == "__main__":
	save(np.random.rand(int(sys.argv[1]), int(sys.argv[2])), 'random.mat');

