
#ifndef __mixr_dafif_Database_HPP__
#define __mixr_dafif_Database_HPP__

#include "mixr/base/Object.hpp"

#include "mixr/dafif/dafifc.hpp"

#include <string>

namespace mixr {
namespace base { class FileReader; class String; }
namespace dafif {

//------------------------------------------------------------------------------
// Class: Database
// Description: Abstract class used to access DAFIF databases; both file
//              loaders and network clients.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: AbstractDatabase
// Slots:
//    pathname  <String>   ! Path to location of file
//    filename  <String>   ! Filename to open
//------------------------------------------------------------------------------
//  Notes:
//
//   1) Many of the member functions of this DAFIF database manager class and
//      its derived classes read files.  The time required to read these files
//      can not be determined.  Therefore, do not access the DAFIF database
//      classes from a time critical thread.
//
//   2) Many of the member functions work together to query and retrieve data
//      records.  One a single thread (single user) application, this is not
//      a problem.  However, on a multi-thread (multi-user) application, the
//      in-use member functions should be used to protect the data during a
//      data query and retrieval operation.
//      Example --
//           if (requestDbInUse()) {
//               // We have exclusive use of the database ...
//               setArea(lat,lon,rng);   // sets the ref area, as needed
//               queryByIdent();         // query for the required records
//               ....                    // access the records
//               clearDbInUse();         // free the database
//           }
//------------------------------------------------------------------------------
class Database : public base::Object
{
   DECLARE_SUBCLASS(Database, base::Object)

public:
   Database();

   // --
   // extension points
   // --
   bool load(const std::string& code = "")   { return loadImpl(code);         }
   int getRecordLength()                     { return getRecordLengthImpl();  }
   int getMaxRecords()                       { return getMaxRecordsImpl();    }
   // find all records within the search area set by setArea()
   int queryByRange()                        { return queryByRangeImpl();     }
   // find all records within the search area with identifier 'id'
   int queryByIdent(const char* id)          { return queryByIdentImpl(id);   }
   // find the record with 'key'
   int queryByKey(const char* key)           { return queryByKeyImpl(key);    }
   // prints the records loaded
   void printLoaded(std::ostream& sout)      { return printLoadedImpl(sout);  }
   // prints the records found by the last query
   void printResults(std::ostream& sout)     { return printResultsImpl(sout); }

   // Requests the exclusive use of the database.  Returns true if use is
   // granted, or false if the database is already in-use.
   bool requestDbInUse()               { return ( (dbInUse && dbLoaded) ? false : (dbInUse = true)); }

   // Clears the database in-use flag.  Returns the previous state of the
   // in-use flag.
   bool clearDbInUse()                 { return (dbInUse ? !(dbInUse = false) : false); }
   bool isDbLoader() const             { return dbLoaded; }

   // returns the number of records in this database
   int numberOfRecords();

   // returns the number of records found by last query
   int numberFound();

   // returns the search area (ref point)
   void getArea(double* const lat, double* const lon);
   void getArea(double* const lat, double* const lon, double* const mrng);

   // Sets the search area (ref point) for queries to lat and lon.
   // If mrng is provided, then query results are limited to mrng.
   void setArea(const double lat, const double lon, const double mrng = 0);

   // If mrec is provided, then the limit on records found by a
   // query is set to mrec.  If mrec is not provided, then there
   // is no limit on the number of records found by a query.
   void setQueryLimit(const int mrec = 0);

   // Returns the limit on records found by a query.  Zero is
   // returned if no limit exists on the number of records found
   // by a query.
   int getQueryLimit();

   // Returns a pointer to n'th record loaded.  This points to an
   // internal buffer and is only valid until the next file access.
   // Range: 0 .. numberOfRecords()-1
   const char* record(const int n, const int size = 0);

   // Get a pointer to the n'th record found by last query.  This
   // points to an internal buffer that is only valid until the
   // next file access.   Range: 0 .. numberFound()-1
   const char* getRecord(const int n, const int size = 0);

   // find all records within the search area with ICAO code
   int queryByIcao(const char* code);
   // prints the records in the ICAO sorted list
   void printIcaoList(std::ostream& sout);

   const std::string& getPathname() const;
   const std::string& getFilename() const;
   bool setPathname(const std::string&);
   bool setFilename(const std::string&);

   // Memory key used for quick dafif record lookup
   struct Key {
      Key() = delete;
      Key(const int idx);
      Key(const float lat, const float lon);
      Key(const char* code);
      ~Key();

      void serialize(std::ostream& sout) const;

      int idx{};        // Database index
      int size{};       // Size of record

      double lat{};     // Latitude
      double lon{};     // Longitude

      double rng2{};    // Range squared to ref point (nm**2)

      char icao[ICAO_CODE_LEN+1]{};  // ICAO code
   };

protected:
   bool openDatabaseFile();

   const char* dbGetRecord(const Key* key, const int size = 0);

   int sQuery(Key** key, Key** base, std::size_t n,
               int (*cmp)(const void*, const void*));

   int mQuery(Key** key, Key** base, std::size_t n,
               int (*cmp)(const void*, const void*));

   void expandResults(Key** key, Key** keyPtr,
                        int (*cmp)(const void*, const void*),
                        Key** base, std::size_t n);

   void createIcaoList();

   int rangeSort();     // Sort results by range; first compute range and then
                        // uses rangeSort2() to sort.

   int rangeSort2();    // Sort results by range; range must have already
                        // been computed.

   double range2(const double lat, const double lon) const;

   static int rlqs(const void* p1, const void* p2);
   static int ol_cmp(const void* p1, const void* p2);

   static void stripSpaces(char buff[], const int n);
   static void fillSpaces(char buff[], const int n);

   base::FileReader* db{};  // The database (loaders)
   long ncache{};           // Number of keys allocated

   Key** rl{};         // List of DAFIF records in the database
   long nrl{};         // Number of Records in rl

   Key** ol{};         // List of DAFIF records in ICAO code order
   long nol{};         // Number of Records in ol

   Key** ql{};         // query list -- results of query (usually sorted
                       //   by range)
   int nql{};          // Number of record found
   int qlimit{};       // Query limit (zero if no limit)

   double refLat{};    // Ref point latitude
   double refLon{};    // Ref point longitude
   double coslat{};    // cos(ref point latitude)
   double mrng{};      // max range (nm)

   bool dbInUse{};     // Database In-Use flag
   bool dbLoaded{};    // Database has been loader

private:
   virtual bool loadImpl(const std::string& code = "") = 0;
   virtual int getRecordLengthImpl() = 0;
   virtual int getMaxRecordsImpl() = 0;
   virtual int queryByRangeImpl() = 0;
   virtual int queryByIdentImpl(const char* id) = 0;
   virtual int queryByKeyImpl(const char* key) = 0;

   virtual void printLoadedImpl(std::ostream&);
   virtual void printResultsImpl(std::ostream&);

private:
   // slot table helper methods
   bool setSlotPathname(const base::String* const);
   bool setSlotFilename(const base::String* const);
};

}
}

#endif
