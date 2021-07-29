/**
 * randomwalk_analysis_2.cpp
 * E. Aaron
 * 06-23-2021
 *
 * Generates an executable ("g++ -o analyze ...") that reads a series of files containing individual thermal histories (created by randomwalk.cpp),
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

void readInput(int& hFirst, int& hLast, vector<int>& tiEval, vector<double>& q, double& d, int& overlap){
    cout << "Enter number of first datafile (e.g. \"1\" for history01.dat): ";
    cin >> hFirst;
    cout << "Enter number of last datafile (e.g. \"10\" for history10.dat): ";
    cin >> hLast;

    //hTot = hLast - hFirst + 1; // Number of histories
    //cout << "Total Runs: " << hTot << endl;

    cout << "Enter three successive time values (assuming t_0 = 0 s) at which to evaluate probability distribution: \n";
    cin >> tiEval.at(0);
    cin >> tiEval.at(1);
    cin >> tiEval.at(2);

    cout << "Enter three successive (int or decimal) q values at which to evaluate self-intermediate scattering function: \n";
    cin >> q.at(0);
    cin >> q.at(1);
    cin >> q.at(2);

    cout << "Enter diffusivity: ";
    cin >> d;

    cout << "1. Non-overlapping time intervals" << endl;
    cout << "2. Overlapping time intervals (choose initial time difference)" << endl;
    cout << "Choose an option (1 or 2) for averaging time intervals: " << endl;
    cin >> overlap;
}
void readData(string path, vector<double>& t, vector<double>& rt, int& sTot, int& tInt){
    ifstream file; // Create file stream
    file.open(path); // Open file stream
    if(!file.is_open()) { // Check for open failure
        cout << "Error: file " << path << " could not be opened.\n";
        file.clear();
        string s;
        file >> s;
    }

    t.clear();
    rt.clear();

    while(!file.eof()) {
        string time;
        getline(file, time, ',');
        t.push_back(stoi(time));
        sTot = stoi(time);

        string displacement;
        getline(file, displacement, '\n');
        rt.push_back(stod(displacement));
    }

    tInt = t.at(1) - t.at(0);

    if(!file.eof() && file.fail()) {
            cout << "Error: unable to read file\n" << path;
            file.clear();
            string s;
            file >> s;
    }
    //cout << endl; // Print for debugging
    file.clear();
    file.close();
}
vector<double> getR(int sTot, int binSize, int ti){
    vector<double> r;
    for(int i = ti % 2; i <= (sTot * 2) + 1; i += binSize) {
        r.push_back(i - (ti % 2) - (sTot));
    }
    return r;
}
void updatePDisavg(vector<double>& pDisavg, vector<double>& rt, int ti, int tInt){
    //cout << (rt.at(time) - time % 2) / 2 - (pDisavg.size() - (1 - time % 2)) / 2;
    int offset = 1 - (ti * tInt) % 2;
    pDisavg.at((rt.at(ti) + (pDisavg.size() - offset)) / 2) += 1;
}
void updateMSD_avg(vector<double>& msd_avg, vector<double> t, vector<double> rt, vector<int>& dInit, vector<int>& n){
    n.assign(t.size(),0);
    for(int ti = 0; ti < t.size(); ti++){
        for(int t0 = 0; t0 < t.size() - ti; t0 += dInit.at(ti)) {
            //cout << "t0 + ti = " << t0 + ti << endl; //print for debugging
            msd_avg.at(ti) += pow(rt.at(t0 + ti) - rt.at(t0), 2);
            n.at(ti)++;
        }
    }
}
void updateFs_avg(vector<double>&fs_t, vector<double> t, vector<double> rt, double qVal, vector<int>& dInit){
    for(int ti = 0; ti < t.size(); ti ++){
        for(int t0 = 0; t0 < t.size() - ti; t0+= dInit.at(ti)) {
            fs_t.at(ti) += (cos(qVal * (rt.at(t0 + ti) - rt.at(t0))));
        }
    }
}
void getPDist_thy(vector<double>& pDist_thy, vector<double>& r, double d, int ti, int tInt){
    for(int ri = 0; ri < pDist_thy.size(); ri++) {
        pDist_thy.at(ri) = 1/sqrt(4 * M_PI * d * ti * tInt) *
        exp(-pow(r.at(ri), 2) / (4 * d * ti * tInt));
        //((r * 2) + (ti % 2) - (pDist_thy.size() - ti % 2) * tInt) / (4 * d * ti * tInt)
    }
}
void getMSD_thy(vector<double>& msd_thy, vector<double> t, double d){
    for(int ti = 0; ti < t.size(); ti++) {
        msd_thy.at(ti) = (2.0 * d * t.at(ti));
    }
}
void getFs_thy(vector<double>&fs_thy, vector<double> t, double d, double q){
    for(int ti = 0; ti < t.size(); ti++) {
        fs_thy.at(ti) = (exp(-pow(q, 2) * d * t.at(ti)));
    }
}
void normalizeTime(vector<double>& y, vector<int>& dInit, int hTot, vector<int> n){
    for(int ti = 0; ti < y.size(); ti++) {
        y.at(ti) = y.at(ti) / (n.at(ti) * hTot);
    }
}
void normalizeDist(vector<double>& y, int binSize, int hTot, int sTot, int tInt){
    for(int i = 0; i < y.size(); i++) {
            y.at(i) = y.at(i) / (hTot * binSize);
    }
}
void printToFile(vector<double>& y, vector<double>& x, string name){
    ofstream file;
    file.open(name + ".dat");
    file << x.at(0) << "," << y.at(0);
    for(int i = 1; i < y.size(); i++) {
        //cout << "i = " << i << ", x = " << x.at(i) << ", y = " << y.at(i) << endl;
        file << endl << x.at(i) << "," << y.at(i);
    }
    file.close();
}
void saveParams(int hTot, int sTot, int tInt, vector<int> tiEval, vector<double> q, double d) {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir( buff, FILENAME_MAX );
    string cDir(buff);
    ofstream pFile;
    pFile.open("params.txt");

    pFile << "CURRENT RUN PARAMETERS" << endl;
    pFile << cDir << endl;
    pFile << __DATE__ << " , " << __TIME__ << endl;
    pFile << "Runs:" << endl;
    pFile << hTot << endl;
    pFile << "Number of steps: " << endl;
    pFile << sTot << endl;
    pFile << "Step interval: " << endl;
    pFile << tInt << endl;
    pFile << "Diffusivity: " << endl;
    pFile << d << endl;
    pFile << "Times for P(R(t))" << endl;
    for(int i = 0 ; i < 3; i++)
        pFile << tiEval.at(i) << endl;
    pFile << "Qs for f_s(q,t)" << endl;
    for(int i = 0 ; i < 3; i++)
        pFile << q.at(i) << endl;
    pFile.close();

    /*
    cout << "Total Time: " << sTot << endl;
    cout << "Time Interval: " << tInt << endl;
    cout << "Total Runs: " << hTot << endl;
    */
}


