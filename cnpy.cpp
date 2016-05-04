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

#include "cnpy.hpp"

int main(int argc, const char *argv[])
{
    vector<double> a = {1.0, 2.0, 3.0, 1.3};
    vector<string> colname = { "time", "value" };
    //save_numpy2<double>( "a.npy", a, colname, "w" );
    save_numpy2<double>( "a.npy", a, colname, "a" );
    return 0;
}

