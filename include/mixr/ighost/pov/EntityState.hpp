
#ifndef __mixr_ighost_pov_EntityState_H__
#define __mixr_ighost_pov_EntityState_H__

#include <cstdint>

namespace mixr {
namespace pov {

struct EntityState
{
  EntityState() = default;

  double x_cg{};
  double y_cg{};
  double z_cg{};          // altitude

  double pilot_eye_x{};
  double pilot_eye_y{};
  double pilot_eye_z{};   // altitude

  double alphad{};
  double betad{};
  double mach{};
  double runalt{};

  float theta{};
  float phi{};
  float psi{};
  float airspeed{}; // knots

  float heading{};

  float dlg{};      // landing gear position 90 is down (scaled to 0-1)
  float dsb{};      // speed break 60 is out (scaled to 0-1)
  float nz{};

  float aetrc{};               // Commanded throttle position
  std::int32_t afterburner{};  // logical, true in in A/B

  std::int32_t target_id{};

  std::int16_t id_self{};      // make use of a hole
  std::int16_t flags{};

  double target_x{};
  double target_y{};
  double target_z{};

  float target_theta{};
  float target_phi{};
  float target_psi{};

  float target_uearth{};
  float target_vearth{};
  float target_wearth{};
  float target_vcas{};

  float spares[4];
};

}
}

#endif
