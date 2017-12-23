
#ifndef __mixr_models_IrSensor_H__
#define __mixr_models_IrSensor_H__

#include "mixr/models/system/IrSystem.hpp"
#include "mixr/base/safe_queue.hpp"

namespace mixr {
namespace base { class Integer; class Number; class String; }
namespace models {
class IrSeeker;
class IrQueryMsg;
class Player;
class TrackManager;

//------------------------------------------------------------------------------
// Class: IrSensor
// Description: Base class for I/R Sensor Models.  Provides a common
//              interface, and some default behavior, for I/R sensors.
//
// An IrSensor must discover its seeker. This operations is performed by the IrSystem bases class,
// either in the background thread or during reset() processing.
// In some cases (during missile launch), waiting for the background thread to perform this
// operation may disrupt a seeker/sensor's ability to track a target.
//
// Slots:
//
//    lowerWavelength     (Real)           The lower wavelength limit in microns
//
//    upperWavelength     (Real)           The upper wavelength limit in microns
//
//    nei                 (Real)           The Noise Equivalent Irradiance in watts/str-cm^2
//
//    threshold           (Real)           The Signal to Noise Threshold
//
//    IFOV                (Real)           The Instantaneous Field of View in steradians
//
//    sensorType          (String)         The type of sensor(CONTRAST, HOTSPOT)
//
//    FOR                 (Real)           The Field of Regard in steradians
//
// Events:
//    bool irQueryReturnEvent(IrQueryMsg* const irQuery);
//    This class is one of the "top level" systems attached to a Player
//    class (see Player.hpp).
//
//
// Public Member functions:
//
//    bool isScanOn()
//       Returns true if the Sensor scanning
//
//    double getLowerWavelength()
//    bool setLowerWavelength(const double w)
//       Gets/Sets the lower wavelength (microns; must be greater than 0)
//
//    double getUpperWavelength()
//    bool setUpperWavelength(const double w)
//       Gets/Sets the upper wavelength (microns; must be greater than 0)
//
//    double getNEI()
//    bool setNEI(const double n)
//       Gets/Sets the Noise Equivalent Irradiance  (watts/str-cm^2); must be greater than 0)
//
//    double getThreshold()
//    bool setThreshold(const double t)
//       Gets/Sets the Signal to Noise Threshold
//
//    double getIFOV()
//    bool setIFOV(const double i)
//       Gets/Sets the Instantaneous Field of View  (steradians)
//       What the sensor can see with no gimbal movement
//
//    SensorType getSensorType()
//    bool setSensorType(const SensorType st)
//       Gets/Sets the Type of sensor whether hot spot (only objects hotter than the environment
//           are detected) or contrast (objects hotter and cooler than the environment are observed)
//
//    double getFieldOfRegard()
//    bool setFieldOfRegard(const double fov)
//       Gets/Sets the Field of Regard  (steradians)
//       What the sensor can see with gimbal's full range of movement
//
//------------------------------------------------------------------------------
class IrSensor : public IrSystem
{
    DECLARE_SUBCLASS(IrSensor, IrSystem)

public:
   enum SensorType {
         CONTRAST,   // Detection by difference (hot or cold) from background
         HOTSPOT     // Detection if hotter than background
   };

public:

   IrSensor();

   virtual double getLowerWavelength() const {return lowerWavelength; };   // Returns the lower wavelength (microns)
   virtual bool setLowerWavelength(const double w);                        // Sets the lower wavelength (microns; must be greater than 0)

   virtual double getUpperWavelength() const {return upperWavelength; };   // Returns the upper wavelength (microns)
   virtual bool setUpperWavelength(const double w);                        // Sets the upper wavelength (microns; must be greater than 0)

   virtual double getNEI() const {return nei; };   // Returns the Noise Equivalent Irradiance  (watts/str-cm^2)
   virtual bool setNEI(const double);              // Sets the Noise Equivalent Irradiance  (watts/str-cm^2); must be greater than 0)

   virtual double getThreshold() const {return threshold; };   // Returns the Signal to Noise Threshold
   virtual bool setThreshold(const double);                    // Sets the Signal to Noise Threshold

   virtual double getIFOVTheta() const {return ifovTheta; };   // Returns ifov planar angle (radians)
   virtual double getIFOV() const {return ifov; };             // Returns the Instantaneous Field of View  (steradians)
   virtual bool setIFOV(const double);                         // Sets the Instantaneous Field of View  (steradians)

   virtual SensorType getSensorType() const {return sensorType; } ;  // Returns the type of sensor
   virtual bool setSensorType(const SensorType);                     // Sets the type of sensor

