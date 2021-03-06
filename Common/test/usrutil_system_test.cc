/**
 * @file    system_test.cc
 * @brief   Testing system interaction functions
 * @author  Yi-Mu "Enoch" Chen (ensc@hep1.phys.ntu.edu.tw)
 */
#include "UserUtils/Common/interface/SystemUtils/Command.hpp"
#include "UserUtils/Common/interface/SystemUtils/Time.hpp"
#include "UserUtils/Common/interface/STLUtils.hpp"

#include <iostream>
using namespace std;

int
main( int argc, char* argv[] )
{
  // Time function test
  cout << "Displaying and getting time functions" << endl;
  {
    cout << usr::CurrentTimeInNanSec() << endl
         << usr::CurrentTime() << endl
         << usr::CurrentTimeInNanSec() << endl
         << usr::CurrentTime() << endl
         << usr::CurrentTimeInNanSec() << endl
         << usr::CurrentTime() << endl
         << usr::CurrentTimeInNanSec() << endl
         << usr::CurrentTime() << endl
         << usr::CurrentTimeInNanSec() << endl
         << usr::CurrentTime() << endl
         << usr::CurrentTimeInNanSec() << endl
         << usr::CurrentTime() << endl
         << usr::CurrentTimeInMuSec() << endl
         << usr::CurrentTime() << endl
         << usr::CurrentTimeInMuSec() << endl
         << usr::CurrentTime() << endl
         << usr::CurrentTimeInMuSec() << endl
         << usr::CurrentTime() << endl
         << usr::CurrentTimeInMuSec() << endl
         << usr::CurrentTime() << endl
         << usr::CurrentTimeInMuSec() << endl
         << usr::CurrentTime() << endl
         << usr::CurrentTimeInMSec() << endl
         << usr::CurrentTime() << endl
         << usr::CurrentTimeInMSec() << endl
         << usr::CurrentTime() << endl
         << usr::CurrentTimeInMSec() << endl
         << usr::CurrentTime() << endl
         << usr::CurrentTimeInMSec() << endl
         << usr::CurrentTime() << endl
         << usr::CurrentTimeInMSec() << endl
         << usr::CurrentTime() << endl
         << usr::CurrentTimeInSec() << endl;
  }

  cout << usr::separator() << endl
       << "Process interactions!"  << endl;
  {
    cout << usr::GetCMDOutput( "ls -l" ) << endl;
    cout << usr::GetCMDERROutput( "ls test/test/test/" ) << endl;

    cout << usr::HasProcess( "sleep" ) << endl;
    cout << usr::HasProcess( "htop" ) << endl;
    usr::WaitProcess( "htop" );
  }

  return 0;
}
