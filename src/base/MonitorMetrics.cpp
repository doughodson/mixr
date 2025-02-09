
#include "mixr/base/MonitorMetrics.hpp"

#include "mixr/base/colors/Cie.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/relations/Table1.hpp"
#include <cstdio>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(MonitorMetrics, "MonitorMetrics")

BEGIN_SLOTTABLE(MonitorMetrics)
    "red",       // 1: ... Red Luminance vs RGB level
    "green",     // 2: ... Green Luminance vs RGB level
    "blue",      // 3: ... Blue Luminance vs RGB level
    "phosphors", // 4: ... Phosphor coordinates
    "whiteRGB",  // 5: ... RGB coordinate of reference white
    "whiteCIE",  // 6: ... CIE coordinate of reference white
END_SLOTTABLE(MonitorMetrics)

BEGIN_SLOT_MAP(MonitorMetrics)
    ON_SLOT(1, setSlotRed,       Table1)
    ON_SLOT(2, setSlotGreen,     Table1)
    ON_SLOT(3, setSlotBlue,      Table1)
    ON_SLOT(4, setSlotPhosphors, List)
    ON_SLOT(5, setSlotWhiteRGB,  List)
    ON_SLOT(6, setSlotWhiteCIE,  List)
END_SLOT_MAP()

MonitorMetrics::MonitorMetrics(const Table1* redLumTbl, const Table1* greenLumTbl, const Table1* blueLumTbl,
                               const Matrixd& phosphorCoordMatrix, const Vec3d& whiteRGB, const Vec3d& whiteCIE)
{
    STANDARD_CONSTRUCTOR()

    redLuminance = redLumTbl;
    greenLuminance = greenLumTbl;
    blueLuminance = blueLumTbl;
    phosphorCoordinates = phosphorCoordMatrix;
    refwhiteRGB = whiteRGB;
    refwhiteCIE = whiteCIE;

    computeMatrix();
}

MonitorMetrics::MonitorMetrics()
{
    // default configuration based on data from an unknown CRT monitor
    const int npoints = 21;
    double luminanceLevels[npoints] = { 0.0f,     0.05f,    0.1f,    0.15f,   0.2f,    0.25f,    0.3f,
                                        0.35f,    0.4f,     0.45f,   0.5f,    0.55f,   0.6f,     0.65f,
                                        0.7f,     0.75f,    0.8f,    0.85f,   0.9f,    0.95f,    1.0f };
    double luminanceRed[npoints] =    { 0.0f,     0.0f,     1.613f,  3.3f,    5.487f,  7.657f,  10.19f,
                                       13.02f,  16.49f,   19.36f,  22.57f,  26.21f,  30.32f,   33.67f,
                                       37.4f,   41.65f,   46.25f,  49.96f,  54.33f,  59.19f,   63.43f };
    double luminanceGreen[npoints] =  { 0.0f,     0.0f,     3.14f,   7.482f, 13.28f,  20.13f,   27.91f,
                                       36.77f,  47.04f,   55.47f,  66.16f,  77.91f,  90.12f,  100.9f,
                                      113.4f, 127.3f,   142.2f,  152.7f,  165.8f,  178.7f,   191.0f };
    double luminanceBlue[npoints] =   { 0.0f,     0.7603f,  2.025f,  3.653f,  5.554f,  7.427f,   9.427f,
                                       11.71f,  14.25f,   16.47f,  18.97f,  21.6f,   24.15f,   26.67f,
                                       29.51f,  32.16f,   35.22f,  37.61f,  40.53f,  43.4f,    45.99f };

    STANDARD_CONSTRUCTOR()

    redLuminance = new Table1(luminanceRed, npoints, luminanceLevels, npoints);
    greenLuminance = new Table1(luminanceGreen, npoints, luminanceLevels, npoints);
    blueLuminance = new Table1(luminanceBlue, npoints, luminanceLevels, npoints);
    phosphorCoordinates.set( 0.628f, 0.346f, 0.026f, 0.0f,
                             0.347f, 0.556f, 0.097f, 0.0f,
                             0.147f, 0.065f, 0.788f, 0.0f,
                             0.0f,   0.0f,   0.0f,   1.0f );
    refwhiteRGB.set( 0.211138f, 0.635777f, 0.153086f);
    refwhiteCIE.set( 0.276f, 0.239f, 0.485f );

    computeMatrix();
}

