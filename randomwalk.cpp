#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include <ctime>
#include "matplotlib-cpp-master/matplotlibcpp.h"
#include "Distribution.h"

using namespace std;
namespace plt = matplotlibcpp;

///TEMPLATES
Distribution buildRSquareThy(int steps); ///Build theoretical mean square displacement wrt time
Distribution buildSDistThy(vector<double> x); ///Build theoretical space prob. dist.
Distribution buildTDistThy(int steps); ///Build theoretical time prob. dist.
Distribution buildFsThy(int steps); ///Build theoretical self-intermediate scattering function wrt time


int main() {
    int steps;
    int runs;
    Distribution m_square, m_square_thy;
    Distribution sdist, sdist_thy;
    Distribution tdist, tdist_thy;
    Distribution f_s, f_s_thy;
    //..
    return 0;
} ///main

void runSpecs(int &steps, int &runs) {
    //..
}

void history(int steps) {
    vector <double> runData;
    srand(time(0));
    double step = 0.00;
    for(int i = 0; i < steps; i++){
        step = rand() % 1;
        if(step >= 0.5) {
            step = 1;
        }
        else step = -1;
    }
}

///Computes quantities using data from successive runs
void compute(int runs, Distribution p, Distribution q, Distribution r, Distribution s) {
    //..
}

Distribution buildRSquareThy(int steps) { 
    //..
    //return...
}

Distribution buildSDistThy(vector<double> x) {
    //..
    //return..
}

Distribution buildTDistThy(int steps) {
    //..
    //return..
}

Distribution buildFsThy(int steps) {
    //..
    //return..
}