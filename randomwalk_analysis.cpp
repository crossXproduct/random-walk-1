/**
 * randomwalk_analysis.cpp
 * E. Aaron
 * 06-23-2021
 *
 * Generates an executable ("analyze") that reads a series of files containing individual thermal histories (created by randomwalk.cpp),
 * and constructs the mean square displacement as a function of time, the spatial probability distribution at three specified run_times.size(),
 * and the self-intermediate scattering function for three specified values of parameter q.
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <time.h>
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

using namespace std;

///FUNCTION DEFINITIONS///

/**
 * getData
 * Creates and returns a vector version of a datafile specified in call.
 *
 * @param filename name of datafile from main (string)
 *                   of lines in datafile (int)
 * @return vector of total displacements (doubles), one per element
 */
void getData(string &filename, vector<double> &run_times, vector<double> &total_displacements, int &interval, int &count_runs) {
    // Open data file to read
    ifstream file; // Create file stream
    file.open(filename); // Open file stream
    if(!file.is_open()) { // Check for open failure
        cout << "Error: file " << filename << " could not be opened.\n";
        file.clear();
        string s;
        file >> s;
    }

    // Read in times & displacements
    //int run_times.size() = 0;

    total_displacements.clear();
    while(!file.eof()) { // Fill vector with data
        //cout << run_times.size() << endl;
        //cout << total_displacements.size() << endl;
        string time_str;
        getline(file, time_str, ',');
        if(count_runs == 0) {
            run_times.push_back(stoi(time_str)); // Push to vector
        }

        string displ_str;
        getline(file, displ_str, '\n');
        total_displacements.push_back(stod(displ_str));
            //cout << r_total; // Print for debugging
    }


    // Read time interval
    interval = run_times.at(1) - run_times.at(0);

    // Check for failure
    if(!file.eof() && file.fail()) {
            cout << "Error: unable to read file\n" << filename;
            file.clear();
            string s;
            file >> s;
    }
    //cout << endl; // Print for debugging
    file.clear();
    file.close();

}

/**
 * buildPDist
 * initializes a vector probability distribution (spatial) of specified history. Each element
 * is a bin whose index is the total displacement.
 *
 * @param p_dist probability distribution as function of discrete position, taken
 *               by reference from main (vector double)
 * @param total_displacements total displacements for a single history (vector double)
 * @param run_times.size() number of run_times.size() per run (int)
 * @param t time at which P(R(t)) is to be evaluated (int)
 */
void buildPDist(vector<double> &p_dist, vector<double> &p_dist_bins, vector<double> &total_displacements, int t, int interval) {
    // Reject invalid input
    if(t > (total_displacements.size() - 1) * interval) {
        cout << "******" << endl;
        cout << "ERROR: Time value(s) too large.";
        cout << "******" << endl;
        return;
    }

    // Condition: if t is even (i = 0) keep all even elements, if odd (i = 1) keep all odd elements
    int parity = 1;
    if(t % 2 == 0) {
        parity = 0;
    }

    p_dist_bins.clear();
    // Fill p_dist_bins with correct values
    for(int i = parity; i < (total_displacements.size() - 1) * interval * 2 + 1;) {
        p_dist_bins.push_back(i - (p_dist_bins.size() - 1));
        i += 2;
    }

    cout << "R: " << total_displacements.at(t / interval);
    cout << "Position: " << (((total_displacements.at(t / interval) + (parity % 1)) / 2) - (parity % 1)) + (((p_dist.size() + (parity % 1)) / 2) - (parity % 1)) << endl;
    // Fill elements
    p_dist.at((((total_displacements.at(t / interval) + (parity % 1)) / 2) - (parity % 1)) + (((p_dist.size() + (parity % 1)) / 2) - (parity % 1))) += 1.0; // Shifted origin to accommodate negative position values, bin size 2
}

/**
 * buildMSquare
 * initializes a vector of mean square displacements for a specified history.
 * Each element is a bin whose index is the time (or number of run_times.size()).
 *
 * @param mean_squares mean square displacement as function of discrete time, taken
 *                       by reference from main (vector double)
 * @param total_displacements total displacements for a single history (vector double)
 * @param run_times.size() number of run_times.size() per run (int)
 */
void buildMSquare(vector<double> &mean_squares, vector<double> &total_displacements) {
    // Fill vector
    for(int i = 0; i < total_displacements.size(); i++) {
        mean_squares.at(i) += pow(total_displacements.at(i), 2); // Push to vector
    }
}

