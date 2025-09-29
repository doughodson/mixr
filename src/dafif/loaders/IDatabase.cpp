
#include "mixr/dafif/loaders/IDatabase.hpp"

#include "mixr/dafif/records/IRecord.hpp"

#include "mixr/base/FileReader.hpp"
#include "mixr/base/String.hpp"
#include "mixr/base/qty/util/angle_utils.hpp"
#include "mixr/base/qty/lengths.hpp"

#include <cstring>
#include <cmath>
#include <cstdlib>

namespace mixr {
namespace dafif {

IMPLEMENT_ABSTRACT_SUBCLASS(IDatabase, "AbstractDatabase")

BEGIN_SLOTTABLE(IDatabase)
    "pathname",      // 1) Path to the file
    "filename",      // 2) File name (appended to pathname)
END_SLOTTABLE(IDatabase)

BEGIN_SLOT_MAP(IDatabase)
    ON_SLOT(1, setSlotPathname, base::String)
    ON_SLOT(2, setSlotFilename, base::String)
END_SLOT_MAP()

IDatabase::IDatabase()
{
   STANDARD_CONSTRUCTOR()
   db = new base::FileReader();
}

void IDatabase::copyData(const IDatabase& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) {
      db = new base::FileReader();
   }

   ncache = 0;
   rl = nullptr;
   nrl = 0;

   ol = nullptr;
   nol = 0;

   ql = nullptr;
   nql = 0;
   qlimit = 0;

   refLat = 0.0;
   refLon = 0.0;
   coslat = 1.0;
   mrng = 0.0;
   dbInUse = false;
   dbLoaded = false;
}

void IDatabase::deleteData()
{
   if (db != nullptr) {
      db->unref();
      db = nullptr;
   }
   dbInUse = false;
   dbLoaded = false;
}

// creates and opens the database file
bool IDatabase::openDatabaseFile()
{
   // Close any old files and set the record length
   db->setRecordLength(getRecordLength());

   // create keys
   ncache = getMaxRecords();
   if (rl != nullptr) delete[] rl;
   rl = new Key*[ncache];
   if (isMessageEnabled(MSG_DEBUG)) {
      std::cout << "db = " << this << ", ncache = " << ncache << std::endl;
   }

   // open the database file
   db->open();
   return db->isReady();
}

//------------------------------------------------------------------------------
// getArea(), setArea() -- get/set the search area (ref point)
//------------------------------------------------------------------------------
void IDatabase::getArea(double* const lat, double* const lon)
{
   *lat = refLat;
   *lon = refLon;
}

void IDatabase::getArea(double* const lat, double* const lon, double* const mr)
{
   *lat = refLat;
   *lon = refLon;
   *mr  = mrng;
}

void IDatabase::setArea(const double lat, const double lon, const double mr)
{
   refLat = lat;
   refLon = lon;
   coslat = std::cos(lat * base::angle::D2RCC);
   mrng   = mr;
}

// get/set query limit
void IDatabase::setQueryLimit(const int mrec)
{
   qlimit = mrec;
}

int IDatabase::getQueryLimit()
{
   return qlimit;
}

// returns the number of records in this database
int IDatabase::numberOfRecords()
{
   return nrl;
}

// returns the number of records found by last query
int IDatabase::numberFound()
{
   return nql;
}

//------------------------------------------------------------------------------
// range2() -- returns the square of the range between lat,lon and
// the reference point.
//------------------------------------------------------------------------------
double IDatabase::range2(const double lat, const double lon) const
{
   const double x{(lat - refLat) * 60.0};
   const double y{(lon - refLon) * coslat * 60.0};
   return (x * x + y * y);
}

//------------------------------------------------------------------------------
// Database access functions -- The strings are only valid until the next
// access.  The user should make a copy of the string if the record is to
// be saved.
//------------------------------------------------------------------------------
const char* IDatabase::record(const int n, const int size)
{
   const char* p{};
   if (n >= 0 && n < nrl) {
      p = dbGetRecord( rl[n], size );
   }
   return p;
}

const char* IDatabase::getRecord(const int n, const int size)
{
   const char* p = nullptr;
   if (n >= 0 && n < nql) {
      p = dbGetRecord( ql[n], size );
   }
   return p;
}


// dbGetRecord() -- get record from the database
const char* IDatabase::dbGetRecord(const Key* key, const int size)
{
   const char* p{};

   // size of record to read
   int ssize{key->size};
   if (size != 0) ssize = size;

#ifndef ALT_ILS_FILE

   p = db->getRecord( key->idx, ssize );

#else    /* Using Alternate ILS file */

   if (key->idx != ALT_ILS_IDX || ssize != ILS_RECORD_LEN) {
      p = db->getRecord( key->idx, ssize );
   } else {
      // must be an ILS record from an Airport loader
      DsAirportLoader* me = (DsAirportLoader*) this;
      p = me->createIlsRecord(key);
   }

#endif

   return p;
}

// get the path name from the database file reader
const std::string& IDatabase::getPathname() const
{
   return db->getPathname();
}

// get the file name from the database file reader
const std::string& IDatabase::getFilename() const
{
   return db->getFilename();
}

// set the pathname for the database file reader
bool IDatabase::setPathname(const std::string& path)
{
   return db->setPathname(path);
}

// set the pathname for the database file reader
bool IDatabase::setFilename(const std::string& file)
{
   return db->setFilename(file);
}

//------------------------------------------------------------------------------
// stripSpaces() -- strips spaces off end of the string
//------------------------------------------------------------------------------
void IDatabase::stripSpaces(char buff[], const int n)
{
   int l{n-1};
   while (l >= 0 && buff[l] == ' ') buff[l--] = '\0';
}

//------------------------------------------------------------------------------
// fillSpaces() -- add spaces to the end of the string to make it n characters
// long plus a null character.
//------------------------------------------------------------------------------
void IDatabase::fillSpaces(char buff[], const int n)
{
   for (int i = static_cast<int>(std::strlen(buff)); i < n; buff[i++] = ' ');
   buff[n] = '\0';
}

//------------------------------------------------------------------------------
// createIcaoList() -- creates a list in ICAO code order from the record list.
//------------------------------------------------------------------------------
void IDatabase::createIcaoList()
{
   nol = 0;
   if (nrl > 0) {

      // allocate space for the ICAO list pointers
      ol = new Key*[nrl];

      // copy keys with a valid ICAO code to the ICAO list and sort
      for (int i = 0; i < nrl; i++) {
         if ( std::strlen(rl[i]->icao) > 2 ) {       // at least three characters
            ol[nol++] = rl[i];
         }
      }

      // sort the ICAO list keys
      std::qsort(ol,nol,sizeof(Key*),ol_cmp);
   }
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------
bool IDatabase::setSlotPathname(const base::String* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setPathname( (*msg).c_str() );
   }
   return ok;
}

bool IDatabase::setSlotFilename(const base::String* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setFilename( (*msg).c_str() );
   }
   return ok;
}


