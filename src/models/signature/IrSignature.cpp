
#include "mixr/models/signature/IrSignature.hpp"

#include "mixr/models/environment/IrAtmosphere.hpp"

#include "mixr/models/IrShapes.hpp"
#include "mixr/models/IrQueryMsg.hpp"
#include "mixr/models/system/IrSensor.hpp"

#include "mixr/base/relations/Table1.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/units/areas.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IrSignature, "IrSignature")

BEGIN_SLOTTABLE(IrSignature)
   "binSizes",                // 1 The size of the wavelength bins used in the tables
   "irShapeSignature",        // 2 IR shape
   "baseHeatSignature",       // 3 Base heat signature in watts per steradian
   "emissivity",              // 4 Emissivity
   "effectiveArea",           // 5 Effective area
END_SLOTTABLE(IrSignature)

BEGIN_SLOT_MAP(IrSignature)
   ON_SLOT(1, setSlotWaveBandSizes,     base::Table1)
   ON_SLOT(2, setSlotIrShapeSignature,  IrShape)
   ON_SLOT(3, setSlotBaseHeatSignature, base::Number)
   ON_SLOT(4, setSlotEmissivity,        base::Number)
   ON_SLOT(5, setSlotEffectiveArea,     base::Area)
END_SLOT_MAP()

IrSignature::IrSignature()
{
   STANDARD_CONSTRUCTOR()
   // create a default waveband, middle infrared band : 3-5 micron (100 THz - 60 THz)
   //double xdata[8] = { 4.0f };
   //double gdata[8] = { 2.0f };
   //waveBandTable = new base::Table1(gdata, 1, xdata, 1);
   //numWaveBands=1;
}

void IrSignature::copyData(const IrSignature& org, const bool)
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
      IrShape* copy{org.irShapeSignature->clone()};
      setSlotIrShapeSignature( copy );
      copy->unref();
   } else {
      setSlotIrShapeSignature(nullptr);
   }
}

void IrSignature::deleteData()
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
bool IrSignature::setSlotWaveBandSizes(const base::Table1* const tbl)
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
bool IrSignature::setSlotIrShapeSignature(IrShape* const s)
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
bool IrSignature::setSlotBaseHeatSignature(base::Number* const num)
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
bool IrSignature::setSlotEmissivity(mixr::base::Number* const num)
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
bool IrSignature::setSlotEffectiveArea(mixr::base::Area* const a)
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

bool IrSignature::getIrSignature(IrQueryMsg* const msg)
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
double IrSignature::getSignatureArea(IrQueryMsg* msg)
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
const double* IrSignature::getWaveBandCenters() const
{
   return ((waveBandTable != nullptr) ? waveBandTable->getXData() : nullptr);
}

//------------------------------------------------------------------------------
// getWaveBandWidths() -- Return widths for all wave band frequencies
//------------------------------------------------------------------------------
const double* IrSignature::getWaveBandWidths() const
{
   return ((waveBandTable != nullptr) ? waveBandTable->getDataTable() : nullptr);
}

}
}
