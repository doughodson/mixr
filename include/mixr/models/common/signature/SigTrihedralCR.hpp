
#ifndef __mixr_models_common_SigTrihedralCR_HPP__
#define __mixr_models_common_SigTrihedralCR_HPP__

#include "mixr/models/common/signature/SigDihedralCR.hpp"

namespace mixr {
namespace models {
class Emission;

//------------------------------------------------------------------------------
// Class: SigTrihedralCR
// Descriptions: RfSignature of a Trihedral Corner Reflector based on edge length,
//               and always facing the transmitter.
// Factory name: SigTrihedralCR
//------------------------------------------------------------------------------
class SigTrihedralCR : public SigDihedralCR
{
    DECLARE_SUBCLASS(SigTrihedralCR, SigDihedralCR)
public:
    SigTrihedralCR();
    SigTrihedralCR(const double);

    double getRCS(const Emission* const) override;
};

}
}

#endif
