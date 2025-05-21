
#ifndef __mixr_models_common_IIrAircraftSignature_HPP__
#define __mixr_models_common_IIrAircraftSignature_HPP__

#include "mixr/models/signature/IIrSignature.hpp"

namespace mixr {
namespace base { class Number; class List; class Table1; class Table2;
                 class Table3; class Table4; class Table5; }
namespace models {
class AirVehicle;
class IrQueryMsg;
class IrShape;

//------------------------------------------------------------------------------
// Class: IIrAircraftSignature
//
// Descriptions: Interface class for an aircraft IR signature
//
//
// Factory name: IIrAircraftSignature
// Slots:
//    airframeSignatureTable          <Table4>      !
//    airframeWavebandFactorTable     <Table2>      !
//    plumeSignatureTable             <Table5>      !
//    plumeWavebandFactorTable        <Table2>      !
//    hotPartsSignatureTable          <Table5>      !
//    hotPartsWavebandFactorTable     <Table2>      !
//
// Public member functions:
//      double getIrSignature(IrQueryMsg* msg)
//          Computes the IR signature for the emission.
//------------------------------------------------------------------------------
class IIrAircraftSignature: public IIrSignature
{
    DECLARE_SUBCLASS(IIrAircraftSignature, IIrSignature)

public:
   IIrAircraftSignature();

   bool getIrSignature(IrQueryMsg* const) override;

protected:
   virtual double* getHeatSignature(IrQueryMsg*);

   virtual double getAirframeSignature(double velocity, double altitude, double azimuth, double elevation);
   virtual double getAirframeWavebandFactor(double midpoint, double width);

   virtual double getPlumeSignature(double pla, double velocity, double altitude, double azimuth, double elevation);
   virtual double getPlumeWavebandFactor(double midpoint, double width);

   virtual double getHotPartsSignature(double pla, double velocity, double altitude, double azimuth, double elevation);
   virtual double getHotPartsWavebandFactor(double midpoint, double width);

   virtual double getCalculatedAirframeHeatSignature(const IrQueryMsg* const);

   // FAB - added to parallel plume and hotparts calculations
   virtual void getAirframeSignatures(const IrQueryMsg* const msg, const double lowerBound, const double upperBound);

   virtual double getPlumeRadiation(const IrQueryMsg* const);

   virtual void getPlumeSignatures(const IrQueryMsg* const msg,
                                   const double lowerBound,
                                   const double upperBound);

   virtual double getHotPartsRadiation(const IrQueryMsg* const);

   virtual void getHotPartsSignatures(const IrQueryMsg* const msg,
                                      const double lowerBound,
                                      const double upperBound);

   virtual double getPLA(const AirVehicle* const airModel);

private:
   const base::Table4* airframeSignatureTable {};
            // mapping of
            // signature  x is the velocity (in mach #) and y is altitude (in sim prevailing units --
            // meters, I believe), z is azimuth in radians, za is
            // elevation in radians. Data is the heat signature in watts per steradian.

   const base::Table2*  airframeWavebandFactorTable {};
            // airframe wavebands
            // x - waveband center
            // y - waveband width
            // data - factor. We multiply the base airframe signature by this
            // factor to get the plume energy in this particular waveband.
            // the different factors should all sum to 1.0  .

   const base::Table5* plumeSignatureTable {};
            // mapping of signature in watts/steradian to
            // x- pla % from 0 - 100.
            // y - velocity (mach #)
            // z - altitude (meters)
            // azimuth (radians)
            // elevation (radians)

   const base::Table2* plumeWavebandFactorTable {};
            // plume wavebands
            // x - waveband center
            // y - waveband width
            // data - factor. We multiply the base plume signature by this
            // factor to get the plume energy in this particular waveband.
            // the different factors should all sum to 1.0

   const base::Table5* hotPartsSignatureTable {};
            // mapping of signature in watts/steradian to
            // x- pla % from 0 - 100.
            // y - velocity (knots)
            // z - altitude (meters)
            // azimuth (radians)
            // elevation (radians)

   const base::Table2* hotPartsWavebandFactorTable {};
            // plume wavebands
            // x - waveband center
            // y - waveband width
            // data - factor. We multiply the base plume signature by this
            // factor to get the plume energy in this particular waveband.
            // the different factors should all sum to 1.0  .

   double* airframeSig {};       // 2 dimensions i = bin, j = lower wavelength, upper wavelength, signature
   double* plumeSigs {};         // 2 dimensions i = bin, j = lower wavelength, upper wavelength, signature
   double* hotPartsSigs {};      // 2 dimensions i = bin, j = lower wavelength, upper wavelength, signature

private:
   // slot table helper methods
   bool setSlotAirframeSignatureTable(const base::Table4* const);
   bool setSlotAirframeWavebandFactorTable(const base::Table2* const);
   bool setSlotPlumeSignatureTable(const base::Table5* const);
   bool setSlotPlumeWavebandFactorTable(const base::Table2* const);
   bool setSlotHotPartsSignatureTable(const base::Table5* const);
   bool setSlotHotPartsWavebandFactorTable(const base::Table2* const);
};

}
}

#endif
