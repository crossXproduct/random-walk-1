/**
 * randomwalk_analysis.cpp
 * E. Aaron
 * 06-23-2021
 *
 * Reads a series of files containing individual thermal histories (created by randomwalk.cpp),
 * and constructs the mean square displacement as a function of time, the spatial probability
 * distribution at three specified times, and the self-intermediate scattering
 * function for three specified values of parameter q.
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;


///FUNCTION DEFINITIONS///

/**
 * getData
 * Creates and returns a vector version of a datafile specified in call.
 *
 * @param filename - name of datafile from main (string)
 * @param steps - number of steps per run, taken by reference from main and set to number
 *                of lines in datafile (int)
 * @return dataRun - vector of total displacements, one per element (vector double)
 */
vector<double> getData(string filename) {
    ifstream file; // Create file stream
    vector<double> dataRun; // Create data vector
    double r_total; // Total displacement

    file.open(filename); // Open file stream

    if(!file.is_open()) { // Check for open failure
        cout << "Error: file " << filename << " could not be opened.\n";
        file.clear();
        string s;
        file >> s;
    }

    int steps = 0;
    while(!file.eof()) { // Fill vector with data
        steps++;
        file >> r_total;
        if(steps >= dataRun.size()) { // Expand vector if filling for the first time
            dataRun.push_back(r_total); // Push to vector
        }
        else {
            dataRun.at(steps) = r_total; // Replace data if refilling with new data
            //dataRun.shrink_to_fit(); // Only needed if datasets have differing number of steps
        }
    }

    if(!file.eof() && file.fail()) {
            cout << "Error: unable to read file\n" << filename;
            file.clear();
            string s;
            file >> s;
    }

    file.close();
    return dataRun;
}


/**
 * buildMSquare
 * Initializes a vector of mean square displacements for a specified history.
 * Each element is a bin whose index is the time (or number of steps).
 *
 * @param mean_squares - mean square displacement as function of discrete time, taken
 *                       by reference from main (vector double)
 * @param dataRun - total displacements for a single history (vector double)
 * @param runs - number of histories to be analyzed, used for averaging (int)
 * @return void
 */
vector<double> buildMSquare(vector<double> &mean_squares, vector<double> dataRun, int runs, int steps) {
    double m_square; // Mean square displacement
    for(int i = 0; i < steps; i++) {
        m_square = pow(dataRun.at(i), 2) / runs; // Calculate square average
        mean_squares.at(i) += m_square; // Push to vector
    }
    return mean_squares;
}


/**
 * buildPDist
 * Initializes a vector probability distribution (spatial) of specified history. Each element
 * is a bin whose index is the total displacement.
 *
 * @param p_dist - probability distribution as function of discrete position, taken
 *                 by reference from main (vector double)
 * @param dataRun - total displacements for a single history (vector double)
 * @return void
 */
vector<double> buildPDist(vector<double> &p_dist, vector<double> dataRun, int t) {
    int r_total; // Total displacement
    // Create vector elements
    for(int i = 0; i < 2 * t; i++) { // Positive and negative r_totals, need 2x space
        p_dist.push_back(0.0);
    }
    // Fill elements
    for(int i = 0; i < dataRun.size(); i++) {
        r_total = dataRun.at(i); // Set total displacement
        p_dist.at(r_total + t) += 1.0; // Shifted to accommodate negative values
    }
    p_dist.shrink_to_fit();

    // Normalize
    int max = 0;
    for(int i = t; i < 2 * t; i++) { // Find max value
        if (p_dist.at(i - t) > max) {
            max = p_dist.at(i - t);
        }
    }
    for(int i = 0; i < 2 * dataRun.size(); i++) { // Divide all bins by max
        p_dist.at(i) /= max;
        //cout << p_dist.at(i);
    }
    return p_dist;
}


/**
 * buildFs
 * Initializes a vector version of the self-intermediate scattering function for
 * specified history. Each element is a bin whose index is the time (or number of steps).
 *
 * @param f_s - self-intermediate scattering function, taken by reference from main (vector double)
 * @param dataRun - total displacements for a single history (vector double)
 * @param q - parameter of f_s (double)
 * @param runs - number of histories to be analyzed, used for averaging (int)
 * @return void
 */
