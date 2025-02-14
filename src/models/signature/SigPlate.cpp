
#include "mixr/models/signature/SigPlate.hpp"

#include "mixr/models/Emission.hpp"

#include "mixr/base/numeric/INumber.hpp"

#include "mixr/base/units/areas.hpp"
#include "mixr/base/units/lengths.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(SigPlate,"SigPlate")
EMPTY_DELETEDATA(SigPlate)

BEGIN_SLOTTABLE(SigPlate)
    "a",        // 1 length of the plate
    "b",        // 2 width of the plate
END_SLOTTABLE(SigPlate)

BEGIN_SLOT_MAP(SigPlate)
    ON_SLOT(1,setA,base::INumber)
    ON_SLOT(2,setB,base::INumber)
END_SLOT_MAP()

SigPlate::SigPlate()
{
    STANDARD_CONSTRUCTOR()
}

SigPlate::SigPlate(const double a1, const double b1)
{
    STANDARD_CONSTRUCTOR()
    a = a1;
    b = b1;
}

void SigPlate::copyData(const SigPlate& org, const bool)
{
    BaseClass::copyData(org);
    a = org.a;
    b = org.b;
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double SigPlate::getRCS(const Emission* const em)
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
bool SigPlate::setA(base::INumber* const x)
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

bool SigPlate::setA(base::ILength* const x)
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
bool SigPlate::setB(base::INumber* const x)
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

bool SigPlate::setB(base::ILength* const x)
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
