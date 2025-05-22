
#ifndef __mixr_models_common_IRfSignature_HPP__
#define __mixr_models_common_IRfSignature_HPP__

#include "mixr/base/IComponent.hpp"

namespace mixr {
namespace models {
class RfEmission;

//------------------------------------------------------------------------------
// Class: IRfSignature
// Descriptions: Interface for RF signatures
//------------------------------------------------------------------------------
class IRfSignature : public base::IComponent
{
    DECLARE_SUBCLASS(IRfSignature, base::IComponent)
public:
    IRfSignature();

    // computes the Radar Cross Section for the emission
    virtual double getRCS(const RfEmission* const em) =0;
};

}
}

#endif
