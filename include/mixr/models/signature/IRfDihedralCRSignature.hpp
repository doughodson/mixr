
#ifndef __mixr_models_common_IRfDihedralCRSignature_HPP__
#define __mixr_models_common_IRfDihedralCRSignature_HPP__

#include "mixr/models/signature/IRfPlateSignature.hpp"

namespace mixr {
namespace models {
class RfEmission;

//------------------------------------------------------------------------------
// Class: IRfDihedralCRSignature
// Descriptions: Interface to RF signature of a Dihedral Corner Reflector based on edge length,
//               and always facing the transmitter.
// Factory name: IRfDihedralCRSignature
//------------------------------------------------------------------------------
class IRfDihedralCRSignature : public IRfPlateSignature
{
    DECLARE_SUBCLASS(IRfDihedralCRSignature, IRfPlateSignature)

public:
    IRfDihedralCRSignature();
    IRfDihedralCRSignature(const double);

    double getRCS(const RfEmission* const) override;

private:
    double length{};      // Length dimension
};

}
}

#endif
