
#include "mixr/dafif/records/Navaid.hpp"

#include "mixr/base/util/nav_utils.hpp"

#include "mixr/base/units/angles.hpp"

#include <string>
#include <iostream>

namespace mixr {
namespace dafif {

IMPLEMENT_SUBCLASS(Navaid, "Navaid")
EMPTY_SLOTTABLE(Navaid)
EMPTY_COPYDATA(Navaid)
EMPTY_DELETEDATA(Navaid)

// Navaid class field Position Table
const Navaid::Ptbl Navaid::ptable = {
   NA_KEY_POS, NA_KEY_LEN,       // key, keyLength
   NA_IDENT_POS, NA_IDENT_LEN,   // ident, identLength
   NA_NAME_POS, NA_NAME_LEN,     // name, nameLength
   NA_TYPE_POS, NA_TYPE_LEN,     // type, typeLength
   NA_CCODE_POS, NA_CCODE_LEN,   // country, countryLength
   NA_STATE_POS, NA_STATE_LEN,   // state, stateLength
   NA_WAC_POS, NA_WAC_LEN,       // wac, wacLength
   NA_LATITUDE,                  // latitude
   NA_LONGITUDE,                 // longitude
   NA_ELEVATION,                 // elevation
   NA_MAGVAR                     // magVariance
};

Navaid::Navaid()
{
   STANDARD_CONSTRUCTOR()
   ptbl = &ptable;
}

Navaid::Navaid(const std::string& s) : IRecord(s)
{
   STANDARD_CONSTRUCTOR()
   ptbl = &ptable;
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

Navaid::NavaidType Navaid::navaidType() const
{
   char tt[NA_TYPE_LEN+1];
   dafifType(tt);
   return NavaidType(tt[0]);
}

bool Navaid::isNavaidType(const Navaid::NavaidType tt) const
{
   return tt == navaidType();
}

bool Navaid::isDmeType() const
{
   NavaidType t = navaidType();
   // Any DME type
   return (t == ANY) || (t == VORTAC) || (t == TACAN) || (t == VOR_DME) || (t == NDB_DME) || (t == DME);
}

bool Navaid::isDirectionalType() const
{
   NavaidType t = navaidType();
   // Any directional beacon type
   return (t == ANY) || (t == VOR) || (t == VORTAC) || (t == TACAN) || (t == VOR_DME);
}

//------------------------------------------------------------------------------
// Printing functions
//------------------------------------------------------------------------------
void Navaid::printRecordImpl(std::ostream& sout) const
{
   char icas[32];
   icaoCode(icas);

   char ikey[32];
   key(ikey);

   char id[12];
   ident(id);

   char ccode[4];
   countryCode(ccode);

   char rc[8];
   radioClass(rc);

   sout << icas << ", ";

   sout << "\"";
   sout << ikey;
   sout << "\", ";

   sout << id;
   sout << "-";
   sout << static_cast<char>(navaidType());
   sout << "-";
   sout << ccode;
   sout << "-";
   sout << keyCode();
   sout << ":";

   std::streamoff old = sout.precision();
   sout.precision(12);

   sout << "  ";
   sout << latitude();
   sout << ", ";
   sout << longitude();

   sout.precision(old);
   sout << ", ";
   sout << elevation();

   sout << "  ";
   sout << frequency();
   sout << "-";
   sout << channel();

   sout << "  ( ";
   sout << magVariance();
   sout << ", ";
   sout << slaveVariance();
   sout << " )";

   sout << " (";
   sout << power();
   sout << "-";
   sout << rc;
   sout << "-";
   sout << range();
   sout << ")";

}

//------------------------------------------------------------------------------
//  print true bearing and range to system
//------------------------------------------------------------------------------
void Navaid::printTrueBearingRange(std::ostream& sout, const double aclat, const double aclon, const double acelev) const
{
   double bearing{}, range{}, grdrange{};
   base::nav::glla2bd(aclat, aclon, acelev, latitude(), longitude(), elevation(), &bearing, &range, &grdrange);

   bool rangeIsValid{true};
   bool bearingIsValid{true};

   if (rangeIsValid && bearingIsValid)
       sout << "     range " << range << "  grdrange " << grdrange << " true_bearing " << bearing << std::endl;
   else if (rangeIsValid)
       sout << "  range " << range << "  grdrange " << grdrange << std::endl;
   else
       sout << "    true bearing " << bearing << std::endl;
}

}
}
