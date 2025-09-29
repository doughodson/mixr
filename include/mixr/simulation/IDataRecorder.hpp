
#ifndef __mixr_simulation_IDataRecorder_HPP__
#define __mixr_simulation_IDataRecorder_HPP__

#include "mixr/simulation/IRecorder.hpp"
#include "mixr/simulation/recorder_macros.hpp"

namespace mixr {
namespace simulation {
class ISimulation;
class IStation;

//------------------------------------------------------------------------------
// Class: IDataRecorder
// Description: Interface to a data recorder
//    1) This as an interface class for the data recorder that acts as a stub class
//       for implementing the recording 'hooks' in the simulation code.
//
//    2) The actual data recorder is implemented by the derived class
//
//    3) Recorded data records are defined by their "recorder event id" tokens;
//       (see mixr/simulation/dataRecorderTokens.hpp)
//------------------------------------------------------------------------------
class IDataRecorder : public IRecorder
{
   DECLARE_SUBCLASS(IDataRecorder, IRecorder)

public:
   IDataRecorder();

   IStation* getStation();                    // Our parent station
   const IStation* getStation() const;        // Our parent station (const version)

   ISimulation* getSimulation();               // The executable simulation
   const ISimulation* getSimulation() const;   // The executable simulation (const version)

   // Record Data function
   bool recordData(
      const unsigned int id,                  // Recorder event Id
      const base::IObject* pObjects[4],       // Sample objects
      const double values[4]                  // Sample values
   );

   // Background thread processing of the data records
   virtual void processRecords();

protected:
   // Implementation of the record data function
   virtual bool recordDataImp(
      const unsigned int id,                  // Recorder event Id
      const base::IObject* pObjects[4],       // Sample objects
      const double values[4]                  // Sample values
   );

   // Process the unhandled or unknown recorder event IDs
   virtual bool processUnhandledId(const unsigned int id) =0;

private:
   IStation* getStationImp();
   ISimulation* getSimulationImp();

   IStation* sta{};         // The station that owns us (not ref()'d)
   ISimulation* sim{};      // The simulation system (not ref()'d)
};

// Record Data function
inline bool IDataRecorder::recordData(
      const unsigned int id,
      const base::IObject* pObjects[4],
      const double values[4]
   )
{
   bool recorded{};
   if (isDataEnabled(id)) {
      recorded = recordDataImp(id, pObjects, values);
      if (!recorded) processUnhandledId(id);
   }
   return recorded;
}

}
}

#endif