   //virtual double getFieldOfRegardTheta() const {return fieldOfRegardTheta; };   // Returns Field of Regard planar angle (radians)
   //virtual double getFieldOfRegard() const { return fieldOfRegard; };            // Returns the Field of Regard  (steradians)
   //virtual bool setFieldOfRegard(const double fov);                              // Sets the Field of Regard  (steradians)

   //virtual double getAzimuthBin() const {return azimuthBin; };       // Returns min azimuth distance
   //virtual bool setAzimuthBin(const double azimuthBin);              // Sets min Azimuth Distance

   //virtual double getElevationBin() const {return elevationBin; };     // Returns min azimuth distance
   //virtual bool setElevationBin(const double elevationBin);            // Sets min Azimuth Distance

   virtual double getMaximumRange() const {return maximumRange; };   // Returns maximum Range
   virtual bool setMaximumRange(const double maximumRange);          // Sets maximum Range

   virtual const base::String* getTrackManagerName() const; // Returns the requested track manager's name
   virtual bool setTrackManagerName(base::String* const);   // Sets the name of the track manager to use

   virtual TrackManager* getTrackManager();               // Returns our current track manager
   virtual const TrackManager* getTrackManager() const;   // Returns our current track manager (const version)
   virtual bool setTrackManager(TrackManager* const);     // Sets the track manager

   // Store sensor reports until we are ready to pass on to track manager.
   void addStoredMessage(IrQueryMsg* msg);

   virtual bool calculateIrQueryReturn(IrQueryMsg* const irQuery);

   void updateData(const double dt = 0.0) override;
   void reset() override;

protected:
   void transmit(const double dt) override;
   void process(const double dt) override;

   bool shutdownNotification() override;

   virtual IrQueryMsg* getStoredMessage();
   virtual IrQueryMsg* peekStoredMessage(unsigned int i);

   base::safe_queue<IrQueryMsg*> storedMessagesQueue {MAX_EMISSIONS};
   mutable long storedMessagesLock {};        // Semaphore to protect 'storedMessagesQueue'

private:
   static const int MAX_EMISSIONS = 10000;   // Max size of emission queues and arrays

   void clearTracksAndQueues();

   // Characteristics
   double lowerWavelength {};          // Lower wavelength limit (microns)
   double upperWavelength {};          // Upper wavelength limit (microns)
   double nei {};                      // Noise Equivalent Irradiance  (watts/str-cm^2)
   double threshold {};                // Signal to Noise Threshold
   double ifov {};                     // Instantaneous Field of View  (steradians) (what is in view without gimbal movement)
   double ifovTheta {};                // IFOV planar angle, where ifov = 2 * pi * (1-cos(ifovTheta/2)

   // results in a simple cone.
   SensorType sensorType {HOTSPOT};    // Sensor Type(CONTRAST, HOTSPOT)
   //double fieldOfRegard {};          // Field of Regard (steradians) (what can be in view to gimbals limits)
   //double fieldOfRegardTheta {};     // Field of Regard planar angle, where fieldOfRegard = 2 * pi * (1-cos(fieldOfRegardTheta/2)

   // above results in a simple cone.
   base::String* tmName {};          // Name of our track manager
   TrackManager* trackManager {};    // Our Track manager -- managed by the onboard computer

   //double azimuthBin {};           // minimum azimuth we can distinguish -- two signals whose
                                     // azimuth differs by less than this will be merged

   //double elevationBin {};         // minimum azimuth we can distinguish -- two signals whose
                                     // azimuth differs by less than this will be merged

   double maximumRange {};           // max sensor range.

private:
   // slot table helper methods
   bool setSlotLowerWavelength(const base::Number* const);   // Sets lower wavelength
   bool setSlotUpperWavelength(const base::Number* const);   // Sets upper wavelength
   bool setSlotNEI(const base::Number* const);               // Sets Noise Equivalent Irradiance
   bool setSlotThreshold(const base::Number* const);         // Sets Signal to Noise Threshold
   bool setSlotIFOV(const base::Number* const);              // Sets Instantaneous Field of View
   bool setSlotSensorType(const base::String* const);        // Sets the Sensor Type
   //bool setSlotFieldOfRegard(const base::Number* const);   // Sets the field of regard
   //bool setSlotAzimuthBin(const base::Number* const);      // Sets the Azimuth Bin
   //bool setSlotElevationBin(const base::Number* const);    // Sets the Elevation Bin
   bool setSlotMaximumRange(const base::Number* const);      // Sets the Maximum Range
   bool setSlotTrackManagerName(base::String* const);        // Sets our track manager by name
};

}
}

#endif

