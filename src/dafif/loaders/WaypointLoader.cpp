
#include "mixr/dafif/loaders/WaypointLoader.hpp"

#include "mixr/base/FileReader.hpp"
#include "mixr/base/util/str_utils.hpp"

#include <string>
#include <cstring>
#include <cstdlib>
#include <limits>

namespace mixr {
namespace dafif {

IMPLEMENT_SUBCLASS(WaypointLoader, "WaypointLoader")
EMPTY_SLOTTABLE(WaypointLoader)
EMPTY_DELETEDATA(WaypointLoader)
EMPTY_COPYDATA(WaypointLoader)

WaypointLoader::WaypointLoader() : IDatabase()
{
   STANDARD_CONSTRUCTOR()
}

WaypointLoader::WaypointLoader(
                  const std::string& country,
                  const std::string& file,
                  const std::string& path)
               : IDatabase()
{
   STANDARD_CONSTRUCTOR()

   db->setPathname(path);
   db->setFilename(file);
   load(country);
}

//------------------------------------------------------------------------------
// load() --
//------------------------------------------------------------------------------
bool WaypointLoader::loadImpl(const std::string& country)
{
   // ---
   // Make sure the database file is open
   // ---
   if ( !openDatabaseFile()  ) {
      // Something is wrong!
       if (isMessageEnabled(MSG_ERROR)) {
          std::cerr << "WaypointLoader::load() -- Error; unable to load Waypoints!" << std::endl;
       }
      return false;
   }


   // ---
   // Read the file and create WaypointKey's for all records in our country
   // ---
   Waypoint waypoint;
   for (const char* r=db->getFirstRecord(); r != nullptr; r=db->getNextRecord()) {

      waypoint.setRecord(r);

      int inArea{true};
      if ( country != "" ) inArea = waypoint.isCountryCode(country.c_str());

      if ( inArea ) {

         if (nrl > ncache) {
            if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "WaypointLoader: key table too small; ";
            std::cerr << "increase WAYPOINT_MAX_RECORDS" << std::endl;
            }
             std::exit(EXIT_FAILURE);
         }

         rl[nrl] = new WaypointKey( db->getRecordNumber(), waypoint );
         nrl++;
      }
   }

   // allocate space for the query results list
   nql = 0;
   if (nrl > 0) {
      ql = new Key*[nrl];
   }

   // create the ICAO list
   createIcaoList();

