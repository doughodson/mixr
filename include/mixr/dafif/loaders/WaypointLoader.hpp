
#ifndef __mixr_dafif_WaypointLoader_HPP__
#define __mixr_dafif_WaypointLoader_HPP__

#include "mixr/dafif/loaders/Database.hpp"

#include "mixr/dafif/records/Waypoint.hpp"

#include <string>

namespace mixr {
namespace dafif {

//------------------------------------------------------------------------------
// Class: WaypointLoader
// Description: Concrete DAFIF Waypoint file class (FILE3)
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: WaypointLoader
// Slots: none
//------------------------------------------------------------------------------
class WaypointLoader final: public Database
{
   DECLARE_SUBCLASS(WaypointLoader,Database)

public:
   WaypointLoader();
   WaypointLoader( const std::string& country,
      const std::string& file = "FILE3",
      const std::string& path = ""
   );

   // Returns the n'th waypoint loaded.
   // Range: 0 .. numberOfRecords()-1
   Waypoint* waypoint(const int);
   // Get the n'th waypoint found by last query.
   // Range: 0 .. numberFound()-1
   Waypoint* getWaypoint(const int);

protected:
   enum { WAYPOINT_MAX_RECORDS = 140000 };

   // Memory key used for quick Waypoint record lookup
   struct WaypointKey : public Database::Key
   {
      WaypointKey(const long idx, const Waypoint& waypoint);
      WaypointKey(const char* id, const char* country);
      WaypointKey(const char* key);

      ~WaypointKey();

      void serialize(std::ostream& sout) const;

      char key[WP_KEY_LEN+1] {};             // DAFIF Key Code
      char ident[WP_IDENT_LEN+1] {};         // Identifier
      char countryCode[WP_CCODE_LEN+1] {};   // Country Code
   };

   static int il_cmp(const void* p1, const void* p2);
   static int kl_cmp(const void* p1, const void* p2);

private:
   bool loadImpl(const std::string& code = "") final;
   int getRecordLengthImpl() final;
   int getMaxRecordsImpl() final;
   int queryByRangeImpl() final;
   int queryByIdentImpl(const char* id) final;
   int queryByKeyImpl(const char* key) final;

   void printLoadedImpl(std::ostream&) final;
   void printResultsImpl(std::ostream&) final;
};

}
}

#endif
