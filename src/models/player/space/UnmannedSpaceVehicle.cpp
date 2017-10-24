
#include "mixr/models/player/space/UnmannedSpaceVehicle.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(UnmannedSpaceVehicle,"UnmannedSpaceVehicle")
EMPTY_SLOTTABLE(UnmannedSpaceVehicle)
EMPTY_COPYDATA(UnmannedSpaceVehicle)
EMPTY_DELETEDATA(UnmannedSpaceVehicle)

UnmannedSpaceVehicle::UnmannedSpaceVehicle()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericUnmannedSpaceVehicle");
    setType(&generic);
}

void UnmannedSpaceVehicle::dynamics(const double dt)
{
   BaseClass::dynamics(dt);
   //base::Vec3d p = getGeocPosition();
   //base::Vec3d v = getGeocVelocity();
   //std::cout << "USV::Dyn(" << this << ")";
   //std::cout << " lla=[ " << getLatitude() << ", " << getLongitude() << ", " << (getAltitudeAglM() * base::Distance::M2NM)  << " ]";
   //std::cout << " pos=[ " << p[0] << ", " << p[1] << ", " << p[2] << " ]";
   //std::cout << " vec=[ " << v[0] << ", " << v[1] << ", " << v[2] << " ]";
   //std::cout << std::endl;
}

}
}
