
#ifndef __mixr_dafif_Runway_H__
#define __mixr_dafif_Runway_H__

#include "mixr/dafif/records/Record.hpp"

#include <string>

namespace mixr {
namespace dafif {

//------------------------------------------------------------------------------
// Class: Runway
// Description: Access to the DAFIF Airport/Runway records
//------------------------------------------------------------------------------
class Runway final: public Record
{
   DECLARE_SUBCLASS(Runway, Record)

public:
   // length of a DAFIF Runway record pair
   enum { RECORD_LENGTH = RUNWAY_RECORD_LEN };

   enum WhichEnd { LOW_END = 0, HIGH_END = 1 };

public:
   Runway();
   Runway(const std::string&);

   // Return the values of the latitude, longitude, elevation, slope
   // and magnetic heading fields for 'whichEnd' of the runway.
   // WhichEnd is Runway::HIGH_END or Runway::LOW_END.
   double latitude(const WhichEnd whichEnd) const;
   double longitude(const WhichEnd whichEnd) const;
   float elevation(const WhichEnd whichEnd) const;
   float magHeading(const WhichEnd whichEnd) const;
   float slope(const WhichEnd whichEnd) const;

   // returns the value of the runway's (end) identifier field in 'id'
   void ident(char ident[], const WhichEnd whichEnd) const;
   // returns true if 'id' is equal to the runway's (end) id field
   int isIdent(const char id[], const WhichEnd whichEnd) const;

   // returns the width of the runway (feet)
   int width() const;
   // returns the length of the runway (feet)
   int length() const;

   // returns the record key of the airport
   void airportKey(char apKey[]) const;

   void getRunwayMagHeading(const double aclat, const double aclon, const double acelev, float* magHeading1, float* magHeading2, double* trueBearing1, double* trueBearing2) const;
   void printRunwayMagHeading(std::ostream& sout, const double aclat, const double aclon, const double acelev) const;

   // returns which end of the runway matches the runway end id
   WhichEnd whichEnd(const char rwEndId[]) const;

private:
   static const Ptbl ptable;

   void printRecordImpl(std::ostream& sout) const final;
};

// ident: returns the runway identifier field
inline void Runway::ident(char ident[], const WhichEnd whichEnd) const
{
   if (whichEnd == LOW_END)
      dsGetString(ident, makePointer(RW_LE_IDENT_POS), RW_XE_IDENT_LEN);
   else
      dsGetString(ident, makePointer(RW_HE_IDENT_POS), RW_XE_IDENT_LEN);
}

// isIdent: returns true if id runway the identifier field
inline int Runway::isIdent(const char id[], const WhichEnd whichEnd) const
{
   if (whichEnd == LOW_END)
      return dsIsString( makePointer(RW_LE_IDENT_POS), id );
   else
      return dsIsString( makePointer(RW_HE_IDENT_POS), id );
}


// latitude: returns the value of the runway end latitude field
inline double Runway::latitude(const WhichEnd whichEnd) const
{
   if (whichEnd == LOW_END)
      return dsLatitude( makePointer(RW_LE_LATITUDE_POS) );
   else
      return dsLatitude( makePointer(RW_HE_LATITUDE_POS) );
}


// longitude: returns the value of the runway end longitude field
inline double Runway::longitude(const WhichEnd whichEnd) const
{
   if (whichEnd == LOW_END)
      return dsLongitude( makePointer(RW_LE_LONGITUDE_POS) );
   else
      return dsLongitude( makePointer(RW_HE_LONGITUDE_POS) );
}


// elevation: returns the value of the runway end elevation field
inline float Runway::elevation(const WhichEnd whichEnd) const
{
   if (whichEnd == LOW_END)
      return dsElevation1( makePointer(RW_LE_ELEVATION_POS) );
   else
      return dsElevation1( makePointer(RW_HE_ELEVATION_POS) );
}


// slope: returns the value of the runway end slope field
inline float Runway::slope(const WhichEnd whichEnd) const
{
   if (whichEnd == LOW_END)
      return static_cast<float>(dsAtoln( makePointer(RW_LE_SLOPE_POS), RW_XE_SLOPE_LEN ));
   else
      return static_cast<float>(dsAtoln( makePointer(RW_HE_SLOPE_POS), RW_XE_SLOPE_LEN ));
}


// magHeading: returns the value of the runway end mag heading field
inline float Runway::magHeading(const WhichEnd whichEnd) const
{
   if (whichEnd == LOW_END)
      return dsMagHeading( makePointer(RW_LE_MAGHDG_POS) );
   else
      return dsMagHeading( makePointer(RW_HE_MAGHDG_POS) );
}


// width: returns the value of the runway width field
inline int Runway::width() const
{
   return dsAtoln( makePointer(RW_WIDTH_POS), RW_WIDTH_LEN );
}


// length: returns the value of the runway length field
inline int Runway::length() const
{
   return dsAtoln( makePointer(RW_LENGTH_POS), RW_LENGTH_LEN );
}


// airportKey: returns the value of the airport identifier
inline void Runway::airportKey(char apKey[]) const
{
   dsGetString( apKey, makePointer(RW_APKEY_POS), AP_KEY_LEN );
}


// whichEnd: which end of the runway are we approaching?
inline Runway::WhichEnd Runway::whichEnd(const char rwEndId[]) const
{
   Runway::WhichEnd we = Runway::HIGH_END;
   if ( !isIdent(rwEndId,we) ) we = Runway::LOW_END;
   return we;
}

}
}

#endif
