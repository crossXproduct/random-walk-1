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

void readInput(int& hFirst, int& hLast, int hTot, vector<int>& tiEval, vector<double>& q, double& d, int& dIniT){
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

    //cout << "Enter time interval for correlation measurements: ";
    //cin >> dIniT;
}
void readData(string path, vector<double>& t, vector<double>& rTot, int& sTot, int& tInt, int& hTot){
    ifstream file; // Create file stream
    file.open(path); // Open file stream
    if(!file.is_open()) { // Check for open failure
        cout << "Error: file " << path << " could not be opened.\n";
        file.clear();
        string s;
        file >> s;
    }

    t.clear();
    rTot.clear();

    while(!file.eof()) {
        string time;
        getline(file, time, ',');
        t.push_back(stoi(time));
        sTot = stoi(time);

        string displacement;
        getline(file, displacement, '\n');
        rTot.push_back(stod(displacement));
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
vector<double> getSpaceBins(int sTot, int binSize, int time){
    vector<double> r;
    for(int i = time % 2; i <= (sTot * 2) + 1; i += binSize) {
        r.push_back(i - (time % 2) - (sTot));
    }
    return r;
}
void updatePDist(vector<double>& pDist_avg, vector<double>& rTot, int time, int tInt){
    //cout << (rTot.at(time) - time % 2) / 2 - (pDist_avg.size() - (1 - time % 2)) / 2;
    int offset = 1 - (time * tInt) % 2;
    pDist_avg.at((rTot.at(time) + (pDist_avg.size() - offset)) / 2) += 1;
}
void updateMeanSquare(vector<double>& msd, vector<double> t, vector<double> rTot){
    for(int di = 0; di < t.size(); di ++){
        int difference = t.size() - di;
        for(int initialIndex = 0; initialIndex < difference; initialIndex++) {
            msd.at(di) += pow(rTot.at(initialIndex + di) - rTot.at(initialIndex), 2);
        }
    }
}
void updateF_s(vector<double>&fs, vector<double> t, vector<double> rTot, double qVal){
    for(int di = 0; di < t.size(); di ++){
        int difference = t.size() - di;
        for(int initialIndex = 0; initialIndex < difference; initialIndex++) {
            fs.at(di) += (cos(qVal * (rTot.at(initialIndex + di) - rTot.at(initialIndex))));
        }
    }
}
void PDistThy(vector<double>& pDist_thy, vector<double>& r, double d, int time, int tInt){
    for(int ri = 0; ri < pDist_thy.size(); ri++) {
        pDist_thy.at(ri) = 1/sqrt(4 * M_PI * d * time * tInt) *
        exp(-pow(r.at(ri), 2) / (4 * d * time * tInt));
        //((r * 2) + (time % 2) - (pDist_thy.size() - time % 2) * tInt) / (4 * d * time * tInt)
    }
}
void meanSquareΤhy(vector<double>& msd_thy, vector<double> t, double d){
    for(int ti = 0; ti < t.size(); ti++) {
        msd_thy.at(ti) = (2.0 * d * t.at(ti));
    }
}
void F_sThy(vector<double>&fs, vector<double> t, double d, double q){
    for(int ti = 0; ti < t.size(); ti++) {
        fs.at(ti) = (exp(-pow(q, 2) * d * t.at(ti)));
    }
}
void normalizeTime(vector<double>& y, int hTot){
    for(int i = 0; i < y.size(); i++) {
        y.at(i) = y.at(i) / (y.size() - i) / hTot;
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
    vector<double> rTot;
    int sTot, tInt, hTot;

    // User Input
    vector<int> tiEval(3);
    vector<double> q(3);
    double d;
    int dIniT; // time interval for correlation measurements

    // Read parameters and fill spatial bins
    cout << "Reading Parameters" << endl;
    readInput(hfNum, hlNum, hTot, tiEval, q, d, dIniT);
    hTot = hlNum - hfNum + 1; // Number of histories
    //cout << "Total Runs: " << hTot << endl;
    //cout << hFirst << " " << hlNum << " " << hTot << " " << endl;

    if(hfNum < 10) { // Create file name from number
            hFirst = "history0" + to_string(hfNum) + ".dat";
        }
        else {
            hFirst = "history" + to_string(hfNum) + ".dat";
        }
    readData(hFirst, t, rTot, sTot, tInt, hTot);

    // Bins for Probabilities
    cout << "Creating Spatial Bins" << endl;
    vector<double> r1 = getSpaceBins(sTot, 2, tiEval.at(0));
    vector<double> r2 = getSpaceBins(sTot, 2, tiEval.at(1));
    vector<double> r3 = getSpaceBins(sTot, 2, tiEval.at(2));

    // Save parameters to file
    saveParams(hTot, sTot, tInt, tiEval, q, d);

    // Scale time values to reflect interval
    for(int i = 0; i < 3; i++) {
        tiEval.at(i) /= tInt;
        cout << "Scaled Time: " << tiEval.at(i);
    }

    cout << "Initializing vectors" << endl;
    cout << "SpaceBins Size: " << r1.size() << endl;
    for(int i = 0; i < r1.size(); i++) {
    cout << r1.at(i) << " ";
    }
    cout << endl;
    // Calculated Distributions
    vector<double> msd_avg(sTot / tInt + 1);
    vector<double> pDist1_avg(sTot + (1 - (tiEval.at(0) * tInt) % 2));
    vector<double> pDist2_avg(sTot + (1 - (tiEval.at(1) * tInt) % 2));
    vector<double> pDist3_avg(sTot + (1 - (tiEval.at(2) * tInt) % 2));
    vector<double> fs1_avg(sTot / tInt + 1);
    vector<double> fs2_avg(sTot / tInt + 1);
    vector<double> fs3_avg(sTot / tInt + 1);

    vector<double> msd_thy(sTot / tInt + 1);
    vector<double> pDist1_thy(sTot + (1 - (tiEval.at(0) * tInt) % 2));
    vector<double> pDist2_thy(sTot + (1 - (tiEval.at(1) * tInt) % 2));
    vector<double> pDist3_thy(sTot + (1 - (tiEval.at(2) * tInt) % 2));
    vector<double> fs1_thy(sTot / tInt + 1);
    vector<double> fs2_thy(sTot / tInt + 1);
    vector<double> fs3_thy(sTot / tInt + 1);

    // Read data and update data distributions with each run
    do {

        if(hfNum < 10) {
            hFirst = "history0" + to_string(hfNum) + ".dat";
        }
        else {
            hFirst = "history" + to_string(hfNum) + ".dat";
        }
        cout << "Reading Data" << endl;
        readData(hFirst, t, rTot, sTot, tInt, hTot);

        cout << "Building Probability Distributions from Data" << endl;
        //cout << "Time 1 = " << tiEval.at(0) << endl;
        updatePDist(pDist1_avg, rTot, tiEval.at(0), tInt);
        //cout << "Time 2 = " << tiEval.at(1) << endl;
        updatePDist(pDist2_avg, rTot, tiEval.at(1), tInt);
        //cout << "Time 3 = " << tiEval.at(2) << endl;
        updatePDist(pDist3_avg, rTot, tiEval.at(2), tInt);

        cout << "Building Mean Square Displacement from Data" << endl;
        updateMeanSquare(msd_avg, t, rTot);

        cout << "Building f_s from Data" << endl;
        updateF_s(fs1_avg, t, rTot, q.at(0));
        updateF_s(fs2_avg, t, rTot, q.at(1));
        updateF_s(fs3_avg, t, rTot, q.at(2));

        hfNum++;
    } while(hfNum <= hlNum);

    /*
    cout << "PDist 1" << endl;
    for(int i = 0; i < pDist1_avg.size(); i++) {
        cout << pDist1_avg.at(i) << " ";
    }
    */

    // Normalize
    cout << "Normalizing" << endl;
    normalizeDist(pDist1_avg, 2, hTot, sTot, tInt);
    normalizeDist(pDist2_avg, 2, hTot, sTot, tInt);
    normalizeDist(pDist3_avg, 2, hTot, sTot, tInt);
    normalizeTime(msd_avg, hTot);
    normalizeTime(fs1_avg, hTot);
    normalizeTime(fs2_avg, hTot);
    normalizeTime(fs3_avg, hTot);


    // Build theory distributions
    cout << "Building Theoretical Probabilities" << endl;
    PDistThy(pDist1_thy, r1, d, tiEval.at(0), tInt);
    PDistThy(pDist2_thy, r2, d, tiEval.at(1), tInt);
    PDistThy(pDist3_thy, r3, d, tiEval.at(2), tInt);

    cout << "Building Theoretical Mean Square Displacement" << endl;
    meanSquareΤhy(msd_thy, t, d);

    cout << "Building Theoretical f_s" << endl;
    F_sThy(fs1_thy, t, d, q.at(0));
    F_sThy(fs2_thy, t, d, q.at(1));
    F_sThy(fs3_thy, t, d, q.at(2));

    // Print to files
    printToFile(msd_avg, t, "MSD");
    printToFile(pDist1_avg, r1, "PDist1");
    printToFile(pDist2_avg, r2, "PDist2");
    printToFile(pDist3_avg, r3, "PDist3");
    printToFile(fs1_avg, t, "f_s1");
    printToFile(fs2_avg, t, "f_s2");
    printToFile(fs3_avg, t, "f_s3");

    printToFile(msd_thy, t, "MSD Theory");
    printToFile(pDist1_thy, r1, "PDist1 Theory");
    printToFile(pDist2_thy, r2, "PDist2 Theory");
    printToFile(pDist3_thy, r3, "PDist3 Theory");
    printToFile(fs1_thy, t, "f_s1 Theory");
    printToFile(fs2_thy, t, "f_s2 Theory");
    printToFile(fs3_thy, t, "f_s3 Theory");

    return 0;

}