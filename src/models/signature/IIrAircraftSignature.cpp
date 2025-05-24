
#include "mixr/models/signature/IIrAircraftSignature.hpp"

#include "mixr/models/environment/IrAtmosphere.hpp"
#include "mixr/models/signature/IIrShapeSignature.hpp"
#include "mixr/models/IrQueryMsg.hpp"
#include "mixr/models/player/air/AirVehicle.hpp"

#include "mixr/models/WorldModel.hpp"

#include "mixr/base/relations/Table2.hpp"
#include "mixr/base/relations/Table4.hpp"
#include "mixr/base/relations/Table5.hpp"
#include "mixr/base/IList.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/units/areas.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IIrAircraftSignature, "IIrAircraftSignature")

BEGIN_SLOTTABLE(IIrAircraftSignature)
   "airframeSignatureTable",  // mapping of
            // signature to  pla, where pla is in % from 0 - 100
            // x of this table is the pla
            //  y is the velocity (in sim prevailing units --
            // knots, I believe) and z is altitude (in sim prevailing units --
            // meters, I believe). Data is the heat signature in watts per steradian.
   "airframeWavebandFactorTable",
            // airframe wavebands
            // x - waveband center
            // y - waveband width
            // data - factor. We multiply the base plume signature by this
            // factor to get the plume energy in this particular waveband.
            // the different factors should all sum to 1.0  .
   "plumeSignatureTable",
            // mapping of signature in watts/steradian to
            // x- PLA 0.0 - 2.0 (0.0=idle, 1.0=mil, 2.0=afterburner)
            // y - velocity (mach #)
            // z - altitude (meters)
            // azimuth (radians)
            // elevation (radians)
   "plumeWavebandFactorTable",
            // plume wavebands
            // x - waveband center
            // y - waveband width
            // data - factor. We multiply the base plume signature by this
            // factor to get the plume energy in this particular waveband.
            // the different factors should all sum to 1.0  .
   "hotPartsSignatureTable",
            // mapping of signature in watts/steradian to
            // x- pla
            // y - velocity (knots)
            // z - altitude (meters)
            // azimuth (radians)
            // elevation (radians)
   "hotPartsWavebandFactorTable",
            // plume wavebands
            // x - waveband center
            // y - waveband width
            // data - factor. We multiply the base plume signature by this
            // factor to get the plume energy in this particular waveband.
            // the different factors should all sum to 1.0  .

END_SLOTTABLE(IIrAircraftSignature)

BEGIN_SLOT_MAP(IIrAircraftSignature)
   ON_SLOT(1,setSlotAirframeSignatureTable,      base::Table4)
   ON_SLOT(2,setSlotAirframeWavebandFactorTable, base::Table2)
   ON_SLOT(3,setSlotPlumeSignatureTable,         base::Table5)
   ON_SLOT(4,setSlotPlumeWavebandFactorTable,    base::Table2)
   ON_SLOT(5,setSlotHotPartsSignatureTable,      base::Table5)
   ON_SLOT(6,setSlotHotPartsWavebandFactorTable, base::Table2)
END_SLOT_MAP()

IIrAircraftSignature::IIrAircraftSignature()
{
    STANDARD_CONSTRUCTOR()
}

void IIrAircraftSignature::copyData(const IIrAircraftSignature& org, const bool)
{
    BaseClass::copyData(org);

    if (org.airframeSignatureTable != nullptr) {
        base::Table4* copy = org.airframeSignatureTable->clone();
        setSlotAirframeSignatureTable( copy );
        copy->unref();
    } else {
        setSlotAirframeSignatureTable(nullptr);
    }

    if (org.airframeWavebandFactorTable != nullptr) {
        base::Table2* copy = org.airframeWavebandFactorTable->clone();
        setSlotAirframeWavebandFactorTable( copy );
        copy->unref();
    } else {
        setSlotAirframeWavebandFactorTable(nullptr);
    }

    if (org.plumeSignatureTable != nullptr) {
        base::Table5* copy = org.plumeSignatureTable->clone();
        setSlotPlumeSignatureTable( copy );
        copy->unref();
    } else {
        setSlotPlumeSignatureTable(nullptr);
    }

    if (org.plumeWavebandFactorTable != nullptr) {
        base::Table2* copy = org.plumeWavebandFactorTable->clone();
        setSlotPlumeWavebandFactorTable( copy );
        copy->unref();
    } else {
        setSlotPlumeWavebandFactorTable(nullptr);
    }

    if (org.hotPartsSignatureTable != nullptr) {
        base::Table5* copy = org.hotPartsSignatureTable->clone();
        setSlotHotPartsSignatureTable( copy );
        copy->unref();
    } else {
        setSlotHotPartsSignatureTable(nullptr);
    }

    if (org.hotPartsWavebandFactorTable != nullptr) {
        base::Table2* copy = org.hotPartsWavebandFactorTable->clone();
        setSlotHotPartsWavebandFactorTable( copy );
        copy->unref();
    } else {
        setSlotHotPartsWavebandFactorTable(nullptr);
    }

    BaseClass::copyData(org);
}

