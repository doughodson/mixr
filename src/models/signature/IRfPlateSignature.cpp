
#include "mixr/models/signature/IRfPlateSignature.hpp"

#include "mixr/models/RfEmission.hpp"

#include "mixr/base/numeric/INumber.hpp"

#include "mixr/base/units/areas.hpp"
#include "mixr/base/units/lengths.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IRfPlateSignature,"IRfPlateSignature")
EMPTY_DELETEDATA(IRfPlateSignature)

BEGIN_SLOTTABLE(IRfPlateSignature)
    "a",        // 1 length of the plate
    "b",        // 2 width of the plate
END_SLOTTABLE(IRfPlateSignature)

BEGIN_SLOT_MAP(IRfPlateSignature)
    ON_SLOT(1,setA,base::INumber)
    ON_SLOT(2,setB,base::INumber)
END_SLOT_MAP()

IRfPlateSignature::IRfPlateSignature()
{
    STANDARD_CONSTRUCTOR()
}

IRfPlateSignature::IRfPlateSignature(const double a1, const double b1)
{
    STANDARD_CONSTRUCTOR()
    a = a1;
    b = b1;
}

void IRfPlateSignature::copyData(const IRfPlateSignature& org, const bool)
{
    BaseClass::copyData(org);
    a = org.a;
    b = org.b;
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double IRfPlateSignature::getRCS(const RfEmission* const em)
{
    double rcs{};
    if (em != nullptr) {
        double lambda{em->getWavelength()};
        double area{a * b};
        if (lambda > 0.0 && area > 0.0) {
            // If we have lambda and the area of the plate, compute the RCS
            rcs = (4.0 * base::PI * area * area) / (lambda * lambda);
        }
    }
    return static_cast<double>(rcs);
}

//------------------------------------------------------------------------------
// setA() -- Set the length
//------------------------------------------------------------------------------
bool IRfPlateSignature::setA(base::INumber* const x)
{
    bool ok{};
    double v{-1.0};

    if (x != nullptr) {
        v = x->asDouble();
    }

    if (v >= 0.0) {
        a = v;
        ok = true;
    } else {
        std::cerr << "SigPlate::setWidthFromSlot: invalid: must be greater than or equal to zero!" << std::endl;
    }
    return ok;
}

bool IRfPlateSignature::setA(base::ILength* const x)
{
   bool ok{};
   double v{-1.0};

   if (x != nullptr) {
      // we need meters
      v = x->getValueInMeters();
   }

   if (v >= 0.0) {
      a = v;
      ok = true;
   }
   else {
      std::cerr << "SigPlate::setWidthFromSlot: invalid: must be greater than or equal to zero!" << std::endl;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setB() -- Set the width
//------------------------------------------------------------------------------
bool IRfPlateSignature::setB(base::INumber* const x)
{
    bool ok{};
    double v{-1.0};

   if (x != nullptr) {
        // Just a Number
        v = x->asDouble();
    }

    if (v >= 0.0) {
        b = v;
        ok = true;
    } else {
        std::cerr << "SigPlate::setHeightFromSlot: invalid: must be greater than or equal to zero!" << std::endl;
    }
    return ok;
}

bool IRfPlateSignature::setB(base::ILength* const x)
{
   bool ok{};
   double v{-1.0};

   if (x != nullptr) {
      // we need meters
      v = x->getValueInMeters();
   }

   if (v >= 0.0) {
      b = v;
      ok = true;
   } else {
      std::cerr << "SigPlate::setHeightFromSlot: invalid: must be greater than or equal to zero!" << std::endl;
   }
   return ok;
}

}
}
