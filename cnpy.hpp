/*
 * =====================================================================================
 *
 *       Filename:  cnpy.h
 *
 *    Description:  Write a stl vector to numpy format 2. 
 *
 *      This program is part of MOOSE simulator. 
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

#ifndef  cnpy_INC
#define  cnpy_INC

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <cstdint>

#if USE_BOOST
#include <boost/algorithm/string/split.hpp>
#else
#include <cstring>
#endif

using namespace std;

void split(vector<string>& strs, string& input, const string& pat) 
{
#ifdef USE_BOOST
    boost::split( strs, input, boost::is_any_of( pat ) );
    return splitVec;
#else
    char* pch;
    pch = strtok( &input[0], pat.c_str() );
    while( pch != NULL )
    {
        strs.push_back( string(pch ) );
        pch = strtok( NULL, pat.c_str() );
    }
    delete pch;
#endif

}

// Preamble of header. 8 bytes long.
array<char, 8> __pre__ = { 
    (char)0x93, 'N', 'U', 'M', 'P', 'Y'     /* Magic */
    , (char)0x02, (char) 0x00               /* format */
};

void write_header( FILE* fp, vector<string> colnames, vector<unsigned int>shape )
{
    // Heder are always at the begining of file.
    fseek( fp, 0, SEEK_SET );

    string dict = ""; // This is the header to numpy file
    dict += "{'descr': [";
    for( auto v : colnames )
        dict += "('" + v + "' , 'f8'),";

    dict += "], 'fortran_order': False, 'shape': (";
    dict += to_string(shape[0]);
    for(size_t i = 1; i < shape.size(); i++) 
    {
        dict += ",";
        dict += to_string(shape[i]);
    }
    if( shape.size() == 1) dict += ",";
    dict += "), }";

    // When appending to this file, we need to rewrite header. Size of header
    // might change since shape values might change. Minimum shape from (1,)
    // could become quite large (13132131321,) etc. For safety region, append a
    // chunk of whitespace characters so that overwriting header does not
    // overwrite the data. This trick would save us from copying data into
    // memory.
    dict += string(11, ' ');                    /* 32 bit number is fit */

    // pad with spaces so that preamble+headerlen+dict is modulo 16 bytes. 
    // preamble is 8 bytes, header len is 4 bytes, total 12.
    // dict needs to end with \n
    unsigned int remainder = 16 - (12 + dict.size()) % 16;
    dict.insert(dict.end(),remainder,' ');
    dict.back() = '\n';
    fwrite( __pre__.data(), sizeof( char ), __pre__.size(), fp );
    uint32_t s = dict.size();
    fwrite( (char*)&s, sizeof( unsigned int), 1, fp);
    fwrite( dict.c_str(), sizeof(char), dict.size(), fp );
}

/**
 * @brief Check if a numpy file is sane or not.
 *
 * Read first 8 bytes and compare with standard header. 
 *
 * @param npy_file Path to file.
 *
 * @return  true if file is sane, else false.
 */
bool is_valid_numpy_file( const string& npy_file )
{
    array<char, 8> buffer;
    FILE* fp = NULL;
    fp = fopen( npy_file.c_str(), "r" );
    fread( buffer.data(), 1, 8, fp );
    fclose( fp );
    return buffer == __pre__;
}

/**
 * @brief Parser header from a numpy file. Store it in vector.
 *
 * @param header
 */
void parse_header( FILE* fp, string& header )
{
    // Read header, till we hit newline character.
    char ch = ' ';
    header.clear();
    while( ch != '\n' and ch != EOF )
    {
        ch = getc( fp );
        header.push_back( ch ); 
    }
}

/**
 * @brief Change shape in numpy header.
 *
 * @param 
 * @param data_len
 * @param 
 */
void change_shape_in_header( const string& filename
        , const size_t data_len, const size_t numcols 
        )
{
    string header;

    // Always open file in r+b mode. a+b mode always append at the end.
    FILE* fp = fopen( filename.c_str(), "r+b" );
    parse_header( fp, header );

    size_t shapePos = header.find( "'shape':" );
    size_t lbrac = header.find( '(', shapePos );
    size_t rbrac = header.find( ')', lbrac );

    string prefixHeader = header.substr( 0, lbrac + 1 );
    string postfixHeader = header.substr( rbrac );

    string shapeStr = header.substr( lbrac + 1, rbrac - lbrac - 1);

    vector<string> tokens;
    split( tokens, shapeStr, "," );

    string newShape = "";
    for (size_t i = 0; i < tokens.size(); i++) 
        newShape += to_string( stoi( tokens[i] ) + data_len/numcols ) + ",";

    string newHeader = prefixHeader + newShape + postfixHeader;
    if( newHeader.size() < header.size() )
    {
        cout << "Warn: Modified header can not be smaller than old header" << endl;
    }

    // Move to at the begining of file and write the new header.
    fseek(fp, 0, SEEK_SET);
    fwrite( newHeader.c_str(), sizeof(char), newHeader.size(), fp );
    fclose( fp );
}

template<typename T>
void save_numpy2( 
        const string& outfile 
        , const vector<double>& vec
        , vector<string> colnames
        , const string openmode 
        )
{
    FILE* fp;

    // In our application, we need to write a vector as matrix. We do not
    // support the stacking of matrices.
    vector<unsigned int> shape;
    shape.push_back( vec.size()/ colnames.size());

    /* In mode "w", open the file and write a header as well. When file is open
     * in mode "a", we assume that file is alreay a valid numpy file.
     */
    if( openmode == "w" )
    {
        fp = fopen( outfile.c_str(), "wb" );
        write_header( fp, colnames, shape );
    }
    else                                        /* Append mode. */
    {
        // Do a sanity check if file is really a numpy file.
        if(! is_valid_numpy_file( outfile ) )
        {
            cout << "Err : File " << outfile << " is not a valid numpy file"
                << " I am not goind to write to it" 
                << endl;
            return;
        }

        // Unfortunetely we need to rewrite the header of the file.
        change_shape_in_header( outfile, vec.size(), colnames.size() );

        //  now open the data in append mode 
        fp = fopen( outfile.c_str(), "ab" );
    }

    // Go to the very end of the file and write the data.
    fwrite( &vec[0], sizeof(T), vec.size(), fp );
    fclose( fp );
}

#endif   /* ----- #ifndef cnpy_INC  ----- */
