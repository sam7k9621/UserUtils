/*******************************************************************************
*
*  Filename    : parameter_format.cc
*  Description : Unit testing for parameter formatting functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "UserUtils/Common/interface/STLUtils.hpp"
#include "UserUtils/MathUtils/interface/Measurement.hpp"

#include <boost/format.hpp>
#include <iostream>
#include <regex>

using namespace std;
using namespace usr;

int
main( int argc, char const* argv[] )
{
/*----------------------------------------------------------------------------*/
  cout << separator() << endl
       << ">>> decimal reinterfacing test testing" << endl;
  {
    for( int i = -1; i < 15; ++i ){
      cout << "Precision: " << i << " | "
           << usr::fmt::decimal( 12345679123456, i ) << endl;
    }

    for( int i = -1; i < 15; ++i ){
      cout << "Precision: " << i << " | "
           << usr::fmt::decimal( 12345679.123456, i ) << endl;
    }
  }
/*----------------------------------------------------------------------------*/
  cout << separator() << endl
       << ">>> scientific reinterfacing test testing" << endl;
  {
    for( int i = -1; i < 15; ++i ){
      cout << "Precision: " << i << " | "
           << usr::fmt::scientific( 12345679123456, i ) << endl;
    }

    for( int i = -1; i < 15; ++i ){
      cout << "Precision: " << i << " | "
           << usr::fmt::scientific( 12345679.123456, i ) << endl;
    }
  }

/*----------------------------------------------------------------------------*/
  cout << separator() << endl
       << ">>> Generic test" << endl;
  {
    const Measurement x( 123, 23, 0.01 );
    const Measurement y( 123, 10.23, 10.21 );

    for( int i = 0; i < 6; ++i ){
      cout << boost::format( "%50s | %50s" )
        % fmt::decimal( x, i )
        % fmt::scientific( x, i )
           << endl;
    }
    for( int i = 0; i < 6; ++i ){
      cout << boost::format( "%50s | %50s" )
        % fmt::decimal( y, i )
        % fmt::scientific( y, i )
           << endl;
    }
  }

/*----------------------------------------------------------------------------*/
  cout << separator() << endl
       << "Auto-precision testing" << endl;
  {
    const vector<Measurement> list = {
      Measurement( 20.1, 1, 0.1 ),
      Measurement( 20.1, 1, 1.2 ),
      Measurement( 201, 1, 1.2 ),
      Measurement( 2.055 , 1, 1.2 )
    };

    for( const auto& m : list ){
      cout << boost::format( "%20s | %20s" )
        % fmt::decimal( m, -1 )
        % fmt::scientific( m, -1 )
           << endl;
    }
  }

  // cout << ">>> Small Measurement testing" << endl;
  // }

  return 0;
}
