
#include "mixr/recorder/FileWriter.hpp"
#include "mixr/recorder/protobuf/DataRecord.pb.h"
#include "mixr/recorder/DataRecordHandle.hpp"
#include "mixr/base/String.hpp"
#include "mixr/base/util/str_utils.hpp"
#include "mixr/base/util/system_utils.hpp"

#include <fstream>
#include <cstring>

namespace mixr {
namespace recorder {

IMPLEMENT_SUBCLASS(FileWriter, "RecorderFileWriter")

BEGIN_SLOTTABLE(FileWriter)
    "filename",         // 1) Data file name (required)
    "pathname",         // 2) Path to the data file directory (optional)
END_SLOTTABLE(FileWriter)

BEGIN_SLOT_MAP(FileWriter)
    ON_SLOT( 1, setSlotFilename, base::String)
    ON_SLOT( 2, setSlotPathName, base::String)
END_SLOT_MAP()

FileWriter::FileWriter()
{
   STANDARD_CONSTRUCTOR()
}

void FileWriter::copyData(const FileWriter& org, const bool)
{
   BaseClass::copyData(org);

   setFilename(org.filename);
   setPathName(org.pathname);

   // Need to re-open the file
   if (sout != nullptr) {
      if (isOpen()) sout->close();
      delete sout;
   }
   sout = nullptr;
   fileOpened = false;
   fileFailed = false;
   eodFlag    = false;
   setFullFilename(nullptr);
}

void FileWriter::deleteData()
{
   if (sout != nullptr) {
      if (isOpen()) sout->close();
      delete sout;
   }
   sout = nullptr;

   setFilename(nullptr);
   setPathName(nullptr);
}


//------------------------------------------------------------------------------
// shutdownNotification() -- Shutdown the simulation
//------------------------------------------------------------------------------
bool FileWriter::shutdownNotification()
{
   // Close the file, if it's still open
   if (isOpen()) closeFile();

   return BaseClass::shutdownNotification();
}


//------------------------------------------------------------------------------
// get functions
//------------------------------------------------------------------------------

// Is the data file open?
bool FileWriter::isOpen() const
{
   return fileOpened && sout != nullptr && sout->is_open();
}

// Did we have an open or write error?
bool FileWriter::isFailed() const
{
   return fileFailed || (sout != nullptr && sout->fail());
}

// File name with path and possible version number
const char* FileWriter::getFullFilename() const
{
   return fullFilename;
}

// File name as entered
const char* FileWriter::getFilename() const
{
   const char* p{};
   if (filename != nullptr) p = filename->c_str();
   return p;
}

// Path to file
const char* FileWriter::getPathname() const
{
   const char* p{};
   if (pathname != nullptr) p = pathname->c_str();
   return p;
}


//------------------------------------------------------------------------------
// Open the data file
//------------------------------------------------------------------------------
bool FileWriter::openFile()
{
   // When we're already open, just return
   if (isOpen()) return true;

   // clear the old 'full' file name
   setFullFilename(nullptr);

   // local flags (default is success)
   bool tOpened{true};
   bool tFailed{};
   eodFlag = false;

   // Need a file name
   if (filename == nullptr || filename->len() ==  0) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "FileWriter::openFile(): Unable to open data file: no file name" << std::endl;
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
      nameLength += 4;                         // add characters for possible version number, "_V99"
      nameLength += 1;                         // Add one for the null(0) at the end of the string

      const auto fullname{new char[nameLength]};
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
      // Make sure that it doesn't already exist (we don't want to over write good data).
      //---
      bool validName{!base::doesFileExist(fullname)};
      if ( !validName ) {
         // If the file already exists, try appending a version number "v99" ..

         const auto origname{new char[nameLength]};
         base::utStrcpy(origname, nameLength, fullname);

         validName = false;
         for (int i = 1; i <= 99 && !validName; i++) {
            std::sprintf(fullname, "%s_v%02d", origname, i);
            validName = !base::doesFileExist(fullname);
         }

         if ( !validName ) {
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "FileWriter::openFile(): All version of the data file already exists: " << origname << std::endl;
            }
            tOpened = false;
            tFailed = true;
         }
         delete[] origname;
      }

