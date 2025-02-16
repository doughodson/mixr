
#ifndef __mixr_dafif_Waypoint_HPP__
#define __mixr_dafif_Waypoint_HPP__

#include "mixr/dafif/records/IRecord.hpp"

#include <string>

namespace mixr {
namespace dafif {

//------------------------------------------------------------------------------
// Class: Waypoint
// Description: Access to the DAFIF Waypoint records
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Waypoint
// Slots: none
//------------------------------------------------------------------------------
class Waypoint final: public IRecord
{
   DECLARE_SUBCLASS(Waypoint, IRecord)

public:
   // length of a DAFIF Waypoint record
   static const int RECORD_LENGTH{WAYPOINT_RECORD_LEN};

public:
   Waypoint();
   Waypoint(const std::string&);

private:
   static const Ptbl ptable;

   void printRecordImpl(std::ostream& sout) const final;
};

}
}

#endif
