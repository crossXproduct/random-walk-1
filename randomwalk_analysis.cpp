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
#include <string>

using namespace std;

// File input
ifstream getfile(string filename); //Return input file stream

// Update distributions
void buildRSquare(vector<double> &r_square, ifstream file); // Build mean square displacement wrt time
void buildPDist(vector<double> &p_dist, int t, ifstream file);
void buildFs(vector<double> &f_s, double q, ifstream file);
vector<double> buildRSquareThy(vector<double> &r_square_thy, int t); // Build theoretical mean square displacement wrt time
vector<double> buildPDistThy(vector<double> &p_dist_thy, int t); // Build theoretical space prob. dist.
vector<double> buildFsThy(vector<double> &f_s_thy, int t, int q); // Build theoretical self-intermediate scattering function wrt time

// Plotting
void makePlots(vector<double> data, vector<double> theory); //Build a single plot of data alongside theoretical function

int main() {
    // Declare variables
    int t1, t2, t3; // Three times at which to evaluate the spatial probability distribution
    double q1, q2, q3; // Three qs at which to evaluate self-intermediate scattering function
    int t; // Number of timesteps in each history ***NEEDS TO BE DETERMINED***
    int name, startname, endname; // Number of first and last data file to process, in numerical order
    ifstream file; // Data file currently being read

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
    cin >> startname;
    filename = "history" + startname + ".txt";

    cout << "Enter number of last datafile (e.g. \"10\" for history10.txt): ";
    cin >> endname;

    cout << "Enter three successive time values (assuming t_0 = 0 s) at which to evaluate probability distribution: ";
    cin >> t1 >> t2 >> t3;

    cout << "Enter three successive q values at which to evaluate self-intermediate scattering function: ";
    cin >> q1 >> q2 >> q3;

    // Build data distributions
    do {
        file = getfile(filename);
        buildRSquare(r_square, file);
        buildPDist(p_dist_t1, t1, file);
        buildPDist(p_dist_t2, t2, file);
        buildPDist(p_dist_t3, t3, file);
        buildFs(f_s_q1, q1, file);
        buildFs(f_s_q2, q2, file);
        buildFs(f_s_q3, q3, file);
        name++;
    } while(name < endname);

    // Build theoretical distributions
    buildRSquareThy(r_square_thy, t);
    buildPDistThy(p_dist_thy_t1, t1);
    buildPDistThy(p_dist_thy_t2, t2);
    buildPDistThy(p_dist_thy_t3, t3);
    buildFsThy(f_s_thy_q1, t, q1);
    buildFsThy(f_s_thy_q2, t, q2);
    buildFsThy(f_s_thy_q3, t, q3);
    
    return 0;
}

ifstream getfile(string filename) {
    ifstream file;
    file.open(filename);
    if(file.fail()) {
        cout << "File " << filename << " could not be opened.";
        file.clear();
    }
}

vector<double> buildRSquare(vector<double> &r_square, ifstream file) {
    //return..
}

vector<double> buildPDist(vector<double> &p_dist, int t, ifstream file) {
    //..
    //return..
}

vector<double> buildFs(vector<double> &f_s, double q, ifstream file) {
    //..
    //return..
}

vector<double> buildRSquareThy(vector<double> &r_square_thy, int t) { 
    //..
    //return...
}

vector<double> buildPDistThy(vector<double> &p_dist_thy, int t) {
    //..
    //return..
}

vector<double> buildFsThy(vector<double> &f_s_thy, int t, int q) {
    //..
    //return..
}