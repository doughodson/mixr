
#include "mixr/dafif/records/Airport.hpp"

#include <string>
#include <iostream>

namespace mixr {
namespace dafif {

IMPLEMENT_SUBCLASS(Airport, "Airport")
EMPTY_SLOTTABLE(Airport)
EMPTY_COPYDATA(Airport)
EMPTY_DELETEDATA(Airport)

// Airport class field Position Table
const Airport::Ptbl Airport::ptable = {
        AP_KEY_POS, AP_KEY_LEN,     // key, keyLength
        AP_IDENT_POS, AP_IDENT_LEN, // ident, identLength
        AP_NAME_POS, AP_NAME_LEN,   // name, nameLength
        AP_TYPE_POS, AP_TYPE_LEN,   // type, typeLength
        AP_CCODE_POS, AP_CCODE_LEN, // country, countryLength
        AP_STATE_POS, AP_STATE_LEN, // state, stateLength
        AP_WAC_POS, AP_WAC_LEN,     // wac, wacLength
        AP_LATITUDE,                // latitude
        AP_LONGITUDE,               // longitude
        AP_ELEVATION,               // elevation
        AP_MAGVAR                   // magVariance
};

Airport::Airport()
{
   STANDARD_CONSTRUCTOR()
   ptbl = &ptable;
}

Airport::Airport(const std::string& s) : IRecord(s)
{
   STANDARD_CONSTRUCTOR()
   ptbl = &ptable;
}

//------------------------------------------------------------------------------
// Type functions
//------------------------------------------------------------------------------
Airport::Type Airport::airportType() const
{
   char tt[AP_TYPE_LEN+1];
   dafifType(tt);
   return Type(tt[0]);
}

int Airport::isAirportType(const Airport::Type tt) const
{
   return tt == airportType();
}

// ident: returns the identifier field
void Airport::ident(char id[]) const
{
   id[0] = '\0';
   const char* p = makePointer(ptbl->ident);
   if (p != nullptr) {
      // look for an ID
      if (*p != 'N' && *(p+1) != ' ') {
         dsGetString( id, p, ptbl->identLength );
      }
      else {
         // when no ID, use the first part of the airport identifier key
         p = makePointer(ptbl->key);
         dsGetString(id, p, ptbl->identLength);
      }
   }
}

//------------------------------------------------------------------------------
// Printing functions
//------------------------------------------------------------------------------
void Airport::printRecordImpl(std::ostream& sout) const
{
   char key1[AP_KEY_LEN+1];
   key(key1);

   char id[AP_IDENT_LEN+1];
   ident(id);

   char ccode[AP_CCODE_LEN+1];
   countryCode(ccode);

   sout << key1;
   sout << ": ";

   sout << id;
   sout << "-";

   sout << ccode;
   sout << "  ";

   sout << char(airportType());
   sout << "  ";

   std::streamoff old = sout.precision();
   sout.precision(12);

   sout << latitude();
   sout << ", ";
   sout << longitude();

   sout.precision(old);

   sout << ", ";
   sout << elevation();
   sout << ", mv=";
   sout << magVariance();

}

}
}
