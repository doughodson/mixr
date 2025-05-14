
#include "mixr/models/signature/IIrSignature.hpp"

#include "mixr/models/environment/IrAtmosphere.hpp"

#include "mixr/models/signature/IIrShape.hpp"
#include "mixr/models/IrQueryMsg.hpp"
#include "mixr/models/system/IrSensor.hpp"

#include "mixr/base/relations/Table1.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/units/areas.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IIrSignature, "IIrSignature")

BEGIN_SLOTTABLE(IIrSignature)
   "binSizes",                // 1 The size of the wavelength bins used in the tables
   "irShapeSignature",        // 2 IR shape
   "baseHeatSignature",       // 3 Base heat signature in watts per steradian
   "emissivity",              // 4 Emissivity
   "effectiveArea",           // 5 Effective area
END_SLOTTABLE(IIrSignature)

BEGIN_SLOT_MAP(IIrSignature)
   ON_SLOT(1, setSlotWaveBandSizes,     base::Table1)
   ON_SLOT(2, setSlotIrShapeSignature,  IIrShape)
   ON_SLOT(3, setSlotBaseHeatSignature, base::INumber)
   ON_SLOT(4, setSlotEmissivity,        base::INumber)
   ON_SLOT(5, setSlotEffectiveArea,     base::IArea)
END_SLOT_MAP()

IIrSignature::IIrSignature()
{
   STANDARD_CONSTRUCTOR()
   // create a default waveband, middle infrared band : 3-5 micron (100 THz - 60 THz)
   //double xdata[8] = { 4.0f };
   //double gdata[8] = { 2.0f };
   //waveBandTable = new base::Table1(gdata, 1, xdata, 1);
   //numWaveBands=1;
}

void IIrSignature::copyData(const IIrSignature& org, const bool)
{
   BaseClass::copyData(org);

   baseHeatSignature = org.baseHeatSignature;
   emissivity        = org.emissivity;
   effectiveArea     = org.effectiveArea;

   if (org.waveBandTable != nullptr) {
      base::Table1* copy{org.waveBandTable->clone()};
      setSlotWaveBandSizes( copy );
      copy->unref();
   } else {
      setSlotWaveBandSizes(nullptr);
   }

   if (org.irShapeSignature != nullptr) {
      IIrShape* copy{org.irShapeSignature->clone()};
      setSlotIrShapeSignature( copy );
      copy->unref();
   } else {
      setSlotIrShapeSignature(nullptr);
   }
}

void IIrSignature::deleteData()
{
   if (irShapeSignature != nullptr)   { irShapeSignature->unref(); irShapeSignature = nullptr; }

   if (waveBandTable != nullptr) {
      waveBandTable->unref();
      waveBandTable = nullptr;
      numWaveBands = 0;
   }
}

//------------------------------------------------------------------------------
// setSlotBinSizes() --  set number of separate bands
//------------------------------------------------------------------------------
bool IIrSignature::setSlotWaveBandSizes(const base::Table1* const tbl)
{
   if (waveBandTable != nullptr) {
      waveBandTable->unref();
      waveBandTable = nullptr;
      numWaveBands = 0;
   }
   if (tbl != nullptr) {
      waveBandTable = tbl;
      tbl->ref();
      numWaveBands = tbl->getNumXPoints();
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotIrShapeSignature() --  set IR shape of the signature
//------------------------------------------------------------------------------
bool IIrSignature::setSlotIrShapeSignature(IIrShape* const s)
{
   if (irShapeSignature != nullptr) {
      irShapeSignature->unref();
      irShapeSignature = nullptr;
   }
   if (s != nullptr) {
      irShapeSignature = s;
      irShapeSignature->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotBaseHeatSignature() -- set base heat signature
//------------------------------------------------------------------------------
bool IIrSignature::setSlotBaseHeatSignature(base::INumber* const num)
{
   bool ok{};
   if (num != nullptr) {
      const double x{num->asDouble()};
      ok = setBaseHeatSignature(x);
      if (!ok) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IrSignature::setBaseHeatSignature: Error setting Base Heat Signature!" << std::endl;
         }
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// setSlotEmissivity() -- set emissivity
//------------------------------------------------------------------------------
bool IIrSignature::setSlotEmissivity(mixr::base::INumber* const num)
{
   bool ok{};
   if (num != nullptr) {
      const double x{num->asDouble()};
      ok = setEmissivity(x);
      if (!ok) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IrSignature::setSlotEmissivity: Error setting Emissivity!" << std::endl;
         }
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotEffectiveArea() -- set effective area
//------------------------------------------------------------------------------
bool IIrSignature::setSlotEffectiveArea(mixr::base::IArea* const a)
{
   bool ok{};

   if (a != nullptr) {
      ok = setEffectiveArea(a->getValueInSquareMeters());
   }

   if (!ok) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "IrSignature::setSlotEffectiveArea: Error setting effective area!" << std::endl;
      }
   }
   return ok;
}

bool IIrSignature::getIrSignature(IrQueryMsg* const msg)
{
   bool ok{};
   //const auto msg = dynamic_cast<IrQueryMsg*>( msg0 );     // FAB - do we really need to cast away const?
   if (msg != nullptr) {
      const double projectedAreaInFOV{getSignatureArea(msg)};
      msg->setProjectedArea(projectedAreaInFOV);
      // if no projectedAreaInFOV, then target was not in FOV
      if (projectedAreaInFOV > 0.0){
         ok = true;
         // FAB - set simple signature value
         msg->setSignatureAtRange(getCalculatedHeatSignature());
         msg->setEmissivity(getEmissivity());
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSignatureArea() -- Determine target's surface area in the FOV (all or nothing)
//------------------------------------------------------------------------------
double IIrSignature::getSignatureArea(IrQueryMsg* msg)
{
   if (irShapeSignature == nullptr) {
      const double angleOffBoresight{msg->getAngleOffBoresight()};
      const double maxAngle{msg->getSendingSensor()->getIFOVTheta()};
      if (angleOffBoresight > maxAngle) return 0;
      return getEffectiveArea();
   } else {
      double reflectorArea{irShapeSignature->getReflectorAreaInFieldOfView(msg)};
      return reflectorArea;
   }
}

//------------------------------------------------------------------------------
// getWaveBandCenters() -- Return center frequency of all wave bands
//------------------------------------------------------------------------------
const double* IIrSignature::getWaveBandCenters() const
{
   return ((waveBandTable != nullptr) ? waveBandTable->getXData() : nullptr);
}

//------------------------------------------------------------------------------
// getWaveBandWidths() -- Return widths for all wave band frequencies
//------------------------------------------------------------------------------
const double* IIrSignature::getWaveBandWidths() const
{
   return ((waveBandTable != nullptr) ? waveBandTable->getDataTable() : nullptr);
}

}
}
