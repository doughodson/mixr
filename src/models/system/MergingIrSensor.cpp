
#include "mixr/models/system/MergingIrSensor.hpp"

#include "mixr/models/player/Player.hpp"
#include "mixr/models/system/IrSeeker.hpp"
#include "mixr/models/system/trackmanager/AngleOnlyTrackManager.hpp"
#include "mixr/models/system/trackmanager/AirAngleOnlyTrkMgrPT.hpp"
#include "mixr/models/system/OnboardComputer.hpp"
#include "mixr/models/environment/IrAtmosphere.hpp"
#include "mixr/models/IrQueryMsg.hpp"

#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/units/lengths.hpp"
#include "mixr/base/units/angles.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(MergingIrSensor, "MergingIrSensor")
EMPTY_DELETEDATA(MergingIrSensor)

BEGIN_SLOTTABLE(MergingIrSensor)
   "azimuthBin",        // 1: azimuthBin
   "elevationBin",      // 2: elevationBin
END_SLOTTABLE(MergingIrSensor)

BEGIN_SLOT_MAP(MergingIrSensor)
   ON_SLOT(1, setSlotAzimuthBin,   base::Number)
   ON_SLOT(1, setSlotAzimuthBin,   base::Angle)
   ON_SLOT(2, setSlotElevationBin, base::Number)
   ON_SLOT(2, setSlotElevationBin, base::Angle)
END_SLOT_MAP()

MergingIrSensor::MergingIrSensor()
{
   STANDARD_CONSTRUCTOR()
}

