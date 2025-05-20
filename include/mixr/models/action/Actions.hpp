
#ifndef __mixr_models_Action_HPP__
#define __mixr_models_Action_HPP__

#include "mixr/base/ubf/IAction.hpp"
#include "mixr/base/safe_ptr.hpp"

namespace mixr {
namespace base { class Integer; class Latitude; class ILength; class Longitude; class INumber; class IComponent; }
namespace models {
class OnboardComputer;
class Sar;
class Steerpoint;

//------------------------------------------------------------------------------
// Class: Action
// Description: Actions that occur at steerpoints.
//
// Notes:
//
//  1) Use process() to process one action frame of delta time, 'dt'.  Follow
//  standard real-time rules since process() may be called from updateTC().
//
//  2) Actions that are not ready to start (isReadyToStart()) may be skipped
//     by the manager.
//
//  3) One shot action will complete during the call to trigger().
//
//  4) Any action that will take time to complete must have a manager
//     passed via trigger().
//
//  5) Derived from an UBF action (see "mixr/base/ubf/AbstractAction.hpp").
//  The execute() function's 'actor' must be our OnboardComputer or our
//  ownship, which can be  used to find our OnboardComputer.  The execute()
//  function will find the OnboardComputer and 'trigger()' the action.
//
//------------------------------------------------------------------------------
class Action : public base::ubf::IAction
{
    DECLARE_SUBCLASS(Action, base::ubf::IAction)

public:
   Action();

   virtual bool isReadyToStart();                               // True if action is ready to start
   virtual bool isInProgress();                                 // True if action is in progress
   virtual bool isCompleted();                                  // True if action has been completed

   virtual bool trigger(OnboardComputer* const);                // Starts this action
   virtual bool cancel();                                       // Cancels this action
   virtual void process(const double dt);                       // Action processing

   int getRefId() const                  { return refId; }      // Message Ref ID
   virtual void setRefId(const int);                            // Sets the message ref ID

   bool execute(base::IComponent* actor) override;

protected:
   OnboardComputer* getManager()   { return manager; }          // Our manager
   bool setManager(OnboardComputer* const);                     // Set our manager

   virtual void setCompleted(const bool);                       // Sets the completed flag

private:
   base::safe_ptr<OnboardComputer> manager;   // Our manager (only set while we're in progress)
   int refId {};                              // Ref ID
   bool completed {};                         // True if action has been completed
};


//------------------------------------------------------------------------------
// Class: ActionImagingSar
// Description: SAR capture, ATR run and Image download
// Factory name: ActionImagingSar
// Slots:
//   sarLatitude   <Latitude>  ! Target's latitude (default: 0)
//   sarLongitude  <Longitude> ! Target's longitude (default: 0)
//   sarElevation  <ILength>   ! Target's elevation (Length) (default: 0)
//   resolution    <ILength>   ! Image (pixel) resolution (default: 3)
//   imageSize     <Integer>   ! Image size: height & width (pixels) (default: 512)
//
//------------------------------------------------------------------------------
class ActionImagingSar : public Action
{
    DECLARE_SUBCLASS(ActionImagingSar, Action)

public:
   ActionImagingSar();

   double getSarLatitude() const       { return sarLatitude;  }   // SAR latitude in degrees
   double getSarLongitude() const      { return sarLongitude; }   // SAR longitude in degrees
   double getSarElevation() const      { return sarElevation; }   // SAR elevation in meters
   double getResolution() const        { return resolution;   }   // Image resolution in meters
   unsigned int getImageSize() const   { return imgSize;      }   // Image size
   double getOrientation() const       { return orientation;  }   // Planned image orientation (Deg) (true)

   virtual bool setSarLatitude(const double);                     // Sets the SAR latitude in degrees
   virtual bool setSarLongitude(const double);                    // Sets the SAR longitude in degrees
   virtual bool setSarElevation(const double);                    // Sets the SAR elevation in meters
   virtual bool setResolution(const double);                      // Sets the SAR image resolution (meters)
   virtual bool setImageSize(const unsigned int);                 // Sets the image size

   // Computes the planned image orientation (degs)
   virtual double computeOrientation(const Steerpoint* const);

   bool trigger(OnboardComputer* const) override;
   bool cancel() override;
   void process(const double dt) override;

protected:
   Sar* getSarSystem()                         { return sar; }
   virtual void setSarSystem(Sar* const);

