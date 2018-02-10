
#include "mixr/base/FileReader.hpp"

#include "mixr/base/String.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/numeric/Integer.hpp"

#include <fstream>
#include <string>
#include <iostream>

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
    ON_SLOT(3, setSlotRecordLength, Integer)
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

   pathname = org.pathname;
   filename = org.filename;

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

// Returns true we're ready to get records
bool FileReader::isReady()
{
   bool ready{};
   if (dbf != nullptr && rec != nullptr) {
      if (rlen > 0 && dbf->is_open()) ready = true;
   }
   return ready;
}

// sets the path name
bool FileReader::setPathname(const std::string& path)
{
   pathname = path;
   return true;
}

// sets the file name
bool FileReader::setFilename(const std::string& file)
{
   filename = file;
   return true;
}

// sets the record length & creates the internal buffer
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

bool FileReader::setSlotPathname(String* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setPathname( (*msg).c_str() );
   }
   return ok;
}

bool FileReader::setSlotFilename(String* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setFilename( (*msg).c_str() );
   }
   return ok;
}

bool FileReader::setSlotRecordLength(const Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setRecordLength( msg->asInt() );
   }
   return ok;
}

bool FileReader::open()
{
   // Create the full file name
   std::string file = pathname + "/" + filename;

   // Open the file ...
   if (dbf != nullptr) {
      // close any previous files
      dbf->close();
   } else {
      // Create the input stream
      dbf = new std::ifstream();
   }

   std::cout << "File being opened\n";
   std::cout << "filename : " << file << std::endl;

   dbf->open(file);
   dbf->clear();

   rnum = 1;
   crnum = -1;
   return (dbf->is_open());
}

const char* FileReader::getRecord(const int nn, const int ll)
{
   // return nothing if we're not ready (e.g., the record length has not been set)
   if ( !isReady() ) return nullptr;

   // Set record number
   int n{nn};
   if (n == -1) n = rnum;
   else rnum = n;

   // Set length to read
   int len{ll};
   if (len == 0) len = rlen;

   // Read the record
   bool ok{};
   if (!dbf->seekg(rlen*(n-1), std::ios::beg).eof()) {
      dbf->read(rec, len);
      if (!dbf->eof() && !dbf->fail()) ok = true;
   }

   if (ok) {
      crnum = rnum;
      rec[len] = '\0';
      return rec;
   } else {
      dbf->clear();
      crnum = -1;
      return nullptr;
   }

}

}
}