      //---
      // When we have a valid file name ...
      //---
      if ( validName ) {

         // The file name with the path and version number
         setFullFilename(fullname);

         //---
         // Make sure we have an output stream
         //---
         if (sout == nullptr) sout = new std::ofstream();

         //---
         // Open the file (binary output mode)
         //---
         sout->open(fullname, std::ios_base::out | std::ios_base::binary );

         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "FileWriter::openFile() Opening data file = " << fullname << std::endl;
         }

         if (sout->fail()) {
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "FileWriter::openFile(): Failed to open data file: " << fullname << std::endl;
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
void FileWriter::closeFile()
{
   if (isOpen()) {

      // ---
      // Write the EOD message, if it hasn't already been.
      if (!eodFlag) {
         eodFlag = true;

         // write something to signify don't read any more (e.g., last message)
         const auto lastMsg = new pb::DataRecord();

         // This will be the token representing the last message, but it can be
         // anything that is not one of the other event messages
         lastMsg->set_id(REID_END_OF_DATA);

         // Time is also required, although not used:
         pb::Time* time {lastMsg->mutable_time()};
         time->set_exec_time(0);
         time->set_sim_time(0);
         time->set_utc_time(0);

         // get a handle
         auto handle{new DataRecordHandle(lastMsg)};

         // write the message
         processRecordImp(handle);
         handle->unref();
         handle = nullptr;
      }

      // now close the file
      sout->close();
      fileOpened = false;
      fileFailed = false;

   }
}


//------------------------------------------------------------------------------
// Serialize and write a DataRecord to a file
//------------------------------------------------------------------------------
void FileWriter::processRecordImp(const DataRecordHandle* const handle)
{
   bool thisIsEodMsg{};

   // ---
   // Open the file, if it hasn't been already ...
   // ---
   if ( !fileOpened && !fileFailed ) openFile();

   // ---
   // When the file is open and ready ...
   // --- serialize and write the data record.
   // ---
   if ( fileOpened ) {

      // The DataRecord to be sent
      const pb::DataRecord* dataRecord{handle->getRecord()};

      // Serialize the DataRecord
      std::string wireFormat;
	   bool ok{dataRecord->SerializeToString(&wireFormat)};

      // Write the serialized DataRecord with its length to the file
      if (ok) {
		  int n{static_cast<int>(wireFormat.length())};

         // Convert size to an integer string
         char nbuff[8];
         std::sprintf(nbuff, "%04d", n);

         // Convert the leading zeros to spaces
         if (nbuff[0] == '0') {
            nbuff[0] = ' ';
            if (nbuff[1] == '0') {
               nbuff[1] = ' ';
               if (nbuff[2] == '0') {
                  nbuff[2] = ' ';
               }
            }
         }

         // Write the size of the serialized DataRecord as an ascii string
         sout->write(nbuff, 4);

         // Write the serialized DataRecord
         sout->write( wireFormat.c_str(), n );
      }

      else if (isMessageEnabled(MSG_ERROR | MSG_WARNING)) {
         // If we had an error serializing the DataRecord
         std::cerr << "FileWriter::processRecordImp() -- SerializeToString() error" << std::endl;
      }

      // Check for END_OF_DATA message
      thisIsEodMsg = (dataRecord->id() == REID_END_OF_DATA);

   }

   // ---
   // Close the file at END_OF_DATA message
   // ---
   if (!eodFlag && thisIsEodMsg) {
      eodFlag = true;
      closeFile();
   }
}


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

void FileWriter::setFullFilename(const char* const name)
{
   if (fullFilename != nullptr) {
      delete[] fullFilename;
      fullFilename = nullptr;
   }
   if (name != nullptr) {
      std::size_t n = std::strlen(name) + 1;
      fullFilename = new char[n];
      base::utStrcpy(fullFilename, n, name);
   }
}

bool FileWriter::setFilename(const base::String* const msg)
{
   if (filename != nullptr) { filename->unref(); filename = nullptr; }
   if (msg != nullptr) filename = new base::String(*msg);

   return true;
}

bool FileWriter::setPathName(const base::String* const msg)
{
   if (pathname != nullptr) { pathname->unref(); pathname = nullptr; }
   if (msg != nullptr) pathname = new base::String(*msg);

   return true;
}

}
}
