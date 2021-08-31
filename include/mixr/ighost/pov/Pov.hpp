
#ifndef __mixr_ighost_pov_Pov_H__
#define __mixr_ighost_pov_Pov_H__

namespace mixr {
namespace pov {

struct Pov
{
  Pov() = default;

  // position
  double north{};       // meters
  double east{};        // meters
  double alt_agl{};     // meters

  // orientation
  double roll{};        // (phi) degrees
  double pitch{};       // (theta) degrees
  double heading{};     // (psi) degrees
};

}
}

#endif
