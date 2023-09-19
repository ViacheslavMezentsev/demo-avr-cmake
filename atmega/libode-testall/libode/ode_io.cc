//! \file ode_io.cc
#include "etl/to_string.h"
#include "ode_io.h"

void ode_check_write ( [[maybe_unused]] const char * fn )
{
#ifdef FILE
    //try to open the file
    FILE * ofile = fopen( fn, "wb" );

    //raise an error if needed
    if ( ofile == NULL ) ode_print_exit( "cannot open file, ensure the requisite directories exist (like the output directory)" );

    //close
    fclose( ofile );
#endif
}

void ode_print_exit ( const char * msg )
{
    //print the given message
    printf( "\nODE FAILURE: %s\n\n", msg );

    //cancel the program
    exit( EXIT_FAILURE );
}

std::string ode_int_to_string ( long i )
{
    // https://www.geeksforgeeks.org/converting-string-to-number-and-vice-versa-in-c/
/*
    std::ostringstream str;
    str << i;
    std::string out = str.str();
*/
    std::string out;

    return ( etl::to_string(i, out) );
}
