
#ifndef __mixr_base_Rgba_H__
#define __mixr_base_Rgba_H__

#include "mixr/base/colors/Rgb.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class:  Rgba
//
// Description:  Defines a color by Red, Green, Blue and Alpha
//
// Factory name: rgba
// Slots:
//    alpha <Number>  ! Alpha component (0.0 to 1.0) (default: 1.0)
//
// Events: None
//
//
// Public methods: Base class public methods, plus ...
//
//      Rgba(const double r, const double g, const double b, const double a)
//          Special constructor what initializes the object to r, g, b and a
//
//------------------------------------------------------------------------------
class Rgba : public Rgb
{
    DECLARE_SUBCLASS(Rgba, Rgb)

public:
    Rgba(const double r, const double g, const double b, const double a);
    Rgba();

private:
    // slot table helper methods
    bool setSlotAlpha(const Number* const);
};

}
}

#endif
