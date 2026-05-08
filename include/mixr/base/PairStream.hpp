
#ifndef __mixr_base_PairStream_HPP__
#define __mixr_base_PairStream_HPP__

#include "mixr/base/IPairStream.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: PairStream
// Description: A PairStream is a concrete list of Pair objects. Pair objects consist of
//              an object and the name of the object.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: PairStream
// Slots: none
// Notes: EDL syntax (a list of Pairs)
//      ident: <object>
//      ...
//      ident: <object>
//------------------------------------------------------------------------------
class PairStream final: public IPairStream
{
    DECLARE_SUBCLASS(PairStream, IPairStream)

public:
   PairStream();
};

}
}

#endif