void MergingIrSensor::copyData(const MergingIrSensor& org, const bool)
{
   BaseClass::copyData(org);
   azimuthBin = org.azimuthBin;
   elevationBin = org.elevationBin;
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void MergingIrSensor::reset()
{
   BaseClass::reset();

   // check the type of our track manager - we need to use the AirAngleOnlyTrkMgrPT
   if (getTrackManager() != nullptr) {
      const auto a = dynamic_cast<const AirAngleOnlyTrkMgrPT*>(getTrackManager());
      if (a == nullptr) {
         if (isMessageEnabled(MSG_WARNING)) {
            std::cerr << "MergingIrSensor::reset() : track manager is not an AirAngleOnlyTrkMgrPT" << std::endl;
         }
      }
   }
}

void MergingIrSensor::receive(const double dt)
{
   BaseClass::receive(dt);
   mergeIrReturns();
}

void MergingIrSensor::mergeIrReturns()
{
   int numRecords = storedMessagesQueue.entries();
   if (numRecords > 0) {

      //const auto deleteArray = new int [numRecords];
      //if (deleteArray == 0) {
      //   if (isMessageEnabled(MSG_ERROR)) {
      //      std::cerr << "Error: Allocation memory failure in IrSensor::mergeIrReturns" << std::endl;
      //   }
      //}
      //else {
         //for (int i=0; i < numRecords; i++) {
         //   deleteArray[i] = 0;
         //}

         base::lock(storedMessagesLock);
         // Traverse the stored message queue using peek (). Examine every
         // message. Compare every stored message against every OTHER stored
         // message. If the two are too close together, merge the two signals
         // and mark the second message in the delete array.
         // Proceed through the loop, ignoring all messages marked "deleted"
         // in the delete array.
         numRecords = storedMessagesQueue.entries();

         if (isMessageEnabled(MSG_DEBUG)) {
            std::cout << "IrSensor: numRecords returned " << numRecords << std::endl;
         }

         for (int i=0; i < numRecords; i++) {
               //if (deleteArray[i] == 0) {  // Do not bother processing those marked
               // for deletion -- these have already been
               // merged and must be ignored.

            IrQueryMsg* currentMsg{storedMessagesQueue.peek0(i)};

            // Do not bother processing those marked
            // for deletion -- these have already been
            // merged and must be ignored.
            if (currentMsg->getQueryMergeStatus()!= IrQueryMsg::MERGED_OUT) {

               for (int j = i+1; j < numRecords; j++) {

                  IrQueryMsg* nextMsg{storedMessagesQueue.peek0(j)};
                  double azimuthDelta{currentMsg->getRelativeAzimuth() - nextMsg->getRelativeAzimuth()};
                  double elevationDelta{currentMsg->getRelativeElevation() - nextMsg->getRelativeElevation()};

                  if (azimuthDelta < 0)
                     azimuthDelta = -azimuthDelta;

                  if (elevationDelta < 0)
                     elevationDelta = -elevationDelta;

                  if ((azimuthDelta < azimuthBin) &&
                     (elevationDelta < elevationBin)) { // two signals are too close together
                    // for the sensor to distinguish between them;
                    // we will merge the two signals based
                    // on their weighted signal-to-noise.
                    double currentRatio{};
                    double nextRatio{};

                    // find current ratio.
                    if (isMessageEnabled(MSG_DEBUG)) {
                       std::cout << "IrSensor: merging target " <<  nextMsg->getTarget()->getName()
                                 << " into target " <<currentMsg->getTarget()->getName()  << std::endl;
                    }

                    if (currentMsg->getSignalToNoiseRatio() >
                       currentMsg->getBackgroundNoiseRatio()) {

                          currentRatio = currentMsg->getSignalToNoiseRatio() +
                             currentMsg->getBackgroundNoiseRatio();

                    } else {
                       if (currentMsg->getSignalToNoiseRatio() < 0) {
                          currentRatio = -currentMsg->getSignalToNoiseRatio() -
                             currentMsg->getBackgroundNoiseRatio();
                       } else {
                          currentRatio = -currentMsg->getSignalToNoiseRatio() -
                             currentMsg->getBackgroundNoiseRatio();
                       } // signaltonoise < 0

                    } // if current signal > background

                    //now do the same thing for the next message.
                    if (nextMsg->getSignalToNoiseRatio() >
                       nextMsg->getBackgroundNoiseRatio()) {
                          nextRatio = nextMsg->getSignalToNoiseRatio() +
                             nextMsg->getBackgroundNoiseRatio();
                    } else {
                       if (nextMsg->getSignalToNoiseRatio() < 0) {
                          nextRatio = -nextMsg->getSignalToNoiseRatio() -
                             nextMsg->getBackgroundNoiseRatio();
                       } else {
                          nextRatio = -nextMsg->getSignalToNoiseRatio() -
                             nextMsg->getBackgroundNoiseRatio();
                       } // signaltonoise < 0

                    } // if next signal > background

                    // use ratios to find weights.
                    double sumRatio{currentRatio + nextRatio};

                    const double currentWeight{currentRatio / sumRatio};
                    const double nextWeight{1.0 - currentWeight};

                    //combine line-of-sight vector using weights
                    currentMsg->setLosVec((currentMsg->getLosVec() * currentWeight) +
                       (nextMsg->getLosVec() * nextWeight));

                    // combine position
                    currentMsg->setPosVec((currentMsg->getPosVec() * currentWeight) +
                       (nextMsg->getPosVec() * nextWeight));

                    // combine velocity
                    currentMsg->setVelocityVec((currentMsg->getVelocityVec() * currentWeight) +
                       (nextMsg->getVelocityVec() * nextWeight));

                    // combine acceleration
                    currentMsg->setAccelVec((currentMsg->getAccelVec() * currentWeight) +
                       (nextMsg->getAccelVec() * nextWeight));

                    // combine signal to noise ratios.
                    sumRatio = sumRatio - currentMsg->getBackgroundNoiseRatio();
                    if (sumRatio < 0)
                       sumRatio = -sumRatio;

                    currentMsg->setSignalToNoiseRatio(sumRatio);

                    //combine Azimuth and Elevation.
                    currentMsg->setAzimuthAoi((currentMsg->getAzimuthAoi() * currentWeight) +
                       nextMsg->getAzimuthAoi() * nextWeight);

                    currentMsg->setElevationAoi((currentMsg->getElevationAoi()* currentWeight) +
                       (nextMsg->getElevationAoi() * nextWeight));

                    currentMsg->setAngleAspect((currentMsg->getAngleAspect() * currentWeight) +
                       (nextMsg->getAngleAspect() * nextWeight));

                    currentMsg->setRelativeAzimuth((currentMsg->getRelativeAzimuth() * currentWeight) +
                       (nextMsg->getRelativeAzimuth() * nextWeight));

                    currentMsg->setRelativeElevation((currentMsg->getRelativeElevation() * currentWeight) +
                       (nextMsg->getRelativeElevation() * nextWeight));

                    // signal that this report has merged targets
                    currentMsg->setQueryMergeStatus(IrQueryMsg::MERGED);
                    nextMsg->setQueryMergeStatus(IrQueryMsg::MERGED_OUT);

                    //deleteArray[j] = 1;  // now that we have merged this signal with the
                    // Ith signal, it must be deleted. It will not
                    // be passed on to the track manager.

                    //if (isMessageEnabled(MSG_INFO)) {
                    //std::cout << "IrSensor: End Merge" << std::endl;
                    //}

                  } // if we merge
               } // end for j = i + 1;
            } // End if delete Array
            else { // debug - this target ws merged into another
               int x=0;
               x=x+1;
            }
         } // end for i = 0;
         base::unlock(storedMessagesLock);
         //delete[] deleteArray;
      //} // newArray is not null.
   } // numRecords > 0
}

// setAzimuthBin() - Sets the lower Azimuth Bin
bool MergingIrSensor::setAzimuthBin(const double w)
{
   azimuthBin = w;
   return true;
}

// setElevationBin() - Sets the lower Elevation Bin
bool MergingIrSensor::setElevationBin(const double w)
{
   elevationBin = w;
   return true;
}

bool MergingIrSensor::setSlotAzimuthBin(const base::Number* const x)
{
   double value{};

   if (x != nullptr) {
      value = x->asDouble();
   }
   setAzimuthBin(value);
   return true;
}

bool MergingIrSensor::setSlotAzimuthBin(const base::Angle* const x)
{
   double value{};

   if (x != nullptr) {
      value = x->getValueInRadians();
   }
   setAzimuthBin(value);
   return true;
}

bool MergingIrSensor::setSlotElevationBin(const base::Number* const x)
{
   double value{};

   if (x != nullptr) {
      value = x->asDouble();
   }
   setElevationBin(value);
   return true;
}

bool MergingIrSensor::setSlotElevationBin(const base::Angle* const x)
{
   double value{};

   if (x != nullptr) {
      value = x->getValueInRadians();
   }
   setElevationBin(value);
   return true;
}

}
}

