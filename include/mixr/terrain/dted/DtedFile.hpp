
#ifndef __mixr_terrain_DtedFile_HPP__
#define __mixr_terrain_DtedFile_HPP__

#include "../DataFile.hpp"

namespace mixr {
namespace base { class Boolean; }
namespace terrain {

//------------------------------------------------------------------------------
// Class: DtedFile
//
// Description: DTED data loader.
//
// Factory name: DtedFile
//------------------------------------------------------------------------------
class DtedFile : public DataFile
{
   DECLARE_SUBCLASS(DtedFile, DataFile)

public:
   DtedFile();

protected:
   bool isVerifyChecksum() const { return verifyChecksum; }

private:
   // Interpret signed-magnitude values from DTED file
   static short readValue(const unsigned char hbyte, const unsigned char lbyte);
   static long readValue(const unsigned char hbyte, const unsigned char byte1, const unsigned char byte2, const unsigned char lbyte);

   // Read in cell parameters from DTED headers
   bool readDtedHeaders(std::istream&);
   bool readDtedData(std::istream&);

   bool loadData() override;

   bool verifyChecksum {true};    // verify the file checksum flag

private:
   // slot table helper methods
   bool setSlotVerifyChecksum(const base::Boolean* const);
};


}
}

#endif
