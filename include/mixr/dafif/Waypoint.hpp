
#ifndef __mixr_dafif_Waypoint_H__
#define __mixr_dafif_Waypoint_H__

#include "Record.hpp"

namespace mixr {
namespace dafif {

//------------------------------------------------------------------------------
// Class: Waypoint
// Base class: base::Object -> base::String -> Record -> Waypoint
//
// Description: Access to the DAFIF Waypoint records.
//
//
// Public member functions:
//
//  [all public members from the base classes]
//
//  Waypoint::recordLength
//    Length of a DAFIF Waypoint record.
//
//------------------------------------------------------------------------------
class Waypoint : public Record
{
   DECLARE_SUBCLASS(Waypoint,Record)

public:
   static const int RECORD_LENGTH = WAYPOINT_RECORD_LEN;

public:
   Waypoint();
   Waypoint(const char* const s);

   void printRecord(std::ostream& sout) const override;

private:
   static const Ptbl ptable;

};

}
}

#endif
