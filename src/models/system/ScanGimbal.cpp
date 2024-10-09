
#include "mixr/models/system/ScanGimbal.hpp"
#include "mixr/models/Emission.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/units/angles.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(ScanGimbal, "ScanGimbal")
EMPTY_DELETEDATA(ScanGimbal)

BEGIN_SLOTTABLE(ScanGimbal)
    "scanMode",             //  1: Sets the type of scan we desire (manual, horizontal, vertical, conical, circular, pseudorandom)
    "leftToRightScan",      //  2: True to scan from left to right (else right to left) (Default: true) (up to down or down to up)
    "scanWidth",            //  3: Width of the scan (for search volume, or if specified manually)
    "searchVolume",         //  4: Scan width & height; sets HORIZONTAL_BAR_SCAN mode (radians)
    "reference",            //  5: Reference angles (center of search volume)
    "barSpacing",           //  6: Scan bar width spacing
    "numBars",              //  7: Sets the number of bars
    "revolutionsPerSec",    //  8: Revolutions per second for conical and circular scans (spiral scan too)
    "scanRadius",           //  9: Radius of the circle we are using for conical scans (radians or base::Angle} (spiral scan too)
    "pseudoRandomPattern",  // 10: Pseudo Random pattern vertices (2D - az and el)
    "maxRevolutions",       // 11: Spiral Scan - Maximum number of revolutions
END_SLOTTABLE(ScanGimbal)

BEGIN_SLOT_MAP(ScanGimbal)
    ON_SLOT( 1, setSlotScanMode,          base::Identifier)
    ON_SLOT( 2, setSlotLeftToRightScan,   base::Boolean)
    ON_SLOT( 3, setSlotScanWidth,         base::Number)
    ON_SLOT( 4, setSlotSearchVolume,      base::List)
    ON_SLOT( 5, setSlotRefPosition,       base::List)
    ON_SLOT( 6, setSlotBarSpacing,        base::Number)
    ON_SLOT( 7, setSlotNumBars,           base::Integer)
    ON_SLOT( 8, setSlotRevPerSec,         base::Number)
    ON_SLOT( 9, setSlotScanRadius,        base::Number)
    ON_SLOT( 9, setSlotScanRadius,        base::Angle)
    ON_SLOT(10, setSlotPRVertices,        base::PairStream)
    ON_SLOT(11, setSlotMaxRevs,           base::Number)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(ScanGimbal)
    ON_EVENT_OBJ(SCAN_START, onStartScanEvent,  base::Integer)
    ON_EVENT_OBJ(SCAN_END,   onEndScanEvent,    base::Integer)
END_EVENT_HANDLER()

ScanGimbal::ScanGimbal()
{
   STANDARD_CONSTRUCTOR()

   scanPos.set(0,0);
   refAngle.set(0,0);
   lastRefAngle.set(0,0);
}

