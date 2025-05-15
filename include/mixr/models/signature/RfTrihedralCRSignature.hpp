
#ifndef __mixr_models_common_RfTrihedralCRSignature_HPP__
#define __mixr_models_common_RfTrihedralCRSignature_HPP__

#include "mixr/models/signature/ISigDihedralCR.hpp"

namespace mixr {
namespace models {
class Emission;

//------------------------------------------------------------------------------
// Class: RfTrihedralCRSignature
//   Description: Concrete RF signature class of a Trihedral Corner Reflector based
//                on edge length, and always facing the transmitter.
//
// Factory name: RfTrihedralCRSignature
//------------------------------------------------------------------------------
class RfTrihedralCRSignature final: public ISigDihedralCR
{
    DECLARE_SUBCLASS(RfTrihedralCRSignature, ISigDihedralCR)
public:
    RfTrihedralCRSignature();
    RfTrihedralCRSignature(const double);

    double getRCS(const Emission* const) override;
};

}
}

#endif
