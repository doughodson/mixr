//------------------------------------------------------------------------------
// Classes: RfSensor, SensorMgr
//------------------------------------------------------------------------------
#ifndef __oe_simulation_RfSensor_H__
#define __oe_simulation_RfSensor_H__

#include "openeaagles/simulation/RfSystem.h"

namespace oe {
   namespace base {
      class Angle;
      class Frequency;
      class Integer;
      class List;
      class String;
      class Time;
   }
namespace simulation {

class Antenna;
class Player;
class TrackManager;

//------------------------------------------------------------------------------
// Class: RfSensor
// Description: Base class for R/F Sensor Models.  Provides a common
//              interface, and some default behavior, for R/F sensors.
//
// Factory name: RfSensor
// Slots:
//    trackManagerName     <String>      ! Name of the requested Track Manager (default: 0)
//
//    modes                <PairStream>  ! List of submodes (default: 0)
//                         <RfSensor>    ! Single (only) submode
//
//    ranges               <List>        ! List of sensor ranges (nm) [vector] (default: 0)
//
//    initRangeIdx         <Number>      ! Initial range index [ 1 ... nRanges ] (default: 1)
//
//    PRF                  <Frequency>   ! Pulse Repetition Frequency (PRF) (must be greater than zero) (default: 0.0)
// ### NES: Initial value not greater than 0)
//                         <Number>      ! PRF in hertz
//
//    pulseWidth           <Time>        ! Pulse Width (must be greater than zero) (default: 0)
// ### NES: Initial value not greater than 0)
//                         <Number>      ! Pulse width in seconds
//
//    beamWidth            <Angle>       ! (Deprecated: moved to Antenna) Beam Width  (must be greater than zero) (default: D2RCC * 3.5f)
//                         <Number>      ! (Deprecated: moved to Antenna) Beam width in radians
//
//    typeId               <String>      ! R/F system type id (default: '\0')
//
//    syncXmitWithScan     <Number>      ! If true, transmitter on is sync'd with the antenna scan (default: false)
//
//
// Events:
//    TGT_DESIGNATE     ()          Target designate switch event
//    SENSOR_RTS        ()          Return-To-Search switch event
//    SCAN_START        (Int)       Start of scan event (bar number)
//    SCAN_END          (Int)       End of scan event (bar number)
//
//    This class is one of the "top level" systems attached to a Player
//    class (see Player.h).
//
//------------------------------------------------------------------------------
class RfSensor : public RfSystem
{
    DECLARE_SUBCLASS(RfSensor,RfSystem)

public:
    RfSensor();

    virtual double getPRF() const;                       // Returns the PRF (hertz)
    virtual bool setPRF(const double v);                 // Sets PRF (hertz; must be greater than 0)

    virtual double getPulseWidth() const;                // Returns the pulse width (seconds)
    virtual bool setPulseWidth(const double v);          // Sets the pulse width (seconds; must be greater than 0)

    virtual double getBeamWidth() const;                 // (Deprecated: moved to Antenna) Returns the beam width (radians; must be greater than 0)
    virtual bool setBeamWidth(const double v);           // (Deprecated: moved to Antenna) Sets the beam width (radians)

    virtual const char* getTypeId() const;               // Returns the type ID
    virtual bool setTypeId(const char* const str);       // Sets the type ID

    virtual double getRange() const;                     // Returns the current range (nm)
    virtual bool setRange(const double v);               // Sets the current range (nm; must be greater than or equal 0)

    virtual bool isScanOn() const;                       // Is the Sensor scanning
    virtual int getScanBar() const;                      // Returns the current bar number for a raster scan

    virtual const base::PairStream* getModes() const;        // Returns the list of sensor submodes
    virtual bool setMasterMode(RfSensor* const m);       // Set our master mode (container)

    virtual const base::String* getTrackManagerName() const; // Returns the requested track manager's name
    virtual bool setTrackManagerName(base::String* const a); // Sets the name of the track manager to use

    virtual TrackManager* getTrackManager();             // Returns our current track manager
    virtual const TrackManager* getTrackManager() const; // Returns our current track manager (const version)
    virtual bool setTrackManager(TrackManager* const a); // Sets the track manager

