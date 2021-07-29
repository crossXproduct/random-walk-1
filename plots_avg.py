## THIS IS A TEMPLATE...use plots.py inside each data directory to get the plots for respective dataset.

## E. Aaron
## 07-07-2021
## Plots data and_thy against old avgs, as well as data against_thy. 14 plots total.

import matplotlib.pyplot as plt
import numpy as np

## Read parameters from file
params = open('params.txt', 'r')
params.readline()
params.readline()
params.readline()
params.readline()
params.readline()
params.readline()
STEPS = params.readline()
params.readline()
INTERVAL = params.readline()
Q1 = params.readline()
Q2 = params.readline()
Q3 = params.readline()
TIME1 = params.readline()
TIME2 = params.readline()
TIME3 = params.readline()
params.close()

R_START = -int(STEPS.strip()) / 2 # Negative # of steps per run
R_END = int(STEPS.strip()) / 2 + 1 # # of steps + 1

R_Range = np.arange(R_START, R_END)
T_Range = np.arange(int(STEPS.strip()))

PLOTSIZE = int(STEPS.strip()) / int(INTERVAL.strip())

TIMES = [TIME1.strip(), TIME2.strip(), TIME3.strip()]
QS = [Q1.strip(), Q2.strip(), Q3.strip()]


##DATA & CHECKS
#mean square displacement data
X_MSD_AVG, Y_MSD_AVG = np.loadtxt('msd_avg.dat', delimiter=',', unpack=True)
X_MSD, Y_MSD = np.loadtxt('msd.dat', delimiter=',', unpack=True)
#old avg
X_MSD_AVG_O, Y_MSD_AVG_O = np.loadtxt('msd_avg_orig.dat', delimiter=',', unpack=True)

#probability distribution data 1
X_P1_AVG, Y_P1_AVG = np.loadtxt('pDist1_avg.dat', delimiter=',', unpack=True)
X_P1_AVG = X_P1_AVG[Y_P1_AVG != 0]
Y_P1_AVG = Y_P1_AVG[Y_P1_AVG != 0]
X_P1, Y_P1 = np.loadtxt('pDist1.dat', delimiter=',', unpack=True)
X_P1 = X_P1[Y_P1 != 0]
Y_P1 = Y_P1[Y_P1 != 0]
#old avg
X_P1_AVG_O, Y_P1_AVG_O = np.loadtxt('pDist1_avg_orig.dat', delimiter=',', unpack=True)

#probability distribution data 2
X_P2_AVG, Y_P2_AVG = np.loadtxt('pDist2_avg.dat', delimiter=',', unpack=True)
X_P2_AVG = X_P2_AVG[Y_P2_AVG != 0]
Y_P2_AVG = Y_P2_AVG[Y_P2_AVG != 0]
X_P2, Y_P2 = np.loadtxt('pDist2.dat', delimiter=',', unpack=True)
X_P2 = X_P2[Y_P2 != 0]
Y_P2 = Y_P2[Y_P2 != 0]
#old avg
X_P2_AVG_O, Y_P2_AVG_O = np.loadtxt('pDist2_avg_orig.dat', delimiter=',', unpack=True)

#probability distribution data 3
X_P3_AVG, Y_P3_AVG = np.loadtxt('pDist3_avg.dat', delimiter=',', unpack=True)
X_P3_AVG = X_P3_AVG[Y_P3_AVG != 0]
Y_P3_AVG = Y_P3_AVG[Y_P3_AVG != 0]
X_P3, Y_P3 = np.loadtxt('pDist3.dat', delimiter=',', unpack=True)
X_P3 = X_P3[Y_P3 != 0]
Y_P3 = Y_P3[Y_P3 != 0]
#old avg
X_P3_AVG_O, Y_P3_AVG_O = np.loadtxt('pDist3_avg_orig.dat', delimiter=',', unpack=True)

