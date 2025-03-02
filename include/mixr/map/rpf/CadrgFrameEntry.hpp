
#ifndef __mixr_map_rpf_CadrgFrameEntry_HPP__
#define __mixr_map_rpf_CadrgFrameEntry_HPP__

#include "mixr/map/rpf/CadrgClut.hpp"
#include "mixr/base/IObject.hpp"

namespace mixr {
namespace rpf {

class CadrgFrame;

//------------------------------------------------------------------------------
// Class: CadrgFrameEntry
//
// Description: Basic Table of Contents Frame entry that holds a frame and the
// Color Look up table associated with that frame.
//
// setPathName() - Sets the a.toc files directory and filename (a.toc)
//    void CadrgFrameEntry::setPathName(const char* dir, const char* file)
//
// setCib() - Set our cib flag
//    void CadrgFrameEntry::setCib(const bool x)
//
// loadClut() - Load our color lookup table, if it isn't already loaded.
//    void CadrgFrameEntry::loadClut()
//
// setFrame() - Sets the child frame
//    void CadrgFrameEntry::setFrame(CadrgFrame* newFrame)
//
//------------------------------------------------------------------------------
class CadrgFrameEntry : public base::IObject
{
    DECLARE_SUBCLASS(CadrgFrameEntry, base::IObject)

public:
    CadrgFrameEntry();

    // Get
    bool doIExist()                   { return exists; }
    const char* getDirectory()        { return directory; }
    const char* getFileName()         { return filename; }
    CadrgFrame* getFrame()            { return frame; }
    const CadrgClut& getClut()        { return clut; }

    // Set
    virtual void setPathName(const char* dir, const char* file);
    virtual void setCib(const bool);
    virtual void loadClut();
    virtual void setFrame(CadrgFrame*);

private:
    bool exists {};        // Does this entry exist (has a path name been set?)
    char* directory {};    // Directory
    char filename[16] {};  // Filename
    CadrgClut clut;        // Color lookup table
    CadrgFrame* frame {};  // Pointer to our child frame
    bool loaded {};        // Loaded flag
    bool cib {};           // Are we a CIB or not?
};

}
}

#endif