//------------------------------------------------------------------------------
// query functions ---
//------------------------------------------------------------------------------

// Single query -- when you want only a single instance of a record that
// matches the key.
int IDatabase::sQuery(Key** key, Key** base,
                            std::size_t n, int (*cmp)(const void*, const void*))
{
   nql = 0;
   // search the record list for matches and sort by range
   if (n != 0) {
      Key** k{static_cast<Key**>(bsearch(key, base, n, sizeof(Key*), cmp))};
      if (k != nullptr) {
         ql[0] = *k;
         nql = 1;
      }
   }

   return nql;
}


// Multi-query -- when you want the full range of records that match the key.
int IDatabase::mQuery(Key** key, Key** base,
                            std::size_t n, int (*cmp)(const void*, const void*))
{
   nql = 0;

   // search the record list for matches and sort by range
   if (n != 0) {
      Key** k{static_cast<Key**>(bsearch(key, base, n, sizeof(Key*), cmp))};
      if (k != nullptr) {
         expandResults(key,k,cmp,base,n);
      }
   }

   // sort by range from ref point
   if (nql > 1) rangeSort();

   // limit results
   if (qlimit > 0 && nql > qlimit) nql = qlimit;

   return nql;
}


//------------------------------------------------------------------------------
// queryByIcao() -- find a record by the ICAO code
//------------------------------------------------------------------------------
int IDatabase::queryByIcao(const char* code)
{
   Key key(code);
   Key* pkey{&key};
   return mQuery(&pkey, static_cast<Key**>(ol), nol, ol_cmp);
}


