
#ifndef __mixr_models_common_Image_HPP__
#define __mixr_models_common_Image_HPP__

#include "mixr/base/IObject.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Image
// Factory name: SarImage
// Description: SAR image structure
//------------------------------------------------------------------------------
class Image : public base::IObject
{
    DECLARE_SUBCLASS(Image, base::IObject)

public:
    Image();

    // Image data
    const unsigned char* getImageData() const                { return data; }                // Image data
    unsigned int getWidth() const                            { return width; }               // Image width (pixels)
    unsigned int getHeight() const                           { return height; }              // Image height (pixels)
    unsigned int getDepth() const                            { return depth; }               // Image depth (bytes)
    unsigned int getImageSize() const                        { return size; }                // Image size (bytes) -- (width*height*depth)
    bool isLoaded() const                                    { return (data != nullptr); }   // True if a valid image has been loaded

    // SAR data
    int getImageId() const                                   { return id; }              // Image ID
    bool isDatalinkTransmitEnabled() const                   { return xmitEnble; }       // True if image is to be transmitted over datalink
    double getLatitude() const                               { return latitude; }        // Stare Point (Deg))
    double getLongitude() const                              { return longitude; }       // Stare Point (Deg)
    double getElevation() const                              { return elevation; }       // Stare Point (Meters)
    double getOrientation() const                            { return orientation; }     // Image orientation (Deg) (true)
    double getResolution() const                             { return resolution; }      // Image resolution (Meters/pixel)
    virtual void setImageId(const int v)                     { id = v; }
    virtual void setDatalinkTransmitEnable(const bool flg)   { xmitEnble = flg; }
    virtual void setLatitude(const double v)                 { latitude = v; }
    virtual void setLongitude(const double v)                { longitude = v; }
    virtual void setElevation(const double v)                { elevation = v; }
    virtual void setOrientation(const double v)              { orientation = v; }
    virtual void setResolution(const double v)               { resolution = v; }

    // Original SAR image data
    const unsigned char* getOrigImageData() const            { return origData; }              // Original Image data
    unsigned int getOrigImageSize() const                    { return origSize; }              // Original Image size (bytes)
    bool isOriginalImageDataValid() const                    { return (origData != nullptr); } // True if original image data is valid
    bool isOrigImageDataCompressed() const                   { return origCompressed; }        // True if original image data is compressed

    // Set image functions
    virtual bool testImage(const unsigned w, const unsigned h);
    virtual bool setImageData(const unsigned char* const image, const unsigned int width, const unsigned int height, const unsigned int depth);
    virtual bool setOriginalImageData(const unsigned char* const image, const unsigned int size, const bool compressed);

private:
    // SAR data
    int    id {};                  // image ID
    double latitude {};            // (stare point) latitude (degrees)
    double longitude {};           // (stare point) longitude (degrees)
    double elevation {};           // (stare point) elevation (meters)
    double orientation {};         // orientation (true) (degrees)
    double resolution {};          // resolution (meters/pixel)

    // Image data
    unsigned char* data {};           // Pointer to the image data
    unsigned int   size {};           // Image size in bytes
    unsigned int   width {};          // Width  (pixels: must be power of two)
    unsigned int   height {};         // Height (pixels: must be power of two)
    unsigned int   depth {};          // Depth (bytes: 1, 2, 3 or 4)
    bool           xmitEnble {};      // Image should be transmitted over datalink

    // Original image data
    unsigned char* origData {};       // Original image data
    unsigned int   origSize {};       // Image size in bytes
    bool           origCompressed {}; // Original image is compressed
};

}
}

#endif
