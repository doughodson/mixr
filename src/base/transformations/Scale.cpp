
#include "mixr/base/transformations/Scale.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Scale, "Scale")
EMPTY_SLOTTABLE(Scale)
EMPTY_COPYDATA(Scale)
EMPTY_DELETEDATA(Scale)
EMPTY_CONSTRUCTOR(Scale)

void Scale::computeMatrix()
{
    m.makeIdentity();
    if (nv == 1) {
       // Single value: scale X and Y by the value and hold Z constant
        Matrixd ss;
        ss.makeScale(v[0], v[0], 1.0f);
        m.preMult(ss);
    }

    if (nv == 2) {
        // Two values: scale X and Y by the values and hold Z constant
        Matrixd ss;
        ss.makeScale(v[0], v[1], 1.0f);
        m.preMult(ss);
    } else if (nv >= 3) {
        // Three values: scale X, Y and Z
        Matrixd ss;
        ss.makeScale(v[0], v[1], v[2]);
        m.preMult(ss);
    }
}

}
}
