
#ifndef __mixr_recorder_FileReader_HPP__
#define __mixr_recorder_FileReader_HPP__

#include "mixr/recorder/protobuf_v2/IInputHandler.hpp"

namespace mixr {
namespace base { class String; }
namespace recorder {
namespace protobuf_v2 {

//------------------------------------------------------------------------------
// Class: FileReader
// Description: Read and parse data records from a file
//
// Factory name: FileReader
// Slots:
//     filename       <String>     ! Data file name (required)
//     pathname       <String>     ! Path to the data file's directory (optional)
//
// Notes
//    1) The data file consists of a sequence of serialized data records
//    that are preceded by 4 bytes that provided the size of each data record
//    in bytes.  The 4 bytes are stored as an ascii string with leading spaces
//    (e.g., " 123")
//------------------------------------------------------------------------------
class FileReader : public IInputHandler
{
    DECLARE_SUBCLASS(FileReader, IInputHandler)

public:
   static const unsigned int MAX_INPUT_BUFFER_SIZE = 2000;

public:
   FileReader();

   bool isOpen() const;             // Is the data file open?
   bool isFailed() const;           // Did we have an open or read error?

   virtual bool openFile();         // Open the data file
   virtual void closeFile();        // Close the data file

   // File and path names; set before calling openFile()
   virtual bool setFilename(const base::String* const);
   virtual bool setPathName(const base::String* const);

protected:
   const DataRecordHandle* readRecordImp() override;

private:
   void initData();

   char* ibuf {};                    // Input data buffer

   std::ifstream* sin {};            // Input stream
   const base::String* filename {};  // File name
   const base::String* pathname {};  // Path to the data file's directory
   bool fileOpened {};               // File opened
   bool fileFailed {};               // Open or read failed
   bool firstPassFlg {true};         // First pass flag

private:
   // slot table helper methods
   bool setSlotFilename(const base::String* const x)                 { return setFilename(x); }
   bool setSlotPathName(const base::String* const x)                 { return setPathName(x); }
};

}
}
}

#endif
