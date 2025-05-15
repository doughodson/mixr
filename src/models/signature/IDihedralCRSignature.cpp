
#include "mixr/models/signature/IDihedralCRSignature.hpp"

#include "mixr/models/Emission.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IDihedralCRSignature, "IDihedralCRSignature")
EMPTY_SLOTTABLE(IDihedralCRSignature)
EMPTY_DELETEDATA(IDihedralCRSignature)

IDihedralCRSignature::IDihedralCRSignature()
{
    STANDARD_CONSTRUCTOR()
    length = 0.0;
}

IDihedralCRSignature::IDihedralCRSignature(const double a) : ISigPlate(a, 0.0)
{
    STANDARD_CONSTRUCTOR()
    length = 0.0;
}

void IDihedralCRSignature::copyData(const IDihedralCRSignature& org, const bool)
{
    BaseClass::copyData(org);
    length = org.length;
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double IDihedralCRSignature::getRCS(const Emission* const em)
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