void IIrAircraftSignature::deleteData()
{
    if (airframeSignatureTable != nullptr) {
        airframeSignatureTable->unref();
        airframeSignatureTable = nullptr;
    }

    if (airframeWavebandFactorTable != nullptr) {
        airframeWavebandFactorTable->unref();
        airframeWavebandFactorTable = nullptr;
    }

    if (plumeSignatureTable != nullptr) {
        plumeSignatureTable->unref();
        plumeSignatureTable = nullptr;
    }

    if (plumeWavebandFactorTable != nullptr) {
        plumeWavebandFactorTable->unref();
        plumeWavebandFactorTable = nullptr;
    }

    if (hotPartsSignatureTable != nullptr) {
        hotPartsSignatureTable->unref();
        hotPartsSignatureTable = nullptr;
    }

    if (hotPartsWavebandFactorTable != nullptr) {
        hotPartsWavebandFactorTable->unref();
        hotPartsWavebandFactorTable = nullptr;
    }

    if (airframeSig !=nullptr ) {
        delete[] airframeSig;
        airframeSig = nullptr;
    }

    if (plumeSigs != nullptr) {
        delete[] plumeSigs;
        plumeSigs = nullptr;
    }

    if (hotPartsSigs != nullptr) {
        delete[] hotPartsSigs;
        hotPartsSigs = nullptr;
    }
}

