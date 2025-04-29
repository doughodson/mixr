
#include "mixr/recorder/FileReader.hpp"
#include "mixr/recorder/protobuf/DataRecord.pb.h"
#include "mixr/recorder/DataRecordHandle.hpp"
#include "mixr/base/String.hpp"
#include "mixr/base/util/str_utils.hpp"
#include "mixr/base/util/system_utils.hpp"

#include <fstream>
#include <cstdlib>

namespace mixr {
namespace recorder {

IMPLEMENT_SUBCLASS(FileReader, "RecorderFileReader")

BEGIN_SLOTTABLE(FileReader)
    "filename",         // 1) Data file name
    "pathname",         // 2) Path to the data file directory (optional)
END_SLOTTABLE(FileReader)

BEGIN_SLOT_MAP(FileReader)
    ON_SLOT( 1, setSlotFilename, base::String)
    ON_SLOT( 2, setSlotPathName, base::String)
END_SLOT_MAP()

FileReader::FileReader()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void FileReader::initData()
{
   ibuf = new char[MAX_INPUT_BUFFER_SIZE];
}

void FileReader::copyData(const FileReader& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   // Need to re-open the file
   if (sin != nullptr) {
      if (isOpen()) sin->close();
      delete sin;
   }
   sin = nullptr;
   setFilename(org.filename);
   setPathName(org.pathname);
   fileOpened = false;
   fileFailed = false;
   firstPassFlg = true;
}

void FileReader::deleteData()
{
   if (sin != nullptr) {
      if (isOpen()) sin->close();
      delete sin;
   }
   sin = nullptr;

   setFilename(nullptr);
   setPathName(nullptr);

   if (ibuf != nullptr) { delete[] ibuf; ibuf = nullptr; }
}

//------------------------------------------------------------------------------
// get functions
//------------------------------------------------------------------------------
bool FileReader::isOpen() const
{
   return fileOpened && sin != nullptr && sin->is_open();
}

bool FileReader::isFailed() const
{
   return fileFailed || (sin != nullptr && sin->fail());
}

//------------------------------------------------------------------------------
// Open the data file
//------------------------------------------------------------------------------
bool FileReader::openFile()
{
   // When we're already open, just return
   if (isOpen()) return true;

   // local flags (default is success)
   bool tOpened{true};
   bool tFailed{};

   // Need a file name
   if (filename == nullptr || filename->len() ==  0) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "FileReader::openFile(): Unable to open data file: no file name" << std::endl;
      }
      tOpened = false;
      tFailed = true;
   }
   else {

      //---
      // Allocate space for the full file name
      //---
      std::size_t nameLength{};
      if (pathname != nullptr) {
         nameLength += pathname->len();     // add the length of the path name
         nameLength += 1;                         // add a character for the slash
      }
      nameLength += filename->len();           // add the length of the file name
      nameLength += 1;                         // Add one for the null(0) at the end of the string

      const auto fullname = new char[nameLength];
      fullname[0] = '\0';

      //---
      // Create the (initial) full file name
      //---
      if (pathname != nullptr && pathname->len() > 0) {
         base::utStrcat(fullname, nameLength , pathname->c_str());
         base::utStrcat(fullname, nameLength, "/");
      }
      base::utStrcat(fullname,nameLength, filename->c_str());

      //---
      // Make sure that it exists
      //---
      bool validName{base::doesFileExist(fullname)};

      //---
      // When we have a valid file name ...
      //---
      if ( validName ) {
         //---
         // Make sure we have an input stream
         //---
         if (sin == nullptr) sin = new std::ifstream();

         //---
         // Open the file (binary input mode)
         //---
         sin->open(fullname, std::ios_base::in | std::ios_base::binary );

         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "FileReader::openFile() Opening data file = " << fullname << std::endl;
         }

         if (sin->fail()) {
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "FileReader::openFile(): Failed to open data file: " << fullname << std::endl;
            }
            tOpened = false;
            tFailed = true;
         }

      }

      delete[] fullname;
   }

   fileOpened = tOpened;
   fileFailed = tFailed;
   return fileOpened;
}


//------------------------------------------------------------------------------
// Close the data file
//------------------------------------------------------------------------------
void FileReader::closeFile()
{
   if (isOpen()) {
      sin->close();
      fileOpened = false;
      fileFailed = false;
   }
}

//------------------------------------------------------------------------------
// Read a record
//------------------------------------------------------------------------------
const DataRecordHandle* FileReader::readRecordImp()
{
   DataRecordHandle* handle{};

   // First pass?  Does the file need to be opened?
   if (firstPassFlg) {
      if ( !isOpen() && !isFailed() ) {
         openFile();
      }
      firstPassFlg = false;
   }


   // When the file is open and ready ...
   if ( isOpen() && !isFailed() && !sin->eof() ) {

      // Number of bytes in the next serialized DataRecord
      unsigned int n{};

      // ---
      // Read the size of the next serialized DataRecord
      // ---
      char nbuff[8]{};
      sin->read(nbuff, 4);

      // Check for error or eof
      if ( sin->eof() || sin->fail() ) {
         fileFailed = sin->fail();
         if (fileFailed && isMessageEnabled(MSG_ERROR | MSG_WARNING)) {
            std::cerr << "FileReader::readRecord() -- error reading data record size" << std::endl;
         }
      }

      // Ok then get the size of the message from the buffer
      else {
         nbuff[4] = '\0';
         n = std::atoi(nbuff);
      }


      // ---
      // Read the serialized DataRecord from the file, parse it as a DataRecord
      // and put it into a Handle.
      // ---
      if (n > 0) {

         // Read message into ibuf
         sin->read(ibuf, n);

         // Check for error or eof
         if ( sin->eof() || sin->fail() ) {
            if (isMessageEnabled(MSG_ERROR | MSG_WARNING)) {
               std::cerr << "FileReader::readRecord() -- error reading data record" << std::endl;
            }
            fileFailed = true;
         }

         // Ok, create the DataRecord with handle
         else {

            // Parse the DataRecord
            std::string wireFormat(ibuf, n);
            auto dataRecord = new pb::DataRecord();
            bool ok{dataRecord->ParseFromString(wireFormat)};

            // Create a handle for the DataRecord (it now has ownership)
            if (ok) {
               handle = new DataRecordHandle(dataRecord);
            }

            // parsing error
            else if (isMessageEnabled(MSG_ERROR | MSG_WARNING)) {
               std::cerr << "FileReader::readRecord() -- ParseFromString() error" << std::endl;
               delete dataRecord;
               dataRecord = nullptr;
            }
         }

      }

   }

   return handle;
}


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool FileReader::setFilename(const base::String* const msg)
{
   if (filename != nullptr) {
      filename->unref();
      filename = nullptr;
   }
   if (msg != nullptr)
      filename = new base::String(*msg);

   return true;
}

bool FileReader::setPathName(const base::String* const msg)
{
   if (pathname != nullptr) {
      pathname->unref();
      pathname = nullptr;
   }
   if (msg != nullptr)
      pathname = new base::String(*msg);

   return true;
}

}
}
