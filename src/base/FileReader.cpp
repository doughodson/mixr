
#include <fstream>
#include <cstring>

#include "mixr/base/FileReader.hpp"

#include "mixr/base/String.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/util/str_utils.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(FileReader, "FileReader")

BEGIN_SLOTTABLE(FileReader)
    "pathname",      // 1) Path to the file
    "filename",      // 2) File name (appended to pathname)
    "recordLength",  // 3) Length (in characters) of the records
END_SLOTTABLE(FileReader)

BEGIN_SLOT_MAP(FileReader)
    ON_SLOT(1, setSlotPathname,     String)
    ON_SLOT(2, setSlotFilename,     String)
    ON_SLOT(3, setSlotRecordLength, Number)
END_SLOT_MAP()

FileReader::FileReader()
{
   STANDARD_CONSTRUCTOR()
}

void FileReader::copyData(const FileReader& org, const bool)
{
   BaseClass::copyData(org);

   // Close the old file (we'll need to open() the new one)
   if (dbf != nullptr) dbf->close();

   utStrcpy(pathname, PATHNAME_LENGTH, org.pathname);
   utStrcpy(filename, FILENAME_LENGTH, org.filename);

   rnum = org.rnum;
   crnum = org.crnum;

   setRecordLength(org.getRecordLength());
}

void FileReader::deleteData()
{
   // Close file and delete stream
   if (dbf != nullptr) {
      dbf->close();
      delete dbf;
      dbf = nullptr;
   }

   // Delete the record buffer
   if (rec != nullptr) {
      delete[] rec;
      rec = nullptr;
   }
}

//------------------------------------------------------------------------------
// isReady() -- Returns true we're ready to get records
//------------------------------------------------------------------------------
bool FileReader::isReady()
{
   bool ready{};
   if (dbf != nullptr && rec != nullptr) {
      if (rlen > 0 && dbf->is_open()) ready = true;
   }
   return ready;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// setPathname() -- sets the path name (limit to 255 chars)
bool FileReader::setPathname(const char* const path)
{
   bool ok{};
   if (path != nullptr) {
      utStrncpy(pathname, (PATHNAME_LENGTH-1), path, (PATHNAME_LENGTH-1));
      pathname[PATHNAME_LENGTH-1] = '\0';
      ok = true;
   }
   return ok;
}

// setFilename() -- sets the file name (limit to 255 chars)
bool FileReader::setFilename(const char* const file)
{
   bool ok{};
   if (file != nullptr) {
      utStrncpy(filename, (FILENAME_LENGTH-1), file, (FILENAME_LENGTH-1));
      filename[FILENAME_LENGTH-1] = '\0';
      ok = true;
   }
   return ok;
}

// setRecordLength() -- sets the record length & creates the internal buffer
bool FileReader::setRecordLength(const int len)
{
   if (rlen != len) {
      if (rec != nullptr) {
         delete[] rec;
      }
      rlen = len;
      int tlen{rlen*4};         // allow space for 4 times the record length
      rec = new char[tlen+1];
      rec[0] = '\0';
   }
   return true;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------
bool FileReader::setSlotPathname(String* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setPathname( *msg );
   }
   return ok;
}

bool FileReader::setSlotFilename(String* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setFilename( *msg );
   }
   return ok;
}

bool FileReader::setSlotRecordLength(const Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setRecordLength( msg->getInt() );
   }
   return ok;
}

//------------------------------------------------------------------------------
// Open
//------------------------------------------------------------------------------
bool FileReader::open()
{
   // Create the full file name
   const std::size_t FILE_LENGTH {512}; // Max length of file
   char file[FILE_LENGTH] {};
   utStrcpy(file, FILE_LENGTH, pathname);
   utStrcat(file, FILE_LENGTH, "/");
   utStrcat(file, FILE_LENGTH, filename);

   // Open the file ...
   if (dbf != nullptr) {
      // close any previous files
      dbf->close();
   } else {
      // Create the input stream
      dbf = new std::ifstream();
   }
   dbf->open(file);
   dbf->clear();

   rnum = 1;
   crnum = -1;
   return (dbf->is_open());
}


//------------------------------------------------------------------------------
// getRecord() --
//------------------------------------------------------------------------------
const char* FileReader::getRecord(const int nn, const int ll)
{
   // return nothing if we're not ready (e.g., the record length has not been set)
   if ( !isReady() ) return nullptr;

   // Set record number
   int n {nn};
   if (n == -1) n = rnum;
   else rnum = n;

   // Set length to read
   int len {ll};
   if (len == 0) len = rlen;

   // Read the record
   bool ok {};
   if (!dbf->seekg(rlen*(n-1), std::ios::beg).eof()) {
      dbf->read(rec, len);
      if (!dbf->eof() && !dbf->fail()) ok = true;
   }

   if (ok) {
      crnum = rnum;
      rec[len] = '\0';
      return rec;
   }
   else {
      dbf->clear();
      crnum = -1;
      return nullptr;
   }

}

}
}
