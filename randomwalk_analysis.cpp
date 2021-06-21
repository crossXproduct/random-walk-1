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
vector<double> getData(string filename, int &steps); //Return input file stream

// Update distributions
void buildMSquare(vector<double> &mean_squares, vector<double> dataRun, int runs); // Build mean square displacement wrt time
void buildPDist(vector<double> &p_dist, int t, vector<double> dataRun);
void buildFs(vector<double> &f_s, vector<double> dataRun, double q, int runs);
void buildMSquareThy(vector<double> &mean_squares_thy, int t); // Build theoretical mean square displacement wrt time
void buildPDistThy(vector<double> &p_dist_thy, int t); // Build theoretical space prob. dist.
void buildFsThy(vector<double> &f_s_thy, int t, int q); // Build theoretical self-intermediate scattering function wrt time

// Plotting
//void makePlots(vector<double> data, vector<double> theory); //Build a single plot of data alongside theoretical function

int main() {
    // Declare variables
    int t1, t2, t3; // Three times at which to evaluate the spatial probability distribution
    double q1, q2, q3; // Three qs at which to evaluate self-intermediate scattering function
    int runs = 0; // Number of histories, incremented by getData()
    int t; // Number of timesteps in each history
    string filename = "";
    int startname, endname; // Number of first and last data file to process, in numerical order
    vector<double> history; // Vector version of current data file
    ifstream file; // Data file currently being read

    // DATA
    // Mean square displacement as a function of time
    vector<double> mean_squares;
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
    vector<double> mean_squares_thy;
    // Probability distributions as a function of position
    vector<double> p_dist_thy_t1; // at t1
    vector<double> p_dist_thy_t2; // at t2
    vector<double> p_dist_thy_t3; // at t3
    // Self-intermediate scattering functions as a function of time
    vector<double> f_s_thy_q1; //  at q = q1
    vector<double> f_s_thy_q2; // at q = q2
    vector<double> f_s_thy_q3; // at q = q3

    //Take user input: t and q values, start and end data files
    cout << "Enter number of first datafile (e.g. \"1\" for history01.txt): ";
    cin >> startname;

    cout << "Enter number of last datafile (e.g. \"10\" for history10.txt): ";
    cin >> endname;

    cout << "Enter three successive time values (assuming t_0 = 0 s) at which to evaluate probability distribution: ";
    cin >> t1 >> t2 >> t3;

    cout << "Enter three successive q values at which to evaluate self-intermediate scattering function: ";
    cin >> q1 >> q2 >> q3;

    // Build data distributions
    do {
        if(startname < 10) {
            filename = "history0" + to_string(startname) + ".txt";
        }
        else {
            filename = "history" + to_string(startname) + ".txt";
        }
        
        history = getData(filename, t);
        buildMSquare(mean_squares, history, runs);
        buildPDist(p_dist_t1, t1, history);
        buildPDist(p_dist_t2, t2, history);
        buildPDist(p_dist_t3, t3, history);
        buildFs(f_s_q1, history, q1, runs);
        buildFs(f_s_q1, history, q1, runs);
        buildFs(f_s_q1, history, q1, runs);
        startname++;
        runs++;
    } while(startname < endname);

    // Build theoretical distributions
    buildMSquareThy(mean_squares_thy, t);
    buildPDistThy(p_dist_thy_t1, t1);
    buildPDistThy(p_dist_thy_t2, t2);
    buildPDistThy(p_dist_thy_t3, t3);
    buildFsThy(f_s_thy_q1, t, q1);
    buildFsThy(f_s_thy_q2, t, q2);
    buildFsThy(f_s_thy_q3, t, q3);
    
    return 0;
}

vector<double> getData(string filename, int &steps) {
    ifstream file;
    vector<double> dataRun;

    // Open file stream
    file.open(filename);
    if(file.fail()) {
        cout << "File " << filename << " could not be opened.";
        file.clear();
    }

    // Fill or overwrite vector
    int r_total = 0;
    steps = 0;
    while(!file.eof()) {
        int r;
        file >> r; // Read in one step
        if(file.fail()) {
            cout << "Error: unable to read file";
        }
        r_total += r; // Calculate total displacement
        if(steps >= dataRun.size()) { // Expand vector if filling for the first time
            dataRun.push_back(r_total); // Push to vector
        }
        else {
            dataRun.at(steps) = r_total; // Replace data if refilling with new data
            //dataRun.shrink_to_fit(); // Only needed if datasets have differing number of steps
        }
        steps++;
    }
    return dataRun;
}

void buildMSquare(vector<double> &mean_squares, vector<double> dataRun, int runs) {
    double r_square;

    // Build distribution
    for(int i = 0; i < dataRun.size(); i++) { 
        r_square = pow(dataRun.at(i), 2); // Calculate square
        if(i >= mean_squares.size()) {
            mean_squares.push_back(r_square); // Expand vector if building for first time, & push r_square to vector
        }
        else {
            mean_squares.at(i) += r_square; // Push to vector
        }
    }

    // Calculate averages
    for(int i = 0; i < mean_squares.size(); i++) {
        mean_squares.at(i) /= runs; // Update vector
    }
}

void buildPDist(vector<double> &p_dist, int t, vector<double> dataRun) {
    int r_total;

    // Build distribution
    for(int i = 0; i < dataRun.size(); i++) { 
        r_total = dataRun.at(i); // Set total displacement
        while(r_total >= p_dist.size()) {
            p_dist.push_back(0.0); // Expand vector so that bins accommodate r_total
        }
        if(r_total > 0) {
            p_dist.at(r_total) += 1.0; // Increment appropriate bin
        }
        //IGNORE r_totals less than 0...this assumes a symmetric distribution
        //OR could shift distribution to accommodate increasingly negative values
    }

    // Normalize
    int max = 0;
    for(int i = 0; i < p_dist.size(); i++) { 
        if (p_dist.at(i) > max) {
            max = p_dist.at(i);
        }
    }
    for(int i = 0; i < p_dist.size(); i++) {
        p_dist.at(i) /= max;
        //cout << p_dist.at(i);
    }
}

void buildFs(vector<double> &f_s, vector<double> dataRun, double q, int runs) {
    double r_total;
    for(int i = 0; i < f_s.size(); i++) {
        r_total = dataRun.at(i);
        f_s.at(i) = cos(q*r_total)/runs;
    }
    //return..
}

void buildMSquareThy(vector<double> &mean_squares_thy, int t) { 
    //..
    //return...
}

void buildPDistThy(vector<double> &p_dist_thy, int t) {
    //..
    //return..
}

void buildFsThy(vector<double> &f_s_thy, int t, int q) {
    //..
    //return..
}