vector<double> buildFs(vector<double> &f_s, vector<double> dataRun, double q, int runs, int steps) {
    double r_total;
    double f_value;
    for(int i = 0; i < f_s.size(); i++) {
        r_total = dataRun.at(i);
        f_value = cos(q*r_total) / runs;
        if(i >= f_s.size()) {
            f_s.push_back(f_value);
        }
        else f_s.at(i) += f_value;
    }
    return f_s;
}


/**
 * buildMSquareThy
 * Given number of steps, initializes a vector of theoretical mean square displacements.
 *
 * @param mean_squares_thy - theoretical mean square displacement as function of discrete time,
 *                           taken by reference from main (vector double)
 * @param d - diffusion coefficient (double)
 * @param runs - number of histories to be analyzed, used for averaging (int)
 * @param steps - number of steps per run
 * @return void
 */
vector<double> buildMSquareThy(int d, int runs, int steps) {
    vector<double> mean_squares_thy;
    for(int i = 0; i <= steps; i++) {
        double m_square = 2.0 * d * i / runs;
        if(i >= mean_squares_thy.size())
            mean_squares_thy.push_back(m_square);
        else mean_squares_thy.at(i) = m_square;
    }
    return mean_squares_thy;
}


/**
 * buildPDistThy
 * Initializes a vector representing theoretical probability distribution (spatial).
 * Each element is a bin whose index is the total displacement.
 *
 * @param p_dist_thy - theoretical probability distribution as function of discrete position,
 *                     taken by reference from main (vector double)
 * @param d - diffusion coefficient (double)
 * @param t - time in s at which to evaluate distribution (int)
 * @return void
 */
vector<double> buildPDistThy(int d, int t, int steps) {
    vector<double> p_dist_thy;
    double n = 1/sqrt(4 * M_PI * d * t);
    for(int i = (-steps * 2); i < (steps * 2); i++) {
        double pdist_value = n * exp(-pow(i, 2) / 4 / d / t);
        if(i + (steps * 2) >= p_dist_thy.size())
            p_dist_thy.push_back(pdist_value);
        else p_dist_thy.at(i + (steps * 2)) = pdist_value; // Origin will be shifted to right by t
    }
    return p_dist_thy;
}


/**
 * buildFsThy
 * Initializes a vector version of the theoretical self-intermediate scattering function. Each
 * element is a bin whose index is the time (or number of steps).
 *
 * @param f_s_thy - theoretical self-intermediate scattering function, taken by reference
 *                  from main (vector double)
 * @param q - parameter of f_s (double)
 * @param steps - number of steps per run
 * @return void
 */
vector<double> buildFsThy(int q, int d, int steps) {
    vector<double> f_s_thy;
    for(int i = 0; i <= steps; i++) {
        double f_s_value = exp(-pow(q, 2) * d * i);
        if(i >= f_s_thy.size())
            f_s_thy.push_back(f_s_value);
        else f_s_thy.at(i) = f_s_value;
    }
    return f_s_thy;
}

/**
 * printDistribution
 * Prints specified vector elements to a file, one element per line. First line is the "origin"
 * or "zero" of respective distribution.
 *
 * @param f_s_thy - theoretical self-intermediate scattering function, taken by reference
 *                  from main (vector double)
 * @param q - parameter of f_s (double)
 * @param steps - number of steps per run
 * @return void
 */
void printDistribution(vector<double> dist, string name) {
    ofstream printfile;
    printfile.open(name + ".dat");
    printfile << 0 << "," << dist.at(0);
    for(int i = 1; i < dist.size(); i++) {
        printfile << endl << i << "," << dist.at(i);
    }
}

