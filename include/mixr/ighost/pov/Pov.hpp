
#ifndef __mixr_ighost_pov_Pov_H__
#define __mixr_ighost_pov_Pov_H__

namespace mixr {
namespace ighost {
namespace pov {

struct Pov
{
  Pov() = default;

  // position
  double north{};       // meters
  double east{};        // meters
  double alt_agl{};     // meters

  // orientation
  double roll{};        // degrees (phi)
  double pitch{};       // degrees (theta)
  double heading{};     // degrees (psi)
};

}
}
}

#endif
