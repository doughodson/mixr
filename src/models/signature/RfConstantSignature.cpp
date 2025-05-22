
#include "mixr/models/signature/RfConstantSignature.hpp"

#include "mixr/models/RfEmission.hpp"

#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/units/areas.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(RfConstantSignature, "RfConstantSignature")
EMPTY_DELETEDATA(RfConstantSignature)

BEGIN_SLOTTABLE(RfConstantSignature)
    "rcs",          // 1 Constant Radar Cross Section value
                    //   base::Number(square meters) or base::Decibel(square meters) or base::Area()
END_SLOTTABLE(RfConstantSignature)

BEGIN_SLOT_MAP(RfConstantSignature)
    ON_SLOT(1, setSlotRCS, base::INumber)
    ON_SLOT(1, setSlotRCS, base::IArea)
END_SLOT_MAP()

RfConstantSignature::RfConstantSignature()
{
    STANDARD_CONSTRUCTOR()
}

RfConstantSignature::RfConstantSignature(const double r)
{
    STANDARD_CONSTRUCTOR()
    rcs = r;
}

RfConstantSignature::RfConstantSignature(const base::INumber* const c)
{
    STANDARD_CONSTRUCTOR()
    setRCS(c);
}

void RfConstantSignature::copyData(const RfConstantSignature& org, const bool)
{
    BaseClass::copyData(org);
    rcs = org.rcs;
}

double RfConstantSignature::getRCS(const RfEmission* const)
{
    return rcs;
}

bool RfConstantSignature::setRCS(const base::INumber* const x)
{
    bool ok{};
    double r{-1.0};

    if (x != nullptr) {
        // square meters (Number or Decibel)
        r = x->asDouble();
    }

    if (r >= 0.0) {
        rcs = r;
        ok = true;
    } else {
        std::cerr << "SigConstant::setRCS: invalid rcs; must be greater than or equal to zero!" << std::endl;
    }
    return ok;
}

bool RfConstantSignature::setRCS(const base::IArea* const x)
{
   bool ok{};
   double r{-1.0};

   if (x != nullptr) {
      // need area in terms of square meters
      r = x->getValueInSquareMeters();
   }

   if (r >= 0.0) { rcs = r; ok = true; }
   else { std::cerr << "SigConstant::setRCS: invalid rcs; must be greater than or equal to zero!" << std::endl; }
   return ok;
}

}
}
