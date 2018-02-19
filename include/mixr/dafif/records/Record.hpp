
#ifndef __mixr_dafif_Record_H__
#define __mixr_dafif_Record_H__

#include "mixr/base/Object.hpp"

#include "mixr/dafif/dafifc.hpp"

#include <string>

namespace mixr {
namespace dafif {

//------------------------------------------------------------------------------
// Class: Record
// Description: Abstract class used to access DAFIF record information
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: AbstractRecord
// Slots: none
//------------------------------------------------------------------------------
class Record : public base::Object
{
   DECLARE_SUBCLASS(Record, base::Object)

public:
   // record types
   enum Type { INVALID = -1, ANY = 0,
      NAVAID = 2, WAYPOINT = 3,
      AIRPORT = 10, RUNWAY = 11, ILS = 12
   };

public:
   Record();
   Record(const std::string&);

   // sets this object's record string to 'string' (does not make a copy of 'string')
   void setRecord(const std::string&);

   // returns the value of the Format Code field
   int formatCode() const;
   // returns true if 'code' is equal to the Format Code field
   bool isFormatCode(const int code) const;

   // returns the character contained in the Transaction Code field
   char transactionCode() const;
   // returns true if 'code' is equal to the Transaction Code field
   bool isTransactionCode(const char c) const;

   // returns the value of the ICAO field in 'code'
   void icaoCode(char code[]) const;
   // returns true if 'code' is equal to the ICAO Code field
   bool isIcaoCode(const char code[]) const;

   // returns the value of the record's key fields in 'key1'
   void key(char k[]) const;
   // returns true if 'key1' is equal to the record's key fields
   bool isKey(const char k[]) const;

   // returns the value of the record's identifier field in 'id'
   virtual void ident(char id[]) const;
   // returns true if 'id' is equal to the record's identifier field
   bool isIdent(const char id[]) const;

   // returns the value of the name field in 'name1'
   void name(char n[]) const;
   // returns true if 'name1' is equal to the Name field
   bool isName(const char name[]) const;

   // returns the value of the record's Type field in 'type'
   void dafifType(char type[]) const;
   // returns true if 'type' is equal to the record's Type field
   bool isDafifType(const char type[]) const;

   // returns the value of the Country Code field in 'country'
   void countryCode(char country[]) const;
   // returns true if 'code' is equal to the Country Code field
   bool isCountryCode(const char country[]) const;

   // returns the value of the State/Province Code field
   int stateCode() const;
   // returns true if 'code' is equal to the State/Province Code field
   bool isStateCode(const int code) const;

   // returns the value of the WAC field in 'code'
   void wac(char code[]) const;
   // returns true if 'code' is equal to the WAC Code field
   bool isWac(const char code[]) const;

   // Return the values of the latitude, longitude, elevation,
   // and magnetic variance fields.
   double latitude() const;
   double longitude() const;
   float  elevation() const;
   float  magVariance() const;

   // returns the value of the Record Number field
   int dafifRecordNumber() const;
   bool isDafifRecordNumber(const int num) const;

   // returns the value of the Cycle Date field in 'date'
   void cycleDate(char date[]) const;
   bool isCycleDate(const char date[]) const;

   virtual void printTrueBearingRange(std::ostream& sout, const double aclat, const double aclon, const double acelev) const;
   virtual void printRecord(std::ostream& x) const { printRecordImpl(x); }

   // ---
   // Common conversion and support functions
   // ---
   static float  dsChan2freq(const int chan, const char type);
   static int    dsFreq2chan(const float freq);
   static double dsAtofn(const char* const s, const int n);
   static long   dsAtoln(const char* const s, const int n);
   static void   dsGetString(char* const d, const char* const s, const int n);
   static bool   dsIsString(const char* const s, const char* const refStr);
   static double dsLatitude(const char* const);       // convert string to latitude
   static double dsLongitude(const char* const);      // convert string to longitude
   static float  dsElevation(const char* const);      // format#1: convert string to elevation (to nearest foot)
   static float  dsElevation1(const char* const);     // format#2: convert string to elevation (to tenths of feet)
   static float  dsMagVariance(const char* const);    // convert string to magnetic variation
   static float  dsSlaveVariance(const char* const);  // convert string to slave magnetic variation
   static float  dsMagHeading(const char* const);     // convert string to magnetic heading
   static float  dsFrequency(const char* const);      // convert string to frequency  (MHz)

protected:
   const char* makePointer(const std::size_t offset) const;
   void resetData();

   char getChar(const std::size_t index) const;  // get a specific character

   // ---
   // Position Table
   // ---
   struct Ptbl {
      int key;             // Offset to key
      int keyLength;       // Length of the key field
      int ident;           // Offset to ident
      int identLength;     // Length of the ident field
      int name;            // Offset to name
      int nameLength;      // Length of the name field
      int type;            // Offset to type
      int typeLength;      // Length of the type field
      int countryCode;     // Offset to country code
      int countryLength;   // Length of the country code field
      int stateCode;       // State/Province Code
      int stateLength;     // Length of State/Province code field
      int wac;             // World Area Code
      int wacLength;       // Length of WAC field
      int latitude;        // Offset to latitude
      int longitude;       // Offset to longitude
      int elevation;       // Offset to elevation
      int magVariance;     // Offset to mag var
   };

   // Object's field position table.  The Record constructor sets
   // this variable to 0.  Constructors of the derived classes must
   // set this variable to their own <class>::ptable.
   const Ptbl* ptbl{};    // Position table; one for each instance

private:
   std::string data;
   virtual void printRecordImpl(std::ostream&) const =0;
};

}
}

#endif
