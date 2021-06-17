#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include <ctime>
#include "Distribution.h"

using namespace std;

///FUNCTION DECLARATIONS (DEFINITIONS BELOW MAIN)
void runSpecs(int &num_histories, int &num_steps); //Process user input for number of runs and steps per run
void history(int num_steps, string filename); //Generate a single history and print to file
void recordHistories(int num_histories, int num_steps); //Generate histories and generate datafiles for specified number of runs
string getFilename(); //Generate destination filename for data


int main() {
    int num_histories;
    int num_steps;
    runSpecs(num_histories, num_steps);
    recordHistories(num_histories, num_steps);
    return 0;
} ///main

void runSpecs(int &num_histories, int &num_steps) {
    cout << "Number of histories: ";
    cin >> num_histories;
    cout << "Number of steps per history: ";
    cin >> num_steps;
}

void history(int num_steps, string filename) {
    ///Open file stream
    ofstream file;
    file.open(filename);

    ///Seed random number generator
    srand(time(0));

    ///Generate
    int step = 0;
    for(int i = 0; i < num_steps; i++){
        step = rand() % 2 - 1;
        //step /= 2;
        file << step << endl;
    }
    ///Close file stream
    file.close();
}

void recordHistories(int num_histories, int num_steps) {
    for(int i = 0; i < num_histories; i++) {
        history(num_steps, getFilename());
    }
}

string getFilename() {
    ifstream name;
    char number1 = ' ' - 32; //ASCII number 0
    char number2 = ' ' - 32;
    string fullname = "";

    do { //Search for filename that does not yet exist, and create the file

        if(name.is_open()) {
            name.close();
            name.clear();
        }

        if(number2 == 9) {
            number1++;
            number2 = 0;
            } 
        else number2++;

        fullname = "history" + to_string(number1) + to_string(number2) + ".txt";
        name.open(fullname);

    } while(name.peek() != EOF); //if file is NOT blank, try again

    name.close();
    return fullname;
}