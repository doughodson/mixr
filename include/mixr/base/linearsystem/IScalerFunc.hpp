
#ifndef __mixr_linearsystem_IScalerFunc_HPP__
#define __mixr_linearsystem_IScalerFunc_HPP__

#include "mixr/base/IObject.hpp"

namespace mixr {
namespace base {
class IFrequency;
class Integer;
class INumber;

//------------------------------------------------------------------------------
// Class: IScalerFunc
//
// Description: Interface to scaler functions
//             (e.g., z-domain difference equations, sample&hold, limits, etc).
//
// Notes:
//
// 1) Each iteration is handled by the g() member function.  We're derived from
//    Object and not Component, so we're not using updateTC(), updateData() or
//    reset().  The simple g() function, which you must call at the master rate
//    and with the current X(n) input, will produce the current Y(n) output.
//
// 2) For g() to generate a valid Y(n) output, the object must be able to return
//    true from 'isValid()' and must be called at the master rate.
//
// 3) By default, the system starts at zero.  Use X0 and Y0 to initialize all of
//    the previous (N-k) values of X and Y.
//
// Slots:
//    rate  <Frequency>  Master rate
//    rate  <Integer>    Master rate Hz
//    x0    <INumber>    Initial (previous) input value: X(0) (default: 0)
//    y0    <INumber>    Initial (previous) output value: Y(0) (default: 0)
//
//------------------------------------------------------------------------------
class IScalerFunc : public IObject
{
   DECLARE_SUBCLASS(IScalerFunc, IObject)

public:
   IScalerFunc();
   IScalerFunc(const unsigned int rate);

   // one iteration of the system:  Y(n) = g(X(n))
   virtual double g(const double x1) =0;

   double getX0() const          { return x0; }    // Returns X(0)
   double getY0() const          { return y0; }    // Returns Y
   unsigned int getRate() const  { return rate; }  // hz

   // Sets the initial X value, X(0), and all past X values, X(-1), X(-2) ,, X(-k+1)
   virtual bool setX0(const double);

   // Sets the initial Y value, Y(0), and all past Y values, Y(-1), Y(-2) ,, Y(-k+1)
   virtual bool setY0(const double);

   // Sets the master update rate (hz)
   virtual bool setRate(const unsigned int);

   bool isValid() const override;

protected:
   // Allocate memory arrays -- removes old arrays, if any
   virtual void allocateMemory(const unsigned int n);

   // Clear the memory arrays
   virtual void clearMemory();

   // Initialize this control/filter
   virtual void initialize();

   unsigned int n {};   // Number coefficients (order)
   double* px {};       // X input array
   double* py {};       // Y output array

   // Parameters and initial values
   unsigned int rate {};
   double x0 {};
   double y0 {};

private:
   // slot table helper methods
   bool setSlotRate(const IFrequency* const);
   bool setSlotRate(const Integer* const);
   bool setSlotX0(const INumber* const);
   bool setSlotY0(const INumber* const);
};

}
}

#endif
