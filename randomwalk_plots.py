## THIS IS A TEMPLATE...use plots.py inside each data directory to get the plots for respective dataset.

## E. Aaron
## 07-07-2021
## Plots data and theory against hand checks, as well as data against theory. 14 plots total.

import matplotlib.pyplot as plt
import numpy as np

##DATA & CHECKS

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


##THEORY & CHECKS

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


##PLOTS

#DATA WITH CHECKS

#mean_squares
plt.scatter(XMS, YMS, color='cyan')
plt.scatter(XMSH, YMSH, marker='x', color='orange')

plt.title('mean square displacement')
plt.xlabel('time')
plt.ylabel('<r^2>')
plt.legend(["Data","Checks"])
plt.savefig("mean_squares_data_checks.png")
plt.clf()

#p_dist_t1
plt.scatter(XP1, YP1, color='cyan')
plt.scatter(XP1H, YP1H, marker='x', color='orange')

plt.title('Probability Distribution at t=1')
plt.xlabel('displacement')
plt.ylabel('probability')
plt.legend(["Data","Checks"])
plt.savefig("p_dist_t1: Data and Checks.png")
plt.clf()

#p_dist_t2
plt.scatter(XP2, YP2, color='cyan')
plt.scatter(XP2H, YP2H, marker='x', color='orange')

plt.title('Probability Distribution at t=2')
plt.xlabel('displacement')
plt.ylabel('probability')
plt.legend(["Data","Checks"])
plt.savefig("p_dist_t2: Data and Checks.png")
plt.clf()

#p_dist_t3
plt.scatter(XP3, YP3, color='cyan')
plt.scatter(XP3H, YP3H, marker='x', color='orange')

plt.title('Probability Distribution at t=3')
plt.xlabel('displacement')
plt.ylabel('probability')
plt.legend(["Data","Checks"])
plt.savefig("p_dist_t3: Data and Checks.png")
plt.clf()

#f_s_q1
plt.scatter(XF1, YF1, color='cyan')
plt.scatter(XF1H, YF1H, marker='x', color='orange')

plt.title('f_s for q1')
plt.xlabel('time')
plt.ylabel('f_s')
plt.legend(["Data","Checks"])
plt.savefig("f_s_q1: Data and Checks.png")
plt.clf()

#f_s_q2
plt.scatter(XF2, YF2, color='cyan')
plt.scatter(XF2H, YF2H, marker='x', color='orange')

plt.title('f_s for q2')
plt.xlabel('time')
plt.ylabel('f_s')
plt.legend(["Data","Checks"])
plt.savefig("f_s_q2: Data and Checks.png")
plt.clf()

#f_s_q3
plt.scatter(XF3, YF3, color='cyan')
plt.scatter(XF3H, YF3H, marker='x', color='orange')

plt.title('f_s for q3')
plt.xlabel('time')
plt.ylabel('f_s')
plt.legend(["Data","Checks"])
plt.savefig("f_s_q3: Data and Checks.png")
plt.clf()

#THEORY WITH CHECKS

#mean_squares
plt.scatter(XMS_T, YMS_T, color='cyan')
plt.scatter(XMSH_T, YMSH_T, marker='x', color='orange')

plt.title('mean square displacement')
plt.xlabel('time')
plt.ylabel('<r^2>')
plt.legend(["Theory","Checks"])
plt.savefig("mean_squares: Theory and Checks.png")
plt.clf()

#p_dist_t1
plt.scatter(XP1_T, YP1_T, color='cyan')
plt.scatter(XP1H_T, YP1H_T, marker='x', color='orange')

plt.title('Probability Distribution at t=1')
plt.xlabel('displacement')
plt.ylabel('probability')
plt.legend(["Theory","Checks"])
plt.savefig("p_dist_t1: Theory and Checks.png")
plt.clf()

#p_dist_t2
plt.scatter(XP2_T, YP2_T, color='cyan')
plt.scatter(XP2H_T, YP2H_T, marker='x', color='orange')

plt.title('Probability Distribution at t=2')
plt.xlabel('displacement')
plt.ylabel('probability')
plt.legend(["Theory","Checks"])
plt.savefig("p_dist_t2: Theory and Checks.png")
plt.clf()

