
#include "mixr/models/Image.hpp"
#include <iostream>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Image, "SarImage")
EMPTY_SLOTTABLE(Image)

Image::Image()
{
    STANDARD_CONSTRUCTOR()
}

void Image::copyData(const Image& org, const bool)
{
    BaseClass::copyData(org);

    // Copy the SAR data
    setDatalinkTransmitEnable( org.xmitEnble );
    setImageId( org.getImageId() );
    setLatitude( org.getLatitude() );
    setLongitude( org.getLongitude() );
    setElevation( org.getElevation() );
    setOrientation( org.getOrientation() );
    setResolution( org.getResolution() );

    // Copy the images
    setImageData(org.getImageData(), org.getWidth(), org.getHeight(), org.getDepth());
    setOriginalImageData(org.getOrigImageData(), org.getOrigImageSize(), org.isOrigImageDataCompressed());
}

void Image::deleteData()
{
    // Clear the images
    setImageData(nullptr, 0, 0, 0);
    setOriginalImageData(nullptr, 0, 0);
}

//------------------------------------------------------------------------------
// setImageData() -- Load image data
//------------------------------------------------------------------------------
bool Image::setImageData(const unsigned char* const image, const unsigned int w, const unsigned int h, const unsigned int d)
{
    bool ok{true};     // Assume all will go well.

    // Allocate space for and transfer the new image
    unsigned int s{w*h*d};
    unsigned char* buff{};
    if (image != nullptr) {
        // Allocate space
        buff = new unsigned char[s];
        if (buff != nullptr) {
            // Transfer the image
            for (unsigned int i = 0; i < s; i++) buff[i] = image[i];
        }
        else ok = false;    // we failed to allocate space
    }

    // If all's well, set our member variables
    if (ok) {
        // Clear the old image (if any)
        if (data != nullptr) delete[] data;
        data = buff;
        size = s;
        width = w;
        height = h;
        depth = d;
    }

    return ok;
}

//------------------------------------------------------------------------------
// setOrigialImageData() -- Load original image data
//------------------------------------------------------------------------------
bool Image::setOriginalImageData(const unsigned char* const ti, const unsigned int s, const bool c)
{
    bool ok{true};     // Assume all will go well.

    // Allocate space for and transfer the new image
    unsigned char* buff{};
    if (ti != nullptr) {
        // Allocate space
        buff = new unsigned char[s];
        if (buff != nullptr) {
            // Transfer the image
            for (unsigned int i = 0; i < s; i++) buff[i] = ti[i];
        }
        else ok = false;    // we failed to allocate space
    }

    // If all's well, set our member variables
    if (ok) {
        // Clear the old image (if any)
        if (origData != nullptr) delete[] origData;
        origData = buff;
        origSize = s;
        origCompressed = c;
    }

    return ok;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool Image::testImage(const unsigned w0, const unsigned h0)
{
    unsigned int d0{1};
    const auto image = new unsigned char[w0*h0*d0];

    for (unsigned int h = 0; h < h0; h++) {
        for (unsigned int w = 0; w < w0; w++) {

            // Edges
            unsigned char c{};
            if (h < 64 || h >= (h0 - 32)) c = 0x7f;
            if (w < 32 || w >= (w0 - 16)) c += 0x7f;

            // Cross hair
            if (h == (h0/2) || w == (w0/2)) c = 0xff;

            // Store the color
            unsigned int os{(h * (w0 * d0)) + (w * d0)};
            image[os] = c;
        }
    }

    bool ok{setImageData(image, w0, h0, d0)};
    delete [] image;
    return ok;
}

}
}
