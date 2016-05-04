/*
 * =====================================================================================
 *
 *       Filename:  cnpy.cpp
 *
 *    Description:  Test the implementation.
 *
 *        Version:  1.0
 *        Created:  05/04/2016 11:02:03 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dilawar Singh (), dilawars@ncbs.res.in
 *   Organization:  NCBS Bangalore
 *
 * =====================================================================================
 */

#include <random>
#include <iterator>
#include <algorithm>
#include "cnpy.hpp"

using namespace std;

int main(int argc, const char *argv[])
{
    random_device rnd_device;

    mt19937 mersenne_engine(rnd_device());
    uniform_real_distribution<double> dist(0, 99.0);

    auto gen = std::bind(dist, mersenne_engine);

    vector<double> vec(100);

    vector<double> a = {1.0, 2.0, 3.0, 1.3};
    vector<string> colname = { "time", "value" };
    save_numpy2<double>( "a.npy", a, colname, "w" );
    for (size_t i = 0; i < 100; i++) 
    {
        cout << "Appending " << vec.size() << " more entries" << endl;
        generate(begin(vec), end(vec), gen);
        save_numpy2<double>( "a.npy", vec, colname, "a" );
    }
    return 0;
}

