import matplotlib.pyplot as plt
import numpy as np

#mean square displacement, data vs theory
#X1, Y1 = np.loadtxt('mean_squares_thy.dat', delimiter=',', unpack=True)
#X2, Y2 = np.loadtxt('mean_squares.dat', delimiter=',', unpack=True)

#mean square displacement, data vs hand check
XMSH = (0,1,2,3,4)
YMSH = np.loadtxt('handcheck_mean_squares.dat', unpack=True)
XMS, YMS = np.loadtxt('mean_squares.dat', delimiter=',', unpack=True)

#probability distribution, data vs hand check
XP1H = (0,1,2,3,4,5,6,7,8)
YP1H = np.loadtxt('handcheck_p_dist_t1.dat', unpack=True)
XP1, YP1 = np.loadtxt('p_dist_t1.dat', delimiter=',', unpack=True)
XP2H = (0,1,2,3,4,5,6,7,8)
YP2H = np.loadtxt('handcheck_p_dist_t2.dat', unpack=True)
XP2, YP2 = np.loadtxt('p_dist_t2.dat', delimiter=',', unpack=True)
XP3H = (0,1,2,3,4,5,6,7,8)
YP3H = np.loadtxt('handcheck_p_dist_t3.dat', unpack=True)
XP3, YP3 = np.loadtxt('p_dist_t3.dat', delimiter=',', unpack=True)

#self-intermediate scattering function, data vs hand check
XF1H = (0,1,2,3,4)
YF1H = np.loadtxt('handcheck_f_s_q1.dat', unpack=True)
XF1, YF1 = np.loadtxt('f_s_q1.dat', delimiter=',', unpack=True)
XF2H = (0,1,2,3,4)
YF2H = np.loadtxt('handcheck_f_s_q2.dat', unpack=True)
XF2, YF2 = np.loadtxt('f_s_q2.dat', delimiter=',', unpack=True)
XF3H = (0,1,2,3,4)
YF3H = np.loadtxt('handcheck_f_s_q3.dat', unpack=True)
XF3, YF3 = np.loadtxt('f_s_q3.dat', delimiter=',', unpack=True)


plt.scatter(XMSH, YMSH, color='cyan')
plt.scatter(XMS, YMS, color='orange')

plt.title('Mean Square Displacement')
plt.xlabel('time')
plt.ylabel('displacement')
plt.legend(["Hand Check","Data"])
plt.show()