   dbLoaded = true;
   return true;
}


//------------------------------------------------------------------------------
// getRecordLength()
//------------------------------------------------------------------------------
int WaypointLoader::getRecordLengthImpl()
{
   return Waypoint::RECORD_LENGTH;
}

//------------------------------------------------------------------------------
// getMaxRecords()
//------------------------------------------------------------------------------
int WaypointLoader::getMaxRecordsImpl()
{
   return WAYPOINT_MAX_RECORDS;
}


//------------------------------------------------------------------------------
// waypoint() -- get the WAYPOINT by number: 0 ... numberOfRecords()-1
// getWaypoint() -- get a WAYPOINT record from the last query:
//                  0 ... numberFound()-1
//------------------------------------------------------------------------------
Waypoint* WaypointLoader::waypoint(const int n)
{
   const char* s = record(n);
    if (s != nullptr)
       return new Waypoint(s);
    else
       return nullptr;
}

Waypoint* WaypointLoader::getWaypoint(const int n)
{
    const char* s = getRecord(n);
    if (s != nullptr)
       return new Waypoint(s);
    else
       return nullptr;
}


//------------------------------------------------------------------------------
// queryByRange() -- find waypoint record(s) less than maxRange from the
// ref point (sorted by range)
//------------------------------------------------------------------------------
int WaypointLoader::queryByRangeImpl()
{
   double mr2(std::numeric_limits<float>::max());
   if (mrng > 0.0f) mr2 = mrng*mrng;

   // compute range**2 to ref point and select all that have range less
   // than maxRange
   nql = 0;
   for (int i = 0; i < nrl; i++) {
      Key* k = rl[i];
      k->rng2 = range2(k->lat,k->lon);
      if (k->rng2 < mr2) {
         ql[nql++] = k;
      }
   }

   // sort and limit by range
   rangeSort2();

   // limit number of result records
   if (qlimit > 0 && nql > qlimit) nql = qlimit;

   return nql;
}


//------------------------------------------------------------------------------
// queryByIdent() -- find waypoint record(s) by identifier
//------------------------------------------------------------------------------
int WaypointLoader::queryByIdentImpl(const char* id)
{
   // Search for the waypoint record(s)
   WaypointKey key(id, nullptr);
   Key* pkey = &key;
   return IDatabase::mQuery(&pkey, rl, nrl, il_cmp);
}


//------------------------------------------------------------------------------
// queryByKey() -- find a waypoint record by the waypoint record key
//------------------------------------------------------------------------------
int WaypointLoader::queryByKeyImpl(const char* waypointkey)
{
   WaypointKey key(waypointkey);
   Key* pkey = &key;
   return IDatabase::sQuery(&pkey, rl, nrl, kl_cmp);
}

//------------------------------------------------------------------------------
// qsort and bsearch callbacks
//------------------------------------------------------------------------------
int WaypointLoader::kl_cmp(const void* p1, const void* p2)
{
   const auto k1 = *(static_cast<const WaypointKey**>(const_cast<void*>(p1)));
   const auto k2 = *(static_cast<const WaypointKey**>(const_cast<void*>(p2)));

   // compare the keys
   const int result = std::strcmp(k1->key, k2->key);

   return result;
}

int WaypointLoader::il_cmp(const void* p1, const void* p2)
{
   const auto k1 = *(static_cast<const WaypointKey**>(const_cast<void*>(p1)));
   const auto k2 = *(static_cast<const WaypointKey**>(const_cast<void*>(p2)));

   // compare the ids
   int result = std::strcmp(k1->ident, k2->ident);

   // if equal then check the country codes (only if k1 has one)
   if (result == 0 && k1->countryCode[0] != '\0') {
      result = std::strcmp(k1->countryCode,k2->countryCode);
   }
   return result;
}


//------------------------------------------------------------------------------
// printing functions
//------------------------------------------------------------------------------
void WaypointLoader::printLoadedImpl(std::ostream& sout)
{
   Waypoint waypoint;
   for (int i=0; i < nrl; i++) {
      waypoint.setRecord( db->getRecord( rl[i]->idx ) ) ;
      waypoint.printRecord(sout);
   }
}


void WaypointLoader::printResultsImpl(std::ostream& sout)
{
   Waypoint waypoint;
   for (int i=0; i < nql; i++) {
      waypoint.setRecord( db->getRecord( ql[i]->idx ) ) ;
      waypoint.printRecord(sout);
   }
}


//------------------------------------------------------------------------------
// WaypointLoader::WaypointKey
//------------------------------------------------------------------------------
WaypointLoader::WaypointKey::WaypointKey(const long idx, const Waypoint& waypoint) : Key(idx)
{
   size = WAYPOINT_RECORD_LEN;
   waypoint.icaoCode(icao);
   waypoint.key(key);
   waypoint.ident(ident);
   waypoint.countryCode(countryCode);

   lat = waypoint.latitude();
   lon = waypoint.longitude();
}

WaypointLoader::WaypointKey::WaypointKey(const char* id, const char* ccode) : Key(0)
{
   size = WAYPOINT_RECORD_LEN;
   key[0] = '\0';

   if (id != nullptr)
      base::utStrcpy(ident,WP_IDENT_LEN+1,id);
   else
      ident[0] = '\0';

   if (ccode != nullptr)
      base::utStrcpy(countryCode,WP_CCODE_LEN+1,ccode);
   else
      countryCode[0] = '\0';
}

WaypointLoader::WaypointKey::WaypointKey(const char* key1) : Key(0)
{
   size = WAYPOINT_RECORD_LEN;
   IRecord::dsGetString(key,key1,WP_KEY_LEN);
   IRecord::dsGetString(ident,key1,8);
   IRecord::dsGetString(countryCode,&key1[8],2);
}

WaypointLoader::WaypointKey::~WaypointKey()
{
}

void WaypointLoader::WaypointKey::serialize(std::ostream& sout) const
{
   sout << idx << ": ";
   sout << key << ": ";

   sout << ident << "-";
   sout << countryCode << ": ";

   sout << lat << ", ";
   sout << lon;

   sout << std::endl;
}

}
}
