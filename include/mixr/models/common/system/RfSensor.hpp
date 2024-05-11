
#ifndef __mixr_models_common_RfSensor_HPP__
#define __mixr_models_common_RfSensor_HPP__

#include "mixr/models/common/system/RfSystem.hpp"
#include "mixr/base/units/util/angle_utils.hpp"

#include <string>

namespace mixr {
namespace base { class Angle; class Boolean; class Frequency; class Identifier; class Integer;
                 class List; class Number; class String; class Time; }
namespace models {
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
//    trackManagerName     <Identifier>  ! Name of the requested Track Manager (default: "")
//
//    modes                <PairStream>  ! List of submodes (default: 0)
//                         <RfSensor>    ! Single (only) submode
//
//    ranges               <List>        ! List of sensor ranges (nm) [vector] (default: 0)
//
//    initRangeIdx         <Integer>     ! Initial range index [ 1 ... nRanges ] (default: 1)
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
//    syncXmitWithScan     <Boolean>     ! If true, transmitter on is sync'd with the antenna scan (default: false)
//
//
// Events:
//    TGT_DESIGNATE     ()          Target designate switch event
//    SENSOR_RTS        ()          Return-To-Search switch event
//    SCAN_START        (Int)       Start of scan event (bar number)
//    SCAN_END          (Int)       End of scan event (bar number)
//
//    This class is one of the "top level" systems attached to a Player
//    class (see Player.hpp).
//
//------------------------------------------------------------------------------
class RfSensor : public RfSystem
{
    DECLARE_SUBCLASS(RfSensor, RfSystem)

public:
    RfSensor();

    virtual double getPRF() const;                           // Returns the PRF (hertz)
    virtual bool setPRF(const double);                       // Sets PRF (hertz; must be greater than 0)

    virtual double getPulseWidth() const;                    // Returns the pulse width (seconds)
    virtual bool setPulseWidth(const double);                // Sets the pulse width (seconds; must be greater than 0)

    virtual double getBeamWidth() const;                     // (Deprecated: moved to Antenna) Returns the beam width (radians; must be greater than 0)
    virtual bool setBeamWidth(const double);                 // (Deprecated: moved to Antenna) Sets the beam width (radians)

    virtual const char* getTypeId() const;                   // Returns the type ID
    virtual bool setTypeId(const char* const);               // Sets the type ID

    virtual double getRange() const;                         // Returns the current range (nm)
    virtual bool setRange(const double);                     // Sets the current range (nm; must be greater than or equal 0)

    virtual bool isScanOn() const;                           // Is the Sensor scanning
    virtual int getScanBar() const;                          // Returns the current bar number for a raster scan

    virtual const base::PairStream* getModes() const;        // Returns the list of sensor submodes
    virtual bool setMasterMode(RfSensor* const);             // Set our master mode (container)

    virtual const std::string& getTrackManagerName() const;  // Returns the requested track manager's name
    virtual bool setTrackManagerName(const std::string&);    // Sets the name of the track manager to use

    virtual TrackManager* getTrackManager();                 // Returns our current track manager
    virtual const TrackManager* getTrackManager() const;     // Returns our current track manager (const version)
    virtual bool setTrackManager(TrackManager* const);       // Sets the track manager

    // Support for a list of sensor ranges
    virtual bool incRange();                                        // Increment range index; returns true if successful
    virtual bool decRange();                                        // Decrement range index; returns true if successful
    virtual int getRanges(double* const rngs, const int max);       // Returns the list of ranges
    virtual bool setRanges(const double* const rngs, const int n);  // Sets the list of ranges
    virtual bool setInitRngIdx(const int idx);                      // Sets the starting range index; returns true if successful

    bool isTransmitting() const override;

    bool event(const int event, base::Object* const obj = nullptr) override;
    void updateData(const double dt = 0.0) override;
    void reset() override;

protected:
    virtual base::PairStream* getModes();                              // Returns the list of submodes

    // event handlers
    virtual bool onStartScanEvent(const base::Integer* const bar);     // Start of scan (TGT_DESIGNATE) event handler
    virtual bool onEndScanEvent(const base::Integer* const bar);       // End of scan (SENSOR_RTS) event handler
    virtual bool onTgtDesignateEvent();                                // Target Designate (SCAN_START) event handler
    virtual bool onReturnToSearchEvent();                              // Return to search (SCAN_END) event handler

    bool shutdownNotification() override;

private:
    bool processModes();

    base::PairStream* modes{};          // Our Submodes
    double*        ranges{};            // List of ranges (nm)
    int            nRanges{};           // Number of ranges
    double         rng{50.0};           // Current range (nm)
    int            rngIdx{1};           // Range index [ 1 .. nRanges ]
    int            initRngIdx{1};       // Initial range [ 1 .. nRanges ]
    bool           scanning{};          // Scanning flag (should be transmitting)
    int            scanBar{};           // Scan (bar) number
    bool           syncXmitWithScan{};  // Sync transmitter with antenna scan flag

    std::string tmName;                 // Name of our track manager
    RfSensor*     masterModePtr{};      // Our Master (Parent) mode (e.g., Sensor)
    TrackManager* trackManager{};       // Our Track manager -- managed by the onboard computer

    static const int TYPE_ID_LENGTH{64};
    char typeId[TYPE_ID_LENGTH]{};      // R/F system type ID

    // Characteristics
    double prf{};                                // Pulse Repetition Frequency   (Hz)
    double pulseWidth{};                         // Pulse Width                  (Sec)
    double beamWidth{base::angle::D2RCC * 3.5};  // Beamwidth                    (R)

private:
   // slot table helper methods
   bool setSlotTrackManagerName(base::Identifier* const);     // Sets our track manager by name
   bool setSlotModeStream(base::PairStream* const);           // Sets a list of R/F sensor submodes
   bool setSlotModeSingle(RfSensor* const);                   // Sets a single (only) R/F sensor submode
   bool setSlotRanges(base::List* const);                     // Sets out list of valid ranges (nm)
   bool setSlotInitRangeIdx(base::Integer* const);            // Sets out initial range index [ 1 .. nRanges ]
   bool setSlotPrf(const base::Frequency* const);             // Sets PRF as a base::Frequency
   bool setSlotPrf(const base::Number* const);                // Sets PRF in hertz
   bool setSlotPulseWidth(const base::Time* const);           // Sets pulse width using base::Time
   bool setSlotPulseWidth(const base::Number* const);         // Sets pulse width in seconds
   bool setSlotBeamWidth(const base::Angle* const);           // Sets beam width as a base::Angle
   bool setSlotBeamWidth(const base::Number* const);          // Sets beam width in radians
   bool setSlotTypeId(const base::String* const);             // Sets the type ID
   bool setSlotSyncXmitWithScan(const base::Boolean* const);  // Sets sync transmitter with antenna scan flag
};

}
}

#endif