/**
 * buildFs
 * initializes a vector version of the self-intermediate scattering function for
 * specified history. Each element is a bin whose index is a time (or number of run_times.size() elapsed).
 *
 * @param f_s self-intermediate scattering function, taken by reference from main (vector double)
 * @param total_displacements total displacements for a single history (vector double)
 * @param q parameter of f_s (double)
 * @param run_times.size() number of run_times.size() per run (int)
 */
void buildFs(vector<double> &f_s, vector<double> &total_displacements, double &q) {
    for(int i = 0; i < total_displacements.size(); i++) {
        f_s.at(i) += (cos(q * total_displacements.at(i)));
    }
}

/**
 * buildMSquareThy
 * initializes a vector of theoretical mean square displacements, given the number of run_times.size() in a history.
 *
 * @param mean_squares_thy theoretical mean square displacement as function of discrete time,
 *                           taken by reference from main (vector double)
 * @param diffusivity diffusivity (double)
 * @param runs number of histories to be analyzed, used for averaging (int)
 * @param run_times.size() number of run_times.size() per run (int)
 *
 * @return vector of square displacements (doubles) whose indices are the run_times.size()...to be normalized later
 */
void buildMSquareThy(vector<double> &mean_squares_thy, vector<double> &run_times, double &diffusivity) {
    for(int i = 0; i < run_times.size(); i++) {
        mean_squares_thy.at(i) = (2.0 * diffusivity * run_times.at(i));
    }
}

/**
 * buildPDistThy
 * initializes a vector representing theoretical probability distribution (spatial).
 * Each element is a bin whose index is the total displacement.
 *
 * @param p_dist_thy theoretical probability distribution as function of discrete position,
 *                   taken by reference from main (vector double)
 * @param diffusivity diffusivity (double)
 * @param t time in s at which to evaluate distribution (int)
 * @param run_times.size() number of run_times.size() per run (int)
 *
 * @return vector of (NOT normalized) probabilities (doubles) whose indices are displacement values
 */
void buildPDistThy(vector<double> &p_dist_thy, vector<double> &total_displacements, double &diffusivity, int &t) {
    for(int i = 0; i < p_dist_thy.size(); i++) {
        p_dist_thy.at(i) = 1/sqrt(4 * M_PI * diffusivity * t) * exp( -pow(i - ((p_dist_thy.size() + (1 - t % 2)) / 2 - (1 - t % 2)), 2) / (4 * diffusivity * t)); // Origin will be shifted to right by t
    }
}

/**
 * buildFsThy
 * initializes a vector version of the theoretical self-intermediate scattering function. Each
 * element is a bin whose index is the time (or number of run_times.size()).
 *
 * @param f_s_thy theoretical self-intermediate scattering function, taken by reference from main (vector double)
 * @param q parameter of f_s (double)
 * @param diffusivity diffusivity (double)
 * @param run_times.size() number of run_times.size() per run
 *
 * @return vector of self-intermediate scattering function values (doubles) whose indices are run_times.size()
 */
vector<double> buildFsThy(vector<double> &f_s_thy, vector<double> run_times, double &q, double &diffusivity) {
    for(int i = 0; i < run_times.size(); i++) {
        f_s_thy.at(i) = (exp(-pow(q, 2) * diffusivity * run_times.at(i)));
    }
    return f_s_thy;
}

/**
 * printToFile
 * prints specified vector elements to a file, one element per line. First line is the "origin"
 * or "zero" of respective distribution.
 *
 * @param dist vector of doubles to be printed to file
 * @param name string denoting name of file to be created / written to
 */
void printToFile(vector<double> x, vector<double> y, string name) {
    cout << "x size is " << x.size() << " , " << "y size is " << y.size() << endl;
    ofstream printfile;
    printfile.open(name + ".dat");
    printfile << 0 << "," << y.at(0);
    for(int i = 1; i < y.size(); i++) {
        //cout << "i = " << i << ", x = " << x.at(i) << ", y = " << y.at(i) << endl;
        printfile << endl << x.at(i) << "," << y.at(i);
    }
    printfile.close();
}

/**
 * normalize
 * divides each element of a vector distribution by the number of histories.
 *
 * @param dist vector of doubles to be normalized W.R.T. number of runs
 * @param runs number of histories ("runs") used to generate distribution (int)
 */
void normalize(vector<double> &dist, int &runs, double bin_size) {
    //If probability distribution, normalize by twice number of runs (bin_size = 2.0), otherwise bin_size = 1.0
    //Normalize
    for(int i = 0; i < dist.size(); i++) {
        dist.at(i) /= runs * bin_size;
    }
}

