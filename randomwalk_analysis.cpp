/**
 * randomwalk_analysis.cpp
 * E. Aaron
 * 06-20-2021
 * 
 * Reads a series of files containing individual thermal histories (created by randomwalk.cpp),
 * and constructs the mean square displacement as a function of time, the spatial probability
 * distribution at three different times, and the self-intermediate scattering
 * function for three different values of parameter q.
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

// File input
ifstream getfile(string filename); //Return input file stream

// Update distributions
void buildRSquare(vector<double> &r_square); // Build mean square displacement wrt time
void buildPDist(vector<double> &p_dist);
void buildFs(vector<double> &f_s);
vector<double> buildRSquareThy(int steps); // Build theoretical mean square displacement wrt time
vector<double> buildPDistThy(vector<double> x); // Build theoretical space prob. dist.
vector<double> buildFsThy(int steps); // Build theoretical self-intermediate scattering function wrt time

// Plotting
void makePlots(vector<double> data, vector<double> theory); //Build a single plot of data alongside theoretical function

int main() {
    // Declare variables
    int t1, t2, t3; // Three times at which to evaluate the spatial probability distribution
    double q1, q2, q3; // Three qs at which to evaluate self-intermediate scattering function
    int startfile, endfile; // Number of first and last data file to process, in numerical order

    // DATA
    // Mean square displacement as a function of time
    vector<double> r_square;
    // Probability distributions
    vector<double> p_dist_t1; // as a function of position at t1
    vector<double> p_dist_t2; // as a function of position at t2
    vector<double> p_dist_t3; // as a function of position at t3
    // Self-intermediate scattering functions
    vector<double> f_s_q1; // as a function of time at q = q1
    vector<double> f_s_q2; // as a function of time at q = q2
    vector<double> f_s_q3; // as a function of time at q = q3

    // THEORY
    // Mean square displacement as a function of time
    vector<double> r_square_thy;
    // Probability distributions as a function of position
    vector<double> p_dist_thy_t1; // at t1
    vector<double> p_dist_thy_t2; // at t2
    vector<double> p_dist_thy_t3; // at t3
    // Self-intermediate scattering functions as a function of time
    vector<double> f_s_thy_q1; //  at q = q1
    vector<double> f_s_thy_q2; // at q = q2
    vector<double> f_s_thy_q3; // at q = q3

    //Take user input: t and q values, start and end data files
    cout << "Enter number of first datafile (e.g. \"1\" for history1.txt): ";
    cin >> startfile;
    cout << "Enter number of last datafile (e.g. \"10\" for history10.txt): ";
    cin >> endfile;

    cout << "Enter three successive time values (assuming t_0 = 0 s) at which to evaluate probability distribution: ";
    cin >> t1 >> t2 >> t3;

    cout << "Enter three successive q values at which to evaluate self-intermediate scattering function: ";
    cin >> q1 >> q2 >> q3;
    
    return 0;
}

ifstream getfile(string filename) {
    
}

vector<double> buildRSquare() {
    //specify start and end files
    //return..
}

vector<double> buildPDist() {
    //..
    //return..
}

vector<double> buildFs() {
    //..
    //return..
}

vector<double> buildRSquareThy(int steps) { 
    //..
    //return...
}

vector<double> buildPDistThy(vector<double> x) {
    //..
    //return..
}

vector<double> buildFsThy(int steps) {
    //..
    //return..
}