/**
 * @file    parameter_approxfunc_test
 * @brief   testing the functionality of likelihood function approximations
 * @author  Yi-Mu "Enoch" Chen (ensc@hep1.phys.ntu.edu.tw)
 */

#include "UserUtils/MathUtils/interface/Measurement.hpp"
#include "UserUtils/MathUtils/interface/GSLUtil.hpp"
#include "UserUtils/MathUtils/interface/StatisticsUtil.hpp"

#include <cstdio>
#include <cassert>
#include <gsl/gsl_deriv.h>

double f( double x, void* p )
{
  return usr::LinearVarianceNLL( x , *((usr::Measurement*)p) ) ;
}

int main(int argc, char* argv[])
{
  usr::Measurement p(1,0.00001,0.00001);
  gsl_function F;
  F.function = &f;
  F.params   = &p;

  double deriv;

  for( double x = -5 ; x < 5 ; x += 0.01 ){
    deriv = usr::gsl::deriv( &F, x );
    printf("%.6lf: %.6lf %.6lf\n", x, f(x,&p), deriv );
    assert( !isnan(deriv) );
  }

  return 0;
}