///MAIN PROGRAM///
int main() {


    //VARIABLES
    int t1, t2, t3; // Three times at which to evaluate the spatial probability distribution
    double q1, q2, q3; // Three qs at which to evaluate self-intermediate scattering function
    double d; // Diffusion coefficient

    int runs; // Number of histories, initialized from user input
    int steps; // Number of timesteps in each history, initialized by getData()

    string filename = ""; // Name of data file to be processed
    //initialized by user input and updated by main program
    int startfile, endfile; // Number of first and last data file to process, in numerical order
    ifstream file; // File input stream for reading data


    //INPUT
    //Initialize t1..t3, q1..q3, runs, and d
    cout << "Enter number of first datafile (e.g. \"1\" for history01.dat): ";
    cin >> startfile;
    cout << "Enter number of last datafile (e.g. \"10\" for history10.dat): ";
    cin >> endfile;
    runs = endfile - startfile + 1;
    cout << "Enter three successive time values (assuming t_0 = 0 s) at which to evaluate probability distribution: \n";
    cin >> t1;
    cin >> t2;
    cin >> t3;
    cout << "Enter three successive (int or decimal) q values at which to evaluate self-intermediate scattering function: \n";
    cin >> q1;
    cin >> q2;
    cin >> q3;
    cout << "Enter diffusion coefficient: ";
    cin >> d;


    //DATA
    // Build & print data distributions
    do {
        if(startfile < 10) {
            filename = "history0" + to_string(startfile) + ".dat";
        }
        else {
            filename = "history" + to_string(startfile) + ".dat";
        }

        // A single thermal history
        vector<double> history = getData(filename);
        steps = history.size();

        // Mean square displacement as a function of time
        vector<double> mean_squares(steps, 0.0); // Initialize all elements to 0.0
        buildMSquare(mean_squares, history, runs, steps);
        printDistribution(mean_squares, "mean_squares"); // Print to file
        /*
        // Probability distributions as functions of position at time t
        vector<double> p_dist_t1(steps, 0.0);
        buildPDist(p_dist_t1, history, t1);
        printDistribution(p_dist_t1, "p_dist_t1");
        vector<double> p_dist_t1(steps, 0.0);
        buildPDist(p_dist_t2, history, t2);
        printDistribution(p_dist_t2, "p_dist_t2");
        vector<double> p_dist_t1(steps, 0.0);
        buildPDist(p_dist_t3, history, t3);
        printDistribution(p_dist_t3, "p_dist_t3");

        // Self-intermediate scattering functions as a function of time
        vector<double> f_s_q1(steps, 0.0);
        buildFs(f_s_q1, history, q1, runs, steps);
        printDistribution(f_s_q1, "f_s_q1");
        vector<double> f_s_q1(steps, 0.0);
        buildFs(f_s_q2, history, q2, runs, steps);
        printDistribution(f_s_q2, "f_s_q2");
        vector<double> f_s_q1(steps, 0.0);
        buildFs(f_s_q3, history, q3, runs, steps);
        printDistribution(f_s_q3, "f_s_q3");
        */
        startfile++; // Go to next datafile

    } while(startfile <= endfile);


    //THEORY
    // Build & print theoretical distributions
    // Mean square displacement as a function of time
    vector<double> mean_squares_thy = buildMSquareThy(d, runs, steps);
    printDistribution(mean_squares_thy, "mean_squares_thy");

    // Probability distributions as functions of position at time t
    vector<double> p_dist_thy_t1 = buildPDistThy(d, t1, steps);
    printDistribution(p_dist_thy_t1, "p_dist_thy_t1");
    vector<double> p_dist_thy_t2 = buildPDistThy(d, t2, steps);
    printDistribution(p_dist_thy_t2, "p_dist_thy_t2");
    vector<double> p_dist_thy_t3 = buildPDistThy(d, t3, steps);
    printDistribution(p_dist_thy_t3, "p_dist_thy_t3");

    // Self-intermediate scattering functions as a function of time
    vector<double> f_s_thy_q1 = buildFsThy(q1, d, steps);
    printDistribution(f_s_thy_q1, "f_s_thy_q1");
    vector<double> f_s_thy_q2 = buildFsThy(q2, d, steps);
    printDistribution(f_s_thy_q2, "f_s_thy_q2");
    vector<double> f_s_thy_q3 = buildFsThy(q3, d, steps);
    printDistribution(f_s_thy_q3, "f_s_thy_q3");

    return 0;
} ///MAIN