
#include "mixr/graphics/ColorRotary.hpp"

#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/Identifier.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/IPairStream.hpp"
#include "mixr/base/osg/Vec4d"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(ColorRotary, "ColorRotary")

BEGIN_SLOTTABLE(ColorRotary)
    "breakcolors",      // set colors
    "breakpoints",      // set our breakpoints
END_SLOTTABLE(ColorRotary)

BEGIN_SLOT_MAP(ColorRotary)
    ON_SLOT(1, setSlotColors, base::IPairStream)
    ON_SLOT(2, setSlotValues, base::IPairStream)
END_SLOT_MAP()

ColorRotary::ColorRotary()
{
    STANDARD_CONSTRUCTOR()
    // default gives us no colors, but just makes us black
    color[IColor::RED] = 0;
    color[IColor::GREEN] = 0;
    color[IColor::BLUE] = 0;
    color[IColor::ALPHA] = getDefaultAlpha();
}

void ColorRotary::copyData(const ColorRotary& org, const bool)
{
    BaseClass::copyData(org);

    if (org.numVals > 0) {
        for (unsigned int i = 0; i < org.numVals; i++) {
            myValues[i] = org.myValues[i];
        }
    }
    if (org.myColors != nullptr) {
        setSlotColors(org.myColors);
    }
    else setSlotColors(nullptr);
    numVals = org.numVals;
}

void ColorRotary::deleteData()
{
    if (myColors != nullptr) myColors->unref();
    myColors = nullptr;
}

//------------------------------------------------------------------------------
// determineColors() - take our value, and look for a corresponding color
// and breakpoint
//------------------------------------------------------------------------------
bool ColorRotary::determineColor(const double value)
{
    bool ok = false;
    int breakPoint = 0;

    // find out where we are in the break table
    unsigned int i = 0;
    // do an endpoint check while we are at it
    if (value >= myValues[numVals-1]) breakPoint = numVals;
    while (!ok && i < numVals) {
        if (value >= myValues[i] && value < myValues[i+1]){
            breakPoint = (i + 1);
            ok = true;
        }
        else i++;
    }

    // now set the proper color (using the breakpoint index)
    if (myColors != nullptr) {
        base::Pair* pair = myColors->getPosition(breakPoint);
        if (pair != nullptr) {
            const auto listcolor = dynamic_cast<base::IColor*>(pair->object());
            if (listcolor != nullptr) {
               const auto vec = static_cast<const base::Vec4d*>(listcolor->getRGBA());
               color = *vec;
               ok = true;
            }
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// slot functions
//------------------------------------------------------------------------------

// set our slot colors via a PairStream
bool ColorRotary::setSlotColors(base::IPairStream* const x)
{
    myColors = x;
    myColors->ref();
    return true;
}

// set our slot values via a pairstream
bool ColorRotary::setSlotValues(const base::IPairStream* const x)
{
    numVals = 0;
    base::IPairStream* a = x->clone();
    base::IList::Item* item = a->getFirstItem();
    while (item != nullptr) {
        const auto pair = static_cast<base::Pair*>(item->getValue());
        if (pair != nullptr) {
            const auto n = dynamic_cast<base::INumber*>(pair->object());
            if (n != nullptr) {
                myValues[numVals] = n->asDouble();
                numVals++;
            }
        }
        item = item->getNext();
    }
    a->unref();
    return true;
}

}
}
