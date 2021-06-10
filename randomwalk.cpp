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

const int STEPS = 50;
const int RUNS = 5;
const double Q = 0; ///to be determined

///FUNCTION DOCUMENTATION
vector <double> run(int time);
/**
 * Generates a random walk with specified number of timesteps,
 * and calculates and returns a vector of total displacements at each timestep.
 * 
 * @param time - the number of steps to generate
 * @return r_totals - vector of total displacements at each timestep
 */

vector < vector<double> > generateData(int runs);
/**
 * Creates a vector containing a specified number of random walks using run().
 * 
 * @param runs - number of random walks to generate
 * @return data - vector containing random walks from run()
 */

double meanSquare(vector <double> data);
/**
 * Calculate and return mean square displacement of data as a
 * function of time (number of timesteps).
 * 
 * @param data - vector containing random walk data
 * @return meanSquare - mean square displacement of all data
 */

vector <double> probDistSpace(vector <double> data);
/**
 * Calculate and return a vector of probabilities for r_totals as a
 * function of r.
 * 
 * @param data - vector containing random walk data
 * @return probDist - vector containing probabilities for r_totals wrt
 *                    spatial coordinates
 */

vector <double> probDistTime(vector <double> data);
/**
 * Calculate and return a vector of probabilities for r_totals as
 * a function of time.
 * 
 * @param data - vector containing random walk data
 * @return probDist - vector containing probabilities for r_totals wrt
 *                    number of timesteps
 */
vector <double> intScatFunc(vector <double> data);
/**
 * Calculate self-intermediate scattering function as a function of time,
 * with adjustable parameter q.
 * 
 * @param data - vector containing random walk data
 * @return scatFunc - vector containing self-intermediate scattering function
 *                    values wrt number of timesteps
 */

///MAIN PROGRAM
int main(){
    return 0;
}