    // Support for a list of sensor ranges
    virtual bool incRange();                                  // Increment range index; returns true if successful
    virtual bool decRange();                                  // Decrement range index; returns true if successful
    virtual int getRanges(double* const rngs, const int max); // Returns the list of ranges
    virtual bool setRanges(const double* const rngs, const int n);// Sets the list of ranges
    virtual bool setInitRngIdx(const int idx);                // Sets the starting range index; returns true if successful

    // Slot functions
    virtual bool setSlotTrackManagerName(base::String* const v);   // Sets our track manager by name
    virtual bool setSlotModeStream(base::PairStream* const obj);   // Sets a list of R/F sensor submodes
    virtual bool setSlotModeSingle(RfSensor* const obj);            // Sets a single (only) R/F sensor submode
    virtual bool setSlotRanges(base::List* const list);            // Sets out list of valid ranges (nm)
    virtual bool setSlotInitRangeIdx(base::Number* const num);     // Sets out initial range index [ 1 .. nRanges ]
    virtual bool setSlotPrf(const base::Frequency* const msg);     // Sets PRF as a base::Frequency
    virtual bool setSlotPrf(const base::Number* const msg);        // Sets PRF in hertz
    virtual bool setSlotPulseWidth(const base::Time* const msg);   // Sets pulse width using base::Time
    virtual bool setSlotPulseWidth(const base::Number* const msg); // Sets pulse width in seconds
    virtual bool setSlotBeamWidth(const base::Angle* const msg);   // Sets beam width as a base::Angle
    virtual bool setSlotBeamWidth(const base::Number* const msg);  // Sets beam width in radians
    virtual bool setSlotTypeId(const base::String* const msg);     // Sets the type ID
    virtual bool setSlotSyncXmitWithScan(const base::Number* const msg); // Sets sync transmitter with antenna scan flag

    bool isTransmitting() const override;

    bool event(const int event, base::Object* const obj = nullptr) override;
    void updateData(const double dt = 0.0) override;
    void reset() override;

protected:
    virtual base::PairStream* getModes();                              // Returns the list of submodes

    // Event handler(s)
    virtual bool onStartScanEvent(const base::Integer* const bar);     // Start of scan (TGT_DESIGNATE) event handler
    virtual bool onEndScanEvent(const base::Integer* const bar);       // End of scan (SENSOR_RTS) event handler
    virtual bool onTgtDesignateEvent();                        // Target Designate (SCAN_START) event handler
    virtual bool onReturnToSearchEvent();                      // Return to search (SCAN_END) event handler

    bool shutdownNotification() override;

private:
    bool processModes();

    base::PairStream*  modes;           // Our Submodes
    double*        ranges;          // List of ranges (nm)
    int            nRanges;         // Number of ranges
    double         rng;             // Current range (nm)
    int            rngIdx;          // Range index [ 1 .. nRanges ]
    int            initRngIdx;      // Initial range [ 1 .. nRanges ]
    bool           scanning;        // Scanning flag (should be transmitting)
    int            scanBar;         // Scan (bar) number
    bool           syncXmitWithScan; // Sync transmitter with antenna scan flag

    base::String* tmName;        // Name of our track manager
    RfSensor*      masterModePtr;   // Our Master (Parent) mode (e.g., Sensor)
    TrackManager*  trackManager;    // Our Track manager -- managed by the onboard computer

    static const unsigned int TYPE_ID_LENGTH = 64;
    char typeId[TYPE_ID_LENGTH];    // R/F system type ID

    // Characteristics
    double prf;                     // Pulse Repetition Frequency   (Hz)
    double pulseWidth;              // Pulse Width                  (Sec)
    double beamWidth;               // Beamwidth                    (R)

};

//------------------------------------------------------------------------------
// Class: SensorMgr
//
// Description: Generic class for managing a list of R/F sensors
// Factory name: SensorMgr
//------------------------------------------------------------------------------
class SensorMgr : public RfSensor
{
    DECLARE_SUBCLASS(SensorMgr,RfSensor)

public:
    SensorMgr();
};

} // End simulation namespace
} // End oe namespace

#endif
