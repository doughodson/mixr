
#ifndef __mixr_base_Locus_HPP__
#define __mixr_base_Locus_HPP__

#include "mixr/base/IObject.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Locus
// Description: A line of evenly spaced data points in a specific direction and
//              range from a reference point.  Or according to Webster, "Set of
//              all points whose location is determined by stated conditions"
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Locus
// Slots: none
//------------------------------------------------------------------------------
class Locus : public IObject
{
    DECLARE_SUBCLASS(Locus, IObject)

public:
    static const int MAX_SIZE{2048};     // Max size of the data array

public:
    Locus();
    Locus(
       const double* const data,    // Array of data points
       const int n,                 // Number of data points
       const double lat,            // Reference latitude (degs)
       const double lon,            // Reference longitude (degs)
       const double direction,      // True direction (heading) angle of the data (degs)
       const double minRng,         // Range to first data point (meters)
       const double maxRng          // Range to last data point (meters)
       );

    bool isDataValid() const           { return data != nullptr; }   // True if we have a valid data array
    double getData(const int) const;                                 // The idx'th data point, [ idx: 0 ... (getNumPts()-1) ]
    double getRange(const int) const;                                // The range to the idx'th point (meters), [ idx: 0 ... (getNumPts()-1) ]

    int getNumPts() const              { return np; }                // Number of data points
    double getRefLatitude() const      { return refLat; }            // Reference latitude (degs)
    double getRefLongitude() const     { return refLon; }            // Reference longitude (degs)
    double getDirection() const        { return angle; }             // The 'true heading' angle of the data array (degs)
    double getMinRng() const           { return minRng; }            // Range to the first data point (meters)
    double getMaxRng() const           { return maxRng; }            // Range to the last data point (meters)
    double getDeltaRange() const       { return deltaRng; }          // Distance between the data points (meters)
    const double* getDataArray() const { return data; }              // The array of data points

    // Sets the data array and its stated conditions
    virtual void setData(
       const double* const data,    // Array of data points
       const int n,                 // Number of points in the data array
       const double lat,            // Reference latitude (degs)
       const double lon,            // Reference longitude (degs)
       const double direction,      // True direction (heading) angle of the data (degs)
       const double minRng,         // Range to first data point (meters)
       const double maxRng          // Range to last data point (meters)
       );

    // Clears the data array
    virtual void clearData();

private:
    double  refLat{};     // Reference latitude (degs)
    double  refLon{};     // Reference longitude (degs)
    double* data{};       // Data array
    double  angle{};      // True direction (heading) angle of the data (degs)
    double  minRng{};     // Range to first data point (meters)
    double  maxRng{};     // Range to last data point  (meters)
    double  deltaRng{};   // Range between data points (meters)
    int np{};             // Number of data points
};

}
}

#endif
