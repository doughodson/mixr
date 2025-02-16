
#ifndef __mixr_dafif_Ils_HPP__
#define __mixr_dafif_Ils_HPP__

#include "mixr/dafif/records/IRecord.hpp"

#include <string>

namespace mixr {
namespace dafif {

//------------------------------------------------------------------------------
// Class: Ils
// Description: Access to the DAFIF ILS records.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Ils
// Slots: none
//------------------------------------------------------------------------------
class Ils final: public IRecord
{
   DECLARE_SUBCLASS(Ils, IRecord)

public:
   Ils();
   Ils(const std::string&);

   // length of an ILS record pair
   enum { RECORD_LENGTH = ILS_RECORD_LEN };

   enum IlsType { INVALID = -1, ANY = 'Y',
          LOCATOR = 'L', DME = 'D', LOCALIZER = 'Z', GLIDESLOPE = 'G',
          BACKCOURSE_MARKER = 'B', INNER_MARKER = 'I', MIDDLE_MARKER = 'M',
          OUTER_MARKER = 'O', UNKNOWN = 'U'
   };

   // returns the ils type
   IlsType ilsType() const;
   // returns true if 'type' is equal to the ils type
   int isIlsType(const IlsType type) const;

   // returns the frequency (MHz)
   float frequency() const;
   // returns the channel number
   int   channel() const;
   // returns the value of the Glide Slope Angle field
   float glideSlopeAngle() const;
   // gets the record key of the airport
   void  airportKey(char apKey[]) const;
   // gets the runway identifier
   void  runwayIdent(char rwId[]) const;
   // gets the runway end identifier
   void  runwayEndIdent(char rwEndId[]) const;

   void  getGlideSlopeData(const double aclat, const double aclon, const double acelev, float* ilsGlideSlope, float* acGlideSlope, float* deltaGlideSlope) const;
   void  printGlideSlopeData(std::ostream& sout, const double aclat, const double aclon, const double acelev) const;

private:
   static const Ptbl ptable;

   void printRecordImpl(std::ostream& sout) const final;
};

// frequency: returns the value of the frequency field
inline float Ils::frequency() const
{
   return dsFrequency( makePointer(ILS_FREQUENCY_POS) );
}

// channel: returns the value of the channel field
inline int Ils::channel() const
{
   return dsAtoln( makePointer(ILS_CHANNEL_POS), ILS_CHANNEL_LEN );
}

inline float Ils::glideSlopeAngle() const
{
   return static_cast<float>(dsAtofn( makePointer(ILS_GSA_POS), ILS_GSA_LEN) / 100.0);
}

// airportKey: returns the value of the airport identifier
inline void Ils::airportKey(char apKey[]) const
{
   dsGetString( apKey, makePointer(ILS_APKEY_POS), AP_KEY_LEN );
}

// runwayIdent: returns the value of the runway identifier
inline void Ils::runwayIdent(char rwKey[]) const
{
   dsGetString( rwKey, makePointer(ILS_RW_ID_POS), ILS_RW_ID_LEN );
}

// runwayEndIdent: returns the value of the runway end identifier
inline void Ils::runwayEndIdent(char rwEndKey[]) const
{
   dsGetString( rwEndKey, makePointer(ILS_RW_ID_POS+AP_KEY_LEN), RW_XE_IDENT_LEN );
}

}
}

#endif
