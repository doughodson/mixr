
#include "mixr/graphics/fonts/AbstractFtglFont.hpp"

#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace graphics {

const int AbstractFtglFont::DEFAULT_FACE_SIZE {1};        // one unit by unit (in this case, inches)

IMPLEMENT_PARTIAL_SUBCLASS(AbstractFtglFont, "AbstractFtglFont")
EMPTY_DELETEDATA(AbstractFtglFont)

BEGIN_SLOTTABLE(AbstractFtglFont)
    "faceSize",
END_SLOTTABLE(AbstractFtglFont)

BEGIN_SLOT_MAP(AbstractFtglFont)
    ON_SLOT(1, setSlotFaceSize, base::Number)
END_SLOT_MAP()

AbstractFtglFont::AbstractFtglFont()
{
    STANDARD_CONSTRUCTOR()
}

AbstractFtglFont::AbstractFtglFont(const AbstractFtglFont& org)
{
    STANDARD_CONSTRUCTOR()
    fSize = org.fSize;
    copyData(org,true);
}

AbstractFtglFont::~AbstractFtglFont()
{
   STANDARD_DESTRUCTOR()
}

AbstractFtglFont& AbstractFtglFont::operator=(const AbstractFtglFont& org)
{
    if (this != &org) copyData(org, false);
    return *this;
}

AbstractFtglFont* AbstractFtglFont::clone() const
{
    return nullptr;
}

void AbstractFtglFont::copyData(const AbstractFtglFont& org, const bool)
{
    BaseClass::copyData(org);
    fSize = org.fSize;
}

bool AbstractFtglFont::setSlotFaceSize(const base::Number* const faceSize)
{
    // get our new face size, else we return to a default face size
    if (faceSize != nullptr)
        fSize = faceSize->getInt();
    else
        fSize = DEFAULT_FACE_SIZE;
    return true;
}

}
}
