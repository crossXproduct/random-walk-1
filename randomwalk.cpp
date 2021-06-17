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
#include <fstream>
#include <cmath>
#include <vector>
#include <ctime>

using namespace std;

const int STEPS = 10; ///number of steps per simulation
const int RUNS = 5; ///number of sims to run
const double Q = 0; ///to be determined
const string FILENAME = "data.txt"; ///name of data file to be generated

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

vector <double> meanSquare(vector <vector<double> > data);
/**
 * Calculate and return mean square displacement of data as a
 * function of time (number of timesteps).
 * 
 * @param data - vector containing random walk data
 * @return msquares - mean square displacement of all data
 */

vector <double> spaceDist(vector <vector<double> > data);
/**
 * Calculate and return a vector of probabilities for r_totals as a
 * function of r.
 * 
 * @param data - vector containing random walk data
 * @return sdist - vector containing probabilities for r_totals wrt
 *                    spatial coordinates
 */

vector <double> timeDist(vector <vector<double> > data);
/**
 * Calculate and return a vector of probabilities for r_totals as
 * a function of time.
 * 
 * @param data - vector containing random walk data
 * @return tdist - vector containing probabilities for r_totals wrt
 *                    number of timesteps
 */
vector <double> intScatFunc(vector<double> tdist);
/**
 * Calculate self-intermediate scattering function as a function of time,
 * with adjustable parameter Q.
 * 
 * @param tdist - vector containing time tdist data created by timeDist()
 * @return f_s - vector containing self-intermediate scattering function
 *                    values wrt number of timesteps
 */

void printToFile(vector<double> values, string name, ofstream &file);
/**
 * Print double vector data to a file in the format:
 * index    value
 * 
 * @param values - vector to print
 * @param name - name of vector, string literal
 * @param file - file to print to
 */

void printToFile(vector< vector<double> > values, string name, ofstream &file);
/**
 * Print vector of vector data to a file in the format:
 * index    value1  value2  value3  ...
 * where index is index of "outer" vector and values are 
 * corresponding elements of "inner" vector.
 * 
 * @param values - vector to print
 * @param name - name of vector, string literal
 * @param file - file to print to
 */

///MAIN PROGRAM
int main(){
    ///Declare & initialize variables
    srand(time(0));
    string dataName = "Data";
    vector < vector<double> > data = generateData(); ///vector of individual runs
    string msquaresName = "Mean Square Displacement";
    vector <double> msquares = meanSquare(data); ///mean square displacement as function of time
    string sdistName = "Space Probability Distribution";
    vector <double> sdist = spaceDist(data); ///probability tdist as function of space
    //string tdistName = "Time Probability Distribution";
    //vector <double> tdist = timeDist(data); ///probability tdist as function of time
    ///self-intermediate scattering function as function of time with parameter Q
    //string f_sName = "Self-Intermediate Scattering Function";
    //vector <double> f_s = intScatFunc(tdist);
    
    
    ///Open file stream and print data
    ofstream file;
    file.open(FILENAME);
    printToFile(data, dataName, file);
    printToFile(msquares, msquaresName, file);
    printToFile(sdist, sdistName, file);
    //printToFile(tdist, tdistName, file);
    //printToFile(f_s, f_sName, file);
    file.close();
    
    ///END OF PROGRAM
    return 0;
} ///main

vector <double> run() { ///WORKING
    vector <double> runData;
    double step = 0.00;
    //srand(time(0));
    for(int i = 0; i < STEPS; i++){
        ///repeatedly generate a random number between -1 and 1
        ///and calculate their sum
        //srand(time(0));
        int power = rand();
        double r_total = pow(-1,power);
        step += r_total;
        runData.push_back(step);
        //cout << r_total << endl; //for debugging
    }
    return runData;
}

vector < vector<double> > generateData(){ ///WORKING
    vector < vector<double> > data;
    ///fill a vector with data from simulated runs
    for(int i = 0; i < RUNS; i++) {
        data.push_back(run());
    }
    //cout << i << endl;
    return data;
}

vector<double> meanSquare(vector< vector<double> > data) { ///WORKING
    ///calculate and store in a vector the squares of each r_total for each run
    ///vector index is the time
    vector <double> msquares(RUNS);
    for(int i = 0; i < RUNS; i++) {
        for(int j = 0; j < STEPS; j++) {
            double r_total_squared = pow((data.at(i)).at(j), 2);
            msquares.at(i) += r_total_squared;
        }
    }
    ///divide each r_total square by corresponding number of steps to get average
    for(int i = 0; i < RUNS; i++) {
        msquares.at(i) /= STEPS;
    }
    return msquares;
}

vector <double> spaceDist(vector< vector<double> > data) { ///Debugging

    ///define a vector whose indices are bin numbers corresponding to
    ///spatial coordinates, and elements contain corresponding probabilities
    vector <double> sdist(1); ///double to fit probabilities later

    for(int i = 0; i < RUNS; i++) { ///tally the number of r_totals in each bin
        int r_total = (data.at(i)).at(STEPS-1); ///last data element of each run is its r_total
        while(r_total >= sdist.size()) { ///add bins as needed to fit data
             sdist.push_back(0.0);
        }
        int bin = r_total; ///find appropriate bin for r_total
        sdist.at(bin) = sdist.at(bin) + 1; ///increment bin count
    }
/*
    ///normalize by finding and dividing all bins by area
    int area = 0;
    for(int i = 0; i < sdist.size(); i++) { ///area = sum of number of counts in each bin * bin size (1)
        area = area + sdist.at(i)*1;
    }
    for(int i = 0; i < sdist.size(); i++) {
        sdist.at(i) = sdist.at(i) / area; ///divide all bins by area
        //cout << sdist.at(i);
    } */
    return sdist;
}

vector <double> timeDist(vector< vector<double> > data) {
    ///define a vector whose indices are time coordinates and whose elements
    ///contain corresponding probabilities
    vector <double> tdist(STEPS);
    for(int i = 0; i < RUNS; i++) {
        for(int j = 0; j < STEPS; j++) {
         tdist.at(j) += (data.at(i)).at(j);
        }
    }
    ///normalize by finding and dividing all bins by the max
    int max = 0;
    for(int i = 0; i < tdist.size(); i++) { 
        if (tdist.at(i) > max) {
            max = tdist.at(i);
        }
    }
    for(int i = 0; i < tdist.size(); i++) {
     tdist.at(i) /= max;
        cout << tdist.at(i);
    }
    return tdist;
}

vector <double> intScatFunc(vector<double> tdist) {
    vector <double> f_s(RUNS);
    for(int i = 0; i < RUNS; i++) {
        double r_total = tdist.at(i); 
        f_s.at(i) = cos(Q*r_total)/RUNS;
    }
    return f_s;
}

void printToFile(vector<double> values, string name, ofstream &file) {
    file << name << endl; ///print vector name
    for(int i = 0; i < values.size(); i++) {
        file << setprecision(6) << left << setw(12) <<setfill(' '); ///output format
        file << i << values.at(i) << endl; ///print elements
    }
    file << '\n' << '\n';
}

void printToFile(vector< vector<double> > values, string name, ofstream &file) {
    file << name << endl;
    for(int i = 0; i < values.size(); i++) {
        file << left << setw(12) <<setfill(' '); ///output format
        file << i; ///print index
        for(int j = 0; j < values.at(i).size(); j++) {
            file << setprecision(6) << left << setw(12) <<setfill(' ');
            file << values.at(i).at(j); ///print elements
        }
        file << '\n';
    }
    file << '\n' << '\n';
}