#self-intermediate scattering function data 1
X_F1_AVG, Y_F1_AVG = np.loadtxt('fs1_avg.dat', delimiter=',', unpack=True)
X_F1, Y_F1 = np.loadtxt('fs1.dat', delimiter=',', unpack=True)
#old avg
X_F1_AVG_O, Y_F1_AVG_O = np.loadtxt('fs1_avg_orig.dat', delimiter=',', unpack=True)

#self-intermediate scattering function data 2
X_F2_AVG, Y_F2_AVG = np.loadtxt('fs2_avg.dat', delimiter=',', unpack=True)
X_F2, Y_F2 = np.loadtxt('fs2.dat', delimiter=',', unpack=True)
#old avg
X_F2_AVG_O, Y_F2_AVG_O = np.loadtxt('fs2_avg_orig.dat', delimiter=',', unpack=True)

#self-intermediate scattering function data 3
X_F3_AVG, Y_F3_AVG = np.loadtxt('fs3_avg.dat', delimiter=',', unpack=True)
X_F3, Y_F3 = np.loadtxt('fs3.dat', delimiter=',', unpack=True)
#old avg
X_F3_AVG_O, Y_F3_AVG_O = np.loadtxt('fs3_avg_orig.dat', delimiter=',', unpack=True)


##THEORY & CHECKS

#mean square displacement_thy
X_MSD_THY, Y_MSD_THY = np.loadtxt('msd_thy.dat', delimiter=',', unpack=True)
#old avg
X_MSD_THY_O, Y_MSD_THY_O = np.loadtxt('msd_thy.dat', delimiter=',', unpack=True)

#probability distribution_thy 1
X_P1_THY, Y_P1_THY = np.loadtxt('pDist1_thy.dat', delimiter=',', unpack=True)
#old avg
X_P1_THY_O, Y_P1_THY_O = np.loadtxt('pDist1_thy.dat', delimiter=',', unpack=True)

#probability distribution_thy 2
X_P2_THY, Y_P2_THY = np.loadtxt('pDist2_thy.dat', delimiter=',', unpack=True)
#old avg
X_P2_THY_O, Y_P2_THY_O = np.loadtxt('pDist2_thy.dat', delimiter=',', unpack=True)

#probability distribution_thy 3
X_P3_THY, Y_P3_THY = np.loadtxt('pDist3_thy.dat', delimiter=',', unpack=True)
#old avg
X_P3_THY_O, Y_P3_THY_O = np.loadtxt('pDist3_thy.dat', delimiter=',', unpack=True)

#self-intermediate scattering function_thy 1
X_F1_THY, Y_F1_THY = np.loadtxt('fs1_thy.dat', delimiter=',', unpack=True)
#old avg
XF1_THY_O, YF1_THY_O = np.loadtxt('fs1_thy.dat', delimiter=',', unpack=True)

#self-intermediate scattering function_thy 2
X_F2_THY, Y_F2_THY = np.loadtxt('fs2_thy.dat', delimiter=',', unpack=True)
#old avg
XF2_THY_O, YF2_THY_O = np.loadtxt('fs2_thy.dat', delimiter=',', unpack=True)

#self-intermediate scattering function_thy 3
X_F3_THY, Y_F3_THY = np.loadtxt('fs3_thy.dat', delimiter=',', unpack=True)
#old avg
XF3_THY_O, YF3_THY_O = np.loadtxt('fs3_thy.dat', delimiter=',', unpack=True)


##PLOTS

#msd
plt.scatter(X_MSD_THY, Y_MSD_THY, marker = '^', color= 'blue')
plt.scatter(X_MSD, Y_MSD, marker='o', color='green')
plt.scatter(X_MSD_AVG_O, Y_MSD_AVG_O, marker='x', color='cyan')
plt.scatter(X_MSD_AVG, Y_MSD_AVG, marker='.', color='red')

plt.title('mean square displacement')
plt.xlabel('time')
plt.ylabel('<R(t)^2>')
plt.legend(["Theory","Unaveraged", "Old Time-Averaged", "New Time-Averaged"])
plt.savefig("msd_avg.png")
plt.clf()

