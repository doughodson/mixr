
#ifndef __mixr_base_Identifier_H__
#define __mixr_base_Identifier_H__

#include "mixr/base/String.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Identifier
// Description: A single unquoted word (usually from the input file), which
//              can contain upper and lower case letters, numbers, special
//              characters, but not spaces.
//
// Note: When setting the identifier, any spaces will be replaced
//       with underscores.
//
// Factory name: Identifier
//------------------------------------------------------------------------------
class Identifier : public String
{
    DECLARE_SUBCLASS(Identifier, String)

public:
    Identifier(const char*);
    Identifier();

    void setStr(const char*) override;
    void catStr(const char*) override;
};

}
}

#endif
