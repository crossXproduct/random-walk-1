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

void readInput(int& firstHistoryNumber, int& lastHistoryNumber, int totalRuns, vector<int>& evalTimes, vector<double>& evalQs, double& diffusivity, int& dt){
    cout << "Enter number of first datafile (e.g. \"1\" for history01.dat): ";
    cin >> firstHistoryNumber;
    cout << "Enter number of last datafile (e.g. \"10\" for history10.dat): ";
    cin >> lastHistoryNumber;

    //totalRuns = lastHistoryNumber - firstHistoryNumber + 1; // Number of histories
    //cout << "Total Runs: " << totalRuns << endl;

    cout << "Enter three successive time values (assuming t_0 = 0 s) at which to evaluate probability distribution: \n";
    cin >> evalTimes.at(0);
    cin >> evalTimes.at(1);
    cin >> evalTimes.at(2);

    cout << "Enter three successive (int or decimal) q values at which to evaluate self-intermediate scattering function: \n";
    cin >> evalQs.at(0);
    cin >> evalQs.at(1);
    cin >> evalQs.at(2);

    cout << "Enter diffusivity: ";
    cin >> diffusivity;

    //cout << "Enter time interval for correlation measurements: ";
    //cin >> dt;
}
void readData(string filename, vector<double>& timeBins, vector<double>& totalDisplacements, int& totalTime, int& timeInterval, int& totalRuns){
    ifstream file; // Create file stream
    file.open(filename); // Open file stream
    if(!file.is_open()) { // Check for open failure
        cout << "Error: file " << filename << " could not be opened.\n";
        file.clear();
        string s;
        file >> s;
    }

    timeBins.clear();
    totalDisplacements.clear();

    while(!file.eof()) {
        string time;
        getline(file, time, ',');
        timeBins.push_back(stoi(time));
        totalTime = stoi(time);

        string displacement;
        getline(file, displacement, '\n');
        totalDisplacements.push_back(stod(displacement));
    }

    timeInterval = timeBins.at(1) - timeBins.at(0);

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
vector<double> getSpaceBins(int totalTime, int binSize, int time){
    vector<double> spaceBins;
    for(int i = time % 2; i <= (totalTime * 2) + 1; i += binSize) {
        spaceBins.push_back(i - (time % 2) - (totalTime));
    }
    return spaceBins;
}
void updatePDist(vector<double>& probabilities, vector<double>& displacements, int time, int timeInterval){
    //cout << (displacements.at(time) - time % 2) / 2 - (probabilities.size() - (1 - time % 2)) / 2;
    int offset = 1 - (time * timeInterval) % 2;
    probabilities.at((displacements.at(time) + (probabilities.size() - offset)) / 2) += 1;
}
void updateMeanSquare(vector<double>& meanSquareDisplacement, vector<double> timeBins, vector<double> totalDisplacements){
    for(int di = 0; di < timeBins.size(); di ++){
        int difference = timeBins.size() - di;
        for(int initialIndex = 0; initialIndex < difference; initialIndex++) {
            meanSquareDisplacement.at(di) += pow(totalDisplacements.at(initialIndex + di) - totalDisplacements.at(initialIndex), 2);
        }
    }
}
void updateF_s(vector<double>&f_s, vector<double> timeBins, vector<double> totalDisplacements, double q){
    for(int di = 0; di < timeBins.size(); di ++){
        int difference = timeBins.size() - di;
        for(int initialIndex = 0; initialIndex < difference; initialIndex++) {
            f_s.at(di) += (cos(q * (totalDisplacements.at(initialIndex + di) - totalDisplacements.at(initialIndex))));
        }
    }
}
void PDistThy(vector<double>& probabilitiesThy, vector<double>& spaceBins, double diffusivity, int time, int timeInterval){
    for(int r = 0; r < probabilitiesThy.size(); r++) {
        probabilitiesThy.at(r) = 1/sqrt(4 * M_PI * diffusivity * time * timeInterval) *
        exp(-pow(spaceBins.at(r), 2) / (4 * diffusivity * time * timeInterval));
        //((r * 2) + (time % 2) - (probabilitiesThy.size() - time % 2) * timeInterval) / (4 * diffusivity * time * timeInterval)
    }
}
void meanSquareΤhy(vector<double>& meanSquareDisplacementThy, vector<double> timeBins, double diffusivity){
    for(int t = 0; t < timeBins.size(); t++) {
        meanSquareDisplacementThy.at(t) = (2.0 * diffusivity * timeBins.at(t));
    }
}
void F_sThy(vector<double>&f_s, vector<double> timeBins, double diffusivity, double q){
    for(int t = 0; t < timeBins.size(); t++) {
        f_s.at(t) = (exp(-pow(q, 2) * diffusivity * timeBins.at(t)));
    }
}
void normalizeTime(vector<double>& function, int totalRuns){
    for(int i = 0; i < function.size(); i++) {
        function.at(i) = function.at(i) / (function.size() - i) / totalRuns;
    }
}

void normalizeDist(vector<double>& distribution, int binSize, int totalRuns, int totalTime, int timeInterval){
    for(int i = 0; i < distribution.size(); i++) {
            distribution.at(i) = distribution.at(i) / (totalRuns * binSize);
    }
}
void printDistToFile(vector<double>& distribution, vector<double>& bins, string name){
    ofstream printfile;
    printfile.open(name + ".dat");
    printfile << bins.at(0) << "," << distribution.at(0);
    for(int i = 1; i < distribution.size(); i++) {
        //cout << "i = " << i << ", x = " << x.at(i) << ", y = " << y.at(i) << endl;
        printfile << endl << bins.at(i) << "," << distribution.at(i);
    }
    printfile.close();
}
void saveParams(int totalRuns, int totalTime, int timeInterval, vector<int> evalTimes, vector<double> evalQs, double diffusivity) {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir( buff, FILENAME_MAX );
    string current_dir(buff);
    ofstream printfile;
    printfile.open("params.txt");

    printfile << "CURRENT RUN PARAMETERS" << endl;
    printfile << current_dir << endl;
    printfile << __DATE__ << " , " << __TIME__ << endl;
    printfile << "Runs:" << endl;
    printfile << totalRuns << endl;
    printfile << "Number of steps: " << endl;
    printfile << totalTime << endl;
    printfile << "Step interval: " << endl;
    printfile << timeInterval << endl;
    printfile << "Diffusivity: " << endl;
    printfile << diffusivity << endl;
    printfile << "Times for P(R(t))" << endl;
    for(int i = 0 ; i < 3; i++)
        printfile << evalTimes.at(i) << endl;
    printfile << "Qs for f_s(q,t)" << endl;
    for(int i = 0 ; i < 3; i++)
        printfile << evalQs.at(i) << endl;
    printfile.close();

    /*
    cout << "Total Time: " << totalTime << endl;
    cout << "Time Interval: " << timeInterval << endl;
    cout << "Total Runs: " << totalRuns << endl;
    */
}


int main() {

    cout << "Initializing Variables" << endl;
    // History Files
    string firstHistory;
    string lastHistory;
    int firstHistoryNumber, lastHistoryNumber;

    // Initial Data
    vector<double> timeBins;
    vector<double> totalDisplacements;
    int totalTime, timeInterval, totalRuns;

    // User Input
    vector<int> evalTimes(3);
    vector<double> evalQs(3);
    double diffusivity;
    int dt; // time interval for correlation measurements

    // Read parameters and fill spatial bins
    cout << "Reading Parameters" << endl;
    readInput(firstHistoryNumber, lastHistoryNumber, totalRuns, evalTimes, evalQs, diffusivity, dt);
    totalRuns = lastHistoryNumber - firstHistoryNumber + 1; // Number of histories
    //cout << "Total Runs: " << totalRuns << endl;
    //cout << firstHistoryNumber << " " << lastHistoryNumber << " " << totalRuns << " " << endl;

    if(firstHistoryNumber < 10) { // Create file name from number
            firstHistory = "history0" + to_string(firstHistoryNumber) + ".dat";
        }
        else {
            firstHistory = "history" + to_string(firstHistoryNumber) + ".dat";
        }
    readData(firstHistory, timeBins, totalDisplacements, totalTime, timeInterval, totalRuns);

    // Bins for Probabilities
    cout << "Creating Spatial Bins" << endl;
    vector<double> spaceBins1 = getSpaceBins(totalTime, 2, evalTimes.at(0));
    vector<double> spaceBins2 = getSpaceBins(totalTime, 2, evalTimes.at(1));
    vector<double> spaceBins3 = getSpaceBins(totalTime, 2, evalTimes.at(2));

    // Save parameters to file
    saveParams(totalRuns, totalTime, timeInterval, evalTimes, evalQs, diffusivity);

    // Scale time values to reflect interval
    for(int i = 0; i < 3; i++) {
        evalTimes.at(i) /= timeInterval;
        cout << "Scaled Time: " << evalTimes.at(i);
    }

    cout << "Initializing vectors" << endl;
    cout << "SpaceBins Size: " << spaceBins1.size() << endl;
    for(int i = 0; i < spaceBins1.size(); i++) {
    cout << spaceBins1.at(i) << " ";
    }
    cout << endl;
    // Calculated Distributions
    vector<double> meanSquareDisplacement(totalTime / timeInterval + 1);
    vector<double> probabilityDistribution1(totalTime + (1 - (evalTimes.at(0) * timeInterval) % 2));
    vector<double> probabilityDistribution2(totalTime + (1 - (evalTimes.at(1) * timeInterval) % 2));
    vector<double> probabilityDistribution3(totalTime + (1 - (evalTimes.at(2) * timeInterval) % 2));
    vector<double> f_s1(totalTime / timeInterval + 1);
    vector<double> f_s2(totalTime / timeInterval + 1);
    vector<double> f_s3(totalTime / timeInterval + 1);

    vector<double> meanSquareDisplacementThy(totalTime / timeInterval + 1);
    vector<double> probabilityDistribution1Thy(totalTime + (1 - (evalTimes.at(0) * timeInterval) % 2));
    vector<double> probabilityDistribution2Thy(totalTime + (1 - (evalTimes.at(1) * timeInterval) % 2));
    vector<double> probabilityDistribution3Thy(totalTime + (1 - (evalTimes.at(2) * timeInterval) % 2));
    vector<double> f_s1Thy(totalTime / timeInterval + 1);
    vector<double> f_s2Thy(totalTime / timeInterval + 1);
    vector<double> f_s3Thy(totalTime / timeInterval + 1);

    // Time Correlations
    vector<double> correlations((totalDisplacements.size() - 1) * timeInterval / dt);
    vector<double> correlationBins((totalDisplacements.size() - 1) * timeInterval / dt);

    // Read data and update data distributions with each run
    do {

        if(firstHistoryNumber < 10) {
            firstHistory = "history0" + to_string(firstHistoryNumber) + ".dat";
        }
        else {
            firstHistory = "history" + to_string(firstHistoryNumber) + ".dat";
        }
        cout << "Reading Data" << endl;
        readData(firstHistory, timeBins, totalDisplacements, totalTime, timeInterval, totalRuns);

        cout << "Building Probability Distributions" << endl;
        //cout << "Time 1 = " << evalTimes.at(0) << endl;
        updatePDist(probabilityDistribution1, totalDisplacements, evalTimes.at(0), timeInterval);
        //cout << "Time 2 = " << evalTimes.at(1) << endl;
        updatePDist(probabilityDistribution2, totalDisplacements, evalTimes.at(1), timeInterval);
        //cout << "Time 3 = " << evalTimes.at(2) << endl;
        updatePDist(probabilityDistribution3, totalDisplacements, evalTimes.at(2), timeInterval);

        cout << "Building Mean Square Displacement" << endl;
        updateMeanSquare(meanSquareDisplacement, timeBins, totalDisplacements);

        cout << "Building f_s" << endl;
        updateF_s(f_s1, timeBins, totalDisplacements, evalQs.at(0));
        updateF_s(f_s2, timeBins, totalDisplacements, evalQs.at(1));
        updateF_s(f_s3, timeBins, totalDisplacements, evalQs.at(2));

        cout << "Building Time Correlations" << endl;

        firstHistoryNumber++;
    } while(firstHistoryNumber <= lastHistoryNumber);

    /*
    cout << "PDist 1" << endl;
    for(int i = 0; i < probabilityDistribution1.size(); i++) {
        cout << probabilityDistribution1.at(i) << " ";
    }
    */

    // Normalize
    cout << "Normalizing" << endl;
    normalizeDist(probabilityDistribution1, 2, totalRuns, totalTime, timeInterval);
    normalizeDist(probabilityDistribution2, 2, totalRuns, totalTime, timeInterval);
    normalizeDist(probabilityDistribution3, 2, totalRuns, totalTime, timeInterval);
    normalizeTime(meanSquareDisplacement, totalRuns);
    normalizeTime(f_s1, totalRuns);
    normalizeTime(f_s2, totalRuns);
    normalizeTime(f_s3, totalRuns);


    // Build theory distributions
    cout << "Building Theoretical Probabilities" << endl;
    PDistThy(probabilityDistribution1Thy, spaceBins1, diffusivity, evalTimes.at(0), timeInterval);
    PDistThy(probabilityDistribution2Thy, spaceBins2, diffusivity, evalTimes.at(1), timeInterval);
    PDistThy(probabilityDistribution3Thy, spaceBins3, diffusivity, evalTimes.at(2), timeInterval);

    cout << "Building Theoretical Mean Square Displacement" << endl;
    meanSquareΤhy(meanSquareDisplacementThy, timeBins, diffusivity);

    cout << "Building Theoretical f_s" << endl;
    F_sThy(f_s1Thy, timeBins, diffusivity, evalQs.at(0));
    F_sThy(f_s2Thy, timeBins, diffusivity, evalQs.at(1));
    F_sThy(f_s3Thy, timeBins, diffusivity, evalQs.at(2));

    // Print to files
    printDistToFile(meanSquareDisplacement, timeBins, "msd_avg_orig");
    printDistToFile(probabilityDistribution1, spaceBins1, "pDist1_avg_orig");
    printDistToFile(probabilityDistribution2, spaceBins2, "pDist2_avg_orig");
    printDistToFile(probabilityDistribution3, spaceBins3, "pDist3_avg_orig");
    printDistToFile(f_s1, timeBins, "fs1_avg_orig");
    printDistToFile(f_s2, timeBins, "fs2_avg_orig");
    printDistToFile(f_s3, timeBins, "fs3_avg_orig");

    printDistToFile(meanSquareDisplacementThy, timeBins, "msd_thy");
    printDistToFile(probabilityDistribution1Thy, spaceBins1, "pDist1_thy");
    printDistToFile(probabilityDistribution2Thy, spaceBins2, "pDist2_thy");
    printDistToFile(probabilityDistribution3Thy, spaceBins3, "pDist3_thy");
    printDistToFile(f_s1Thy, timeBins, "fs1_thy");
    printDistToFile(f_s2Thy, timeBins, "fs2_thy");
    printDistToFile(f_s3Thy, timeBins, "fs3_thy");

    return 0;

}