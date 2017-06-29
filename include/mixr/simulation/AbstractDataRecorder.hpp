
#ifndef __mixr_simulation_AbstractDataRecorder_H__
#define __mixr_simulation_AbstractDataRecorder_H__

#include "mixr/simulation/AbstractRecorderComponent.hpp"
#include "mixr/simulation/recorder_macros.hpp"

namespace mixr {
namespace simulation {
class Simulation;
class Station;

//------------------------------------------------------------------------------
// Class: AbstractDataRecorder
// Description: Abstract data recorder
//    1) This as an abstract class for the data record that acts as a stub class
//       for implementing the recording 'hooks' in the simulation code.
//
//    2) The actual data recorder is implemented by the derived class
//       mixr::recorder::DataRecorder (see "mixr/recorder/DataRecorder.hpp")
//
//    3) Recorded data records are defined by their "recorder event id" tokens;
//       (see mixr/simulation/dataRecorderTokens.hpp)
//------------------------------------------------------------------------------
class AbstractDataRecorder : public AbstractRecorderComponent
{
   DECLARE_SUBCLASS(AbstractDataRecorder, AbstractRecorderComponent)

public:
   AbstractDataRecorder();

   Station* getStation();                     // Our parent station
   const Station* getStation() const;         // Our parent station (const version)

   Simulation* getSimulation();               // The executable simulation
   const Simulation* getSimulation() const;   // The executable simulation (const version)

   // Record Data function
   bool recordData(
      const unsigned int id,                  // Recorder event Id
      const base::Object* pObjects[4],        // Sample objects
      const double values[4]                  // Sample values
   );

   // Background thread processing of the data records
   virtual void processRecords();

protected:
   // Implementation of the record data function
   virtual bool recordDataImp(
      const unsigned int id,                  // Recorder event Id
      const base::Object* pObjects[4],        // Sample objects
      const double values[4]                  // Sample values
   );

   // Process the unhandled or unknown recorder event IDs
   virtual bool processUnhandledId(const unsigned int id) =0;

private:
   Station* getStationImp();
   Simulation* getSimulationImp();

   Station* sta {};         // The station that owns us (not ref()'d)
   Simulation* sim {};      // The simulation system (not ref()'d)
};

// Record Data function
inline bool AbstractDataRecorder::recordData(
      const unsigned int id,
      const base::Object* pObjects[4],
      const double values[4]
   )
{
   bool recorded = false;
   if (isDataEnabled(id)) {
      recorded = recordDataImp(id, pObjects, values);
      if (!recorded) processUnhandledId(id);
   }
   return recorded;
}

}
}

#endif
