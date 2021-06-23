import matplotlib.pyplot as plt
import numpy as np

X, Y = np.loadtxt('mean_squares_thy.dat', delimiter=',', unpack=True)

plt.scatter(X, Y)
plt.title('Theoretical Mean Square Displacement as a Function of Time')
plt.xlabel('time (s)')
plt.ylabel('mean square displacement (arb. units)')
plt.show()