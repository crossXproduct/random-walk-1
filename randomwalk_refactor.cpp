/**
 * randomwalk.cpp
 * E. Aaron
 * 06-23-2021
 *
 * Generates a specified number of thermal histories , each with a specified number of time-
 * sTot, and prints them to files named "history*.dat" where * is the number of the history
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
//void runSpecs(int &hTot, int &sTot, string fName); //Process user input for number of runs and sTot per run
void history(const int &sTot, const int &tInt, const string &path); //Generate a single history and print to file
void recordHistories(const int &hTot, const int &sTot, const int &tInt, const char *fName); //Generate histories and generate datafiles for specified number of runs
string createPath(const char *fName); //Generate destination path for data


int main() {
    int hTot;
    int sTot;
    int tInt;

    char *fName = new char[20];
    cout << "Number of histories: ";
    cin >> hTot;
    cout << "Number of steps per history: ";
    cin >> sTot;
    cout << "Step interval: ";
    cin >> tInt;
    cout << "Folder: ";
    cin >> fName;

    recordHistories(hTot, sTot, tInt, fName);
    cout << "Done";
    return 0;
} ///main

/*
void runSpecs(int &hTot, int &sTot, string fName) {
    cout << "Number of histories: ";
    cin >> hTot;
    cout << "Number of sTot per history: ";
    cin >> sTot;
    cout << "Folder: ";
    cin >> fName;
}
*/

void history(const int &sTot, const int &tInt, const string &path) {
    ///Open file stream
    ofstream file;
    file.open(path);

    ///GENERATE
    //int r = 0;
    //Initial Condition
    int rTot = 0;
    file << 0 << "," << rTot;
    for(int ti = 1; ti <= sTot; ti++){
        //Generate Data
        //r = (rand() % 2) * 2 - 1;
        rTot += (rand() % 2) * 2 - 1;
        if(ti % tInt == 0) {
            //Print to File
            file << endl;
            file << ti << ",";
            file << rTot;
            //cout << "t = " << ti << endl << "r_total = " << r_total << endl;
        }
    }
    ///Close file stream
    file.close();
}

void recordHistories(const int &hTot, const int &sTot, const int &tInt, const char *fName) {
    for(int i = 0; i < hTot; i++) {
        cout << "Generating history #" << i + 1 << "..." << endl;
        srand(i + time(0));
        history(sTot, tInt, createPath(fName));
    }
}

string createPath(const char *fName) {
    ifstream file;
    char number1 = ' ' - 32; //ASCII number 0
    char number2 = ' ' - 32;
    string fullpath = "";

    //const char* directory = new char[fName.length()];
    //directory = fName;
    string folder_name;
    int i = 0;
    while(fName[i] != '\0') {
        folder_name += to_string(stoi(to_string(fName[i])) - 48);
        i++;
    }
    //cout << "Name: " << folder_name << endl;


    if (mkdir(fName, 0777) == -1)
        //cout << "Folder " << folder_name << " exists." << endl;
        cerr << strerror(errno) << endl;

    //else cout << "Folder " << fName << " created." << endl;

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

        fullpath = folder_name + "/history" + to_string(number1) + to_string(number2) + ".dat";
        //cout << "fullpath: " << fullpath;
        file.open(fullpath);

    } while(file.peek() != EOF); //if file is NOT blank, try again

    file.close();
    return fullpath;
}