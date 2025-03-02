
#ifndef __mixr_base_MonitorMetrics_HPP__
#define __mixr_base_MonitorMetrics_HPP__

#include "mixr/base/IObject.hpp"
#include "mixr/base/colors/IColor.hpp"
#include "mixr/base/relations/Table1.hpp"
#include "mixr/base/safe_ptr.hpp"
#include "mixr/base/osg/Matrixd"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/osg/Vec4d"

namespace mixr {
namespace base {
class List;

//------------------------------------------------------------------------------
// Class: MonitorMetrics
// Description:  Defines the characteristics of a monitor, needed to accurately match color
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: MonitorMetrics
// Slots:
//     red       <Table1>   ! Red luminance vs RGB level ( both luminance and RGB level range from 0..1 )
//                          ! (Default: redLumTbl)
//     green     <Table1>   ! Green luminance vs RGB level (default: greenLumTbl)
//     blue      <Table1>   ! Blue luminance vs RGB level (default: blueLumTbl)
//     phosphors <List>     ! Phosphor coordinates ( 6 element list, 2 each for red, green, blue )
//                          ! (default: phosphorCoordMatrix)
//     whiteRGB  <List>     ! RGB value of reference white ( 3 element list ) (default: whiteRGB)
//     whiteCIE  <List>     ! CIE coordinate of reference white ( 3 element list ) (default: whiteCIE)
//------------------------------------------------------------------------------
// Events: None
//------------------------------------------------------------------------------
class MonitorMetrics : public IObject
{
    DECLARE_SUBCLASS(MonitorMetrics, IObject)

public:
    MonitorMetrics();
    // special constructor to initialize the object with the given values.
    MonitorMetrics(const Table1* redLuminance, const Table1* greenLuminance, const Table1* blueLuminance,
                   const Matrixd& phosphorCoordinates, const Vec3d& whiteRGB, const Vec3d& whiteCIE);

    // convert a CIE color into an RGB value for this monitor.
    void cie2rgb(Vec4d& rgba, const Vec3d& cie) const;

private:
    bool computeMatrix();

    // transform matrix from CIE to RGB for this monitor
    Matrixd transform;

    // RGB luminance vs RGB level on this monitor
    safe_ptr<const Table1> redLuminance;
    safe_ptr<const Table1> greenLuminance;
    safe_ptr<const Table1> blueLuminance;

    // CIE coordinates of Red, Green, and Blue for this monitor
    Matrixd phosphorCoordinates;

    // CIE and RGB coordinates of a reference white
    Vec3d refwhiteRGB;
    Vec3d refwhiteCIE;

private:
    // slot table helper methods
    bool setSlotRed(const Table1* const);
    bool setSlotGreen(const Table1* const);
    bool setSlotBlue(const Table1* const);
    bool setSlotPhosphors(const List* const);
    bool setSlotWhiteRGB(const List* const);
    bool setSlotWhiteCIE(const List* const);
};

}
}

#endif

