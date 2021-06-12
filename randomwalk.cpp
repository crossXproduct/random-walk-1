/**
 *    @file: randomwalk.cpp
 *  @author: E. Aaron
 *    @date: 06-09-2021
 *   @brief: ...
 * 
 * 
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <ctime>

using namespace std;

const int STEPS = 10; ///number of steps per simulation
const int RUNS = 5; ///number of sims to run
const double Q = 0; ///to be determined
const double DIFFUSION = 0; ///to be determined

///FUNCTION PROTOTYPES
vector <double> run();
/**
 * Generates a random walk with specified number of timesteps,
 * and calculates and returns a vector of total displacements at each timestep.
 * 
 * @param time - the number of steps to generate
 * @return r_totals - vector of total displacements at each timestep
 */

vector < vector<double> > generateData();
/**
 * Creates a vector containing a specified number of random walks using run().
 * 
 * @param runs - number of random walks to generate
 * @return data - vector containing random walks from run()
 */

double meanSquare(vector <vector<double> > data);
/**
 * Calculate and return mean square displacement of data as a
 * function of time (number of timesteps).
 * 
 * @param data - vector containing random walk data
 * @return meanSquare - mean square displacement of all data
 */

vector <double> probDistSpace(vector <vector<double> > data);
/**
 * Calculate and return a vector of probabilities for r_totals as a
 * function of r.
 * 
 * @param data - vector containing random walk data
 * @return probDist - vector containing probabilities for r_totals wrt
 *                    spatial coordinates
 */

vector <double> probDistTime(vector <vector<double> > data);
/**
 * Calculate and return a vector of probabilities for r_totals as
 * a function of time.
 * 
 * @param data - vector containing random walk data
 * @return probDist - vector containing probabilities for r_totals wrt
 *                    number of timesteps
 */
vector <double> intScatFunc(vector <vector<double> > data);
/**
 * Calculate self-intermediate scattering function as a function of time,
 * with adjustable parameter q.
 * 
 * @param data - vector containing random walk data
 * @return scatFunc - vector containing self-intermediate scattering function
 *                    values wrt number of timesteps
 */

///MAIN PROGRAM
int main(){
    vector < vector<double> > data = generateData();
    cout << meanSquare(data) << endl;
    return 0;
} ///main

vector <double> run() {
    vector <double> runData;
    double step = 0.00;
    srand(time(0));
    for(int i = 0; i < STEPS; i++){
        ///repeatedly generate a random number between -1 and 1
        ///and calculate their sum
        step += (rand() * 1.0 / RAND_MAX) * pow(-1,rand());
        runData.push_back(step);
        //cout << runData.at(i) << endl;
    }
    return runData;
}

vector < vector<double> > generateData(){
    vector < vector<double> > data;
    ///fill a vector with data from simulated runs
    for(int i = 0; i < RUNS; i++) {
        data.push_back(run());
    }
    //cout << i << endl;
    return data;
}

double meanSquare(vector <vector<double> > data) {
    ///calculate and store in a vector the r_totals for each run
    vector <double> squares;
    for(int i = 0; i < RUNS; i++) {
        double r_total_squared = pow(((data.at(i)).size()-1), 2);
        squares.push_back(r_total_squared);
    }
    ///calculate the mean square displacement from the r_totals
    double meanSquare;
    for(int i = 0; i < squares.size(); i++) {
        meanSquare = squares.at(i);
    }
    meanSquare /= squares.size();
    return meanSquare;
}

vector <double> probDistSpace(vector <vector<double> > data) {
    ///define a vector whose indices are bin numbers corresponding to
    ///spatial coordinates, and elements contain corresponding probabilities
    vector <double> counts; ///double to fit probabilities later
    ///tally the number of r_totals in each bin
    for(int i = 0; i < RUNS; i++) {
        ///last data element of each run is its r_total
        double r_total = (data.at(i)).size()-1;
        ///add bins as needed to fit data
        while(r_total >= counts.size()) {
                counts.push_back(0.0);
        }
        int bin = r_total; ///assign r_total to its appropriate bin
        counts.at(bin) += 1.0; ///increment that bin's count
    }

    ///normalize by finding and dividing all bins by the max
    int max = 0;
    for(int i = 0; i < counts.size(); i++) { 
        if(counts.at(i) > max) {
            max = counts.at(i);
        }
    }
    for(int i = 0; i < counts.size(); i++) {
        counts.at(i) /= max;
    }
    return counts;
}

vector <double> probDistTime(vector <vector<double> > data) {
    return {};
}