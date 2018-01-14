
#include "mixr/models/signatures/SigDihedralCR.hpp"

#include "mixr/models/Emission.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(SigDihedralCR, "SigDihedralCR")
EMPTY_SLOTTABLE(SigDihedralCR)
EMPTY_DELETEDATA(SigDihedralCR)

SigDihedralCR::SigDihedralCR()
{
    STANDARD_CONSTRUCTOR()
    length = 0.0;
}

SigDihedralCR::SigDihedralCR(const double a) : SigPlate(a, 0.0)
{
    STANDARD_CONSTRUCTOR()
    length = 0.0;
}

void SigDihedralCR::copyData(const SigDihedralCR& org, const bool)
{
    BaseClass::copyData(org);
    length = org.length;
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double SigDihedralCR::getRCS(const Emission* const em)
{
    double rcs{};
    if (em != nullptr) {
        const double lambda{em->getWavelength()};
        if (lambda > 0.0) {
            // If we have lambda and the area of the plate, compute the RCS
            const double a{getA()};
            rcs = (8.0 * base::PI * a*a*a*a) / (lambda*lambda);
        }
    }
    return static_cast<double>(rcs);
}

}
}