void saveParams(int &runs, int num_times, int &interval, vector<int> t_vals, vector<double> q_vals, double &diffusivity) {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir( buff, FILENAME_MAX );
    string current_dir(buff);
    ofstream printfile;
    printfile.open("params.txt");

    printfile << current_dir << endl;
    printfile << __DATE__ << " , " << __TIME__ << endl << endl;
    printfile << "CURRENT RUN PARAMETERS" << endl;
    printfile << "Runs: " << runs << endl;
    printfile << "Number of steps: " << num_times << endl;
    printfile << "Step interval: " << interval << endl;
    printfile << "Diffusivity: " << diffusivity << endl;
    printfile << "Times for P(R(t)):" << endl;
    for(int i = 0 ; i < 3; i++)
        printfile << setw(5) << right << "t" << i + 1 << " = " << t_vals.at(i) << endl;
    printfile << "Qs for f_s(q,t):" << endl;
    for(int i = 0 ; i < 3; i++)
        printfile << setw(5) << right << "q" << i + 1 << " = " << q_vals.at(i) << endl;
    printfile.close();
}

/**
 * printToScreen
 * outputs values of each distribution for debugging.
 *
 * @param dists vector of vector<double>s to be shown on screen
 * @param runs number of histories ("runs") used to generate distribution (int)
 * @param run_times.size() number of run_times.size() per run
 * @param q_vals vector of doubles containing q values at which f_s is evaluated
 * @param t_vals vector of doubles containing time values at which P(R(t)) is evaluated
 * @param diffusivity diffusivity (double)
 */
void printToScreen(vector<vector<double> > dists, int runs, int num_times, vector<int> t_vals, vector<double> q_vals, double diffusivity){
    // Prints for debugging
    cout << endl << "**********************************************" << endl;
    cout << "Run: " << runs << endl;
    cout << "Steps: " << num_times << endl;
    cout << "Diffusivity: " << diffusivity << endl;
    cout << "**********************************************" << endl << endl;

    // <r^2>
    cout << left << setw(10) << "Time";
    cout << left << setw(10) << "<r^2>";
    cout << endl;
    for(int i = 0; i < dists.at(0).size(); i++) {
        cout << left << setw(10) << i;
        cout << left << setw(10) << fixed << setprecision(4) << dists.at(0).at(i);
        cout << endl;
    }

    // P(t)
    for(int i = 0; i < 3; i++) {
        cout << left << "Time = " << t_vals.at(i) << endl;
        for(int j = 0; j < 2 * num_times - 1; j++) {
            cout << left << setw(10) << "P(r = " + to_string(j - (num_times - 1)) + ")";
        }
        cout << endl;
        for(int j = 0; j < dists.at(i + 1).size(); j++) {
            cout << left << setw(10) << fixed << setprecision(4) << dists.at(i + 1).at(j);
        }
        cout << endl;

    }

   // f_s(q,t)
   for(int i = 0; i < 3; i++) {
        cout << left << setw(10) << "Time";
        cout << left << setw(10) << "f_s(q = " << fixed << setprecision(2) << q_vals.at(i) << ")";
        cout << endl;
        for(int j = 0; j < dists.at(i + 4).size(); j++) {
            cout << left << setw(10) << j;
            cout << left << setw(10) << fixed << setprecision(4) << dists.at(i + 4).at(j);
            cout << endl;
        }
   }
}

