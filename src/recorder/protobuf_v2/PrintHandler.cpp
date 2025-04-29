
#include "mixr/recorder/protobuf_v2/PrintHandler.hpp"
#include "mixr/base/String.hpp"
#include "mixr/base/util/str_utils.hpp"
#include "mixr/base/util/system_utils.hpp"

#include <cstring>

namespace mixr {
namespace recorder {

IMPLEMENT_SUBCLASS(PrintHandler,"PrintHandler")

BEGIN_SLOTTABLE(PrintHandler)
   "filename",     // 1) Data file name (required)
   "pathname",     // 2) Path to the data file directory (optional)
END_SLOTTABLE(PrintHandler)

BEGIN_SLOT_MAP(PrintHandler)
   ON_SLOT( 1, setSlotFilename, base::String)
   ON_SLOT( 2, setSlotPathName, base::String)
END_SLOT_MAP()

PrintHandler::PrintHandler()
{
   STANDARD_CONSTRUCTOR()
}

void PrintHandler::copyData(const PrintHandler& org, const bool)
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
   firstPassFlg = true;
   fileEmpty = true;
   setFullFilename(nullptr);
}

void PrintHandler::deleteData()
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
// get functions
//------------------------------------------------------------------------------

// Is the data file open?
bool PrintHandler::isOpen() const
{
   return fileOpened && sout != nullptr && sout->is_open();
}

// Did we have an open or write error?
bool PrintHandler::isFailed() const
{
   return fileFailed || (sout != nullptr && sout->fail());
}

// File name with path and possible version number
const char* PrintHandler::getFullFilename() const
{
   return fullFilename;
}

// File name as entered
const char* PrintHandler::getFilename() const
{
   const char* p{};
   if (filename != nullptr) p = filename->c_str();
   return p;
}

// Path to file
const char* PrintHandler::getPathname() const
{
   const char* p{};
   if (pathname != nullptr) p = pathname->c_str();
   return p;
}

// Is the data file empty?
bool PrintHandler::isFileEmpty() const
{
   return fileEmpty;
}

//------------------------------------------------------------------------------
// Open the data file
//------------------------------------------------------------------------------
bool PrintHandler::openFile()
{
   // When we're already open, just return
   if (isOpen()) return true;

   // If we don't have a file name then we're using the standard output
   if (filename == nullptr || filename->len() == 0) return true;

   // clear the old 'full' file name
   setFullFilename(nullptr);

   // local flags (default is success)
   bool tOpened{true};
   bool tFailed{};


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
   // Make sure that it doesn't already exist (we don't want to over write good data).
   //---
   bool validName{!base::doesFileExist(fullname)};
   if ( !validName ) {
      // If the file already exists, try appending a version number "v99" ..

      const auto origname = new char[nameLength];
      base::utStrcpy(origname, nameLength, fullname);

      validName = false;
      for (unsigned int i = 1; i <= 99 && !validName; i++) {
         std::sprintf(fullname, "%s_v%02d", origname, i);
         validName = !base::doesFileExist(fullname);
      }

      if ( !validName ) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "PrintHandler::openFile(): All version of the data file already exists: " << origname << std::endl;
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
      // Open the file
      //---
      sout->open(fullname, std::ios_base::out);

      if (isMessageEnabled(MSG_INFO)) {
         std::cout << "PrintHandler::openFile() Opening data file = " << fullname << std::endl;
      }

      if (sout->fail()) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "PrintHandler::openFile(): Failed to open data file: " << fullname << std::endl;
         }
         tOpened = false;
         tFailed = true;
      }

   }

   delete[] fullname;

   fileOpened = tOpened;
   fileFailed = tFailed;

   return fileOpened;
}


//------------------------------------------------------------------------------
// Close the data file
//------------------------------------------------------------------------------
void PrintHandler::closeFile()
{
   if (isOpen()) {

      // close the file
      if (sout != nullptr) sout->close();

      fileOpened = false;
      fileFailed = false;
   }
}


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

void PrintHandler::setFullFilename(const char* const name)
{
   if (fullFilename != nullptr) {
      delete[] fullFilename;
      fullFilename = nullptr;
   }
   if (name != nullptr) {
      std::size_t n{std::strlen(name) + 1};
      fullFilename = new char[n];
      base::utStrcpy(fullFilename, n, name);
   }
}

bool PrintHandler::setFilename(const base::String* const msg)
{
   if (filename != nullptr) { filename->unref(); filename = nullptr; }
   if (msg != nullptr) filename = new base::String(*msg);

    return true;
}

bool PrintHandler::setPathName(const base::String* const msg)
{
   if (pathname != nullptr) { pathname->unref(); pathname = nullptr; }
   if (msg != nullptr) pathname = new base::String(*msg);

   return true;
}


//------------------------------------------------------------------------------
// print to output stream
//------------------------------------------------------------------------------
void PrintHandler::printToOutput(const char* const msg)
{
   // First pass?  Do we need to open a file?
   if (firstPassFlg) {
      if ( filename != nullptr && !isOpen() && !isFailed() ) {
         openFile();
      }
      firstPassFlg = false;
   }

   // Output to a file?
   if (sout != nullptr && isOpen()) {
      *sout << msg << std::endl;
      fileEmpty = false;
   }

   // Or to the standard output stream
   else {
      std::cout << msg << std::endl;
   }
}

//------------------------------------------------------------------------------
// Print the data from a DataRecord
//------------------------------------------------------------------------------
void PrintHandler::processRecordImp(const DataRecordHandle* const handle)
{
   // baseclass
}

}
}
