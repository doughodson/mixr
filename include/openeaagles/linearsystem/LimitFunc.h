//------------------------------------------------------------------------------
// LimitFunc class -- General Limit function
//------------------------------------------------------------------------------
#ifndef __oe_linearsystem_LimitFunc_H__
#define __oe_linearsystem_LimitFunc_H__

#include "openeaagles/linearsystem/ScalerFunc.h"

namespace oe {
namespace linearsystem {

//------------------------------------------------------------------------------
// Class: LimitFunc
// Base class:  base::Object -> ScalerFunc -> LimitFunc
//
// Description: Generic limits function: limits X to the upper and lower boundaries.
//
//------------------------------------------------------------------------------
class LimitFunc : public ScalerFunc {
    DECLARE_SUBCLASS(LimitFunc,ScalerFunc)

public:
   LimitFunc();
   LimitFunc(const double lower, const double upper);

   double getLowerLimit() const  { return lower; }
   double getUpperLimit() const  { return upper; }

   virtual bool setLowerLimit(const double v);
   virtual bool setUpperLimit(const double v);

   double g(const double x) override;
   bool isValid() const override;

protected:
   void initialize() override;

private:
   void initData();  // Initialize our data

   static const unsigned int ORDER = 1;

   double       lower;      // Lower limit
   double       upper;      // Upper limit
};

} // End linearsystem namespace
} // End oe namespace

#endif