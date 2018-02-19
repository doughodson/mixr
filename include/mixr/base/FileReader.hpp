
#ifndef __mixr_base_FileReader_H__
#define __mixr_base_FileReader_H__

#include "mixr/base/Object.hpp"

#include <string>

namespace mixr {
namespace base {
class Integer;
class String;

//------------------------------------------------------------------------------
// Class: FileReader
// Description: Manages the reading of fixed record length files
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: FileReader
// Slots:
//     pathname      <String>      ! Path to the file (default: "")
//     filename      <String>      ! File name (appended to pathname) (default: "")
//     recordLength  <Integer>     ! Length (in characters) of the records (default: 0)
//------------------------------------------------------------------------------
// Notes:
//
//  1) Record numbers range from 1 to the number of records in the file.
//
//  2) getRecord(), getFirstRecord() and getNextRecord() return 0
//     for invalid record numbers (e.g., beyond end of file).
//
//  3) The 'const char*' returned from getRecord(), getFirstRecord(),
//     and getNextRecord() point to an internal buffer which is only
//     valid until the next read.
//
//  4) The file name and path names are limited to 255 characters.
//------------------------------------------------------------------------------
class FileReader : public Object
{
   DECLARE_SUBCLASS(FileReader, Object)

public:
   FileReader();

   // Opens 'filename' in directory 'pathname' with 'recordLength' characters per record.
   // If 'recordLength' is not provided, it must be set using setRecordLength() before
   // the first access. Returns true of all is well.
   bool open();
   // Returns true if the file is open and ready to use.
   bool isReady();

   // Sets the path name to the file.  If provided, this string is used
   // as a prefix to file name.
   bool setPathname(const std::string&);
   const std::string& getPathname() const        { return pathname; }

   // Sets the name of the file to be read.  If the path name is provided,
   // the file name is appended to the path name.
   bool setFilename(const std::string&);
   const std::string& getFilename() const        { return filename; }

   // Sets the current record length to 'len' and returns true if all is well.
   bool setRecordLength(const int len);
   // Returns the current record length.
   int getRecordLength() const            { return rlen; }

   // Sets the current record number to 'num' and returns true if all is well.
   bool setRecordNumber(const int num);
   // Returns the current record number.
   int getRecordNumber() const            { return rnum; }

   // Reads 'len' characters starting at record number 'num'.
   // 'Len' can be up to 4 times recordLength().
   const char* getRecord(const int n = -1, const int len = 0);

   // Decrements the current record by 'n' or 1.
   void backRecord(const int n = 1);
   // Increments the current record by 'n' or 1.
   void skipRecord(const int n = 1);

   // Reads the first/next record.
   const char* getFirstRecord();
   const char* getNextRecord();

private:
   std::ifstream* dbf{};

   int   rnum {1};         // record number
   int   crnum {-1};       // current (in memory) record number
   int   rlen {};          // record length
   char* rec {};           // the (in memory) record

   std::string filename;
   std::string pathname;

private:
   // slot table helper methods
   bool setSlotPathname(String* const);
   bool setSlotFilename(String* const);
   bool setSlotRecordLength(const Integer* const);
};

inline bool FileReader::setRecordNumber(const int num)
{
   rnum = num;
   return true;
}

inline void FileReader::backRecord(const int n)
{
   int i{rnum - n};
   if (i >= 0) rnum = i;
}

inline void FileReader::skipRecord(const int n)
{
   rnum += n;
}

inline const char* FileReader::getNextRecord()
{
   rnum++;
   return getRecord();
}

inline const char* FileReader::getFirstRecord()
{
   rnum = 1;
   return getRecord();
}

}
}

#endif
