/*
 * =====================================================================================
 *
 *       Filename:  cnpy.h
 *
 *    Description:  Write a stl vector to numpy format 2.
 *
 *        Version:  1.0
 *        Created:  05/04/2016 10:36:19 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dilawar Singh (), dilawars@ncbs.res.in
 *   Organization:  NCBS Bangalore
 *
 * =====================================================================================
 */

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


//! Byte swap unsigned int
unsigned int swap_uint32( unsigned int num )
{
    cout << "Got " << num << endl;
    unsigned int res = ((num>>24)&0xff) | // move byte 3 to byte 0
        ((num<<8)&0xff0000) | // move byte 1 to byte 2
        ((num>>8)&0xff00) | // move byte 2 to byte 1
        ((num<<24)&0xff000000); // by
    cout << " Fixed to " << res << endl;
    return res;
}

void write_header( FILE* fp, vector<string> colnames, vector<unsigned int>shape )
{
    string dict; // This is the header to numpy file
    dict += "{'descr': [";
    for( auto v : colnames )
        dict += "( '" + v + "' , '<f8'), ";
    dict += "], 'fortran_order': False, 'shape': (";
    dict += to_string(shape[0]);
    for(int i = 1; i < shape.size(); i++) 
    {
        dict += ", ";
        dict += to_string(shape[i]);
    }
    if( shape.size() == 1) dict += ",";
    dict += "), }";
    //pad with spaces so that preamble+dict is modulo 16 bytes. preamble is 12 bytes. dict needs to end with \n
    unsigned int remainder = 16 - (12 + dict.size()) % 16;
    dict.insert(dict.end(),remainder,' ');
    dict.back() = '\n';

    string header;
    header += (char) 0x93;
    header += "NUMPY";
    header += (char) 0x02;
    header += (char) 0x00;
    fwrite( header.c_str(), sizeof( char ), header.size(), fp );
    unsigned int s = dict.size();
    fwrite( (char*) &s, sizeof( unsigned int), 1, fp);
    fwrite( dict.c_str(), sizeof(char), dict.size(), fp );
}

template<typename T>
void save_numpy2( const vector<double>& vec
        , vector<string> colnames
        , const string& outfile )
{
    FILE* fp;
    fp = fopen( outfile.c_str(), "wb" );
    vector<unsigned int> shape = { vec.size()/ colnames.size(), colnames.size() };
    write_header( fp, colnames, shape );
    fwrite( &vec[0], sizeof(double), vec.size(), fp );
    fclose( fp );
}