//------------------------------------------------------------------------------
// getIrSignature() - Computes the IR signature for this IR query message
//------------------------------------------------------------------------------
bool IIrAircraftSignature::getIrSignature(IrQueryMsg* const msg)
{
    bool ok{};
    //const auto msg = dynamic_cast<IrQueryMsg*>( msg0 );
    if (msg != nullptr) {
        const double projectedAreaInFOV{getSignatureArea(msg)};
        msg->setProjectedArea(projectedAreaInFOV);

        // if no projectedAreaInFOV, then target was not in FOV
        if (projectedAreaInFOV > 0.0){
            ok = true;
            double* heatSignature{getHeatSignature(msg)};
            msg->setSignatureByWaveband(heatSignature);
            // FAB - set simple signature value
            msg->setSignatureAtRange(getCalculatedHeatSignature());
            msg->setEmissivity(getEmissivity());
        } else{
            msg->setSignatureByWaveband(nullptr);
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotAirframeSignatureTable()
//------------------------------------------------------------------------------
bool IIrAircraftSignature::setSlotAirframeSignatureTable(const base::Table4* const tbl)
{
   if (airframeSignatureTable != nullptr) {
      airframeSignatureTable->unref();
   }
   airframeSignatureTable = tbl;
   if (airframeSignatureTable != nullptr) {
      airframeSignatureTable->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotPlumeSignatureTable()
//------------------------------------------------------------------------------
bool IIrAircraftSignature::setSlotPlumeSignatureTable(const base::Table5* const tbl)
{
   if (plumeSignatureTable != nullptr) {
      plumeSignatureTable->unref();
   }
   plumeSignatureTable = tbl;
   if (plumeSignatureTable != nullptr) {
      plumeSignatureTable->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotAirframeWavebandFactorTable()
//------------------------------------------------------------------------------
bool IIrAircraftSignature::setSlotAirframeWavebandFactorTable(const base::Table2* const tbl)
{
   if (airframeWavebandFactorTable != nullptr) {
      airframeWavebandFactorTable->unref();
   }
   airframeWavebandFactorTable = tbl;
   if (airframeWavebandFactorTable != nullptr) {
      airframeWavebandFactorTable->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotPlumeWavebandFactorTable()
//------------------------------------------------------------------------------
bool IIrAircraftSignature::setSlotPlumeWavebandFactorTable(const base::Table2* const tbl)
{
   if (plumeWavebandFactorTable != nullptr) {
      plumeWavebandFactorTable->unref();
   }
   plumeWavebandFactorTable = tbl;
   if (plumeWavebandFactorTable != nullptr) {
      plumeWavebandFactorTable->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotHotPartsSignatureTable()
//------------------------------------------------------------------------------
bool IIrAircraftSignature::setSlotHotPartsSignatureTable(const base::Table5* const tbl)
{
   if (hotPartsSignatureTable != nullptr) {
      hotPartsSignatureTable->unref();
   }
   hotPartsSignatureTable = tbl;
   if (hotPartsSignatureTable != nullptr) {
      hotPartsSignatureTable->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotHotPartsWavebandFactorTable()
//------------------------------------------------------------------------------
bool IIrAircraftSignature::setSlotHotPartsWavebandFactorTable(const base::Table2* const tbl)
{
   if (hotPartsWavebandFactorTable != nullptr) {
      hotPartsWavebandFactorTable->unref();
   }
   hotPartsWavebandFactorTable = tbl;
   if (hotPartsWavebandFactorTable != nullptr) {
      hotPartsWavebandFactorTable->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// getAirframeSignature()
//------------------------------------------------------------------------------
double IIrAircraftSignature::getAirframeSignature(double velocity, double altitude, double azimuth, double elevation)
{
    return (airframeSignatureTable->lfi(velocity, altitude, azimuth, elevation));
}

//------------------------------------------------------------------------------
// getAirframeWavebandFactor()
//------------------------------------------------------------------------------
double IIrAircraftSignature::getAirframeWavebandFactor(double midpoint, double width)
{
    return (airframeWavebandFactorTable->lfi(midpoint, width));
}

//------------------------------------------------------------------------------
// getPlumeSignature()
//------------------------------------------------------------------------------
double IIrAircraftSignature::getPlumeSignature(double pla, double velocity, double altitude, double azimuth, double elevation)
{
    return (plumeSignatureTable->lfi(pla, velocity, altitude, azimuth, elevation));
}

//------------------------------------------------------------------------------
// getPlumeWavebandFactor()
//------------------------------------------------------------------------------
double IIrAircraftSignature::getPlumeWavebandFactor(double midpoint, double width)
{
    return (plumeWavebandFactorTable->lfi(midpoint, width));
}

//------------------------------------------------------------------------------
// getHotPartsSignature()
//------------------------------------------------------------------------------
double IIrAircraftSignature::getHotPartsSignature(double pla, double velocity, double altitude, double azimuth, double elevation)
{
   return (hotPartsSignatureTable->lfi(pla, velocity, altitude, azimuth, elevation));
}

//------------------------------------------------------------------------------
// getHotPartsWavebandFactor()
//------------------------------------------------------------------------------
double IIrAircraftSignature::getHotPartsWavebandFactor(double midpoint, double width)
{
   return (hotPartsWavebandFactorTable->lfi(midpoint, width));
}

//------------------------------------------------------------------------------
// getCalculatedAirframeHeatSignature()
//------------------------------------------------------------------------------
double IIrAircraftSignature::getCalculatedAirframeHeatSignature(const IrQueryMsg* const msg) {

    double irPower{};
    const auto targetAircraft = dynamic_cast<const AirVehicle*>(static_cast<const IPlayer*>(msg->getTarget()));
    if(targetAircraft != nullptr) {
        // this will need checks to ensure targetAircraft is , in fact,
        // an airvehicle and not something else.
        const double targetAlt{static_cast<double>(targetAircraft->getAltitudeM())};
        const double targetVel{targetAircraft->getMach()};
        double targetAzimuth{msg->getAzimuthAoi()};
        const double targetElevation{msg->getElevationAoi()};
        if (targetAzimuth < 0) {
            targetAzimuth = -targetAzimuth;
        }
        irPower = getAirframeSignature(targetVel, targetAlt, targetAzimuth, targetElevation);
    }
    return irPower;
}

// FAB -  parallel hotparts and plume
//------------------------------------------------------------------------------
// getAirframeSignatures()
//------------------------------------------------------------------------------
void IIrAircraftSignature::getAirframeSignatures(const IrQueryMsg* const msg, const double lowerBound, const double upperBound)
{
    if (airframeWavebandFactorTable != nullptr && airframeSignatureTable != nullptr) {
        // find airframe factor.
        const double* centerWavelengths{airframeWavebandFactorTable->getXData()};
        const double* widths{airframeWavebandFactorTable->getYData()};
        double irPower{getCalculatedAirframeHeatSignature(msg)};

        for (unsigned int i = 0; i < static_cast<unsigned int>(airframeWavebandFactorTable->getNumXPoints()); i++) {
            const double centerWavelength{centerWavelengths[i]};
            const double lowerWavelength{centerWavelength - (widths[i] / 2.0)};
            const double upperWavelength{lowerWavelength + widths[i]};

            airframeSig[i*3] = lowerWavelength;
            airframeSig[i*3 + 1] = upperWavelength;

            if (upperBound >= lowerWavelength && lowerBound <= upperWavelength) {
                // sensor band overlaps this bin
                const double airframeWaveBandFactor{airframeWavebandFactorTable->lfi(centerWavelength,widths[i])};
                airframeSig[i*3 + 2] = irPower * airframeWaveBandFactor;
            } else {
                airframeSig[i*3 + 2] = 0.0f;
            }
        }
    }
}

//------------------------------------------------------------------------------
// getPlumeRadiation()
//------------------------------------------------------------------------------
double IIrAircraftSignature::getPlumeRadiation(const IrQueryMsg* const msg)
{
    double irPower{};
    const IPlayer* targetAircraft{msg->getTarget()};
    if (targetAircraft != nullptr) {
        double currentPla{1.0};
        if (targetAircraft->isClassType(typeid(AirVehicle))) {
            currentPla = getPLA(static_cast<const AirVehicle*>(targetAircraft));
        }
        const double targetAlt{targetAircraft->getAltitudeM()};
        const double targetVel{targetAircraft->getMach()};
        double targetAzimuth{msg->getAzimuthAoi()};
        const double targetElevation = msg->getElevationAoi();
        if (targetAzimuth < 0) {
            targetAzimuth = -targetAzimuth;
        }

        irPower = getPlumeSignature(currentPla, targetVel, targetAlt, targetAzimuth, targetElevation);
    }
    return irPower;
}

//------------------------------------------------------------------------------
// getPlumeSignatures()
//------------------------------------------------------------------------------
void IIrAircraftSignature::getPlumeSignatures(const IrQueryMsg* const msg, const double lowerBound, const double upperBound)
{
    if (plumeWavebandFactorTable != nullptr) {
        const double* centerWavelengths{plumeWavebandFactorTable->getXData()};
        const double* widths{plumeWavebandFactorTable->getYData()};
        const double irPower{getPlumeRadiation(msg)};
        for (unsigned int i = 0; i < static_cast<unsigned int>(plumeWavebandFactorTable->getNumXPoints()); i++) {
            const double centerWavelength{centerWavelengths[i]};
            const double lowerWavelength{centerWavelength - (widths[i] / 2.0)};
            const double upperWavelength{lowerWavelength + widths[i]};
            plumeSigs[i*3] = lowerWavelength;
            plumeSigs[i*3 + 1] = upperWavelength;
            if (upperBound >= lowerWavelength && lowerBound <= upperWavelength) {
                // sensor band overlaps this bin
                const double plumeFactor{plumeWavebandFactorTable->lfi(centerWavelength,widths[i])};
                // plume has this factor contribution in this bin
                plumeSigs[i*3 + 2] = irPower * plumeFactor;
                // this number would represent the contribution if this bin were completely
                // contained by the sensor band.

                // FAB - overlapRatio could be implemented for each table like this -
                // but only one piece needed if it is desired that bins not all be the same for all tables.
                // also have to fix loop on i in heat signature

                // the overlap ratio is calculated to correct this number if the sensor band only
                // partially covers this bin.
                //double lowerOverlap = atmos->getLowerEndOfWavelengthOverlap(
                //                                               lowerWavelength,
                //                                               lowerBound);
                //double upperOverlap = atmos->getUpperEndOfWavelengthOverlap(
                //                                               upperWavelength,
                //                                               upperBound);
                //if (upperOverlap < lowerOverlap) upperOverlap = lowerOverlap;

                // how much of this bin is contained in the sensor band?
                //double overlapRatio = (upperOverlap - lowerOverlap)
                //                   (upperWavelength - lowerWavelength);

                //plumeSigs[i*3 + 2] *= overlapRatio
            } else {
                plumeSigs[i*3 + 2] = 0.0f;
            }
        }
    } // if plumeWavebandFactor != 0
}

//------------------------------------------------------------------------------
// getPLA() - Get the power level angle from the air vehicle. This represents a number
//    between 0.0 and 2.0. O.0 represents idle power on the engine, 1.0 represents
//    Mil Power, and 2.0 represents after burner
//------------------------------------------------------------------------------

double IIrAircraftSignature::getPLA(const AirVehicle* const airModel) {
    // use only for air vehicles.
    double currentPla{};
    double idleValue{};
    double milValue{};
    double maxValue{};
    double currentThrust{};

    airModel->getEngThrust(&currentThrust, 1);
    airModel->getEngThrustIdle(&idleValue, 1);
    airModel->getEngThrustMil(&milValue, 1);
    airModel->getEngThrustAb(&maxValue, 1);

    if (currentThrust < milValue) {
        currentPla = ((currentThrust - idleValue) / (milValue - idleValue));
    } else if (currentThrust == milValue) {
        currentPla = 1.0;
    } else if (currentThrust < maxValue) {
        currentPla = 1.0f + ((currentThrust - milValue) / (maxValue - milValue));
    } else {currentPla = 2.0;}

    return currentPla;

}

//------------------------------------------------------------------------------
// getHotPartsRadiation()
//------------------------------------------------------------------------------
double IIrAircraftSignature::getHotPartsRadiation(const IrQueryMsg* const msg)
{
    double currentPla{1.0};
    double targetAlt{};
    double targetVel{};

    const IPlayer* targetAircraft{msg->getTarget()};
    if (targetAircraft != nullptr) {
        if (targetAircraft->isClassType(typeid(AirVehicle))) {
            currentPla = getPLA(static_cast<const AirVehicle*>(targetAircraft));
            targetAlt = static_cast<double>(targetAircraft->getAltitudeM());
            targetVel = targetAircraft->getMach();
        }
    }

    double targetAzimuth{msg->getAzimuthAoi()};
    const double targetElevation{msg->getElevationAoi()};
    if (targetAzimuth < 0) targetAzimuth = -targetAzimuth;
    const double irPower{getHotPartsSignature(currentPla, targetVel, targetAlt, targetAzimuth, targetElevation)};

    return irPower;
}

//------------------------------------------------------------------------------
// getHotPartsSignatures()
//------------------------------------------------------------------------------
void IIrAircraftSignature::getHotPartsSignatures(const IrQueryMsg* const msg, const double lowerBound, const double upperBound)
{
    if (hotPartsWavebandFactorTable != nullptr) {
        const double* centerWavelengths{hotPartsWavebandFactorTable->getXData()};
        const double* widths{hotPartsWavebandFactorTable->getYData()};
        const double irPower{getHotPartsRadiation(msg)};
        for (unsigned int i = 0; i < static_cast<unsigned int>(hotPartsWavebandFactorTable->getNumXPoints()); i++) {
            const double centerWavelength{centerWavelengths[i]};
            const double lowerWavelength{centerWavelength - (widths[i] / 2.0)};
            const double upperWavelength{lowerWavelength + widths[i]};
            hotPartsSigs[i*3] = lowerWavelength;
            hotPartsSigs[i*3 + 1] = upperWavelength;
            if (upperBound >= lowerWavelength && lowerBound <= upperWavelength) {
                // sensor band overlaps this bin
                const double hotPartsFactor{hotPartsWavebandFactorTable->lfi(centerWavelength,widths[i])};
                hotPartsSigs[i*3 + 2] = irPower * hotPartsFactor;
            } else {
                hotPartsSigs[i*3 + 2] = 0.0f;
            }
        }
    } // if waveband is not null.
}

//------------------------------------------------------------------------------
// getHeatSignature() - Get the heat signature
//------------------------------------------------------------------------------
double* IIrAircraftSignature::getHeatSignature(IrQueryMsg* msg)
{
    IPlayer* target{msg->getTarget()};
    if (target != nullptr) {
        IrAtmosphere* atmos{};
        WorldModel* sim{target->getWorldModel()};
        if (sim != nullptr) {
            atmos = dynamic_cast<IrAtmosphere*>( sim->getAtmosphere() );
        }

        const int numBins{getNumWaveBands()};
        if (airframeSig == nullptr)  airframeSig = new double[numBins * 3];
        if (plumeSigs == nullptr)    plumeSigs = new double[numBins * 3];
        if (hotPartsSigs == nullptr) hotPartsSigs = new double[numBins * 3];

        //double reflectivity = 1.0f - getEmissivity();
        const double lowerBound{msg->getLowerWavelength()};
        const double upperBound{msg->getUpperWavelength()};

        if (atmos != nullptr) {
            if (atmos->getNumWaveBands() != getNumWaveBands()) {
                // warning message
            }
        }

        // apparently no emissivity factor in these airframe signatures
        getAirframeSignatures(msg, lowerBound, upperBound);
        getPlumeSignatures(msg, lowerBound, upperBound);
        getHotPartsSignatures(msg, lowerBound, upperBound);

        const double* centerWavelengths{getWaveBandCenters()};
        const double* widths{getWaveBandWidths()};
        //double totalWavelengthRange = ((centerWavelengths[getNumWaveBands() - 1] + (widths[getNumWaveBands() - 1] / 2.0f))-(centerWavelengths[0] - (widths[0] / 2.0f)));

        for (int i=0; i<getNumWaveBands(); i++) {
            // determine if our sensor band overlap this signature band
            double lowerBandBound{centerWavelengths[i] - (widths[i] / 2.0)};
            double upperBandBound{lowerBandBound + widths[i]};
            if (upperBound > lowerBandBound && lowerBound < upperBandBound) {

                // calculate how much of this wave band overlaps the sensor limits
                const double lowerOverlap{getLowerEndOfWavelengthOverlap(lowerBandBound, lowerBound)};
                double upperOverlap{getUpperEndOfWavelengthOverlap(upperBandBound, upperBound)};
                if (upperOverlap < lowerOverlap)
                    upperOverlap = lowerOverlap;

                const double overlapRatio{(upperOverlap - lowerOverlap) / (upperBandBound - lowerBandBound)};

                // get our main signature piece - airframe
                double baseHeatSignatureInBand{airframeSig[i*3 + 2]};

                //if (isMessageEnabled(MSG_INFO)) {
                //std::cout << "For wavelength " << currentWavelength << " Airframe Heat Signature: " << baseHeatSignatureInBand << std::endl;
                //std::cout << "For wavelength " << currentWavelength << " Plume Signature: " << plumeSigs[i *3 +2] << std::endl;
                //std::cout << "For wavelength " << currentWavelength << " Hot Parts: " << hotPartsSigs[i * 3 + 2] << std::endl;
                //}

                // assume plume bins & hot parts bins are same as airframe bins, so we
                // can re-use the same overlap ratios and fractions. If not,
                // they will have to be separately calculated.
                baseHeatSignatureInBand += plumeSigs[i*3 + 2];
                baseHeatSignatureInBand += hotPartsSigs[i*3 + 2];

                // if we have an atmosphere model, get the reflected solar radiation contribution
                // Solar signature is solar value * reflectivity i.e. (1 - emissivity)
                // use of reflectivity here suggests that this is solar radiation reflected from the target
                // this now done in the atmosphere model, during query return processing
                //if (atmos != nullptr)
                //   baseHeatSignatureInBand += (reflectivity * atmos->getSolarRadiation(centerWavelengths[i], (double) target->getAltitudeM()));

                airframeSig[i*3 + 2] = baseHeatSignatureInBand * overlapRatio;
            }
        } // for loop over waveBand
    } // if (target != nullptr)
    return airframeSig;
}

}
}
