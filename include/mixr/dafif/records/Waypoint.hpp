
#ifndef __mixr_dafif_Waypoint_H__
#define __mixr_dafif_Waypoint_H__

#include "mixr/dafif/records/Record.hpp"

#include <string>

namespace mixr {
namespace dafif {

//------------------------------------------------------------------------------
// Class: Waypoint
// Description: Access to the DAFIF Waypoint records
//------------------------------------------------------------------------------
class Waypoint final: public Record
{
   DECLARE_SUBCLASS(Waypoint,Record)

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
