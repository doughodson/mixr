
#include "mixr/graphics/fonts/IFtglFont.hpp"

#include "mixr/base/numeric/Integer.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_PARTIAL_SUBCLASS(IFtglFont, "IFtglFont")
EMPTY_DELETEDATA(IFtglFont)

BEGIN_SLOTTABLE(IFtglFont)
    "faceSize",
END_SLOTTABLE(IFtglFont)

BEGIN_SLOT_MAP(IFtglFont)
    ON_SLOT(1, setSlotFaceSize, base::Integer)
END_SLOT_MAP()

IFtglFont::IFtglFont()
{
    STANDARD_CONSTRUCTOR()
}

IFtglFont::IFtglFont(const IFtglFont& org)
{
    STANDARD_CONSTRUCTOR()
    fSize = org.fSize;
    copyData(org,true);
}

IFtglFont::~IFtglFont()
{
   STANDARD_DESTRUCTOR()
}

IFtglFont& IFtglFont::operator=(const IFtglFont& org)
{
    if (this != &org) copyData(org, false);
    return *this;
}

IFtglFont* IFtglFont::clone() const
{
    return nullptr;
}

void IFtglFont::copyData(const IFtglFont& org, const bool)
{
    BaseClass::copyData(org);
    fSize = org.fSize;
}

bool IFtglFont::setSlotFaceSize(const base::Integer* const faceSize)
{
    fSize = faceSize->asInt();
    return true;
}

}
}
