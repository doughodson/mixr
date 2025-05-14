
#include "mixr/models/signature/ISigDihedralCR.hpp"

#include "mixr/models/Emission.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(ISigDihedralCR, "ISigDihedralCR")
EMPTY_SLOTTABLE(ISigDihedralCR)
EMPTY_DELETEDATA(ISigDihedralCR)

ISigDihedralCR::ISigDihedralCR()
{
    STANDARD_CONSTRUCTOR()
    length = 0.0;
}

ISigDihedralCR::ISigDihedralCR(const double a) : ISigPlate(a, 0.0)
{
    STANDARD_CONSTRUCTOR()
    length = 0.0;
}

void ISigDihedralCR::copyData(const ISigDihedralCR& org, const bool)
{
    BaseClass::copyData(org);
    length = org.length;
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double ISigDihedralCR::getRCS(const Emission* const em)
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
