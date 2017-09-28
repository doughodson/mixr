
#include "mixr/map/vpf/VpfDataType.hpp"

#include "mixr/base/String.hpp"

#include <cstdlib>

namespace mixr {
namespace vpf {

IMPLEMENT_SUBCLASS(VpfDataType, "VpfDataType")
EMPTY_SLOTTABLE(VpfDataType)
EMPTY_COPYDATA(VpfDataType)

VpfDataType::VpfDataType()
{
    STANDARD_CONSTRUCTOR()
    value = new base::String();
}

void VpfDataType::deleteData()
{
    if (value != nullptr) {
        value->unref();
        value = 0;
    }
}

void VpfDataType::setValue(char* x)
{
    value->setStr(x);
    if (length != -1) type = FIXED_TEXT;
    else type = VARIABLE_TEXT;
}

const char* VpfDataType::getValue()
{
    if (value != 0 && !value->isEmpty()) return value->getString();
    else return "";
}

// returns a float value (if we are a float!)
float VpfDataType::getFloat()
{
    const float x = static_cast<float>(std::atof(value->getString()));
    return x;
}

void VpfDataType::setLength(const int x)
{
    length = x;
}

}
}
