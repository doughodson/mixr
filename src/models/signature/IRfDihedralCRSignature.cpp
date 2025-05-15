
#include "mixr/models/signature/IRfDihedralCRSignature.hpp"

#include "mixr/models/Emission.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IRfDihedralCRSignature, "IRfDihedralCRSignature")
EMPTY_SLOTTABLE(IRfDihedralCRSignature)
EMPTY_DELETEDATA(IRfDihedralCRSignature)

IRfDihedralCRSignature::IRfDihedralCRSignature()
{
    STANDARD_CONSTRUCTOR()
    length = 0.0;
}

IRfDihedralCRSignature::IRfDihedralCRSignature(const double a) : IRfPlateSignature(a, 0.0)
{
    STANDARD_CONSTRUCTOR()
    length = 0.0;
}

void IRfDihedralCRSignature::copyData(const IRfDihedralCRSignature& org, const bool)
{
    BaseClass::copyData(org);
    length = org.length;
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double IRfDihedralCRSignature::getRCS(const Emission* const em)
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
