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

const int FRACTION = 1;
///FUNCTION DEFINITIONS///

/**
 * getData
 * Creates and returns a vector version of a datafile specified in call.
 *
 * @param filename name of datafile from main (string)
 *                   of lines in datafile (int)
 * @return vector of total displacements (doubles), one per element
 */
void getData(string &filename, vector<double> &run_times, vector<double> &total_displacements) {
    // Open data file to read
    ifstream file; // Create file stream
    file.open(filename); // Open file stream
    if(!file.is_open()) { // Check for open failure
        cout << "Error: file " << filename << " could not be opened.\n";
        file.clear();
        string s;
        file >> s;
    }

    // Read in times
    //int run_times.size() = 0;
    while(!file.eof()) { // Fill vector with data
        //run_times.size()++;
        string time_str;
        getline(file, time_str, ',');
        int time = stoi(time_str);
        run_times.push_back(time); // Push to vector
        file.ignore(100,'/n');
        //cout << r_total; // Print for debugging
    }
    // Read in displacements
    while(!file.eof()) { // Fill vector with data
        string disp_str;
        getline(file, disp_str, ',');
        double displacement = stoi(disp_str);
        total_displacements.push_back(displacement); // Push to vector
        file.ignore(100,'/n');
        //cout << r_total; // Print for debugging
    }
    // Check for failure
    if(!file.eof() && file.fail()) {
            cout << "Error: unable to read file\n" << filename;
            file.clear();
            string s;
            file >> s;
    }
    //cout << endl; // Print for debugging
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
void buildPDist(vector<double> &p_dist, vector<double> &p_dist_bins, vector<double> &total_displacements, int t) {
    // Reject invalid input
    if(t > total_displacements.size() * FRACTION) {
        cout << "******" << endl;
        cout << "ERROR: Time value(s) too large. ";
        cout << "******" << endl;
        return;
    }

    // Condition: if t is even, delete all odd elements, delete all even otherwise
    int i = 0;
    if(t % 2 == 0)
        i = 1;
    // For all elements in bin vect, delete every other one
    for(; i < p_dist_bins.size();) {
        p_dist_bins.erase(p_dist_bins.begin() + i);
        i += 2;
    }

    // Fill elements
    p_dist.at((total_displacements.at(t / FRACTION) + (total_displacements.size() - 1)) / 2) += 1.0; // Shifted origin to accommodate negative position values, bin size 2
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
        f_s.at(i) += cos(q * total_displacements.at(i));
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
void buildMSquareThy(vector<double> &mean_squares_thy, vector<double> &run_times double &diffusivity) {
    for(int i = 0; i < run_times.size(); i++) {
        mean_squares_thy.push_back(2.0 * diffusivity * run_times.at(i));
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
void buildPDistThy(vector<double> &p_dist_thy, vector<double> run_times, double &diffusivity, int &t) {
    double n = 1/sqrt(4 * M_PI * diffusivity * t);
    for(int i = 0; i < (run_times.size() * 2 - 1); i++) {
        p_dist_thy.push_back(n * exp( -pow(run_times.at(i) - ((run_times.size() * 2 - 1) - 1), 2) / (4 * diffusivity * t))); // Origin will be shifted to right by t
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
        f_s_thy.push_back(exp(-pow(q, 2) * diffusivity * run_times.at(i)));
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
    ofstream printfile;
    printfile.open(name + ".dat");
    printfile << 0 << "," << y.at(0);
    for(int i = 1; i < y.size(); i++) {
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
void normalize(vector<double> &dist, int runs) {
    for(int i = 0; i < dist.size(); i++) {
        dist.at(i) /= runs * 1.0;
    }
}

void saveParams(int runs, int num_times, vector<double> t_vals, vector<double> q_vals, double diffusivity) {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir( buff, FILENAME_MAX );
    string current_dir(buff);
    ofstream printfile;
    printfile.open("params.txt");

    printfile << "Run Parameters" << endl;
    printfile << current_dir << endl;
    printfile << __DATE__ << " , " << __TIME__ << endl;
    printfile << "Runs: " << runs << endl;
    printfile << "run_times.size()teps: " << run_times.size() << endl;
    printfile << "diffusivity: " << diffusivity << endl;
    printfile << "run_times.size() for P(R(t)):" << endl;
    for(int i = 0 ; i < 3; i++)
        printfile << "\tt" << i + 1 << " = " << t_vals.at(i) << endl;
    printfile << "Qs for f_s(q,t):" << endl;
    for(int i = 0 ; i < 3; i++)
        printfile << "\tq" << i + 1 << " = " << q_vals.at(i) << endl;
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
    vector<double> p_dist_bins = total_displacements; // Same as displacements, but will skip every other displacement to give p_dist bin size 2
    getData(filename, run_times, total_displacements);
    saveParams(runs, run_times.size(), t_vals, q_vals, diffusivity);

    // Theory vectors (initialized)
    vector<double> mean_squares_thy;
    mean_squares_thy = buildMSquareThy(mean_squares_thy, run_times, diffusivity);
    vector<double> p_dist_thy_t1;
    p_dist_thy_t1 = buildPDistThy(p_dist_thy_t1, run_times, diffusivity, t_vals.at(0));
    vector<double> p_dist_thy_t2;
    p_dist_thy_t2 = buildPDistThy(p_dist_thy_t2, run_times, diffusivity, t_vals.at(1));
    vector<double> p_dist_thy_t3;
    p_dist_thy_t3 = buildPDistThy(p_dist_thy_t3, run_times, diffusivity, t_vals.at(2));
    vector<double> f_s_thy_q1;
    f_s_thy_q1 = buildFsThy(f_s_thy_q1, run_times, q_vals.at(0), diffusivity);
    vector<double> f_s_thy_q2;
    f_s_thy_q2 = buildFsThy(f_s_thy_q2, run_times, q_vals.at(1), diffusivity);
    vector<double> f_s_thy_q3;
    f_s_thy_q3 = buildFsThy(f_s_thy_q3, run_times, q_vals.at(2), diffusivity);

    // Empty data distribution vectors
    vector<double> mean_squares(run_times.size(), 0.0); // Mean square displacement as function of time
    vector<double> p_dist_t1(2 * run_times.size() - 1, 0.0); // Spatial probability distribution at time t
    vector<double> p_dist_t2(2 * run_times.size() - 1, 0.0);
    vector<double> p_dist_t3(2 * run_times.size() - 1, 0.0);
    vector<double> f_s_q1(run_times.size(), 0.0); // Self-intermediate scattering funct. as function of time
    vector<double> f_s_q2(run_times.size(), 0.0); // with parameter q
    vector<double> f_s_q3(run_times.size(), 0.0);

    //FILL DATA VECTORS FROM FILE
    ifstream file; // File input stream for reading data
    int count_runs = 1; // Counter for debugging
    do {
        cout << "Run " << count_runs << " of " << runs << endl;
        if(startfile < 10) {
            filename = "history0" + to_string(startfile) + ".dat";
        }
        else {
            filename = "history" + to_string(startfile) + ".dat";
        }

        // A single thermal history
        vector< vector<double> > history;
        getData(filename, run_times, total_displacements);

        // Mean square displacement as a function of time
        buildMSquare(mean_squares, total_displacements);
        // Probability distributions as functions of position at time t
        buildPDist(p_dist_t1, p_dist_bins, total_displacements, t_vals.at(0));
        buildPDist(p_dist_t2, p_dist_bins, total_displacements, t_vals.at(1));
        buildPDist(p_dist_t3, p_dist_bins, total_displacements, t_vals.at(2));
        // Self-intermediate scattering functions as a function of time
        buildFs(f_s_q1, total_displacements, q_vals.at(0));
        buildFs(f_s_q2, total_displacements, q_vals.at(1));
        buildFs(f_s_q3, total_displacements, q_vals.at(2));

        count_runs++;

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

    } while(count_runs <= runs);

    //NORMALIZE DATA VECTORS
    normalize(mean_squares, runs);

    normalize(p_dist_t1, runs);
    normalize(p_dist_t2, runs);
    normalize(p_dist_t3, runs);

    normalize(f_s_q1, runs);
    normalize(f_s_q2, runs);
    normalize(f_s_q3, runs);

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
    printToFile(run_times, mean_squares_thy, "mean_squares_thy");
    // Probability distributions
    printToFile(total_displacements, p_dist_thy_t1, "p_dist_thy_t1");
    printToFile(total_displacements, p_dist_thy_t2, "p_dist_thy_t2");
    printToFile(total_displacements, p_dist_thy_t3, "p_dist_thy_t3");
    //   Self-intermediate scattering functions
    printToFile(run_times, f_s_thy_q1, "f_s_thy_q1");
    printToFile(run_times, f_s_thy_q2, "f_s_thy_q2");
    printToFile(run_times, f_s_thy_q3, "f_s_thy_q3");

    // Data
    //   Mean square displacement
    printToFile(run_times, mean_squares, "mean_squares");
    // Probability distributions
    printToFile(total_displacements, p_dist_t1, "p_dist_t1");
    printToFile(total_displacements, p_dist_t2, "p_dist_t2");
    printToFile(total_displacements, p_dist_t3, "p_dist_t3");
    //   Self-intermediate scattering functions
    printToFile(run_times, f_s_q1, "f_s_q1");
    printToFile(run_times, f_s_q2, "f_s_q2");
    printToFile(run_times, f_s_q3, "f_s_q3");

    cout << "Done";

    return 0;
} ///MAIN