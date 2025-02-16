
#ifndef __mixr_dafif_Airport_HPP__
#define __mixr_dafif_Airport_HPP__

#include "mixr/dafif/records/IRecord.hpp"

#include <string>

namespace mixr {
namespace dafif {

//------------------------------------------------------------------------------
// Class: Airport
// Description: DAFIF Airport record class
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Airport
// Slots: none
//------------------------------------------------------------------------------
class Airport final: public IRecord
{
   DECLARE_SUBCLASS(Airport, IRecord)

public:

   Airport();
   Airport(const std::string&);

   // length of a Airport record
   enum { RECORD_LENGTH = AIRPORT_RECORD_LEN };

   enum class Type:int { INVALID = -1, ANY = 'Z', CIVIL  = 'A', JOINT = 'B', MILITARY = 'C', INACTIVE = 'D' };

   // returns the airport type
   Type airportType() const;
   // returns true if 'type' is equal to the airport type
   int isAirportType(const Type) const;

   // Returns the value of the airports's host country identifier in 'id'.
   // When no host country identifier exists, the airport identifier key
   // is used.  Therefore, the size of 'id' must be one more than the
   // longer of AP_IDENT_LEN and AP_KEY_LEN.
   void ident(char id[]) const final;

private:
   static const Ptbl ptable;

   void printRecordImpl(std::ostream& sout) const final;
};

}
}

#endif