   void setCompleted(const bool) override;

private:
   double sarLatitude {};        // Latitude (deg)
   double sarLongitude {};       // Longitude (deg)
   double sarElevation {};       // Elevation (meters)
   double resolution {3};        // Image resolution (meters)
   double orientation {};        // orientation (true) (degrees)
   unsigned int imgSize {512};   // Image size (pixels)
   double timer {};              // Time-Out timer
   Sar* sar {};                  // SAR system

private:
   // slot table helper methods
   bool setSlotSarLat(const base::Latitude* const);
   bool setSlotSarLon(const base::Longitude* const);
   bool setSlotSarElev(const base::ILength* const);
   bool setSlotResolution(const base::ILength* const);
   bool setSlotImageSize(const base::Integer* const);
};


//------------------------------------------------------------------------------
// Class: ActionWeaponRelease
// Description: Launches a weapon
//
// Factory name: ActionWeaponRelease
// Slots:
//   targetLatitude   <Latitude>  ! Target's latitude (default: 0)
//   targetLongitude  <Longitude> ! Target's longitude (default: 0)
//   targetElevation  <INumber>   ! Target's elevation (feet) (default: 0)
//   station          <INumber>   ! Station number to use (default: 0)
//
//------------------------------------------------------------------------------
class ActionWeaponRelease : public Action
{
    DECLARE_SUBCLASS(ActionWeaponRelease, Action)

public:
   ActionWeaponRelease();

   double getTargetLatitude() const       { return targetLatitude; }    // Target latitude in degrees
   double getTargetLongitude() const      { return targetLongitude; }   // Target longitude in degrees
   double getTargetElevation() const      { return targetElevation; }   // Target elevation in meters
   unsigned int getStation() const        { return station; }           // Weapon station number

   // Sets the target location (latitude & longitude in radians, elevation in meters)
   virtual bool setTargetLocation(const double tgtLat, const double tgtLon, const double tgtElev);

   // Set planned station number
   virtual bool setStation(const unsigned int num);

   bool trigger(OnboardComputer* const mgr) override;

private:
   double targetLatitude {};      // latitude (deg)
   double targetLongitude {};     // longitude (deg)
   double targetElevation {};     // elevation (meters)
   unsigned int station {};       // station number

private:
   bool setSlotTargetLat(const base::Latitude*);
   bool setSlotTargetLon(const base::Longitude*);
   bool setSlotTargetElev(const base::INumber*);
   bool setSlotStationNum(const base::Integer*);
};

//------------------------------------------------------------------------------
// Class: ActionDecoyRelease
// Description:  Releases a decoy
//
// Factory name: ActionDecoyRelease
// Slots:
//   numToLaunch  <Integer>  ! number of decoys to launch (default: 1)
//   interval     <INumber>  ! time, in seconds, between launches (default: 0)
//
//------------------------------------------------------------------------------
class ActionDecoyRelease : public Action
{
    DECLARE_SUBCLASS(ActionDecoyRelease, Action)

public:
    ActionDecoyRelease();

    // set functions
    virtual bool setInterval(const double x)              { interval = x; return true; }
    virtual bool setNumToLaunch(const int x)              { numToLaunch = x; return true; }
    void process(const double dt) override;

    bool trigger(OnboardComputer* const mgr) override;

private:
    int numToLaunch {1};   // how many decoys to launch this action?
    double interval {};    // seconds delay between launch... default is 0
    double tod {};         // the sim time of day (we are going to add to it)
    double startTOD {};    // our starting tod (that way we know when we've elapsed so far)

private:
   bool setSlotInterval(const base::INumber*);
   bool setSlotNumToLaunch(const base::Integer*);
};


//------------------------------------------------------------------------------
// Class: ActionCamouflageType
// Description: On trigger, switches our ownship's camouflage type
//
// Factory name: ActionCamouflageType
// Slots:
//   camouflageType  <Integer>  ! User-defined camouflage type (positive integer or zero for none)
//                              ! (default: 0)
//
//------------------------------------------------------------------------------
class ActionCamouflageType : public Action
{
    DECLARE_SUBCLASS(ActionCamouflageType, Action)

public:
   ActionCamouflageType();

   unsigned int getCamouflageType() const   { return camouflage; }         // Returns the user defined camouflage type (or zero for none)
   virtual bool setCamouflageType(const unsigned int);                     // Sets the user defined camouflage type (or zero for none)

   bool trigger(OnboardComputer* const mgr) override;

private:
   unsigned int camouflage {};    // Camouflage type (0 is none)

private:
   // slot table helper methods
   bool setSlotCamouflageType(const base::Integer* const);   // Sets user defined camouflage type
};

}
}

#endif