#p_dist_t3
plt.scatter(XP3_T, YP3_T, color='cyan')
plt.scatter(XP3H_T, YP3H_T, marker='x', color='orange')

plt.title('Probability Distribution at t=3')
plt.xlabel('displacement')
plt.ylabel('probability')
plt.legend(["Theory","Checks"])
plt.savefig("p_dist_t3: Theory and Checks.png")
plt.clf()

#f_s_q1
plt.scatter(XF1_T, YF1_T, color='cyan')
plt.scatter(XF1H_T, YF1H_T, marker='x', color='orange')

plt.title('f_s for q1')
plt.xlabel('time')
plt.ylabel('f_s')
plt.legend(["Theory","Checks"])
plt.savefig("f_s_q1: Theory and Checks.png")
plt.clf()

#f_s_q2
plt.scatter(XF2_T, YF2_T, color='cyan')
plt.scatter(XF2H_T, YF2H_T, marker='x', color='orange')

plt.title('f_s for q2')
plt.xlabel('time')
plt.ylabel('f_s')
plt.legend(["Theory","Checks"])
plt.savefig("f_s_q2: Theory and Checks.png")
plt.clf()

#f_s_q3
plt.scatter(XF3_T, YF3_T, color='cyan')
plt.scatter(XF3H_T, YF3H_T, marker='x', color='orange')

plt.title('f_s for q3')
plt.xlabel('time')
plt.ylabel('f_s')
plt.legend(["Theory","Checks"])
plt.savefig("f_s_q3: Theory and Checks.png")
plt.clf()

#DATA WITH THEORY

#mean_squares
plt.scatter(XMS, YMS, color='cyan')
plt.scatter(XMS_T, YMS_T, marker='x', color='orange')

plt.title('mean square displacement')
plt.xlabel('time')
plt.ylabel('<r^2>')
plt.legend(["Data","Theory"])
plt.savefig("mean_squares: Data and Theory.png")
plt.clf()

#p_dist_t1
plt.scatter(XP1, YP1, color='cyan')
plt.scatter(XP1_T, YP1_T, marker='x', color='orange')

plt.title('Probability Distribution at t=1')
plt.xlabel('displacement')
plt.ylabel('probability')
plt.legend(["Data","Theory"])
plt.savefig("p_dist_t1: Data and Theory.png")
plt.clf()

#p_dist_t2
plt.scatter(XP2, YP2, color='cyan')
plt.scatter(XP2_T, YP2_T, marker='x', color='orange')

plt.title('Probability Distribution at t=2')
plt.xlabel('displacement')
plt.ylabel('probability')
plt.legend(["Data","Theory"])
plt.savefig("p_dist_t2: Data and Theory.png")
plt.clf()

#p_dist_t3
plt.scatter(XP3, YP3, color='cyan')
plt.scatter(XP3_T, YP3_T, marker='x', color='orange')

plt.title('Probability Distribution at t=3')
plt.xlabel('displacement')
plt.ylabel('probability')
plt.legend(["Data","Theory"])
plt.savefig("p_dist_t3: Data and Theory.png")
plt.clf()

#f_s_q1
plt.scatter(XF1, YF1, color='cyan')
plt.scatter(XF1_T, YF1_T, marker='x', color='orange')

plt.title('f_s for q1')
plt.xlabel('time')
plt.ylabel('f_s')
plt.legend(["Data","Theory"])
plt.savefig("f_s_q1: Data and Theory.png")
plt.clf()

#f_s_q2
plt.scatter(XF2, YF2, color='cyan')
plt.scatter(XF2_T, YF2_T, marker='x', color='orange')

plt.title('f_s for q2')
plt.xlabel('time')
plt.ylabel('f_s')
plt.legend(["Data","Theory"])
plt.savefig("f_s_q2: Data and Theory.png")
plt.clf()

#f_s_q3
plt.scatter(XF3, YF3, color='cyan')
plt.scatter(XF3_T, YF3_T, marker='x', color='orange')

plt.title('f_s for q3')
plt.xlabel('time')
plt.ylabel('f_s')
plt.legend(["Data","Theory"])
plt.savefig("f_s_q3: Data and Theory.png")
plt.clf()