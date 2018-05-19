/**
 * @file    Ratio1DCanvas.hpp
 * @brief   Canvas containing two Pad1Ds, with the top for main plotting
 *          while the bottom is designed for ratio plotting.
 * @author  Yi-Mu "Enoch" Chen (ensc@hep1.phys.ntu.edu.tw)
 * @details
 *
 * By default the Plot<Obj> function would plot directly onto the top
 * pad. Instance function for generating new plots for the bottom canvas would
 * be owned by the canvas object, ensuring that they are deleted when the
 * canvas goes out of scope, allowing for less user level memeroy handling.
 * By default, the generated objects have their attributes bound to the
 * numerator, copying all the attributs at *SAVE TIME* such that the objects
 * would have identical styling. This behaviour could be overwritten by a
 * Unbind option that could be passed when plotting the bottom canvas objects.
 */

#ifndef USERUTILS_PLOTUTILS_RATIO1DCANVAS_HPP
#define USERUTILS_PLOTUTILS_RATIO1DCANVAS_HPP

#include "UserUtils/Common/interface/STLUtils/VectorUtils.hpp"
#include "UserUtils/PlotUtils/interface/Canvas.hpp"
#include "UserUtils/PlotUtils/interface/Pad1D.hpp"

#include <memory>
#include <vector>

namespace usr  {

namespace plt  {


/*-----------------------------------------------------------------------------
 *  Constructor helper classes
   --------------------------------------------------------------------------*/
struct PadRatio
{
  static float default_ratio;
  static float default_gap;
  PadRatio( const float _ratio = default_ratio,
            const float _gap   = default_gap ) :
    ratio( _ratio ),
    gap( _gap ){}
  const float ratio;
  const float gap;
};

/*-----------------------------------------------------------------------------
 *  Specializing for Top and bottom Pad, mainly for overloading the
 *  SetAxisFont options
   --------------------------------------------------------------------------*/
class Top1DPad : public Pad1D
{
public:
  // Constructors must be redeclared
  Top1DPad( const PadSize& x, const RangeByVar& y )
    : Pad1D( x, y ){}
  Top1DPad( const PadSize& x, const double y, const double z )
    : Pad1D( x, y, z ){}
  Top1DPad( const PadSize& x )
    : Pad1D( x ){}
  virtual
  ~Top1DPad (){}

  virtual void SetAxisFont();
};

/*----------------------------------------------------------------------------*/

class Bottom1DPad : public Pad1D
{
public:
  // Constructors must be redeclared
  Bottom1DPad( const PadSize& x, const RangeByVar& y )
    : Pad1D( x, y ){}
  Bottom1DPad( const PadSize& x, const double y, const double z )
    : Pad1D( x, y, z ){}
  Bottom1DPad( const PadSize& x )
    : Pad1D( x ){}
  virtual
  ~Bottom1DPad(){}

  virtual void SetAxisFont();
};


/*-----------------------------------------------------------------------------
 *  Main class
   --------------------------------------------------------------------------*/
class Ratio1DCanvas : public Canvas
{
public:
  // Static member functions for simple divide operations, these generate
  // objects where the user will have to handle the plotting and object
  // ownership all by themselves.

  // Scaling division
  // Scaling division is simple scaling the numerator object by the central
  // value of the denominator. The scaling includes the uncertainties of the
  // numerator and the uncertainties in the denominator object is ignored.
  static TH1D* ScaleDivide(
    const TH1D*  num,
    const TH1D*  den,
    const double cen = 1. );

  static TGraphAsymmErrors* ScaleDivide(
    const TGraph* num,
    const TGraph* den,
    const double  cen = 1. );

  // Default setting are hidden away in cpp file for simpler compilation
  static length_t default_width;
  static length_t default_height;

public:
  Ratio1DCanvas(
    const length_t width  = default_width,
    const length_t height = default_height,
    const PadRatio& = PadRatio(),
    const FontSet&  = HalfPageFont
    );
  Ratio1DCanvas(
    const RangeByVar&,
    const length_t width  = default_width,
    const length_t height = default_height,
    const PadRatio& = PadRatio(),
    const FontSet&  = HalfPageFont
    );

  virtual
  ~Ratio1DCanvas();

  inline Top1DPad&
  TopPad(){ return GetPad<Top1DPad>( 0 ); }
  inline Bottom1DPad&
  BottomPad(){ return GetPad<Bottom1DPad>( 1 ); }

