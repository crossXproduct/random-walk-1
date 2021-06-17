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

        Distribution(int steps) {
            //..
        }

        ///Getters & Setters
        void setSteps(int steps) {
            //..
        }

        void setX(double x) {
            //..
        }

        void setY(double y) {
            //..
        }

        ///Helpers
        void printX() {
            //..
        }

        void printY() {
            //..
        }

        void update(double r, int index) {
            //..
        }

};

#endif