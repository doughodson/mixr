
#include "mixr/models/signature/SigTrihedralCR.hpp"

#include "mixr/base/util/constants.hpp"

#include "mixr/models/Emission.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(SigTrihedralCR, "SigTrihedralCR")
EMPTY_SLOTTABLE(SigTrihedralCR)
EMPTY_DELETEDATA(SigTrihedralCR)
EMPTY_COPYDATA(SigTrihedralCR)

SigTrihedralCR::SigTrihedralCR()
{
    STANDARD_CONSTRUCTOR()
}

SigTrihedralCR::SigTrihedralCR(const double a) : ISigDihedralCR(a)
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double SigTrihedralCR::getRCS(const Emission* const em)
{
    double rcs{};
    if (em != nullptr) {
        const double lambda{em->getWavelength()};
        if (lambda > 0.0) {
            // If we have lambda and the area of the plate, compute the RCS
            const double a{getA()};
            rcs = (12.0 * base::PI * a*a*a*a) / (lambda*lambda);
        }
    }
    return static_cast<double>(rcs);
}

}
}
