import matplotlib.pyplot as plt
import numpy as np

X1, Y1 = np.loadtxt('mean_squares_thy.dat', delimiter=',', unpack=True)
X2, Y2 = np.loadtxt('mean_squares.dat', delimiter=',', unpack=True)

plt.scatter(X1, Y1, color='cyan')
#plt.Line2D(X1, Y1, color='red')
plt.scatter(X2, Y2, color='orange')

plt.title('Mean Square Displacement as a Function of Time')
plt.xlabel('time (s)')
plt.ylabel('mean square displacement (arb. units)')
plt.legend(["Theory","Data"])
plt.show()