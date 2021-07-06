import matplotlib.pyplot as plt
import numpy as np



#DATA & CHECKS

#mean square displacement data
XMS, YMS = np.loadtxt('mean_squares.dat', delimiter=',', unpack=True)
#hand check
XMSH = (0,1,2,3,4)
YMSH = np.loadtxt('handcheck_mean_squares.dat', unpack=True)

#probability distribution data 1
XP1, YP1 = np.loadtxt('p_dist_t1.dat', delimiter=',', unpack=True)
XP1 = (-4,-3,-2,-1,0,1,2,3,4)
#hand check
XP1H = (-4,-3,-2,-1,0,1,2,3,4)
YP1H = np.loadtxt('handcheck_p_dist_t1.dat', unpack=True)
#probability distribution data 2
XP2, YP2 = np.loadtxt('p_dist_t2.dat', delimiter=',', unpack=True)
XP2 = (-4,-3,-2,-1,0,1,2,3,4)
#hand check
XP2H = (-4,-3,-2,-1,0,1,2,3,4)
YP2H = np.loadtxt('handcheck_p_dist_t2.dat', unpack=True)
#probability distribution data 3
XP3, YP3 = np.loadtxt('p_dist_t3.dat', delimiter=',', unpack=True)
XP3 = (-4,-3,-2,-1,0,1,2,3,4)
#hand check
XP3H = (-4,-3,-2,-1,0,1,2,3,4)
YP3H = np.loadtxt('handcheck_p_dist_t3.dat', unpack=True)

#self-intermediate scattering function data 1
XF1, YF1 = np.loadtxt('f_s_q1.dat', delimiter=',', unpack=True)
#hand check
XF1H = (0,1,2,3,4)
YF1H = np.loadtxt('handcheck_f_s_q1.dat', unpack=True)
#self-intermediate scattering function data 1
XF2, YF2 = np.loadtxt('f_s_q2.dat', delimiter=',', unpack=True)
#hand check
XF2H = (0,1,2,3,4)
YF2H = np.loadtxt('handcheck_f_s_q2.dat', unpack=True)
#self-intermediate scattering function data 1
XF3, YF3 = np.loadtxt('f_s_q3.dat', delimiter=',', unpack=True)
#hand check
XF3H = (0,1,2,3,4)
YF3H = np.loadtxt('handcheck_f_s_q3.dat', unpack=True)


#THEORY & CHECKS

#mean square displacement theory
XMS_T, YMS_T = np.loadtxt('mean_squares_thy.dat', delimiter=',', unpack=True)
#hand check
XMSH_T = (0,1,2,3,4)
YMSH_T = np.loadtxt('handcheck_mean_squares_thy.dat', unpack=True)

#probability distribution theory 1
XP1_T, YP1_T = np.loadtxt('p_dist_thy_t1.dat', delimiter=',', unpack=True)
XP1_T = (-4,-3,-2,-1,0,1,2,3,4)
#hand check
XP1H_T = (-4,-3,-2,-1,0,1,2,3,4)
YP1H_T = np.loadtxt('handcheck_p_dist_thy_t1.dat', unpack=True)
#probability distribution theory 2
XP2_T, YP2_T = np.loadtxt('p_dist_thy_t2.dat', delimiter=',', unpack=True)
XP2_T = (-4,-3,-2,-1,0,1,2,3,4)
#hand check
XP2H_T = (-4,-3,-2,-1,0,1,2,3,4)
YP2H_T = np.loadtxt('handcheck_p_dist_thy_t2.dat', unpack=True)
#probability distribution theory 3
XP3_T, YP3_T = np.loadtxt('p_dist_thy_t3.dat', delimiter=',', unpack=True)
XP3_T = (-4,-3,-2,-1,0,1,2,3,4)
#hand check
XP3H_T = (-4,-3,-2,-1,0,1,2,3,4)
YP3H_T = np.loadtxt('handcheck_p_dist_thy_t3.dat', unpack=True)

#self-intermediate scattering function theory 1
XF1_T, YF1_T = np.loadtxt('f_s_thy_q1.dat', delimiter=',', unpack=True)
#hand check
XF1H_T = (0,1,2,3,4)
YF1H_T = np.loadtxt('handcheck_f_s_thy_q1.dat', unpack=True)
#self-intermediate scattering function theory 1
XF2_T, YF2_T = np.loadtxt('f_s_thy_q2.dat', delimiter=',', unpack=True)
#hand check
XF2H_T = (0,1,2,3,4)
YF2H_T = np.loadtxt('handcheck_f_s_thy_q2.dat', unpack=True)
#self-intermediate scattering function theory 1
XF3_T, YF3_T = np.loadtxt('f_s_thy_q3.dat', delimiter=',', unpack=True)
#hand check
XF3H_T = (0,1,2,3,4)
YF3H_T = np.loadtxt('handcheck_f_s_thy_q3.dat', unpack=True)


#PLOTS
plt.scatter(XP3, YP3, color='cyan')
plt.scatter(XP3_T, YP3_T, color='orange')

plt.title('Probability Distribution at t=3')
plt.xlabel('displacement')
plt.ylabel('probability')
plt.legend(["Data","Theory"])
plt.show()