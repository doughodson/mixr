
#ifndef __mixr_map_rpf_CadrgFrame_HPP__
#define __mixr_map_rpf_CadrgFrame_HPP__

#include "mixr/base/IObject.hpp"
#include "mixr/map/rpf/map_utils.hpp"

namespace mixr {
namespace rpf {

class CadrgFrameEntry;

//------------------------------------------------------------------------------
// Class: CadrgFrame
//
// Description: One frame of a TocEntry, holds the color table information for the
// texture. Load a frame's data and decompress it.
//
// load() - Load a frame's data.
//        virtual void load(CadrgFrameEntry* entry);
//
// decompressSubframe() - Take our frame and decompress the subframe image
//        virtual int decompressSubframe(const int x, const int y, Subframe& subFrame);
//
//------------------------------------------------------------------------------
class CadrgFrame : public base::IObject
{
    DECLARE_SUBCLASS(CadrgFrame, base::IObject)

public:
    CadrgFrame();

    // Load our entry for each frame
    virtual void load(CadrgFrameEntry* entry);
    // Decompress our subframe
    virtual int decompressSubframe(const int x, const int y, Subframe& subFrame);

private:
    static const int frameSize = 6144;                 // Total frame size
    CadrgFrameEntry* frameEntry {};                    // Pointer to our frame entry parent
    unsigned char subFrameTable[6][6][frameSize] {};   // Subframe table array
    int nitfHdrLength {};                              // Nitf header length
    int masked[6][6] {};                               // Subframe masked array
    unsigned char lookupTable[4096][4][4] {};          // Lookup table
};

}
}

#endif

