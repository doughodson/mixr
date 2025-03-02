
#ifndef __mixr_base_ITransform_HPP__
#define __mixr_base_ITransform_HPP__

#include "mixr/base/IObject.hpp"
#include "mixr/base/osg/Matrixd"

#include <array>

namespace mixr {
namespace base {
class IAngle;
class INumber;

//------------------------------------------------------------------------------
// Class: ITransform
// Description: Interface for coordinate transformations (e.g., translate, rotate, scale)
//------------------------------------------------------------------------------
// Factory name: ITransform
//------------------------------------------------------------------------------
// Slots:
//  x    <INumber>   ! x transform (default: identity)
//  x    <IAngle>    ! x transform (default: identity)
//  y    <INumber>   ! y transform (default: identity)
//  y    <IAngle>    ! y transform (default: identity)
//  z    <INumber>   ! z transform (default: identity)
//  z    <IAngle>    ! z transform (default: identity)
//  w    <INumber>   ! w transform (default: identity)
//  w    <IAngle>    ! w transform (default: identity)
//------------------------------------------------------------------------------
class ITransform : public IObject
{
   DECLARE_SUBCLASS(ITransform, IObject)

public:
   ITransform();
   // returns a copy of the transformation matrix
   explicit operator const Matrixd&()              { return m; }

protected:
   Matrixd m;                   // transformation matrix
   std::array<double, 4> v{};   // values
   int nv{};                    // Number of values

private:
   virtual void computeMatrix() =0;

private:
   // slot table helper methods
   // sets the n'th parameter to the transformation function
   // and create the transformation matrix.  Returns true if the matrix
   // is valid.  These functions must be called in order.  For example,
   // "x" must be set before "y".
   bool setSlotComputematrix1(const IAngle* const);
   bool setSlotComputematrix1(const INumber* const);

   bool setSlotComputematrix2(const IAngle* const);
   bool setSlotComputematrix2(const INumber* const);

   bool setSlotComputematrix3(const IAngle* const);
   bool setSlotComputematrix3(const INumber* const);

   bool setSlotComputematrix4(const IAngle* const);
   bool setSlotComputematrix4(const INumber* const);
};

}
}

#endif
