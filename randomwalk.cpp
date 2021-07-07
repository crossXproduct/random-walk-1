/**
 * randomwalk.cpp
 * E. Aaron
 * 06-23-2021
 *
 * Generates a specified number of thermal histories , each with a specified number of time-
 * steps, and prints them to files named "history*.dat" where * is the number of the history
 * (i.e. 01, 02, ... , 10, etc). Each line of the files contains the total displacement at
 * that timestep (line 1 is t = 0).
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;

///FUNCTION DECLARATIONS (DEFINITIONS BELOW MAIN)
void runSpecs(int &num_histories, int &num_steps, char * folder); //Process user input for number of runs and steps per run
void history(const int &num_steps, const string &filename); //Generate a single history and print to file
void recordHistories(const int &num_histories, const int &num_steps, char * &folder); //Generate histories and generate datafiles for specified number of runs
string createPath(char * &folder); //Generate destination filename for data


int main() {
    int num_histories;
    int num_steps;
    char *folder;
    runSpecs(num_histories, num_steps, folder);
    recordHistories(num_histories, num_steps, folder);
    return 0;
} ///main

void runSpecs(int &num_histories, int &num_steps, char * folder) {
    cout << "Number of histories: ";
    cin >> num_histories;
    cout << "Number of steps per history: ";
    cin >> num_steps;
    cout << "Folder: ";
    cin >> folder;
}

void history(const int &num_steps, const string &filename) {
    ///Open file stream
    ofstream file;
    file.open(filename);

    ///Generate
    int r = 0;
    int r_total = 0;
    file << r_total;
    for(int i = 1; i <= num_steps; i++){
        file << endl;
        r = (rand() % 2) * 2 - 1;
        r_total += r;
        file << r_total;
    }
    ///Close file stream
    file.close();
}

void recordHistories(const int &num_histories, const int &num_steps, char * &folder) {
    for(int i = 0; i < num_histories; i++) {
        srand(i);
        history(num_steps, createPath(folder));
    }
}

string createPath(char * &folder) {
    ifstream file;
    char number1 = ' ' - 32; //ASCII number 0
    char number2 = ' ' - 32;
    string fullpath = "";

    if (mkdir(folder, 0777) == -1)
        cerr << "Error :  " << strerror(errno) << endl;

    else cout << "Folder " << folder << " created." << endl;

    do { //Search for filename that does not yet exist, and create the file

        if(!file.eof()) {
            file.close();
            file.clear();
        }

        if(number2 == 9) {
            number1++;
            number2 = 0;
            }
        else number2++;

        fullpath = to_string(folder) + "/history" + to_string(number1) + to_string(number2) + ".dat";
        file.open(fullpath);

    } while(file.peek() != EOF); //if file is NOT blank, try again

    file.close();
    return fullpath;
}