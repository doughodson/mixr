
#ifndef __mixr_ighost_pov_Pov_H__
#define __mixr_ighost_pov_Pov_H__

namespace mixr {
namespace pov {

struct Pov
{
  Pov() = default;

  // position
  double x{};
  double y{};
  double z{};   // altitude

  // orientation
  double theta{};
  double phi{};
  double psi{};

};

}
}

#endif
