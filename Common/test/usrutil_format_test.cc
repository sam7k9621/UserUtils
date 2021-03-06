/**
 * @file    format_test.cc
 * @brief   unit testing for format related functions.
 * @author  Yi-Mu "Enoch" Chen (ensc@hep1.phys.ntu.edu.tw)
 * @details Detailed descriptions
 */
#include "UserUtils/Common/interface/Format.hpp"
#include "UserUtils/Common/interface/STLUtils.hpp"

#include <boost/format.hpp>
#include <iostream>

using namespace std;

void
testdecimal( double x )
{
  cout << "Default: " << usr::fmt::base::decimal( x ) << endl
       << usr::separator() << endl;

  for( int i = 0; i < 30; ++i ){
    cout << boost::format( "Precision: %3d | %40s | %40s | %40s" )
      % i
      % usr::fmt::base::decimal( x, i )
      % usr::fmt::base::decimal( x, -i )
      % usr::fmt::base::decimal( x, i ).spacestr( "  " ) << endl;
  }

  cout << usr::separator() << endl;
}

void
testscientific( double x )
{
  for( int i = 0; i < 30; ++i ){
    cout <<
      boost::format( "Precision %3d | %40s | %40s " )
        % i
        % usr::fmt::base::scientific( x, i )
        % usr::fmt::base::scientific( x, i ).spacestr( "  " ) << endl;
  }

  cout << usr::separator() << endl;
}

int
main( int argc, char* argv[] )
{
  cout << usr::separator() << ">>> General decimal point testing" << endl;

  testdecimal( 123456789 );
  testdecimal( 12345679.123456 );

  testscientific( 123456789 );
  testscientific( 12345679.123456 );

  return 0;
}