void ScanGimbal::copyData(const ScanGimbal& org, const bool)
{
   BaseClass::copyData(org);

   scanPos = org.scanPos;
   scanMode = org.scanMode;
   scanState = org.scanState;
   scanWidth = org.scanWidth;
   scanHeight = org.scanHeight;
   refAngle = org.refAngle;
   barSpacing = org.barSpacing;
   numBars = org.numBars;
   oddNumberOfBars = org.oddNumberOfBars;
   reverseScan = org.reverseScan;
   leftToRightScan = org.leftToRightScan;
   barNum = org.barNum;
   conAngle = org.conAngle;
   revPerSec = org.revPerSec;
   scanRadius = org.scanRadius;
   myLastAngle = org.myLastAngle;
   prScanVertices = org.prScanVertices;
   nprv = org.nprv;
   cprv = org.cprv;
   lastRefAngle = org.lastRefAngle;
   numRevs = org.numRevs;
   maxNumRevs = org.maxNumRevs;
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void ScanGimbal::reset()
{
   resetScan();
   BaseClass::reset();
}

//------------------------------------------------------------------------------
// dynamics() -- System class "Dynamics phase" call back
//------------------------------------------------------------------------------
void ScanGimbal::dynamics(const double dt)
{
   scanController(dt);

   // Call BaseClass after to scan controller since the servo controller
   // is located in BaseClass.
   BaseClass::dynamics(dt);
}

//------------------------------------------------------------------------------
// onStartScanEvent() -- process the start of a scan
//------------------------------------------------------------------------------
bool ScanGimbal::onStartScanEvent(base::Integer* const)
{
   return true;
}

//------------------------------------------------------------------------------
// onEndScanEvent() -- process the end of a scan
//------------------------------------------------------------------------------
bool ScanGimbal::onEndScanEvent(base::Integer* const)
{
   return true;
}

//------------------------------------------------------------------------------
// scanController() -- control the gimbal's scanning
//------------------------------------------------------------------------------
void ScanGimbal::scanController(const double dt)
{
   switch (getScanMode()) {

      case ScanMode::CONICAL_SCAN : {
         conicalScanController(dt);
         break;
      }

      case ScanMode::CIRCULAR_SCAN : {
         circularScanController(dt);
         break;
      }

      case ScanMode::MANUAL_SCAN : {
         manualScanController(dt);
         break;
      }

      case ScanMode::PSEUDO_RANDOM_SCAN :  {
         pseudoRandomScanController(dt);
         break;
      }

      case ScanMode::SPIRAL_SCAN : {
         spiralScanController(dt);
         break;
      }

      case ScanMode::HORIZONTAL_BAR_SCAN :
      case ScanMode::VERTICAL_BAR_SCAN : {
         barScanController(dt);
         break;
      }

      default : {
         userModesScanController(dt);
         break;
      }
   };

}

//------------------------------------------------------------------------------
// conicalScanController() -- controls the conical scans
//------------------------------------------------------------------------------
void ScanGimbal::conicalScanController(const double dt)
{
    const double degPerDT{(getRevPerSec() * 360.0) * dt};
    static base::Integer iBar(1);

    switch(getScanState()) {
        // reset state: move to ref position
        case 0: {
            setServoMode(ServoMode::POSITION);
            setFastSlewMode(true);
            setScanState(1);
            setConAngle(0);
        }
            break;

        case 1: {
        // wait state
            if (isPositioned() || isAtLimits()) { setScanState(2); } // out of state 1
            break;// fall into state 2
        }

        case 2: {
            // start scan
            setFastSlewMode(false);
            setConAngle( base::angle::aepcdDeg(degPerDT + getConAngle()) );

            // Trigger the SCAN_START event handler
            onStartScanEvent(&iBar);

            setScanState(3);
        }
            break;

        case 3: {

            // turn revolutions per second into degrees per sec per frame
            // now we get this to each time step
            double conAngleN1{getConAngle()};
            setConAngle( base::angle::aepcdDeg(degPerDT + getConAngle()) );

            // end scan - finished with one rotation, check if our reference has moved
            bool onceAround{};

            // clockwise rotation
            if (getRevPerSec() >= 0.0) {
                if (conAngleN1 < 0.0 && getConAngle() >= 0.0) { onceAround = true; }
            }
            // counter-clockwise rotation
            else {
                if (conAngleN1 < 0.0 && getConAngle() >= 0.0) { setConAngle(0); onceAround = true; }
            }

            // after one revolution
            if (onceAround) {
                // Trigger the SCAN_END event handler
                onEndScanEvent(&iBar);
                setScanState(2);
            }
        }
            break;
    }

    // azimuth
    const double newX{getScanRadius() * std::sin(getConAngle() * base::angle::D2RCC)};
    // elevation
    const double newY{getScanRadius() * std::cos(getConAngle() * base::angle::D2RCC)};
    setScanPos(newX, newY);

    // command our new position
    setCmdPos(getRefPosition() + getScanPos());
}

//------------------------------------------------------------------------------
// spiralScanController() -- controls the spiral scan
//------------------------------------------------------------------------------
void ScanGimbal::spiralScanController(const double dt)
{
    const double degPerDT{(getRevPerSec() * 360.0) * dt};
    static base::Integer iBar(1);

    switch(getScanState()) {
        // reset state: move to ref position
        case 0: {
            setServoMode(ServoMode::POSITION);
            setFastSlewMode(true);
            setScanState(1);
            setConAngle(0);
            setNumRevs(0.0);
        }
            break;

        case 1: {
        // wait state
            if (isPositioned() || isAtLimits()) { setScanState(2); } // out of state 1
            break;// fall into state 2
        }

        case 2: {
            // start scan
            setFastSlewMode(false);
            setConAngle(getConAngle() + degPerDT);
            if (std::fabs(getConAngle()) > 360.0) {
                setNumRevs(getNumRevs()+1);
                if (getConAngle() >= 0.0) {
                    setConAngle(getConAngle() - 360.0);
                } else {
                    setConAngle(getConAngle() + 360.0);
                }
            }

            // Trigger the SCAN_START event handler
            onStartScanEvent(&iBar);

            setScanState(3);
        }
            break;

        case 3: {

            // turn revolutions per second into degrees per sec per frame
            // now we get this to each time step
            setConAngle(getConAngle() + degPerDT);
            if (std::fabs(getConAngle()) > 360.0) {
                setNumRevs(getNumRevs()+1);
                if (getConAngle() >= 0.0) {
                    setConAngle(getConAngle() - 360.0);
                } else {
                    setConAngle(getConAngle() + 360.0);
                }
            }

            // end scan - finished with one rotation, check if our reference has moved
            bool onceAround{};

            if (getNumRevs() >= getMaxNumRevs()) {
                onceAround = true;
            }

            // after one revolution
            if (onceAround) {
                // Trigger the SCAN_END event handler
                onEndScanEvent(&iBar);
                setConAngle(0.0);
                setNumRevs(0.0);
                setScanState(2);
            }
        }
            break;
    }

    double fullAngleRadians{getNumRevs() * 360.0};
    if (getRevPerSec() < 0.0) {
        fullAngleRadians = -fullAngleRadians;
    }
    fullAngleRadians = (fullAngleRadians + getConAngle()) * base::angle::D2RCC;

    // azimuth
    const double newX{getScanRadius() * (fullAngleRadians / (2.0 * base::PI)) * std::sin(fullAngleRadians)};
    // elevation
    const double newY{getScanRadius() * (fullAngleRadians / (2.0 * base::PI)) * std::cos(fullAngleRadians)};
    setScanPos(newX, newY);

    // command our new position
    setCmdPos(getRefPosition() + getScanPos());
}


//------------------------------------------------------------------------------
// circularScanController() -- controls the circular scans
//------------------------------------------------------------------------------
void ScanGimbal::circularScanController(const double)
{
    static base::Integer iBar(1);

    // Depending on our scan state, we will either start or stop the bar
    switch(getScanState()) {
        // reset state: move to ref position
        case 0: {
            setServoMode(ServoMode::POSITION);
            setFastSlewMode(true);
            setCmdPos(getRefPosition());
            setScanState(1);
        }
            break;

        case 1: {
        // wait state
            if (isPositioned() || isAtLimits()) { setScanState(2); } // out of state 1
            break;// fall into state 2
        }


        case 2: {
        // start scan - switch to a rate servo, and begin spinning at a commanded rate
            // Trigger the SCAN_START event handler
            onStartScanEvent(&iBar);
            setServoMode(ServoMode::RATE);
            setFastSlewMode(false);
            myLastAngle = base::angle::aepcdRad(getPosition().x() - getRefPosition().x());
            setScanState(3);
        }
            break;

        case 3: {
            // end scan - finished with one rotation, start over again
            bool onceAround{};

            double myAngle{base::angle::aepcdRad(getPosition().x() - getRefPosition().x())};
            // clockwise
            if (getCmdAzRate() >= 0.0) {
                onceAround = (myLastAngle < 0.0 && myAngle >= 0.0);
            }
            // we are going counter-clockwise
            else {
                onceAround = (myLastAngle >= 0.0 && myAngle < 0.0);
            }
            myLastAngle = myAngle;

            if (onceAround) {
                // Trigger the SCAN_END event handler
                onEndScanEvent(&iBar);
                // start over with our scan (another revolution)
                setScanState(2);
            }
        }
    }
}

//------------------------------------------------------------------------------
// manualScanController() -- we do nothing here
//------------------------------------------------------------------------------
void ScanGimbal::manualScanController(const double)
{}

//------------------------------------------------------------------------------
// pseudoRandomScanController() -- steps through an array of vertices (fast slew)
//------------------------------------------------------------------------------
void ScanGimbal::pseudoRandomScanController(const double)
{
    static base::Integer iBar(1);

    // Depending on our scan state, we will either start or stop the bar
    switch(getScanState()) {
        // reset state, must be in electronic mode or we will not operate
        case 0: {
            if (prScanVertices != nullptr) {
                if ( isGimbalType(Type::ELECTRONIC) ) {
                    setServoMode(ServoMode::POSITION);
                    setFastSlewMode(true);
                    setScanState(1);
                }
                else setScanMode(ScanMode::MANUAL_SCAN);
            }
        }
            break;


        case 1: {
        // start state - go to the desired pseudo random point (must be in electric mode)
            if (isPositioned() || isAtLimits()) {
                // make sure we have at least one vertice, then move to it
                if (nprv != 0 && cprv <= nprv) {
                    // if this is the first vertice, send a start event
                    if (cprv == 0) {
                        // Trigger the SCAN_START event handler
                        onStartScanEvent(&iBar);
                    }
                    else {
                        setScanPos(prScanVertices[cprv].x(), prScanVertices[cprv].y());
                        cprv++;
                    }
                }
                // when we reach the end
                else setScanState(2);
            }
        }
            break;

        case 2: {
        // end state - reset our vertice count and send an end event
            if (isPositioned() || isAtLimits()) {
                // Trigger the SCAN_END event handler
                onEndScanEvent(&iBar);

                // set us back to the first vertice
                cprv = 0;
                setScanState(1);
            }
        }
            break;
    }

    // now set our commanded position accordingly
    setCmdPos(getRefPosition()+getScanPos());
}


//------------------------------------------------------------------------------
// barScanController() -- control the bar scans
//------------------------------------------------------------------------------
void ScanGimbal::barScanController(const double)
{
    static base::Integer iBar(1);

    // Depending on our scan state, we will either start or stop the bar
    switch(getScanState()) {
        // reset state, we must set our bar number back to 1
        case 0: {
            setBarNumber(1);
            computeNewBarPos(getBarNumber(), Side::BEGINNING);
            setScanState(1);
            setServoMode(ServoMode::POSITION);
            setFastSlewMode(true);
        }
            break;

        case 1: {
        // start state - slow slew and compute the end position
            if (isPositioned() || isAtLimits()) {
                iBar.setValue(static_cast<int>(getBarNumber()));
                // Trigger the SCAN_START event handler
                onStartScanEvent(&iBar);
                computeNewBarPos(getBarNumber(), Side::ENDING);
                setScanState(2);
                setFastSlewMode(false);
            }
        }
            break;

        case 2: {
        // end state - fast slew and compute the next bar's position (if any)
            if (isPositioned() || isAtLimits()) {
                iBar.setValue(static_cast<int>(getBarNumber()));
                // Trigger the SCAN_END event handler
                onEndScanEvent(&iBar);
                nextBar();
                computeNewBarPos(getBarNumber(), Side::BEGINNING);
                setScanState(1);
                setFastSlewMode(true);
            }
        }
            break;
    }

    // now set our commanded position accordingly
    setCmdPos(getRefPosition()+getScanPos());
}

//------------------------------------------------------------------------------
// userModesScanController() -- the user will handle this
//------------------------------------------------------------------------------
void ScanGimbal::userModesScanController(const double)
{}

//------------------------------------------------------------------------------
// nextBar() - steps through the bar count, until we reach our number of bars
// limit.
//------------------------------------------------------------------------------
void ScanGimbal::nextBar()
{
    const unsigned int nbars{getNumBars()};
    const unsigned int bn{getBarNumber()};

    if (isOddNumberOfBars()) {
        if (nbars == 1) {
            setReverseScan( !isReverseScan() );
        }
        else if (bn >= nbars) {
            setReverseScan(true);
            setBarNumber(nbars-1);
        }
        else if (isReverseScan() && bn > 1) {
            setBarNumber(getBarNumber()-1);
        }
        else {
            unsigned int newBar{bn};
            if (newBar < 1) {
                newBar = 1;
            }
            setBarNumber(newBar+1);
            setReverseScan(false);
        }
    }
    else {
        setReverseScan(false);
        unsigned int newBar{bn + 1};
        if (newBar > nbars) newBar = 1;
        setBarNumber(newBar);
    }
}


//------------------------------------------------------------------------------
// computeNewBarPos() - computes the beginning or end point of the bar to be scanned
//------------------------------------------------------------------------------
void ScanGimbal::computeNewBarPos(const int bar, const Side side)
{
    // Lookup tables
    // 1 bar scan
    static double table1[2][2] { { -1.0, 0.0 }, { 1.0, 0.0 } };
    // 2 bar scan
    static double table2[2][2][2] { { {-1.0, 0.5}, {1.0, 0.5} }, { {1.0, -0.5}, {-1.0, -0.5} }
    };

    // 3 bar scan
    static double table3[3][2][2] {
        { { -1.0,  1.0 }, {  1.0,  1.0 } },
        { {  1.0,  0.0 }, { -1.0,  0.0 } },
        { { -1.0, -1.0 }, {  1.0, -1.0 } }
    };

    // 4 bar scan
    static double table4[4][2][2] {
        { { -1.0,  1.5},   {  1.0,  1.5} },
        { {  1.0,  0.5},   { -1.0,  0.5} },
        { { -1.0, -0.5},   {  1.0, -0.5} },
        { {  1.0, -1.5},   { -1.0, -1.5} }
    };

    // Now we determine which table to use, depending on the number of bars
    double x{};
    double y{};

    // now we draw the Unitless numbers from the tables
    unsigned int nb{getNumBars()};
    if (nb == 1) {
        x = table1[static_cast<int>(side)][0];
        y = table1[static_cast<int>(side)][1];
    }
    else if (nb == 2) {
        x = table2[bar-1][static_cast<int>(side)][0];
        y = table2[bar-1][static_cast<int>(side)][1];
    }
    else if (nb == 3) {
        x = table3[bar-1][static_cast<int>(side)][0];
        y = table3[bar-1][static_cast<int>(side)][1];
    }
    else if (nb == 4) {
        x = table4[bar-1][static_cast<int>(side)][0];
        y = table4[bar-1][static_cast<int>(side)][1];
    }

    // swap x values if we are scanning right to left
    if (!isLeftToRightScan()) x = -x;

    // if we have an odd number of bars and are in the reverse scan sequence swap
    // x values on even bar numbers or on bar if only a 1 bar scan
    if (nb == 1 && isReverseScan()) {
        x = -x;
    }

    // now turn our Unitless numbers in something we can use
    const double x1{x * (0.5 * getScanWidth())};
    const double y1{y * (getBarSpacing())};

    // We need to find which mode we are in before computing the start position
    if (getScanMode() == ScanMode::HORIZONTAL_BAR_SCAN) { setScanPos(x1, y1); }
    else { setScanPos(y1, x1); }
}

//------------------------------------------------------------------------------
// resetScan() - Resets the scan pattern
//------------------------------------------------------------------------------
bool ScanGimbal::resetScan()
{
    scanState = 0;
    return true;
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

double ScanGimbal::getScanWidthD() const
{
   return scanWidth * base::angle::R2DCC;
}

double ScanGimbal::getScanHeightD() const
{
   return scanHeight * base::angle::R2DCC;
}

void ScanGimbal::getScanVolume(double* const width, double* const height) const
{
   if (width != nullptr)  *width  = scanWidth;
   if (height != nullptr) *height = scanHeight;
}

void ScanGimbal::getScanVolumeD(double* const width, double* const height) const
{
   if (width != nullptr) *width = scanWidth * base::angle::R2DCC;
   if (height != nullptr) *height = scanHeight * base::angle::R2DCC;
}

double ScanGimbal::getScanRadiusD() const
{
   return scanRadius * base::angle::R2DCC;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// setLeftToRightScan(): sets the flag for scanning bars left to right
bool ScanGimbal::setLeftToRightScan(const bool newLeftToRightScan)
{
    leftToRightScan = newLeftToRightScan;
    return true;
}

// setScanWidth(): sets the scan width (for search volume)
bool ScanGimbal::setScanWidth(const double newWidth)
{
    scanWidth = newWidth;
    return true;
}

// sets the antenna scan pattern
bool ScanGimbal::setScanMode(const ScanMode m, const bool resetRequired)
{
    if (scanMode != m) {
        scanMode = m;
        if (resetRequired) resetScan();
    }
    return true;
}

//------------------------------------------------------------------------------
// setSearchVolume() -- set the scanning volume width & height and scan mode
//------------------------------------------------------------------------------
bool ScanGimbal::setSearchVolume(const double width, const double height, const int reqBars)
{
    // In all cases, set the volume size
    setScanWidth(width);
    scanHeight = height;

    if (reqBars != 1 && reqBars != 2 && reqBars != 3 && reqBars != 4) {
        if (scanHeight < (base::angle::D2RCC * 1.0f))
            numBars = 1;
        else if (scanHeight < (base::angle::D2RCC * 5.0f))
            numBars = 2;
        else if (scanHeight < (base::angle::D2RCC * 10.0f))
            numBars = 3;
        else
            numBars = 4;
    }
    else numBars = reqBars;

    if ((numBars % 2) != 0)
        oddNumberOfBars = true;
    else {
        oddNumberOfBars = false;
        reverseScan = false;
    }

    // set our barSpacing
    setBarSpacing(scanHeight/(numBars));

    // and start the bar scan (always horizontal)
    setScanMode(ScanMode::HORIZONTAL_BAR_SCAN);
    return true;
}

//------------------------------------------------------------------------------
// setRefPosition:  Set reference angles (center of the search volume)
//------------------------------------------------------------------------------
bool ScanGimbal::setRefPosition(const double refAz,  const double refEl)
{
    bool ok{};

    // set one, and if it is ok, set the other
    ok = setRefAzimuth(refAz);
    if (ok) ok = setRefElevation(refEl);

    return ok;
}

//------------------------------------------------------------------------------
// setBarSpacing(): sets the bar spacing
//------------------------------------------------------------------------------
bool ScanGimbal::setBarSpacing(const double newSpacing)
{
    barSpacing = newSpacing;
    // calculate our scan height
    scanHeight = barSpacing * (numBars - 1);
    return true;
}

//------------------------------------------------------------------------------
// setNumBars(): sets the number of bars that we are using
//------------------------------------------------------------------------------
bool ScanGimbal::setNumBars(const double newNumBars)
{
    numBars = static_cast<int>(newNumBars);

    // Odd or even?
    if ((numBars % 2) != 0)
        oddNumberOfBars = true;
    else {
        oddNumberOfBars = false;
        reverseScan = false;
    }

    // now calculate scan height
    scanHeight = barSpacing * (numBars - 1);

    return true;
}

//------------------------------------------------------------------------------
// setRevPerSec(): revolutions per second
//------------------------------------------------------------------------------
bool ScanGimbal::setRevPerSec(const double newRevPerSec)
{
    revPerSec = newRevPerSec;
    return true;
}

//------------------------------------------------------------------------------
// setScanRadius(): sets the scan radius (rad)
//------------------------------------------------------------------------------
bool ScanGimbal::setScanRadius(const double newScanRadius)
{
    scanRadius = newScanRadius;
    return true;
}

//------------------------------------------------------------------------------
// setRefPosition(Vec2) -- set the reference position of the search volume
//------------------------------------------------------------------------------
void ScanGimbal::setRefPosition(const base::Vec2d& nla)
{
    refAngle = nla;
}

//------------------------------------------------------------------------------
// setRefAzimuth(az) -- set the reference azimuth of the search volume
//------------------------------------------------------------------------------
bool ScanGimbal::setRefAzimuth(const double az)
{
    refAngle[AZ_IDX] = az;
    return true;
}

//------------------------------------------------------------------------------
// setRefElevation(az) -- set the reference elevation of the search volume
//------------------------------------------------------------------------------
bool ScanGimbal::setRefElevation(const double el)
{
    refAngle[ELEV_IDX] = el;
    return true;
}

//------------------------------------------------------------------------------
// setMaxRevs(): sets the degrees per second
//------------------------------------------------------------------------------
bool ScanGimbal::setMaxRevs(const double newMaxRevs)
{
    maxNumRevs = newMaxRevs;
    return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// setSlotScanMode() -- calls setScanMode()
bool ScanGimbal::setSlotScanMode(base::Identifier* const x)
{
    // set our scan mode
    bool ok{true};
    if (x != nullptr) {
        if (*x == "manual")            ok = setScanMode(ScanMode::MANUAL_SCAN);
        else if (*x == "horizontal")   ok = setScanMode(ScanMode::HORIZONTAL_BAR_SCAN);
        else if (*x == "vertical")     ok = setScanMode(ScanMode::VERTICAL_BAR_SCAN);
        else if (*x == "conical")      ok = setScanMode(ScanMode::CONICAL_SCAN);
        else if (*x == "circular")     ok = setScanMode(ScanMode::CIRCULAR_SCAN);
        else if (*x == "pseudorandom") ok = setScanMode(ScanMode::PSEUDO_RANDOM_SCAN);
        else if (*x == "spiral")       ok = setScanMode(ScanMode::SPIRAL_SCAN);
        else ok = false;
    }
    return ok;
}

// setSlotLeftToRightScan() - calls setLeftToRightScan()
bool ScanGimbal::setSlotLeftToRightScan(const base::Boolean* const newLeftToRightScan)
{
    bool ok{};
    if (newLeftToRightScan != nullptr) {
        const bool x{newLeftToRightScan->asBool()};
        ok = setLeftToRightScan(x);
    }

    return ok;
}

// setSlotScanWidth() -- calls setScanWidth()
bool ScanGimbal::setSlotScanWidth(const base::Number* const newWidth)
{
    bool ok{};
    if (newWidth != nullptr) {
        const double x{newWidth->asDouble()};
        ok = setScanWidth(x);
    }
    return ok;
}

// setSlotSearchVolume() -- calls setSearchVolume()
bool ScanGimbal::setSlotSearchVolume(base::List* const numList)
{
    bool ok{};
    double values[2]{};
    if (numList != nullptr) {
        const std::size_t n{numList->getNumberList(values, 2)};
        if (n == 2) ok = setSearchVolume(values[0], values[1]);
    }
    return ok;
}

// setSlotRefPosition() --  calls setRefPosition
bool ScanGimbal::setSlotRefPosition(const base::List* const numList)
{
    bool ok{};
    double values[2]{};
    if (numList != nullptr) {
        const std::size_t n{numList->getNumberList(values, 2)};
        if (n == 2) ok = setRefPosition(values[0], values[1]);
    }
    return ok;
}

// setSlotBarSpacing() --
bool ScanGimbal::setSlotBarSpacing(const base::Number* const newSpacing)
{
    bool ok{};
    if (newSpacing != nullptr) {
        const double x{newSpacing->asDouble()};
        ok = setBarSpacing(x);
    }
    return ok;
}

// setSlotNumBars() --
bool ScanGimbal::setSlotNumBars(const base::Integer* const newNumBars)
{
    bool ok{};
    if (newNumBars != nullptr) {
        const int temp{newNumBars->asInt()};
        ok = setNumBars(temp);
    }
    return ok;
}

// setSlotRevPerSec() --
bool ScanGimbal::setSlotRevPerSec(const base::Number* const newRevPerSec)
{
    bool ok{};
    if (newRevPerSec != nullptr) {
        const double x{newRevPerSec->asDouble()};
        ok = setRevPerSec(x);
    }
    return ok;
}

// setSlotScanRadius()
bool ScanGimbal::setSlotScanRadius(const base::Number* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setScanRadius(x->asDouble());
   }
   return ok;
}

bool ScanGimbal::setSlotScanRadius(const base::Angle* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setScanRadius(x->getValueInRadians());
   }
   return ok;
}

// setSlotPRVertices() -- gets a pairstream and puts the vertices into an array
// example --
//     vertices: { [ 1 2 ]  [ 3 4 ] [ 5 6 ] }
bool ScanGimbal::setSlotPRVertices(const base::PairStream* const prObj)
{
   bool ok{true};

   if (prObj != nullptr) {
        // find how many vertices we have
        const std::size_t n{prObj->entries()};
        // Get the vertices from the pair stream
        nprv = 0;
        const base::List::Item* item{prObj->getFirstItem()};
        // holds our array values
        base::Vec2d tempVerts(0.0, 0.0);

        while (item != nullptr && nprv < n) {
            const auto p = dynamic_cast<const base::Pair*>(item->getValue());
            if (p != nullptr) {
                const base::Object* obj2{p->object()};
                const auto msg2 = dynamic_cast<const base::List*>(obj2);
                if (msg2 != nullptr) {
                    double values[2]{};
                    const std::size_t nl{msg2->getNumberList(values, 2)};

                    if (nl == 2) {
                        // set our values in our vector array
                        prScanVertices[nprv].set(values[0],values[1]);
                        nprv++;
                    }
                    else ok = false;
                }
            }
            item = item->getNext();
        }
    }
    return ok;
}

// setSlotMaxRevs() --
bool ScanGimbal::setSlotMaxRevs(const base::Number* const newMaxRevs)
{
    bool ok{};
    if (newMaxRevs != nullptr) {
        const double x{newMaxRevs->asDouble()};
        ok = setMaxRevs(x);
    }
    return ok;
}

}
}