  // Overloading the TopPad Pad1D functions to look like directly
  // 'plotting on this canvas' (Bottom canvases plot objects generated by the
  // static functions).
#define PASSTHROUGH_TOPPLOTFUNC( FUNC_NAME, PLOT_TYPE, RET_TYPE ) \
  template<typename ... Args>                                     \
  inline RET_TYPE &FUNC_NAME( PLOT_TYPE& obj, Args ... args )     \
  { return TopPad().FUNC_NAME( obj, args ... );                   \
  }                                                               \
  template<typename ... Args>                                     \
  inline RET_TYPE &FUNC_NAME( PLOT_TYPE* obj, Args ... args )     \
  { return TopPad().FUNC_NAME( obj, args ... ); }

  PASSTHROUGH_TOPPLOTFUNC( PlotHist,  TH1D,       TH1D );
  PASSTHROUGH_TOPPLOTFUNC( PlotGraph, TGraph,     TGraph );
  PASSTHROUGH_TOPPLOTFUNC( PlotData,  RooAbsData, TGraphAsymmErrors );
  PASSTHROUGH_TOPPLOTFUNC( PlotPdf,   RooAbsPdf,  TGraph );
#undef PASSTHROUGH_TOPPLOTFUNC

#define PASSTHOUGH_TOPVOIDFUNC( FUNC_NAME ) \
  template<typename ... Args>               \
  inline void FUNC_NAME( Args ... args )    \
  { TopPad().FUNC_NAME( args ... ); }

  PASSTHOUGH_TOPVOIDFUNC( DrawCMSLabel );
  PASSTHOUGH_TOPVOIDFUNC( DrawLuminosity );
  PASSTHOUGH_TOPVOIDFUNC( SetLogy );
#undef PASSTHOUGH_TOPVOIDFUNC

  // Instance divide functions, using marco expansion multiple interface
  // generation. Allowing for pointer and reference interface for everything
#define DIVIDE_FUNCTION( FUNC_NAME, RET_TYPE, NUM_TYPE, DEN_TYPE )           \
  RET_TYPE& FUNC_NAME(                                                       \
    const NUM_TYPE&, const DEN_TYPE&, const std::vector<RooCmdArg> & );      \
  inline NUM_TYPE& FUNC_NAME( const NUM_TYPE &num, const DEN_TYPE &den ){    \
    return FUNC_NAME( num, den, {} ); }                                      \
  template<typename ... Args>                                                \
  inline RET_TYPE &                                                          \
  FUNC_NAME( const NUM_TYPE &num, const DEN_TYPE &den,                       \
    const RooCmdArg &arg1, Args ... args )                                   \
  { return FUNC_NAME( num, den, MakeVector<RooCmdArg>( arg1, args ... ) ); } \
  inline RET_TYPE& FUNC_NAME( const NUM_TYPE* num, const DEN_TYPE &den ){    \
    return FUNC_NAME( *num, den, {} ); }                                     \
  template<typename ... Args>                                                \
  inline RET_TYPE &                                                          \
  FUNC_NAME( const NUM_TYPE* num, const DEN_TYPE &den,                       \
    const RooCmdArg &arg1, Args ... args )                                   \
  { return FUNC_NAME( *num, den, MakeVector<RooCmdArg>( arg1, args ... ) ); } \
  inline RET_TYPE& FUNC_NAME( const NUM_TYPE &num, const DEN_TYPE* den ){    \
    return FUNC_NAME( num, *den, {} ); }                                     \
  template<typename ... Args>                                                \
  inline RET_TYPE &                                                          \
  FUNC_NAME( const NUM_TYPE &num, const DEN_TYPE* den,                       \
    const RooCmdArg &arg1, Args ... args )                                   \
  { return FUNC_NAME( num, *den, MakeVector<RooCmdArg>( arg1, args ... ) ); } \
  inline RET_TYPE& FUNC_NAME( const NUM_TYPE* num, const DEN_TYPE* den ){    \
    return FUNC_NAME( *num, *den, {} ); }                                    \
  template<typename ... Args>                                                \
  inline RET_TYPE &                                                          \
  FUNC_NAME( const NUM_TYPE* num, const DEN_TYPE* den,                       \
    const RooCmdArg &arg1, Args ... args )                                   \
  { return FUNC_NAME( *num, *den, MakeVector<RooCmdArg>( arg1, args ... ) ); }

  DIVIDE_FUNCTION( PlotScale, TH1D,              TH1D,   TH1D );
  DIVIDE_FUNCTION( PlotScale, TGraphAsymmErrors, TGraph, TGraph );

#undef DIVIDE_FUNCTION

  // Overloading marging setting functions to ensure marging matching
  // of top and bottom cavasis
  void SetTopMargin( const float x );
  void SetLeftMargin( const float x );
  void SetRightMargin( const float x );
  void SetBottomMargin( const float x );

protected:
  // Constructor helper functions
  float _splitNDC( const float ratio ) const;
  void  _init_margin( const float gap );
};

}/* plt  */

}/* usr  */

#endif/* end of include guard: USERUTILS_PLOTUTILS_RATIO1DCANVAS_HPP  */