
#ifndef __mixr_models_common_IIrSignature_HPP__
#define __mixr_models_common_IIrSignature_HPP__

#include "mixr/base/IComponent.hpp"

namespace mixr {
namespace base { class IArea; class INumber; class List; class Table1; }
namespace models {
class IrQueryMsg;
class IIrShapeSignature;

//------------------------------------------------------------------------------
// Class: IIrSignature
//
// Descriptions: Interface class for IR signatures
//
// Factory name: IIrSignature
// Slots:
//    binSizes            <Table1>             ! The size of the wavelength bins used in the tables
//    irShapeSignature    <IIrShapeSignature>  ! Shape of radiation source
//    baseHeatSignature   <INumber>            ! Base heat signature - units: watts per steradian
//    emissivity          <INumber>            ! Emissivity - unitless, values range from
//                                             !     0.0 (total reflection) to 1.0 (total absorption)
//    effectiveArea       <base::IArea>        ! Effective area - units: meters squared (m^2)
//
// Notes:
//    1) Simple IR signature class, specifies player-specific properties related to IR modeling.
//    2) If no signature area provided, signature is simply baseHeatSignature*emissivity/radius^2
//    3) An irAtmosphere class is not required
//    4) If signature area is specified and if irAtmosphere used can calculate background,
//       then backgroundBlockedByTarget can be calculated (by irAtmosphere)
//    5) Signature area can be specified either by effectiveArea or by irShapeSignature,
//       where irShapeSignature is used if both are provided
//    6) This specific class really only supports a single band, since it provides no logic for calculating
//       different signature contributions for different bands
//    7) This simple signature class is compatible with a multi-band atmosphere-
//       if a multi-band atmosphere is specified, that atmosphere class should distribute
//       signature through bands
//
// Example Input:
//
//   ( IrSignature
//
//      irShapeSignature:
//         ( IrBoxSignature
//            x: (Meters 1.0)        // dimensions of source shape
//            y: (Meters 1.0)
//            z: (Meters 1.0)
//         )
//      baseHeatSignature: 320.0
//      emissivity: 0.75
//      effectiveArea: (SquareMeters 1.0)
//   )
//
//------------------------------------------------------------------------------
class IIrSignature : public base::IComponent
{
    DECLARE_SUBCLASS(IIrSignature, base::IComponent)

public:

   IIrSignature();

   // IrSignature class interface
   virtual bool getIrSignature(IrQueryMsg* const);
   virtual double getSignatureArea(IrQueryMsg*);

protected:

   // get the number of waveband bins
   int getNumWaveBands()                         { return numWaveBands; };

   // get the waveband centers from each bin
   const double* getWaveBandCenters() const;

   // get the waveband widths from each bin
   const double* getWaveBandWidths() const;

   // return the lowest wavelength for which data for the atmosphere is required.
   //        It is higher of the bottom sensor waveband and the lowest waveband represented by the atmosphere.
   double getLowerEndOfWavelengthOverlap(
      const double lowerRadiationWaveband, // Lower end of the wavebands represented by the atmosphere (microns)
      const double lowerSensorWaveband     // Lower end of the sensor waveband (microns)
   ) const
   {
     return ((lowerRadiationWaveband > lowerSensorWaveband) ? lowerRadiationWaveband : lowerSensorWaveband);
   }

   // return the highest wavelength for which data for the atmosphere is required.
   //        It is lower of the top of the sensor waveband and the highest waveband represented by the atmosphere.
   double getUpperEndOfWavelengthOverlap(
      const double upperRadiationWaveband, // Upper end of the wavebands represented by the atmosphere (microns)
      const double upperSensorWaveband     // Upper end of the sensor waveband (microns)
   ) const
   {
     return ((upperRadiationWaveband < upperSensorWaveband) ? upperRadiationWaveband : upperSensorWaveband);
   }

   // Base heat signature (Watts per steradian)
   double getBaseHeatSignature() const                    { return baseHeatSignature; }

   //Set the Base heat signature (Watts per steradian)
   bool setBaseHeatSignature(const double x)              { baseHeatSignature = x; return true; }

   // The emissivity for the player
   double getEmissivity() const                           { return emissivity; }

   // Set the emissivity for the player
   bool setEmissivity(const double x)                     { emissivity = x; return true; }

   // The effective area for heat calculations (m^2)
   double getEffectiveArea() const                        { return effectiveArea; }

   // Set the effective area for heat calculations (m^2)
   bool setEffectiveArea(const double x)                  { effectiveArea = x; return true; }

   double getCalculatedHeatSignature() const              { return baseHeatSignature * emissivity; }

private:

   int numWaveBands{};                     // Total number of wavebands
   const base::Table1* waveBandTable{};    // Table of waveband centers and widths
   IIrShapeSignature* irShapeSignature{};  // Shape of this IR signature
   double baseHeatSignature{};             // The base heat signature   (Watts per steradian)
   double emissivity{};                    // The emissivity for the player, what part of incident radiation
                                           // is absorbed. The value ranges from 0.0 for total reflection to
                                           // 1.0 for total absorption.
   double effectiveArea{1e-12};            // The effective area for heat calculations (m^2) (near zero, but non-zero)

private:
   // slot table helper methods
   bool setSlotWaveBandSizes(const base::Table1* const);
   bool setSlotIrShapeSignature(IIrShapeSignature* const);
   bool setSlotBaseHeatSignature(base::INumber* const);
   bool setSlotEmissivity(base::INumber* const);
   bool setSlotEffectiveArea(base::IArea* const);
};

}
}

#endif