int main() {

    cout << "Initializing Variables" << endl;
    // History Files
    string hFirst;
    string hLast;
    int hfNum, hlNum;

    // Initial Data
    vector<double> t;
    vector<double> rt;
    int sTot, tInt, hTot;

    // User Input
    vector<int> tiEval(3);
    vector<double> q(3);
    double d;
    int overlap;

    // Read parameters and fill spatial bins
    cout << "Reading Parameters" << endl;
    readInput(hfNum, hlNum, tiEval, q, d, overlap);
    hTot = hlNum - hfNum + 1; // Number of histories

    if(hfNum < 10) { // Create file name from number
            hFirst = "history0" + to_string(hfNum) + ".dat";
    }
    else {
        hFirst = "history" + to_string(hfNum) + ".dat";
    }

    readData(hFirst, t, rt, sTot, tInt);

    vector<int> dInit(t.size()); // difference btw initial averaging time indices
    vector<int> n(t.size(),0);
    //cout << "dInit size: " << dInit.size() << endl;
    //cout << "tiDif size: " << tiDif.size() << endl;
    if(overlap == 1) {
        dInit.at(0) = 1;
        for(int ti = 1; ti < t.size(); ti++) {
            dInit.at(ti) = ti;
            //cout << "dInit: " << dInit.at(i) << endl;
        }
    }

    else {
        cout << "Enter a value for initial time index (NOT time) difference: ";
        int di;
        cin >> di;
        for(int ti = 0; ti < t.size(); ti++) {
            dInit.at(ti) = di; // assign selected time index value to all elements of dInit
            //cout << "dInit: " << dInit.at(i) << endl;
        }
    }

    // Bins for Probabilities
    cout << "Creating Spatial Bins" << endl;
    vector<double> r1 = getR(sTot, 2, tiEval.at(0));
    vector<double> r2 = getR(sTot, 2, tiEval.at(1));
    vector<double> r3 = getR(sTot, 2, tiEval.at(2));

    // Save parameters to file
    saveParams(hTot, sTot, tInt, tiEval, q, d);

    // Scale time values to reflect interval
    for(int i = 0; i < 3; i++) {
        tiEval.at(i) /= tInt;
        //cout << "Scaled Time: " << tiEval.at(i);
    }

    /*
    cout << "Initializing vectors" << endl;
    cout << "R Size: " << r1.size() << endl;
    for(int i = 0; i < r1.size(); i++) {
    cout << r1.at(i) << " ";
    }
    cout << endl;
    */

    // Calculated Distributions
    vector<double> msd_avg(t.size());
    vector<double> pDist1_avg(sTot + (1 - (tiEval.at(0) * tInt) % 2));
    vector<double> pDist2_avg(sTot + (1 - (tiEval.at(1) * tInt) % 2));
    vector<double> pDist3_avg(sTot + (1 - (tiEval.at(2) * tInt) % 2));
    vector<double> fs1_avg(t.size());
    vector<double> fs2_avg(t.size());
    vector<double> fs3_avg(t.size());

    vector<double> msd_thy(t.size());
    vector<double> pDist1_thy(sTot + (1 - (tiEval.at(0) * tInt) % 2));
    vector<double> pDist2_thy(sTot + (1 - (tiEval.at(1) * tInt) % 2));
    vector<double> pDist3_thy(sTot + (1 - (tiEval.at(2) * tInt) % 2));
    vector<double> fs1_thy(t.size());
    vector<double> fs2_thy(t.size());
    vector<double> fs3_thy(t.size());

    // Read data and update data distributions with each run
    do {

        if(hfNum < 10) {
            hFirst = "history0" + to_string(hfNum) + ".dat";
        }
        else {
            hFirst = "history" + to_string(hfNum) + ".dat";
        }
        cout << "Reading Data" << endl;
        readData(hFirst, t, rt, sTot, tInt);

        cout << "Building Probability Distributions from Data" << endl;
        //cout << "Time 1 = " << tiEval.at(0) << endl;
        updatePDisavg(pDist1_avg, rt, tiEval.at(0), tInt);
        //cout << "Time 2 = " << tiEval.at(1) << endl;
        updatePDisavg(pDist2_avg, rt, tiEval.at(1), tInt);
        //cout << "Time 3 = " << tiEval.at(2) << endl;
        updatePDisavg(pDist3_avg, rt, tiEval.at(2), tInt);

        cout << "Building Mean Square Displacement from Data" << endl;
        updateMSD_avg(msd_avg, t, rt, dInit, n);

        cout << "Normalization Accumulator" << endl;
        for(int i = 0; i < n.size(); i++)
            cout << n.at(i) << endl;

        cout << "Building f_s from Data" << endl;
        updateFs_avg(fs1_avg, t, rt, q.at(0), dInit);
        updateFs_avg(fs2_avg, t, rt, q.at(1), dInit);
        updateFs_avg(fs3_avg, t, rt, q.at(2), dInit);

        hfNum++;
    } while(hfNum <= hlNum);

    cout << "Normalization Accumulator" << endl;
    for(int i = 0; i < n.size(); i++)
        cout << n.at(i) << endl;
    // Normalize
    cout << "Normalizing" << endl;
    normalizeDist(pDist1_avg, 2, hTot, sTot, tInt);
    normalizeDist(pDist2_avg, 2, hTot, sTot, tInt);
    normalizeDist(pDist3_avg, 2, hTot, sTot, tInt);
    normalizeTime(msd_avg, dInit, hTot, n);
    normalizeTime(fs1_avg, dInit, hTot, n);
    normalizeTime(fs2_avg, dInit, hTot, n);
    normalizeTime(fs3_avg, dInit, hTot, n);


    // Build theory distributions
    cout << "Building Theoretical Probabilities" << endl;
    getPDist_thy(pDist1_thy, r1, d, tiEval.at(0), tInt);
    getPDist_thy(pDist2_thy, r2, d, tiEval.at(1), tInt);
    getPDist_thy(pDist3_thy, r3, d, tiEval.at(2), tInt);

    cout << "Building Theoretical Mean Square Displacement" << endl;
    getMSD_thy(msd_thy, t, d);

    cout << "Building Theoretical f_s" << endl;
    getFs_thy(fs1_thy, t, d, q.at(0));
    getFs_thy(fs2_thy, t, d, q.at(1));
    getFs_thy(fs3_thy, t, d, q.at(2));

    // Print to files
    printToFile(msd_avg, t, "msd_avg");
    printToFile(pDist1_avg, r1, "pDist1_avg");
    printToFile(pDist2_avg, r2, "pDist2_avg");
    printToFile(pDist3_avg, r3, "pDist3_avg");
    printToFile(fs1_avg, t, "fs1_avg");
    printToFile(fs2_avg, t, "fs2_avg");
    printToFile(fs3_avg, t, "fs3_avg");

    printToFile(msd_thy, t, "msd_thy");
    printToFile(pDist1_thy, r1, "pDist1_thy");
    printToFile(pDist2_thy, r2, "pDist2_thy");
    printToFile(pDist3_thy, r3, "pDist3_thy");
    printToFile(fs1_thy, t, "fs1_thy");
    printToFile(fs2_thy, t, "fs2_thy");
    printToFile(fs3_thy, t, "fs3_thy");

    cout << "DONE" << endl;

    return 0;

}