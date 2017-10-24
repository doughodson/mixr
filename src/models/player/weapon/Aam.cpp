
#include "mixr/models/player/weapon/Aam.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Aam, "AamMissile")
EMPTY_SLOTTABLE(Aam)
EMPTY_COPYDATA(Aam)
EMPTY_DELETEDATA(Aam)

int Aam::getCategory() const               { return (MISSILE | GUIDED); }
const char* Aam::getDescription() const    { return "AIM"; }
const char* Aam::getNickname() const       { return "GenericAam"; }

Aam::Aam()
{
   STANDARD_CONSTRUCTOR()

   static base::String gaam(getDescription());
   setType(&gaam);

   // Default guidance & dynamics for generic A/A missile
   // (derived models will override these values)
    setMaxTOF(60.0);
    setLethalRange(30.0);
    setMaxBurstRng(150.0);
    setSOBT(0.0);
    setEOBT(50.0);
    setTSG(1.0);
    setVpMin(0.0);
    setVpMax(800.0);
    setVpMaxG(800.0);
    setMaxG(5.0);
    setMaxAccel(50.0);
}

}
}
