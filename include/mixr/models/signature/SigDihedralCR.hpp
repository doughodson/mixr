
#ifndef __mixr_models_SigDihedralCR_HPP__
#define __mixr_models_SigDihedralCR_HPP__

#include "mixr/models/signature/SigPlate.hpp"

namespace mixr {
namespace models {
class Emission;

//------------------------------------------------------------------------------
// Class: SigDihedralCR
// Descriptions: RfSignature of a Dihedral Corner Reflector based on edge length,
//               and always facing the transmitter.
// Factory name: SigDihedralCR
//------------------------------------------------------------------------------
class SigDihedralCR : public SigPlate
{
    DECLARE_SUBCLASS(SigDihedralCR, SigPlate)

public:
    SigDihedralCR();
    SigDihedralCR(const double);

    double getRCS(const Emission* const) override;

private:
    double length{};      // Length dimension
};

}
}

#endif
