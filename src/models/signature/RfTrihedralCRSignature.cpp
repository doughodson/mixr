
#include "mixr/models/signature/RfTrihedralCRSignature.hpp"

#include "mixr/base/util/constants.hpp"

#include "mixr/models/Emission.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(RfTrihedralCRSignature, "RfTrihedralCRSignature")
EMPTY_SLOTTABLE(RfTrihedralCRSignature)
EMPTY_DELETEDATA(RfTrihedralCRSignature)
EMPTY_COPYDATA(RfTrihedralCRSignature)

RfTrihedralCRSignature::RfTrihedralCRSignature()
{
    STANDARD_CONSTRUCTOR()
}

RfTrihedralCRSignature::RfTrihedralCRSignature(const double a) : IRfDihedralCRSignature(a)
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double RfTrihedralCRSignature::getRCS(const Emission* const em)
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