///MAIN PROGRAM///
int main() {

    //VARIABLES
    // Constants & parameters
    int interval;
    int startfile, endfile; // Number of first and last data file to process, in numerical order
    cout << "Enter number of first datafile (e.g. \"1\" for history01.dat): ";
    cin >> startfile;
    cout << "Enter number of last datafile (e.g. \"10\" for history10.dat): ";
    cin >> endfile;

    int runs = endfile - startfile + 1; // Number of histories

    vector<int> t_vals(3); // Three run_times.size() at which to evaluate the spatial probability distribution
    cout << "Enter three successive time values (assuming t_0 = 0 s) at which to evaluate probability distribution: \n";
    cin >> t_vals.at(0);
    cin >> t_vals.at(1);
    cin >> t_vals.at(2);

    vector<double> q_vals(3); // Three q_vals at which to evaluate self-intermediate scattering function
    cout << "Enter three successive (int or decimal) q values at which to evaluate self-intermediate scattering function: \n";
    cin >> q_vals.at(0);
    cin >> q_vals.at(1);
    cin >> q_vals.at(2);

    double diffusivity; // diffusivity
    cout << "Enter diffusivity: ";
    cin >> diffusivity;

    string filename; // Name of current data file
    if(startfile < 10) // Get name of first data file
        filename = "history0" + to_string(startfile) + ".dat";
    else filename = "history" + to_string(startfile) + ".dat";

    // Raw data vectors
    vector<double> run_times;
    vector<double> total_displacements;
    vector<double> p_dist_bins; // Same as displacements, but will skip every other displacement to give p_dist bin size 2
    int count_runs = 0; // Counter for debugging
    cout << "Reading Parameters" << endl;
    getData(filename, run_times, total_displacements, interval, count_runs);
    saveParams(runs, run_times.size(), interval, t_vals, q_vals, diffusivity);

    cout << "Building MSD Theory" << endl;
    // Theory vectors (initialized)
    vector<double> mean_squares_thy(run_times.size());
    buildMSquareThy(mean_squares_thy, run_times, diffusivity);

    cout << "Building PDist Theory" << endl;
    vector<double> p_dist_thy_t1((total_displacements.size() - 1) * interval + (1 - t_vals.at(0) % 2));
    cout << "PDist Size: " << p_dist_thy_t1.size() << endl;
    buildPDistThy(p_dist_thy_t1, total_displacements, diffusivity, t_vals.at(0));
    vector<double> p_dist_thy_t2((total_displacements.size() - 1) * interval + (1 - t_vals.at(0) % 2));
    cout << "PDist Size: " << p_dist_thy_t1.size() << endl;
    buildPDistThy(p_dist_thy_t2, total_displacements, diffusivity, t_vals.at(1));
    vector<double> p_dist_thy_t3((total_displacements.size() - 1) * interval + (1 - t_vals.at(0) % 2));
    cout << "PDist Size: " << p_dist_thy_t1.size() << endl;
    buildPDistThy(p_dist_thy_t3, total_displacements, diffusivity, t_vals.at(2));

    cout << "Building F_s Theory" << endl;
    vector<double> f_s_thy_q1(run_times.size());
    buildFsThy(f_s_thy_q1, run_times, q_vals.at(0), diffusivity);
    vector<double> f_s_thy_q2(run_times.size());
    buildFsThy(f_s_thy_q2, run_times, q_vals.at(1), diffusivity);
    vector<double> f_s_thy_q3(run_times.size());
    buildFsThy(f_s_thy_q3, run_times, q_vals.at(2), diffusivity);

    // Empty data distribution vectors
    cout << "Initializing Data Vectors" << endl;
    vector<double> mean_squares(run_times.size()); // Mean square displacement as function of time
    vector<double> p_dist_t1((total_displacements.size() - 1) * interval + (1 - t_vals.at(0) % 2)); // Spatial probability distribution at time t
    vector<double> p_dist_t2((total_displacements.size() - 1) * interval + (1 - t_vals.at(0) % 2));
    vector<double> p_dist_t3((total_displacements.size() - 1) * interval + (1 - t_vals.at(0) % 2));
    vector<double> f_s_q1(run_times.size()); // Self-intermediate scattering funct. as function of time
    vector<double> f_s_q2(run_times.size()); // with parameter q
    vector<double> f_s_q3(run_times.size());

    cout << "Total Displacements:" << endl;
    for(int i = 0; i < total_displacements.size(); i++) {
        cout << total_displacements.at(i) << " , ";
    }
    cout << endl;

    //FILL DATA VECTORS FROM FILE
    ifstream file; // File input stream for reading data
    do {
        count_runs++;
        cout << "Run " << count_runs << " of " << runs << endl;
        if(startfile < 10) {
            filename = "history0" + to_string(startfile) + ".dat";
        }
        else {
            filename = "history" + to_string(startfile) + ".dat";
        }
        cout << "Filename is: " << filename << endl;

        // A single thermal history
        cout << "Reading Data" << endl;
        vector< vector<double> > history;
        getData(filename, run_times, total_displacements, interval, count_runs);

        cout << "Total Displacements:" << endl;
        for(int i = 0; i < total_displacements.size(); i++) {
        cout << total_displacements.at(i) << " , ";
        }
        cout << endl;
        // Mean square displacement as a function of time
        cout << "Building MSD from Data" << endl;
        buildMSquare(mean_squares, total_displacements);// Probability distributions as functions of position at time t

        cout << "Building PDist from Data" << endl;
        buildPDist(p_dist_t1, p_dist_bins, total_displacements, t_vals.at(0), interval);
        cout << "PDist Bins Size is " << p_dist_bins.size() << endl;
        cout << "PDist 1 Size is " << p_dist_t1.size() << endl;
        buildPDist(p_dist_t2, p_dist_bins, total_displacements, t_vals.at(1), interval);
        cout << "PDist Bins Size is " << p_dist_bins.size() << endl;
        cout << "PDist 2 Size is " << p_dist_t1.size() << endl;
        buildPDist(p_dist_t3, p_dist_bins, total_displacements, t_vals.at(2), interval);
        cout << "PDist Bins Size is " << p_dist_bins.size() << endl;
        cout << "PDist 3 Size is " << p_dist_t1.size() << endl;

        // Self-intermediate scattering functions as a function of time
        cout << "Building F_s from Data" << endl;
        buildFs(f_s_q1, total_displacements, q_vals.at(0));
        buildFs(f_s_q2, total_displacements, q_vals.at(1));
        buildFs(f_s_q3, total_displacements, q_vals.at(2));

        startfile++;
        //PRINTS FOR DEBUGGING
        /*
        vector<vector<double> > dataDists;
        dataDists.push_back(mean_squares);
        dataDists.push_back(p_dist_t1);
        dataDists.push_back(p_dist_t2);
        dataDists.push_back(p_dist_t3);
        dataDists.push_back(f_s_q1);
        dataDists.push_back(f_s_q2);
        dataDists.push_back(f_s_q3);
        cout << ">>>>>>>>>DATA<<<<<<<<<<" << endl;
        printToScreen(dataDists, count_runs, run_times.size(), t_vals, q_vals, diffusivity);
        */

    } while(startfile <= endfile);

    //NORMALIZE DATA VECTORS
    cout << "Normalizing MSD" << endl;
    normalize(mean_squares, runs, 1.0);
    normalize(p_dist_t1, runs, 2.0);
    normalize(p_dist_t2, runs, 2.0);
    normalize(p_dist_t3, runs, 2.0);
    cout << "Normalizing f_s" << endl;
    normalize(f_s_q1, runs, 1.0);
    normalize(f_s_q2, runs, 1.0);
    normalize(f_s_q3, runs, 1.0);
    cout << "printing" << endl;

    //PRINTS FOR DEBUGGING
    /*
    vector<vector<double> > dataDists;
    dataDists.push_back(mean_squares);
    dataDists.push_back(p_dist_t1);
    dataDists.push_back(p_dist_t2);
    dataDists.push_back(p_dist_t3);
    dataDists.push_back(f_s_q1);
    dataDists.push_back(f_s_q2);
    dataDists.push_back(f_s_q3);
    cout << ">>>>>>>>>DATA<<<<<<<<<<" << endl;
    printToScreen(dataDists, count_runs, run_times.size(), t_vals, q_vals, diffusivity);

    vector<vector<double> > theoryDists;
    theoryDists.push_back(mean_squares_thy);
    theoryDists.push_back(p_dist_thy_t1);
    theoryDists.push_back(p_dist_thy_t2);
    theoryDists.push_back(p_dist_thy_t3);
    theoryDists.push_back(f_s_thy_q1);
    theoryDists.push_back(f_s_thy_q2);
    theoryDists.push_back(f_s_thy_q3);
    cout << ">>>>>>>>>THEORY<<<<<<<<<<" << endl;
    printToScreen(theoryDists, count_runs, run_times.size(), t_vals, q_vals, diffusivity);
    */

    //PRINT VECTORS TO FILES
    // Theory
    //   Mean square displacement
    cout << "Printing MSD Theory" << endl;
    printToFile(run_times, mean_squares_thy, "mean_squares_thy");
    // Probability distributions
    printToFile(p_dist_bins, p_dist_thy_t1, "p_dist_thy_t1");
    printToFile(p_dist_bins, p_dist_thy_t2, "p_dist_thy_t2");
    printToFile(p_dist_bins, p_dist_thy_t3, "p_dist_thy_t3");
    //   Self-intermediate scattering functions
    cout << "Printing f_s Theory" << endl;
    printToFile(run_times, f_s_thy_q1, "f_s_thy_q1");
    printToFile(run_times, f_s_thy_q2, "f_s_thy_q2");
    printToFile(run_times, f_s_thy_q3, "f_s_thy_q3");

    // Data
    //   Mean square displacement
    cout << "Printing MSD" << endl;
    printToFile(run_times, mean_squares, "mean_squares");

    // Probability distributions
    printToFile(p_dist_bins, p_dist_t1, "p_dist_t1");
    printToFile(p_dist_bins, p_dist_t2, "p_dist_t2");
    printToFile(p_dist_bins, p_dist_t3, "p_dist_t3");
    //   Self-intermediate scattering functions
    cout << "Printing f_s" << endl;
    printToFile(run_times, f_s_q1, "f_s_q1");
    printToFile(run_times, f_s_q2, "f_s_q2");
    printToFile(run_times, f_s_q3, "f_s_q3");
    cout << "Done";

    return 0;
} ///MAIN