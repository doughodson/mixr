
#ifndef __mixr_models_common_RfTrihedralCRSignature_HPP__
#define __mixr_models_common_RfTrihedralCRSignature_HPP__

#include "mixr/models/signature/IRfDihedralCRSignature.hpp"

namespace mixr {
namespace models {
class RfEmission;

//------------------------------------------------------------------------------
// Class: RfTrihedralCRSignature
//   Description: Concrete RF signature class of a Trihedral Corner Reflector based
//                on edge length, and always facing the transmitter.
//
// Factory name: RfTrihedralCRSignature
//------------------------------------------------------------------------------
class RfTrihedralCRSignature final: public IRfDihedralCRSignature
{
    DECLARE_SUBCLASS(RfTrihedralCRSignature, IRfDihedralCRSignature)
public:
    RfTrihedralCRSignature();
    RfTrihedralCRSignature(const double);

    double getRCS(const RfEmission* const) override;
};

}
}

#endif
