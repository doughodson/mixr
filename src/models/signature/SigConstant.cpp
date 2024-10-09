
#include "mixr/models/signature/SigConstant.hpp"

#include "mixr/models/Emission.hpp"

#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/units/areas.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(SigConstant, "SigConstant")
EMPTY_DELETEDATA(SigConstant)

BEGIN_SLOTTABLE(SigConstant)
    "rcs",          // 1 Constant Radar Cross Section value
                    //   base::Number(square meters) or base::Decibel(square meters) or base::Area()
END_SLOTTABLE(SigConstant)

BEGIN_SLOT_MAP(SigConstant)
    ON_SLOT(1, setSlotRCS, base::Number)
    ON_SLOT(1, setSlotRCS, base::Area)
END_SLOT_MAP()

SigConstant::SigConstant()
{
    STANDARD_CONSTRUCTOR()
}

SigConstant::SigConstant(const double r)
{
    STANDARD_CONSTRUCTOR()
    rcs = r;
}

SigConstant::SigConstant(const base::Number* const c)
{
    STANDARD_CONSTRUCTOR()
    setRCS(c);
}

void SigConstant::copyData(const SigConstant& org, const bool)
{
    BaseClass::copyData(org);
    rcs = org.rcs;
}

double SigConstant::getRCS(const Emission* const)
{
    return rcs;
}

bool SigConstant::setRCS(const base::Number* const x)
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

bool SigConstant::setRCS(const base::Area* const x)
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
