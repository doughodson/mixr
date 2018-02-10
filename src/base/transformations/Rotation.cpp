
#include "mixr/base/transformations/Rotation.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Rotation, "Rotation")
EMPTY_SLOTTABLE(Rotation)
EMPTY_COPYDATA(Rotation)
EMPTY_DELETEDATA(Rotation)
EMPTY_CONSTRUCTOR(Rotation)

void Rotation::computeMatrix()
{
    m.makeIdentity();
    if (nv == 1) {
        // Single value: rotate about the Z axis
        Matrixd rr;
        rr.makeRotate(v[0], 0.0f, 0.0f, 1.0f);
        m.preMult(rr);
    } else if (nv == 4) {
        // Four values: rotate about vector [ v[0] v[1] v[2] ] by v[3] degrees
        Matrixd rr;
        rr.makeRotate(v[3], v[0], v[1], v[2]);
        m.preMult(rr);
    }
}

}
}
