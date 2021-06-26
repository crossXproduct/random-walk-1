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
        //cout << r_total; // Print for debugging
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
    //cout << endl; // Print for debugging
    file.close();
    return dataRun;
}

/**
 * buildPDist
 * initializes a vector probability distribution (spatial) of specified history. Each element
 * is a bin whose index is the total displacement.
 *
 * @param p_dist probability distribution as function of discrete position, taken
 *               by reference from main (vector double)
 * @param dataRun total displacements for a single history (vector double)
 */
void buildPDist(vector<double> &p_dist, vector<double> dataRun, int runs, int steps, int t) {
    // Fill elements
    if(t > dataRun.size()) {
        cout << "******" << endl;
        cout << "ERROR: Time value(s) too large. ";
        cout << "******" << endl;
        return;
    }
    p_dist.at(dataRun.at(t - 1) + steps) += 1.0; // Shifted origin to accommodate negative position values

    // Prints for debugging
    cout << left << setw(8) << t;
    for(int i = 0; i < p_dist.size(); i++) {
        cout << left << setw(8) << fixed << setprecision(2) << p_dist.at(i);
    }
    cout << endl;
}

/**
 * buildMSquare
 * initializes a vector of mean square displacements for a specified history.
 * Each element is a bin whose index is the time (or number of steps).
 *
 * @param mean_squares mean square displacement as function of discrete time, taken
 *                       by reference from main (vector double)
 * @param dataRun total displacements for a single history (vector double)
 * @param runs number of histories to be analyzed, used for averaging (int)
 */
void buildMSquare(vector<double> &mean_squares, vector<double> dataRun, int runs, int steps) {
    // Fill vector
    for(int i = 0; i < steps; i++) {
        mean_squares.at(i) += pow(dataRun.at(i), 2); // Push to vector
    }

    // Prints for debugging
    for(int i = 0; i < mean_squares.size(); i++) {
        cout << left << setw(8) << i;
        cout << left << setw(8) << fixed << setprecision(2) << mean_squares.at(i);
        cout << endl;
    }
}

/**
 * buildFs
 * initializes a vector version of the self-intermediate scattering function for
 * specified history. Each element is a bin whose index is the time (or number of steps).
 *
 * @param f_s self-intermediate scattering function, taken by reference from main (vector double)
 * @param dataRun total displacements for a single history (vector double)
 * @param q parameter of f_s (double)
 * @param runs number of histories to be analyzed, used for averaging (int)
 * @param steps number of timesteps
 */
void buildFs(vector<double> &f_s, vector<double> dataRun, double q, int steps) {
    for(int i = 0; i < steps; i++) {
        f_s.at(i) += cos(q * dataRun.at(i));
    }

    // Prints for debugging
    for(int i = 0; i < f_s.size(); i++) {
        cout << left << setw(8) << i;
        cout << left << setw(8) << fixed << setprecision(2) << f_s.at(i);
        cout << endl;
    }

}

/**
 * buildMSquareThy
 * initializes a vector of theoretical mean square displacements, given the number of steps in a history.
 *
 * @param mean_squares_thy theoretical mean square displacement as function of discrete time,
 *                           taken by reference from main (vector double)
 * @param d diffusion coefficient (double)
 * @param runs number of histories to be analyzed, used for averaging (int)
 * @param steps number of steps per run
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
 * initializes a vector representing theoretical probability distribution (spatial).
 * Each element is a bin whose index is the total displacement.
 *
 * @param p_dist_thy theoretical probability distribution as function of discrete position,
 *                   taken by reference from main (vector double)
 * @param d diffusion coefficient (double)
 * @param t time in s at which to evaluate distribution (int)
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
 * initializes a vector version of the theoretical self-intermediate scattering function. Each
 * element is a bin whose index is the time (or number of steps).
 *
 * @param f_s_thy theoretical self-intermediate scattering function, taken by reference from main (vector double)
 * @param q parameter of f_s (double)
 * @param steps number of steps per run
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
 * prints specified vector elements to a file, one element per line. First line is the "origin"
 * or "zero" of respective distribution.
 *
 * @param f_s_thy theoretical self-intermediate scattering function, taken by reference from main (vector double)
 * @param q parameter of f_s (double)
 * @param steps number of steps per run
 */
void printDistribution(vector<double> dist, string name) {
    ofstream printfile;
    printfile.open(name + ".dat");
    printfile << 0 << "," << dist.at(0);
    for(int i = 1; i < dist.size(); i++) {
        printfile << endl << i << "," << dist.at(i);
    }
}

/**
 * normalize
 * divides each element of a vector distribution by the number of histories.
 *
 * @param dist probability or other distribution from history data to be normalized. Taken by reference.
 * @param runs number of histories used to generate the distribution.
 */
void normalize(vector<double> &dist, int runs) {
    for(int i = 0; i < dist.size(); i++) {
        dist.at(i) /= runs;
    }
}


