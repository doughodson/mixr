
#ifndef __mixr_dafif_AirportLoader_H__
#define __mixr_dafif_AirportLoader_H__

#include "mixr/dafif/loaders/Database.hpp"

#include "mixr/dafif/records/Airport.hpp"
#include "mixr/dafif/records/Runway.hpp"
#include "mixr/dafif/records/Ils.hpp"

#ifdef ALT_ILS_FILE
#include "AIParser.h"
#endif

#include <string>

namespace mixr {
namespace dafif {

//------------------------------------------------------------------------------
// Class: AirportLoader
// Description: Concrete DAFIF Airport file class (FILE0)
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: AirportLoader
// Slots: none
//------------------------------------------------------------------------------
// Notes:
//
//    Standard record length is set to airport format records (1), so after
//    an airport query these records can be retrieved using getRecord(i).
//    However, after a runway query, use getRecord(n, RUNWAY_RECORD_LEN),
//    and after an ils query, use getRecord(n,ILS_RECORD_LEN).
//------------------------------------------------------------------------------
class AirportLoader final: public Database
{
   DECLARE_SUBCLASS(AirportLoader, Database)

public:
   AirportLoader();
   AirportLoader(
      const std::string& country,
      const std::string& file = "FILE0",
      const std::string& path = ""
   );

   // Returns the n'th airport loaded.
   // Range: 0 .. numberOfRecords()-1
   Airport* airport(const int n);

   // Get the n'th airport found by last query.
   // Range: 0 .. numberFound()-1
   Airport* getAirport(const int n);

   Runway* getRunway(const int n);
   Ils* getIls(const int n);

   // find all records within the search area with a runway length
   // of at least 'minRwLen' feet.
   int queryByLength(const float minRwLen);
   int queryByType(const Airport::Type);
   int queryByFreq(const float freq);
   int queryByChannel(const int chan);

   int getNumRunwayRecords(const char* key);
   int queryRunwayByNumber(const char* key, const int n);
   int queryRunwayByIdent(const char* id);
   int queryRunwayBySubkey(const char* subkey);
   int queryRunwayByRange();
   int queryRunwayByLength(const float length);
   int queryRunwayByFreq(const float freq);
   int queryRunwayByChannel(const int chan);

   int getNumIlsRecords(const char* key);
   int queryIlsByNumber(const char* key, const int n);
   int queryIlsByIdent(const char* id);
   int queryIlsBySubkey(const char* subkey);
   int queryIlsByRange();
   int queryIlsByType(const Ils::IlsType type);
   int queryIlsByFreq(const float freq);
   int queryIlsByChannel(const int chan);

   struct AirportKey;
   struct RunwayKey;

   // Key used for quick ILS record lookup
   struct IlsKey : public Database::Key
   {
      RunwayKey* parent {};
      char key[ILS_KEY_LEN+1] {};    // DAFIF Key Code
      char id[ILS_IDENT_LEN+1] {};   // DAFIF Identifier
      IlsKey* next {};               // Next ILS key in list
      float freq {};                 // ILS component frequency (MHz)
      int chan {};                   // ILS component channel#
      Ils::IlsType type {Ils::ANY};  // ILS component type

#ifdef ALT_ILS_FILE
      float gs {};                   // ILS glide slope
      float magvar {};               // magvar
      int loc {};                    // position of antenna
      int elev {};                   // elevation
      char name[ILS_NAME_LEN+1] {};  // ILS name

      IlsKey(const long idx);
#endif

      IlsKey(const long idx, const Ils& ils);
      IlsKey(const char* key);

      ~IlsKey();

      void serialize(std::ostream& sout) const;
   };

   // Key used for quick Runway record lookup
   struct RunwayKey : public Database::Key
   {
      AirportKey* parent {};
      char key[RW_KEY_LEN+1] {}; // DAFIF Key Code
      RunwayKey* next {};        // Next runway key in list
      IlsKey* ils {};            // List of ILS records
      int rwlen {};              // Runway length

      RunwayKey(const long idx, const Runway& runway);
      RunwayKey(const char* key);

      ~RunwayKey();

      void serialize(std::ostream& sout) const;
   };

   // Key used for quick Airport record lookup
   struct AirportKey : public Database::Key
   {
      AirportKey(const long idx, const Airport& airport);
      AirportKey(const char* key);

      ~AirportKey();

      void serialize(std::ostream& sout) const;

      char key[AP_KEY_LEN+1] {};                  // DAFIF Key Code
      RunwayKey* runways {};                      // List of runways for this AP
      AirportKey* next {};                        // Next Airport key in list
      Airport::Type type {Airport::Type::ANY};    // Airport component type
   };

#ifdef ALT_ILS_FILE    /* Alternate ILS File */
   const char* createIlsRecord(const Key* key);
#endif

protected:
   enum { AIRPORT_MAX_RECORDS = 40000 };

   int queryAirport(const Airport::Type type, const float minRwLen);

   int chkRwLen(const AirportKey* key, const float minRwLen);

   int chkIlsFreq(const AirportKey* key, const float freq);
   int chkRwIlsFreq(const RunwayKey* key, const float freq);

   int chkIlsChan(const AirportKey* key, const int chan);
   int chkRwIlsChan(const RunwayKey* key, const int chan);

   void makeSimpleLinkedList();

   void findGlideSlope(const RunwayKey* rwKey, const IlsKey* ilsKey);

   static int kl_cmp(const void* p1, const void* p2);

   AirportKey* firstAirport {};  // first airport in linked-list

private:
   bool loadImpl(const std::string& code = "") final;
   int getRecordLengthImpl() final;
   int getMaxRecordsImpl() final;
   int queryByRangeImpl() final;
   int queryByIdentImpl(const char* id) final;
   int queryByKeyImpl(const char* key) final;

   void printLoadedImpl(std::ostream& sout) final;
   void printResultsImpl(std::ostream& sout) final;

#ifdef ALT_ILS_FILE    /* Alternate ILS File */
   char altIlsFile[256];
   char irec[ILS_RECORD_LEN+1];
#endif

};

}
}

#endif
