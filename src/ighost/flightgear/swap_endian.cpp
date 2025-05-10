
#include <cstdint>

#include "mixr/base/util/endian_utils.hpp"

#include "mixr/ighost/flightgear/FGNetFDM.hpp"
#include "mixr/ighost/flightgear/FGNetCtrls.hpp"

namespace mixr {
namespace ighost {
namespace flightgear {

void swap_endian(FGNetFDM* const x)
{
   base::swap_endian<std::uint32_t>(x->version);
   base::swap_endian<std::uint32_t>(x->padding);

   // positions
   base::swap_endian<double>(x->latitude);
   base::swap_endian<double>(x->longitude);
   base::swap_endian<double>(x->altitude);
   base::swap_endian<float>(x->agl);
   base::swap_endian<float>(x->phi);
   base::swap_endian<float>(x->theta);
   base::swap_endian<float>(x->psi);
   base::swap_endian<float>(x->alpha);
   base::swap_endian<float>(x->beta);

   // velocities
   base::swap_endian<float>(x->phidot);
   base::swap_endian<float>(x->thetadot);
   base::swap_endian<float>(x->psidot);
   base::swap_endian<float>(x->vcas);
   base::swap_endian<float>(x->climb_rate);
   base::swap_endian<float>(x->v_north);
   base::swap_endian<float>(x->v_east);
   base::swap_endian<float>(x->v_down);
   base::swap_endian<float>(x->v_body_u);
   base::swap_endian<float>(x->v_body_v);
   base::swap_endian<float>(x->v_body_w);

   // accelerations
   base::swap_endian<float>(x->A_X_pilot);
   base::swap_endian<float>(x->A_Y_pilot);
   base::swap_endian<float>(x->A_Z_pilot);

   // stall
   base::swap_endian<float>(x->stall_warning);
   base::swap_endian<float>(x->slip_deg);

   // engine status
   base::swap_endian<std::uint32_t>(x->num_engines);
   for (std::size_t i{}; i < FGNetFDM::FG_MAX_ENGINES; i++) {
      base::swap_endian<std::uint32_t>(x->eng_state[i]);
      base::swap_endian<float>(x->rpm[i]);
      base::swap_endian<float>(x->fuel_flow[i]);
      base::swap_endian<float>(x->fuel_px[i]);
      base::swap_endian<float>(x->egt[i]);
      base::swap_endian<float>(x->cht[i]);
      base::swap_endian<float>(x->mp_osi[i]);
      base::swap_endian<float>(x->tit[i]);
      base::swap_endian<float>(x->oil_temp[i]);
      base::swap_endian<float>(x->oil_px[i]);
   }

   // consumables
   base::swap_endian<std::uint32_t>(x->num_tanks);
   for (std::size_t i{}; i < FGNetFDM::FG_MAX_TANKS; i++) {
      base::swap_endian<float>(x->fuel_quantity[i]);
   }

   // gear status
   base::swap_endian<std::uint32_t>(x->num_wheels);
   for (std::size_t i{}; i < FGNetFDM::FG_MAX_WHEELS; i++) {
      base::swap_endian<std::uint32_t>(x->wow[i]);
      base::swap_endian<float>(x->gear_pos[i]);
      base::swap_endian<float>(x->gear_steer[i]);
      base::swap_endian<float>(x->gear_compression[i]);
   }

   // environment
   base::swap_endian<std::uint32_t>(x->cur_time);
   base::swap_endian<std::int32_t>(x->warp);
   base::swap_endian<float>(x->visibility);

   // control surface positions (normalized values)
   base::swap_endian<float>(x->elevator);
   base::swap_endian<float>(x->elevator_trim_tab);
   base::swap_endian<float>(x->left_flap);
   base::swap_endian<float>(x->right_flap);
   base::swap_endian<float>(x->left_aileron);
   base::swap_endian<float>(x->right_aileron);
   base::swap_endian<float>(x->rudder);
   base::swap_endian<float>(x->nose_wheel);
   base::swap_endian<float>(x->speedbrake);
   base::swap_endian<float>(x->spoilers);
}

void swap_endian(FGNetCtrls* const x)
{
   base::swap_endian<std::uint32_t>(x->version);

   // aero controls
   base::swap_endian<double>(x->aileron);
   base::swap_endian<double>(x->elevator);
   base::swap_endian<double>(x->rudder);
   base::swap_endian<double>(x->aileron_trim);
   base::swap_endian<double>(x->elevator_trim);
   base::swap_endian<double>(x->rudder_trim);
   base::swap_endian<double>(x->flaps);
   base::swap_endian<double>(x->spoilers);
   base::swap_endian<double>(x->speedbrake);

}

}
}
}

