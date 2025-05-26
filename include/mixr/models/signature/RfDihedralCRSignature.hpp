
#ifndef __mixr_models_common_RfDihedralCRSignature_HPP__
#define __mixr_models_common_RfDihedralCRSignature_HPP__

#include "mixr/models/signature/IRfDihedralCRSignature.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: RfDihedralCRSignature
//    Descriptions: Concrete RF signature
//
// Factory name: RfDihedralCRSignature
//------------------------------------------------------------------------------
class RfDihedralCRSignature final: public IRfDihedralCRSignature
{
    DECLARE_SUBCLASS(RfDihedralCRSignature, IRfDihedralCRSignature)
public:
    RfDihedralCRSignature();
    RfDihedralCRSignature(const double);
};

}
}

#endif