//------------------------------------------------------------------------------
// expandResults() -- The record we found by the last bsearch, keyPtr, may
// have found a record somewhere in the middle of a set that matched our
// search pattern; this function looks above and below 'keyPtr' for more
// records and put the final results in 'ql'.
//------------------------------------------------------------------------------
void IDatabase::expandResults(Key** key, Key** keyPtr,
                                    int (*cmp)(const void*, const void*),
                                    Key** base, std::size_t n)
{
   // Look for the bottom end
   Key** b{keyPtr - 1};
   int rr{};
   while (rr == 0 && b >= base) {
      rr = (*cmp)(key,b);
      if (rr == 0) --b;
   }
   b++;

   // Look for the top end
   Key** t{keyPtr + 1};
   Key** vt{base + n};
   rr = 0;
   while (rr == 0 && t < vt) {
      rr = (*cmp)(key,t);
      if (rr == 0) t++;
   }
   --t;

   // move the results to ql
   nql = 0;
   for (Key** p = b; p <= t; p++) {
      ql[nql++] = *p;
   }
}


//------------------------------------------------------------------------------
// rangeSort() -- first compute range and then uses rangeSort2() to sort.
// rangeSort2() -- sort results by range; range must have already been computed.
//------------------------------------------------------------------------------

// rangeSort2: sort results by range; range must have already been computed.
int IDatabase::rangeSort2()
{
   // if more than one key, sort them by range
   if (nql > 1) std::qsort(ql, nql, sizeof(Key*), rlqs);

   // reject records with a range greater than mrng
   if (mrng > 0.0f) {
      const double mr2 = mrng*mrng;
      int done = 0;
      while (nql > 0 && !done) {
         Key* k = ql[nql-1];
         if (k->rng2 > mr2) nql--;
         else done = true;
      }
   }

   return nql;
}

// rangeSort: first compute range and then uses rangeSort2() to sort.
int IDatabase::rangeSort()
{
   // compute ranges
   for (int i = 0; i < nql; i++) {
      Key* k = ql[i];
      k->rng2 = range2(k->lat,k->lon);
   }

   return rangeSort2();
}


//------------------------------------------------------------------------------
// bsearch and qsort callbacks
//------------------------------------------------------------------------------
int IDatabase::rlqs(const void* p1, const void* p2)
{
   const auto k1 = *(static_cast<const Key**>(const_cast<void*>(p1)));
   const auto k2 = *(static_cast<const Key**>(const_cast<void*>(p2)));

   int result{};
   if (k1->rng2 < k2->rng2)      result = -1;
   else if (k1->rng2 > k2->rng2) result =  1;

   return result;
}

int IDatabase::ol_cmp(const void* p1, const void* p2)
{
   const auto k1 = *(static_cast<Key**>(const_cast<void*>(p1)));
   const auto k2 = *(static_cast<Key**>(const_cast<void*>(p2)));

   return std::strcmp(k1->icao,k2->icao);
}


//------------------------------------------------------------------------------
// printing functions
//------------------------------------------------------------------------------
void IDatabase::printLoadedImpl(std::ostream& sout)
{
   for (int i=0; i < nrl; i++) {
      rl[i]->serialize(sout);
   }
}

void IDatabase::printResultsImpl(std::ostream& sout)
{
   for (int i=0; i < nql; i++) {
      ql[i]->serialize(sout);
   }
}

void IDatabase::printIcaoList(std::ostream& sout)
{
   std::cout << "printIcaoList: nol = " << nol << std::endl;
   for (int i = 0; i < nol; i++) {
      ol[i]->serialize(sout);
   }
}

//------------------------------------------------------------------------------
// Database::Key
//------------------------------------------------------------------------------
IDatabase::Key::Key(const int idx1): idx(idx1)
{
}

IDatabase::Key::Key(const float lat1, const float lon1): lat(lat1), lon(lon1)
{
}

IDatabase::Key::Key(const char* code)
{
   IRecord::dsGetString(icao,code,ICAO_CODE_LEN);
}

IDatabase::Key::~Key()
{
}

void IDatabase::Key::serialize(std::ostream& sout) const
{
   sout << idx << ": ";

   sout << icao << ": ";

   sout << lat << ", ";
   sout << lon << "  ";

   sout << rng2;

   sout << std::endl;
}

}
}
