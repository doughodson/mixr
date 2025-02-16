
#ifndef __mixr_models_common_IRfSignature_HPP__
#define __mixr_models_common_IRfSignature_HPP__

#include "mixr/base/Component.hpp"

namespace mixr {
namespace models {
class Emission;

//------------------------------------------------------------------------------
// Class: IRfSignature
// Descriptions: Interface for signatures
//------------------------------------------------------------------------------
class IRfSignature : public base::Component
{
    DECLARE_SUBCLASS(IRfSignature, base::Component)
public:
    IRfSignature();

    // computes the Radar Cross Section for the emission
    virtual double getRCS(const Emission* const em) =0;
};

}
}

#endif
