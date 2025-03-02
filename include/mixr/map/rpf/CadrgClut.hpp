
#ifndef __mixr_map_rpf_CadrgClut_HPP__
#define __mixr_map_rpf_CadrgClut_HPP__

#include "mixr/base/IObject.hpp"

#include <array>

namespace mixr {
namespace rpf {
class CadrgFrameEntry;

//------------------------------------------------------------------------------
// Class: CadrgClut
//
// Description: Used by CadrgFile, this is a Color Lookup Table (Clut), which loads a
// CadrgFrameEntry with its proper color table
//
// Load our color table
//     void load(CadrgFrameEntry& frame, int cib, ColorTableSizes clutSize = CLUT_216);
// Get a color from the color table at a particular index
//     const CadrgClut::Rgb& getColor(unsigned int index) const;
//
//------------------------------------------------------------------------------
class CadrgClut: public base::IObject
{
    DECLARE_SUBCLASS(CadrgClut, base::IObject)

public:
    CadrgClut();

    // Structure to hold our RGB values.
    struct Rgb {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
        unsigned char alpha;
        Rgb() { red = 0; green = 0; blue = 0; alpha = 0; }
    };
    // Color lookup table - 216, 32 or 16 colors
    enum ColorTableSizes { CLUT_216, CLUT_32, CLUT_16 };

    // Load our color lookup table
    void load(CadrgFrameEntry& frame, int cib, ColorTableSizes clutSize = CLUT_216);
    // Get a color at a particular index from the lookup table
    const CadrgClut::Rgb& getColor(unsigned int index) const;

private:
    enum { CADRG_COLORS = 217 };             // Max CADRG colors
    static const int MAXRECLEN      {500};   // Max record length
    static const int MAXOFFSETRECS  {10};    // Max # of color/gray offset records: usually 3
    static const int MAXCCRECS      {5};     // Max # of color converter records: usually 2

    std::array<unsigned int, CADRG_COLORS> cct {};  // Holds the list of RGB colors

    std::array<Rgb, 255> colorTable;        // Holds our individual colors
};

}
}

#endif
