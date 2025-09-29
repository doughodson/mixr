
#include "mixr/simulation/IDataRecorder.hpp"

#include "mixr/simulation/ISimulation.hpp"
#include "mixr/simulation/IStation.hpp"

#include "mixr/base/IList.hpp"
#include <iostream>

namespace mixr {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(IDataRecorder, "IDataRecorder")
EMPTY_SLOTTABLE(IDataRecorder)

IDataRecorder::IDataRecorder()
{
   STANDARD_CONSTRUCTOR()
}

void IDataRecorder::copyData(const IDataRecorder& org, const bool)
{
   BaseClass::copyData(org);

   sta = nullptr;
   sim = nullptr;
}

void IDataRecorder::deleteData()
{
   sta = nullptr;
   sim = nullptr;
}

//------------------------------------------------------------------------------
// Background thread processing of the data records
//------------------------------------------------------------------------------
void IDataRecorder::processRecords()
{
   // nothing to do at this level
}

// Our parent Station
IStation* IDataRecorder::getStation()
{
   if (sta == nullptr) {
      getStationImp();
   }
   return sta;
}

// Our parent Station (const version)
const IStation* IDataRecorder::getStation() const
{
   if (sta == nullptr) {
      (const_cast<IDataRecorder*>(this))->getStationImp();
   }
   return sta;
}

// Find our parent Station
IStation* IDataRecorder::getStationImp()
{
   if (sta == nullptr) {
      sta = static_cast<IStation*>(findContainerByType(typeid(IStation)));
      if (sta == nullptr && isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Datarecorder::getStationImp(): ERROR, unable to locate the Station class!" << std::endl;
      }
   }
   return sta;
}

// the simulation
ISimulation* IDataRecorder::getSimulation()
{
   if (sim == nullptr) {
      getSimulationImp();
   }
   return sim;
}

const ISimulation* IDataRecorder::getSimulation() const
{
   if (sim == nullptr) {
      (const_cast<IDataRecorder*>(this))->getSimulationImp();
   }
   return sim;
}

// The simulation
ISimulation* IDataRecorder::getSimulationImp()
{
   if (sim == nullptr) {
      IStation* p = getStation();
      if (p != nullptr) sim = p->getSimulation();
   }
   return sim;
}


bool IDataRecorder::recordDataImp(const unsigned int id, const base::IObject* pObjects[4], const double values[4])
{
   return true;
}

}
}
