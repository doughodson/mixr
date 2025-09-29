
#include "mixr/dafif/records/IRecord.hpp"

#include "mixr/base/String.hpp"

#include "mixr/base/util/nav_utils.hpp"
#include "mixr/base/util/str_utils.hpp"

#include <string>
#include <iostream>

#include <cstring>
#include <cstdlib>

namespace mixr {
namespace dafif {

IMPLEMENT_ABSTRACT_SUBCLASS(IRecord, "IRecord")
EMPTY_SLOTTABLE(IRecord)

IRecord::IRecord()
{
   STANDARD_CONSTRUCTOR()
}

IRecord::IRecord(const std::string& s)
{
   STANDARD_CONSTRUCTOR()
   data = s;
}

void IRecord::copyData(const IRecord& org, const bool)
{
   BaseClass::copyData(org);
   data = org.data;

//   setStr( org.c_str() );
   ptbl = org.ptbl;
}

void IRecord::deleteData()
{
   resetData();
}

//------------------------------------------------------------------------------
// resetData: clears our member data
//------------------------------------------------------------------------------
void IRecord::resetData()
{
//   setStr( nullptr );
   data.empty();
}

// set our record string to 's'.
void IRecord::setRecord(const std::string& s)
{
   data = s;
//   setStr( (*s).c_str() );
}

//------------------------------------------------------------------------------
// setRecord() -- set our record string to 's'.
//------------------------------------------------------------------------------
/*
void IRecord::setRecord(const char* const s)
{
   setStr( s );
}
*/

//------------------------------------------------------------------------------
// data access functions
//------------------------------------------------------------------------------

// formatCode: returns the value of the Format Code field
int IRecord::formatCode() const
{
   return dsAtoln( makePointer(FORMAT_CODE_POS), FORMAT_CODE_LEN );
}

// isFormatCode: returns true if code matches the Format Code field
bool IRecord::isFormatCode(const int code) const
{
   return formatCode() == code;
}

// transactionCode: returns the Transaction Code field
char IRecord::transactionCode() const
{
   const char* p{makePointer(TRANSACTION_CODE_POS)};
   if (p != nullptr)
      return *p;
   else
      return '\0';
}

// isTransactionCode: returns true if code matches the Transaction Code field
bool IRecord::isTransactionCode(const char code) const
{
   return transactionCode() == code;
}

// icaoCode: returns the ICAO code field
void IRecord::icaoCode(char code[]) const
{
   dsGetString( code, makePointer(ICAO_CODE_POS), ICAO_CODE_LEN );
}

// isIcaoCode: returns true if code matches the ICAO field
bool IRecord::isIcaoCode(const char code[]) const
{
   return dsIsString( makePointer(ICAO_CODE_POS), code );
}

// key: returns the key field
void IRecord::key(char k[]) const
{
   dsGetString( k, makePointer(ptbl->key), ptbl->keyLength );
}

// isKey: returns true if k matches the key field
bool IRecord::isKey(const char k[]) const
{
   return dsIsString( makePointer(ptbl->key), k );
}

// ident: returns the identifier field
void IRecord::ident(char id[]) const
{
   dsGetString( id, makePointer(ptbl->ident), ptbl->identLength );
}

// isIdent: returns true if id matches the identifier field
bool IRecord::isIdent(const char id[]) const
{
   return dsIsString( makePointer(ptbl->ident), id );
}

// name: returns the name field
void IRecord::name(char nname[]) const
{
   dsGetString( nname, makePointer(ptbl->name), ptbl->nameLength );
}

// isName: returns true if name matches the name field
bool IRecord::isName(const char name[]) const
{
   return dsIsString( makePointer(ptbl->name), name );
}

// dafifType: returns the DAFIF Type field
void IRecord::dafifType(char type[]) const
{
   dsGetString( type, makePointer(ptbl->type), ptbl->typeLength );
}

// isDafifType: returns true if name matches the DAFIF Type field
bool IRecord::isDafifType(const char type[]) const
{
   return dsIsString( makePointer(ptbl->type), type );
}

// countryCode: returns the country code field
void IRecord::countryCode(char country[]) const
{
   dsGetString( country, makePointer(ptbl->countryCode), ptbl->countryLength );
}

// isCountryCode: returns true if country matches the country code field
bool IRecord::isCountryCode(const char country[]) const
{
   return dsIsString( makePointer(ptbl->countryCode), country );
}

// stateCode: returns the value of the State/Province Code field
int IRecord::stateCode() const
{
   return dsAtoln( makePointer(ptbl->stateCode), ptbl->stateLength );
}

// isStateCode: returns true if code matches the State/Province Code field
bool IRecord::isStateCode(const int code) const
{
   return stateCode() == code;
}

// wac: returns the World Area Code field
void IRecord::wac(char code[]) const
{
   dsGetString( code, makePointer(ptbl->wac), ptbl->wacLength );
}

// isWac: returns true if code matches the World Area Code field
bool IRecord::isWac(const char code[]) const
{
   return dsIsString( makePointer(ptbl->wac), code );
}

// latitude: returns the value of the latitude field
double IRecord::latitude() const
{
   return dsLatitude( makePointer(ptbl->latitude) );
}

// longitude: returns the value of the longitude field
double IRecord::longitude() const
{
   return dsLongitude( makePointer(ptbl->longitude) );
}

// elevation: returns the value of the elevation field
float IRecord::elevation() const
{
   return dsElevation( makePointer(ptbl->elevation) );
}

// magVariance: returns the value of the magnetic variance field
float IRecord::magVariance() const
{
   return dsMagVariance( makePointer(ptbl->magVariance) );
}

// dafifRecordNumber: returns the value of the DAFIF Record Number field
int IRecord::dafifRecordNumber() const
{
   return dsAtoln( makePointer(RECORD_NUMBER_POS), RECORD_NUMBER_LEN );
}

// isDafifRecordNumber: returns true if num matches the Record Number field
bool IRecord::isDafifRecordNumber(const int num) const
{
   return dafifRecordNumber() == num;
}

// cycleData: returns the Cycle Date field
void IRecord::cycleDate(char date[]) const
{
   dsGetString( date, makePointer(CYCLE_DATE_POS), CYCLE_DATE_LEN );
}

// isCycleDate: returns true if date matches the Cycle Date field
bool IRecord::isCycleDate(const char date[]) const
{
   return dsIsString( makePointer(CYCLE_DATE_POS), date );
}

//------------------------------------------------------------------------------
// makePointer: make a pointer to the string starting at record column 'n'
//------------------------------------------------------------------------------
const char* IRecord::makePointer(const std::size_t n) const
{
   const char* p{};
   if (!data.empty()) {
      const char* rec {this->data.c_str()};
      if (n > 0 && n <= data.length()) p = (rec + n - 1);
   }

   return p;
}

// get a specific character
char IRecord::getChar(const std::size_t index) const
{
   if (index <= data.length()) {
      return data.at(index);
   }
   return '\0';
}

//------------------------------------------------------------------------------
// dsFreq2chan() -- freq (MHz) to TACAN channel
//------------------------------------------------------------------------------
int IRecord::dsFreq2chan(const float f)
{
   long freq = static_cast<long>(f*10000.0f);
   int c = 0;

   if (freq < 1123000)
      c = (freq - 1063000) / 1000;
   else if (freq >= 1123000 && freq < 1180000)
      c = (freq - 1053000) / 1000;
   else if (freq >= 1133000 && freq < 1343000)
      c = (freq - 1333000) / 1000 + 60;
   else if (freq >= 1344000)
      c = (freq - 1344000) / 1000 + 1;

   return c;
}

//------------------------------------------------------------------------------
// dsChan2freq() -- TACAN channel to freq (MHz)
//------------------------------------------------------------------------------
float IRecord::dsChan2freq(const int chan, const char type)
{
   long freq = 0;

   if (chan >= 1 && chan <= 16)
      freq = (chan - 1) * 1000 + 1344000;
   else if (chan >= 17 && chan <= 59)
      freq = (chan + 1063) * 1000;
   else if (chan >= 60 && chan <= 69)
      freq = (chan - 60) * 1000 + 1333000;
   else if (chan >= 70 && chan <= 126)
      freq = (chan + 1053) * 1000;

   if (type == 'Y')
      freq += 500;

   return static_cast<float>(freq)/10000.0f;
}


//------------------------------------------------------------------------------
// dsAtofn() -- convert n-characters to float
//------------------------------------------------------------------------------
double IRecord::dsAtofn(const char* const s, const int n)
{
   if (s == nullptr) return 0.0;
   if (*s == 'U') return 0.0;

   const std::size_t BUF_LENGTH{256};
   char buf[BUF_LENGTH];

   base::utStrncpy(buf, BUF_LENGTH, s, n);
   buf[n] = '\0';

   return atof(buf);
}


//------------------------------------------------------------------------------
// dsAtoln() -- convert n-characters to long
//------------------------------------------------------------------------------
long IRecord::dsAtoln(const char* const s, const int n)
{
   if (s == nullptr) return 0;
   if (*s == 'U') return 0;

   const std::size_t BUF_LENGTH = 256;
   char buf[BUF_LENGTH];

   base::utStrncpy(buf, BUF_LENGTH, s, n);
   buf[n] = '\0';

   return std::atol(buf);
}


//------------------------------------------------------------------------------
// dsGetString() -- like strncpy() with 0 char* checking and the
// spaces removed from the tail of the string.
//------------------------------------------------------------------------------
void IRecord::dsGetString(char* const d, const char* const s, const int n)
{
   // Make sure we have a place to copy it
   if (d == nullptr) return;

   // Make sure we have something to copy
   if (s == nullptr || n <= 0) {
      d[0] = '\0';
      return;
   }

   // Set temp pointers
   const char* p = s;
   char*       q = d;

   // copy the string
   for (int i = 0; i < n; i++) { *q++ = *p++; }

   // terminate the string
   *q-- = '\0';

   // strip spaces of the tail
   for (; q >= d && *q == ' '; q--) { *q = '\0'; }
}


//------------------------------------------------------------------------------
// dsIsString() -- true if string p matches refStr
//------------------------------------------------------------------------------
bool IRecord::dsIsString(const char* const p, const char* const refStr)
{
   std::size_t len;
   if (p == nullptr || refStr == nullptr) return false;
   len = std::strlen(refStr);
   return std::strncmp(p,refStr,len) == 0;
}

//------------------------------------------------------------------------------
// dsLatitude() -- convert string to latitude
//------------------------------------------------------------------------------
double IRecord::dsLatitude(const char* const p)
{
   double lat {};
   if (p != nullptr) {
      char   h {*p};
      double d {dsAtofn(p+1, 2)};
      double m {dsAtofn(p+3, 2)};
      double s {dsAtofn(p+5, 2)};
      double f {dsAtofn(p+7, 2)};
      double v {d + m / 60.0 + (s + f / 100.0) / 3600.0};
      lat = (h == 'N' ? v : -v);
   }
   return lat;
}

//------------------------------------------------------------------------------
// dsLongitude() --  convert string to longitude
//------------------------------------------------------------------------------
double IRecord::dsLongitude(const char* const p)
{
   double lon = 0.0;
   if (p != nullptr) {
      char   h = *p;
      double d = dsAtofn(p+1, 3);
      double m = dsAtofn(p+4, 2);
      double s = dsAtofn(p+6, 2);
      double f = dsAtofn(p+8, 2);
      double v = d + m / 60.0 + (s + f / 100.0) / 3600.0;
      lon = (h == 'E' ? v : -v);
   }
   return lon;
}

//------------------------------------------------------------------------------
// dsMagVariance() --  convert string to magnetic variation
//------------------------------------------------------------------------------
float IRecord::dsMagVariance(const char* const p)
{
   double mv = 0.0;
   if (p != nullptr) {
      double d = dsAtofn(p+1, 3);
      double m = dsAtofn(p+4, 2);
      double f = dsAtofn(p+6, 1);
      double v = d + (m + f / 10.0f) / 60.0f;
      mv = ( (*p == 'E') ? v : -v );
   }
   return static_cast<float>(mv);
}

//------------------------------------------------------------------------------
// dsSlaveVariance() -- convert string to slave magnetic variation
//------------------------------------------------------------------------------
float IRecord::dsSlaveVariance(const char* const p)
{
   double sv = 0.0;
   if (p != nullptr) {
      if (*p != ' ') {
         double v = dsAtofn(p+1, 3);
         sv = ( (*p == 'E') ? v : -v );
      }
   }
   return static_cast<float>(sv);
}

//------------------------------------------------------------------------------
// dsMagHeading() -- magnetic heading
//------------------------------------------------------------------------------
float IRecord::dsMagHeading(const char* const p)
{
   double mh = 0.0;
   if (p != nullptr) {
      mh = dsAtofn(p,3);    // qty
      char ct = *(p+3);     // tenths
      if (ct != '/') mh += static_cast<float>(ct - '0')/10.0f;
   }
   return static_cast<float>(mh);
}

//------------------------------------------------------------------------------
// dsElevation() -- format#1: convert string to elevation (to nearest foot)
//------------------------------------------------------------------------------
float IRecord::dsElevation(const char* const p)
{
   if (p == nullptr) return 0.0f;
   if (*p == '\0') return 0.0f;

   if (*p == '-')
      return static_cast<float>(-dsAtofn(p+1, 4));
   else
      return static_cast<float>(dsAtofn(p, 5));
}

//------------------------------------------------------------------------------
// dsElevation1() -- format#2: convert string to elevation (to tenths
// of feet)
//------------------------------------------------------------------------------
float IRecord::dsElevation1(const char* const p)
{
   double e, et;
   char ct;

   if (p == nullptr) return 0.0f;
   if (*p == '\0') return 0.0f;

   e = 0.0;
   if (*p == '-') e = dsAtofn(p+1, 4);
   else e = dsAtofn(p, 5);

   et = 0.0;
   ct = *(p+5);
   if (ct != ' ') et = ct - '0';

   if (*p == '-')
      return static_cast<float>(-(e + et/10.0));
   else
      return static_cast<float>(e + et/10.0);
}


//------------------------------------------------------------------------------
// dsFrequency() -- convert string to frequency  (MHz)
//------------------------------------------------------------------------------
float IRecord::dsFrequency(const char* const p)
{
   long freq = 0;

   if (*p != 'U') {
      int i;
      for (i = 0; i < 7 && p[i] == ' '; i++)
      if (i < 7) {
         freq = dsAtoln(p, 7);
         if (*(p+7) == 'M') freq *= 1000;
      }
   }

   return static_cast<float>(freq)/1000000.0f;
}

//------------------------------------------------------------------------------
// serialize true bearing and range to system
//------------------------------------------------------------------------------
void IRecord::printTrueBearingRange(std::ostream& sout, const double aclat, const double aclon, const double acelev)const
{
   double bearing, range, grdrange;
   base::nav::glla2bd(aclat, aclon, acelev, latitude(), longitude(), elevation(), &bearing, &range, &grdrange);
   sout << "  range " << range << "  grdrange " << grdrange << " true_bearing " << bearing;
}

}
}