///MAIN PROGRAM///
int main() {

    //VARIABLES
    // Constants & parameters
    int startfile, endfile; // Number of first and last data file to process, in numerical order
    cout << "Enter number of first datafile (e.g. \"1\" for history01.dat): ";
    cin >> startfile;
    cout << "Enter number of last datafile (e.g. \"10\" for history10.dat): ";
    cin >> endfile;

    int runs = endfile - startfile + 1; // Number of histories

    int t1, t2, t3; // Three times at which to evaluate the spatial probability distribution
    cout << "Enter three successive time values (assuming t_0 = 0 s) at which to evaluate probability distribution: \n";
    cin >> t1;
    cin >> t2;
    cin >> t3;

    double q1, q2, q3; // Three qs at which to evaluate self-intermediate scattering function
    cout << "Enter three successive (int or decimal) q values at which to evaluate self-intermediate scattering function: \n";
    cin >> q1;
    cin >> q2;
    cin >> q3;

    double d; // Diffusion coefficient
    cout << "Enter diffusion coefficient: ";
    cin >> d;

    string filename; // Name of current data file
    if(startfile < 10) // Get name of first data file
        filename = "history0" + to_string(startfile) + ".dat";
    else filename = "history" + to_string(startfile) + ".dat";

    int steps = getData(filename).size(); // Number of timesteps in each history (number of lines in first datafile)

    // Theory vectors (initialized)
    vector<double> mean_squares_thy = buildMSquareThy(d, runs, steps);
    vector<double> p_dist_thy_t1 = buildPDistThy(d, t1, steps);
    vector<double> p_dist_thy_t2 = buildPDistThy(d, t2, steps);
    vector<double> p_dist_thy_t3 = buildPDistThy(d, t3, steps);
    vector<double> f_s_thy_q1 = buildFsThy(q1, d, steps);
    vector<double> f_s_thy_q2 = buildFsThy(q2, d, steps);
    vector<double> f_s_thy_q3 = buildFsThy(q3, d, steps);

    // Empty data vectors
    vector<double> mean_squares(steps, 0.0); // Mean square displacement as function of time
    vector<double> p_dist_t1(2 * steps, 0.0); // Spatial probability distribution at time t
    vector<double> p_dist_t2(2 * steps, 0.0);
    vector<double> p_dist_t3(2 * steps, 0.0);
    vector<double> f_s_q1(steps, 0.0); // Self-intermediate scattering funct. as function of time
    vector<double> f_s_q2(steps, 0.0); // with parameter q
    vector<double> f_s_q3(steps, 0.0);

    // FILL DATA VECTORS FROM FILE
    ifstream file; // File input stream for reading data
    int countRuns = 1; // Counter for debugging
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

        // Prints for debugging
        cout << endl << "**********************************************" << endl;
        cout << "Run: " << countRuns << endl;
        cout << "Steps: " << steps << endl;
        cout << "**********************************************" << endl << endl;

        // Prints for debugging <r^2>
        cout << left << setw(8) << "Time";
        cout << left << setw(8) << "<r^2>";
        cout << endl;

        // Mean square displacement as a function of time
        buildMSquare(mean_squares, history, runs, steps);
        normalize(mean_squares, runs);

        // Prints for debugging P(r)
        cout << left << setw(8) << "Time";
        for(int i = 0; i < 2 * steps; i++) {
            cout << left << setw(8) << "P(r = " + to_string(i) + ")";
        }
        cout << endl;

        // Probability distributions as functions of position at time t
        buildPDist(p_dist_t1, history, runs, steps, t1);
        normalize(p_dist_t1, runs);
        buildPDist(p_dist_t2, history, runs, steps, t2);
        normalize(p_dist_t2, runs);
        buildPDist(p_dist_t3, history, runs, steps, t3);
        normalize(p_dist_t3, runs);

        // Self-intermediate scattering functions as a function of time
        // Prints for debugging f_s(q,t)
        cout << left << setw(8) << "Time";
        cout << left << setw(8) << "f_s(q = " << fixed << setprecision(2) << q1 << ")";
        cout << endl;
        buildFs(f_s_q1, history, q1, steps);
        normalize(f_s_q1, runs);
        // Prints for debugging f_s(q,t)
        cout << left << setw(8) << "Time";
        cout << left << setw(8) << "f_s(q = " << fixed << setprecision(2) << q2 << ")";
        cout << endl;
        buildFs(f_s_q2, history, q2, steps);
        normalize(f_s_q2, runs);
        // Prints for debugging f_s(q,t)
        cout << left << setw(8) << "Time";
        cout << left << setw(8) << "f_s(q = " << fixed << setprecision(2) << q3 << ")";
        cout << endl;
        buildFs(f_s_q3, history, q3, steps);
        normalize(f_s_q3, runs);

        startfile++; // Go to next datafile
        countRuns++;
    } while(startfile <= endfile);


    //PRINT VECTORS TO FILES

    // Theory
    // Mean square displacement
    printDistribution(mean_squares_thy, "mean_squares_thy");
    // Probability distributions
    printDistribution(p_dist_thy_t1, "p_dist_thy_t1");
    printDistribution(p_dist_thy_t2, "p_dist_thy_t2");
    printDistribution(p_dist_thy_t3, "p_dist_thy_t3");
    // Self-intermediate scattering functions
    printDistribution(f_s_thy_q1, "f_s_thy_q1");
    printDistribution(f_s_thy_q2, "f_s_thy_q2");
    printDistribution(f_s_thy_q3, "f_s_thy_q3");

    // Data
    // Mean square displacement
    printDistribution(mean_squares, "mean_squares");
    // Probability distributions
    printDistribution(p_dist_t1, "p_dist_t1");
    printDistribution(p_dist_t2, "p_dist_t2");
    printDistribution(p_dist_t3, "p_dist_t3");
    // Self-intermediate scattering functions
    printDistribution(f_s_q1, "f_s_q1");
    printDistribution(f_s_q2, "f_s_q2");
    printDistribution(f_s_q3, "f_s_q3");

    return 0;
} ///MAIN