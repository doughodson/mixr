
#ifndef __mixr_models_common_SigTrihedralCR_HPP__
#define __mixr_models_common_SigTrihedralCR_HPP__

#include "mixr/models/signature/ISigDihedralCR.hpp"

namespace mixr {
namespace models {
class Emission;

//------------------------------------------------------------------------------
// Class: SigTrihedralCR
// Descriptions: RfSignature of a Trihedral Corner Reflector based on edge length,
//               and always facing the transmitter.
// Factory name: SigTrihedralCR
//------------------------------------------------------------------------------
class SigTrihedralCR final: public ISigDihedralCR
{
    DECLARE_SUBCLASS(SigTrihedralCR, ISigDihedralCR)
public:
    SigTrihedralCR();
    SigTrihedralCR(const double);

    double getRCS(const Emission* const) override;
};

}
}

#endif
