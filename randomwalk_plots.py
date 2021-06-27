import matplotlib.pyplot as plt
import numpy as np

#mean square displacement, data vs theory
#X1, Y1 = np.loadtxt('mean_squares_thy.dat', delimiter=',', unpack=True)
#X2, Y2 = np.loadtxt('mean_squares.dat', delimiter=',', unpack=True)

#probability distribution, data vs hand check
Y1 = (0,0,0,0,0.333,0,0.333,0,0.333,0)
X1 = (0,1,2,3,4,5,6,7,8,9)

X2, Y2 = np.loadtxt('p_dist_t3.dat', delimiter=',', unpack=True)
X2 = (0,1,2,3,4,5,6,7,8,9)


plt.scatter(X1, Y1, color='cyan')
#plt.Line2D(X1, Y1, color='red')
plt.scatter(X2, Y2, color='orange')

plt.title('Probability Distribution at t=3s')
plt.xlabel('displacement (arb. units)')
plt.ylabel('Probability')
plt.legend(["Data","Hand Check"])
plt.show()