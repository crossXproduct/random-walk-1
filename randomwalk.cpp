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

const int FRACTION = 1;

///FUNCTION DECLARATIONS (DEFINITIONS BELOW MAIN)
//void runSpecs(int &num_histories, int &num_steps, string folder_pointer); //Process user input for number of runs and steps per run
void history(const int &num_steps, const string &filename); //Generate a single history and print to file
void recordHistories(const int &num_histories, const int &num_steps, const char *folder_pointer); //Generate histories and generate datafiles for specified number of runs
string createPath(const char *folder_pointer); //Generate destination filename for data


int main() {
    int num_histories;
    int num_steps;

    char *folder_pointer = new char[20];
    cout << "Number of histories: ";
    cin >> num_histories;
    cout << "Number of steps per history: ";
    cin >> num_steps;
    cout << "Folder: ";
    cin >> folder_pointer;

    recordHistories(num_histories, num_steps, folder_pointer);
    cout << "Done";
    return 0;
} ///main

/*
void runSpecs(int &num_histories, int &num_steps, string folder_pointer) {
    cout << "Number of histories: ";
    cin >> num_histories;
    cout << "Number of steps per history: ";
    cin >> num_steps;
    cout << "Folder: ";
    cin >> folder_pointer;
}
*/

void history(const int &num_steps, const string &filename) {
    ///Open file stream
    ofstream file;
    file.open(filename);

    ///GENERATE
    //int r = 0;
    //Initial Condition
    int r_total = 0;
    file << 0 << "," << r_total;
    for(int i = 1; i <= num_steps; i++){
        if(i % FRACTION == 0) {
            //Generate Data
            //r = (rand() % 2) * 2 - 1;
            r_total += (rand() % 2) * 2 - 1;
            //Print to File
            file << endl;
            file << i << ",";
            file << r_total;
            //cout << "t = " << i << endl << "r_total = " << r_total << endl;
        }
    }
    ///Close file stream
    file.close();
}

void recordHistories(const int &num_histories, const int &num_steps, const char *folder_pointer) {
    for(int i = 0; i < num_histories; i++) {
        cout << "Generating history #" << i + 1 << "..." << endl;
        srand(i);
        history(num_steps, createPath(folder_pointer));
    }
}

string createPath(const char *folder_pointer) {
    ifstream file;
    char number1 = ' ' - 32; //ASCII number 0
    char number2 = ' ' - 32;
    string fullpath = "";

    //const char* directory = new char[folder_pointer.length()];
    //directory = folder_pointer;
    string folder_name;
    int i = 0;
    while(folder_pointer[i] != '\0') {
        folder_name += to_string(stoi(to_string(folder_pointer[i])) - 48);
        i++;
    }
    //cout << "Name: " << folder_name << endl;


    if (mkdir(folder_pointer, 0777) == -1)
        //cout << "Folder " << folder_name << " exists." << endl;
        cerr << "Error :  " << strerror(errno) << endl;

    //else cout << "Folder " << folder_pointer << " created." << endl;

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