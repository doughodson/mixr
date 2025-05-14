
#ifndef __mixr_models_common_ISigDihedralCR_HPP__
#define __mixr_models_common_ISigDihedralCR_HPP__

#include "mixr/models/signature/ISigPlate.hpp"

namespace mixr {
namespace models {
class Emission;

//------------------------------------------------------------------------------
// Class: ISigDihedralCR
// Descriptions: RfSignature of a Dihedral Corner Reflector based on edge length,
//               and always facing the transmitter.
// Factory name: SigDihedralCR
//------------------------------------------------------------------------------
class ISigDihedralCR : public ISigPlate
{
    DECLARE_SUBCLASS(ISigDihedralCR, ISigPlate)

public:
    ISigDihedralCR();
    ISigDihedralCR(const double);

    double getRCS(const Emission* const) override;

private:
    double length{};      // Length dimension
};

}
}

#endif
