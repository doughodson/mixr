
#ifndef __mixr_linearsystem_LimitFunc_HPP__
#define __mixr_linearsystem_LimitFunc_HPP__

#include "mixr/base/linearsystem/IScalerFunc.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: LimitFunc
//
// Description: Generic limits function: limits X to the upper and lower boundaries.
//------------------------------------------------------------------------------
class LimitFunc : public IScalerFunc
{
   DECLARE_SUBCLASS(LimitFunc, IScalerFunc)

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

   static const unsigned int ORDER{1};

   double lower {};      // Lower limit
   double upper {};      // Upper limit
};

}
}

#endif
