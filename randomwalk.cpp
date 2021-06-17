#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include <ctime>
//#include "matplotlib-cpp-master/matplotlibcpp.h"
#include "Distribution.h"

using namespace std;
//namespace plt = matplotlibcpp;

///TEMPLATES
void runSpecs(int &steps, int &runs); //Process user input for number of steps & runs
void history(int steps); //Generate a single history and print to file
//void compute(int runs, Distribution &p, Distribution &q, Distribution &r, Distribution &s); ///Update quantities
//Data distributions
Distribution buildRSquare();
Distribution buildSDist();
Distribution buildTDist();
Distribution buildFs();
//Theoretical distributions
Distribution buildRSquareThy(int steps); //Build theoretical mean square displacement wrt time
Distribution buildSDistThy(vector<double> x); //Build theoretical space prob. dist.
Distribution buildTDistThy(int steps); //Build theoretical time prob. dist.
Distribution buildFsThy(int steps); //Build theoretical self-intermediate scattering function wrt time
//Plotting
void makePlots(Distribution data, Distribution theory); //Build a single plot of data alongside theoretical function

int main() {
    int steps; ///Number of steps in a history 
    int runs; ///Number of histories to generate
    Distribution m_square, m_square_thy; ///Mean square displacement, data and theory
    Distribution sdist, sdist_thy; ///Spatial distribution, data and theory
    Distribution tdist, tdist_thy; ///Time distribution, data and theory
    Distribution f_s, f_s_thy; ///Self-intermediate scattering function, data and theory
    //..
    return 0;
} ///main

void runSpecs(int &steps, int &runs) {
    //..
}

void history(int steps) {
    ///Open a file stream
    string filename = "data.txt";
    ofstream file;
    file.open(filename);

    ///Prepare for generation
    srand(time(0));
    double step = 0.00;

    ///Generate
    for(int i = 0; i < steps; i++){
        step = rand() % 1;
        if(step >= 0.5) {
            step = 1;
        }
        else step = -1;
        file << step << endl;
    }

    ///Close file stream
    file.close();
}

Distribution buildRSquare() {
    //..
    //return..
}

Distribution buildSDist() {
    //..
    //return..
}

Distribution buildTDist() {
    //..
    //return..
}

Distribution buildFs() {
    //..
    //return..
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