void MonitorMetrics::copyData(const MonitorMetrics& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc) {
        redLuminance = nullptr;
        greenLuminance = nullptr;
        blueLuminance = nullptr;
    }

    transform = org.transform;
    redLuminance = org.redLuminance;
    greenLuminance = org.greenLuminance;
    blueLuminance = org.blueLuminance;
    phosphorCoordinates = org.phosphorCoordinates;
    refwhiteRGB = org.refwhiteRGB;
    refwhiteCIE = org.refwhiteCIE;
}

void MonitorMetrics::deleteData()
{
    redLuminance = nullptr;
    greenLuminance = nullptr;
    blueLuminance = nullptr;
}

bool MonitorMetrics::setSlotRed(const Table1* const red)
{
    if ( red == nullptr ) return false;
    redLuminance = red;
    return computeMatrix();
}

bool MonitorMetrics::setSlotGreen(const Table1* const green)
{
    if ( green == nullptr ) return false;
    greenLuminance = green;
    return computeMatrix();
}

bool MonitorMetrics::setSlotBlue(const Table1* const blue)
{
    if ( blue == nullptr ) return false;
    blueLuminance = blue;
    return computeMatrix();
}

bool MonitorMetrics::setSlotPhosphors(const List* const phosphors)
{
    double listItems[6]{};

    if ( phosphors == nullptr ) return false;
    if ( phosphors->entries() != 6 ) return false;
    if ( phosphors->getNumberList(listItems, 6) != 6 ) return false;

    phosphorCoordinates.set( listItems[0], listItems[1], 1-listItems[0]-listItems[1], 0,
                             listItems[2], listItems[3], 1-listItems[2]-listItems[3], 0,
                             listItems[4], listItems[5], 1-listItems[4]-listItems[5], 0,
                                        0,            0,                           0, 1 );

    return computeMatrix();
}

bool MonitorMetrics::setSlotWhiteRGB(const List* const whiteRGB)
{
    double listItems[3]{};

    if ( whiteRGB == nullptr ) return false;
    if ( whiteRGB->entries() != 6 ) return false;
    if ( whiteRGB->getNumberList(listItems, 3) != 3 ) return false;

    refwhiteRGB.set( listItems[0], listItems[1], listItems[2] );
    return computeMatrix();
}

bool MonitorMetrics::setSlotWhiteCIE(const List* const whiteCIE)
{
    double listItems[3]{};

    if ( whiteCIE == nullptr ) return false;
    if ( whiteCIE->entries() != 6 ) return false;
    if ( whiteCIE->getNumberList(listItems, 3) != 3 ) return false;

    refwhiteCIE.set( listItems[0], listItems[1], listItems[2] );
    return computeMatrix();
}

bool MonitorMetrics::computeMatrix()
{
    Matrixd phosInv;
    Vec3d k;

    // Invert phosphor matrix
    if ( phosInv.invert( phosphorCoordinates ) == false ) {
        std::fprintf(stderr, "MonitorMetrics: Cannot invert phosphor coordinate matrix!");
        return false;
   }

    // compute vector k
    k = refwhiteCIE / refwhiteCIE[1];
    k = k * phosInv;
    k[0] = refwhiteRGB[0] / k[0];
    k[1] = refwhiteRGB[1] / k[1];
    k[2] = refwhiteRGB[2] / k[2];

    // Compute transform matrix
    transform.set( phosInv(0,0)*k[0], phosInv(1,0)*k[0], phosInv(2,0)*k[0], 0,
                   phosInv(0,1)*k[1], phosInv(1,1)*k[1], phosInv(2,1)*k[1], 0,
                   phosInv(0,2)*k[2], phosInv(1,2)*k[2], phosInv(2,2)*k[2], 0,
                                   0,                 0,                 0, 1 );
    return true;
}

void MonitorMetrics::cie2rgb(Vec4d& rgba, const Vec3d& cie) const
{
    Vec3d rgb, ciexyz;

    ciexyz.set(cie[Cie::X], cie[Cie::Y], 1-cie[Cie::X]-cie[Cie::Y]);
    rgb = transform * ciexyz;
    rgb *= cie[Cie::LUMINANCE]/(rgb[IColor::RED]+rgb[IColor::GREEN]+rgb[IColor::BLUE]);

    rgba[IColor::RED] = redLuminance->lfi( rgb[IColor::RED] );
    rgba[IColor::GREEN] = redLuminance->lfi( rgb[IColor::GREEN] );
    rgba[IColor::BLUE] = redLuminance->lfi( rgb[IColor::BLUE] );
    rgba[IColor::ALPHA] = IColor::getDefaultAlpha();
}

}
}
