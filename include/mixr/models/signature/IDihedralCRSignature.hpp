
#ifndef __mixr_models_common_IDihedralCRSignature_HPP__
#define __mixr_models_common_IDihedralCRSignature_HPP__

#include "mixr/models/signature/ISigPlate.hpp"

namespace mixr {
namespace models {
class Emission;

//------------------------------------------------------------------------------
// Class: IDihedralCRSignature
// Descriptions: RfSignature of a Dihedral Corner Reflector based on edge length,
//               and always facing the transmitter.
// Factory name: SigDihedralCR
//------------------------------------------------------------------------------
class IDihedralCRSignature : public ISigPlate
{
    DECLARE_SUBCLASS(IDihedralCRSignature, ISigPlate)

public:
    IDihedralCRSignature();
    IDihedralCRSignature(const double);

    double getRCS(const Emission* const) override;

private:
    double length{};      // Length dimension
};

}
}

#endif
