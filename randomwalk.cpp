/**
 *    @file: randomwalk.cpp
 *  @author: E. Aaron
 *    @date: 06-09-2021
 *   @brief: ...
 * 
 * 
 */

#include <cstdlib>
#include <cmath>
#include <vector>

using namespace std;

vector <double> run(int time);
/**
 * Generates a random walk with specified number of timesteps,
 * and returns a vector with total displacements at each timestep.
 * 
 * @param time - the number of steps to generate.
 * @return r_totals - vector of total displacements at each timestep.
 */

vector < vector<double> > generateData(int runs);
/**
 * Creates a vector containing a specified number of random walks using run().
 * 
 * @param runs - number of random walks to generate.
 * @return data - vector containing random walks from run().
 */

double meanSquare(vector <double> data);
/**
 * Calculate and return mean square displacement of data as a function of time (number of timesteps).
 * 
 * @return meanSquare - mean square displacement of all data.
 */

vector <double> probDistSpace();
/**
 * Calculate and return a vector of probabilities for r_totals as a function of r.
 */
vector <double> intScatFunc();

int main(){
    return 0;
}