
#ifndef __mixr_dafif_NavaidLoader_HPP__
#define __mixr_dafif_NavaidLoader_HPP__

#include "mixr/dafif/loaders/Database.hpp"

#include "mixr/dafif/records/Navaid.hpp"

#include <string>

namespace mixr {
namespace dafif {

//------------------------------------------------------------------------------
// Class: NavaidLoader
// Description: Concrete DAFIF NAVAID file class (FILE2)
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: NavaidLoader
// Slots: none
//------------------------------------------------------------------------------
class NavaidLoader final: public Database
{
   DECLARE_SUBCLASS(NavaidLoader, Database)

public:
   NavaidLoader();
   NavaidLoader( const std::string& country,
      const std::string& file = "FILE2",
      const std::string& path = ""
   );

   // Returns the n'th NAVAID loaded.
   // Range: 0 .. numberOfRecords()-1
   Navaid* navaid(const int n);
   // Get the n'th NAVAID found by last query.
   // Range: 0 .. numberFound()-1
   Navaid* getNavaid(const int n);

   // find 'type' NAVAID records within the search area
   int queryByType(const Navaid::NavaidType type);
   // find all records within the search area with a given frequency
   int queryByFreq(const float freq);
   // find all records within the search area with a given channel number
   int queryByChannel(const long chan, const char band = 'X');

   // prints the records loaded in frequency order
   void printFreqList(std::ostream& sout);
   // prints the records loaded in channel number order
   void printChannelList(std::ostream& sout);

protected:
   enum { NAVAID_MAX_RECORDS = 80000 };

   // Memory key used for quick Navaid record lookup
   struct NavaidKey : public Database::Key
   {
      NavaidKey(const long idx, const Navaid& navaid);
      NavaidKey(const char* key);
      NavaidKey(const char* id, const char* country);
      NavaidKey(const float freq);
      NavaidKey(const long chan);

      ~NavaidKey();

      void serialize(std::ostream& sout) const;

      float freq {};            // Frequency
      int channel {};           // TACAN channel
                                //   (+channel ==> X band)
                                //   (-channel ==> Y band)
      char key[NA_KEY_LEN+1] {};               // DAFIF Key
      char ident[NA_IDENT_LEN+1] {};           // Identifier
      char countryCode[NA_CCODE_LEN+1] {};     // Country Code
      Navaid::NavaidType type {Navaid::ANY};   // Type
   };

   static int il_cmp(const void* p1, const void* p2);
   static int kl_cmp(const void* p1, const void* p2);
   static int fl_cmp(const void* p1, const void* p2);
   static int cl_cmp(const void* p1, const void* p2);

private:
   bool loadImpl(const std::string& code = "") final;
   int getRecordLengthImpl() final;
   int getMaxRecordsImpl() final;
   int queryByRangeImpl() final;
   int queryByIdentImpl(const char* id) final;
   int queryByKeyImpl(const char* key) final;

   void printLoadedImpl(std::ostream&) final;
   void printResultsImpl(std::ostream&) final;

   NavaidKey** fl {};   // List of DAFIF records in frequency order
   long nfl {};         // Number of Records in fl

   NavaidKey** cl {};   // List of DAFIF records in channel order
   long ncl {};         // Number of Records in cl
};

}
}

#endif
