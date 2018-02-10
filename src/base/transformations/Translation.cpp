
#include "mixr/base/transformations/Translation.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Translation, "Translation")
EMPTY_SLOTTABLE(Translation)
EMPTY_COPYDATA(Translation)
EMPTY_DELETEDATA(Translation)
EMPTY_CONSTRUCTOR(Translation)

void Translation::computeMatrix()
{
    m.makeIdentity();
    if (nv == 2) {
        // Translate X and Y
        Matrixd tt;
        tt.makeTranslate(v[0], v[1], 0.0f);
        m.preMult(tt);
    } else if (nv >= 3) {
        // Translate X, Y and Z
        Matrixd tt;
        tt.makeTranslate(v[0], v[1], v[2]);
        m.preMult(tt);
    }

}

}
}