#pDist1
plt.scatter(X_P1_THY, Y_P1_THY, marker = '^', color= 'blue')
plt.scatter(X_P1, Y_P1, marker='o', color='green')
plt.scatter(X_P1_AVG_O, Y_P1_AVG_O, marker='x', color='cyan')
plt.scatter(X_P1_AVG, Y_P1_AVG, marker='.', color='red')

plt.title('Probability at t=' + str(TIMES[0]))
plt.xlabel('R(t)')
plt.ylabel('Probability')
plt.legend(["Theory","Unaveraged", "Old Time-Averaged", "New Time-Averaged"])
plt.savefig("pDist1_avg.png")
plt.clf()

#pDist2
plt.scatter(X_P2_THY, Y_P2_THY, marker = '^', color= 'blue')
plt.scatter(X_P2, Y_P2, marker='o', color='green')
plt.scatter(X_P2_AVG_O, Y_P2_AVG_O, marker='x', color='cyan')
plt.scatter(X_P2_AVG, Y_P2_AVG, marker='.', color='red')

plt.title('Probability Distribution at t=' + str(TIMES[1]))
plt.xlabel('R(t)')
plt.ylabel('probability')
plt.legend(["Theory","Unaveraged", "Old Time-Averaged", "New Time-Averaged"])
plt.savefig("pDist2_avg.png")
plt.clf()

#pDist3
plt.scatter(X_P3_THY, Y_P3_THY, marker = '^', color= 'blue')
plt.scatter(X_P3, Y_P3, marker='o', color='green')
plt.scatter(X_P3_AVG_O, Y_P3_AVG_O, marker='x', color='cyan')
plt.scatter(X_P3_AVG, Y_P3_AVG, marker='.', color='red')

plt.title('Probability Distribution at t=' + str(TIMES[2]))
plt.xlabel('R(t)')
plt.ylabel('probability')
plt.legend(["Theory","Unaveraged", "Old Time-Averaged", "New Time-Averaged"])
plt.savefig("pDist3_avg.png")
plt.clf()

#fs1
plt.scatter(X_F1_THY, Y_F1_THY, marker='^', color='blue')
plt.scatter(X_F1, Y_F1, marker='o', color='green')
plt.scatter(X_F1_AVG_O, Y_F1_AVG_O, marker='x', color='cyan')
plt.scatter(X_F1_AVG, Y_F1_AVG, marker='.', color='red')

plt.title('fs for q=' + QS[0])
plt.xlabel('time')
plt.ylabel('fs')
plt.legend(["Theory","Unaveraged", "Old Time-Averaged", "New Time-Averaged"])
plt.savefig("fs1_avg.png")
plt.clf()

#fs2
plt.scatter(X_F2_THY, Y_F2_THY, marker='^', color='blue')
plt.scatter(X_F2, Y_F2, marker='o', color='green')
plt.scatter(X_F2_AVG_O, Y_F2_AVG_O, marker='x', color='cyan')
plt.scatter(X_F2_AVG, Y_F2_AVG, marker='.', color='red')

plt.title('fs for q=' + QS[1])
plt.xlabel('time')
plt.ylabel('fs')
plt.legend(["Theory","Unaveraged", "Old Time-Averaged", "New Time-Averaged"])
plt.savefig("fs2_avg.png")
plt.clf()

#fs3
plt.scatter(X_F3_THY, Y_F3_THY, marker='^', color='blue')
plt.scatter(X_F3, Y_F1, marker='o', color='green')
plt.scatter(X_F3_AVG_O, Y_F3_AVG_O, marker='x', color='cyan')
plt.scatter(X_F3_AVG, Y_F3_AVG, marker='.', color='red')

plt.title('fs for q=' + QS[2])
plt.xlabel('time')
plt.ylabel('fs')
plt.legend(["Theory","Unaveraged", "Old Time-Averaged", "New Time-Averaged"])
plt.savefig("fs3_avg.png")
plt.clf()