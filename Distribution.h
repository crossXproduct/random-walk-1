#ifndef PLOT_H
#define PLOT_H

#include <cmath>
#include <vector>
using namespace std;

class Distribution {
    private:
        int steps;
        vector<double> x;
        vector<double> y;

    public:
        ///Constructors
        Distribution() {
            //..
        }

        Distribution(int n) {
            //..
        }

        ///Getters & Setters
        void setSteps(int n) { steps = n; }

        void setX(int index, double x_value) { x.at(index) = x_value; }

        void setY(int index, double y_value) { y.at(index) = y_value; }

        int getSteps() { return steps; }

        vector<double> getX() { return x; }
        
        vector<double> getY() { return y; }

        ///Helpers
        void printX() {
            //..
        }

        void printY() {
            //..
        }

};

#endif