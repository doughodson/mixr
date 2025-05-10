#ifndef __mixr_ighost_flightgear_FGNetFDM_HPP__
#define __mixr_ighost_flightgear_FGNetFDM_HPP__

#include <cstdint>

namespace mixr {
namespace ighost {
namespace flightgear {

const std::uint32_t FG_NET_FDM_VERSION{24};

//-------------------------------------------------------------------
// Struct: FGNetFDM
// Description: Defines a structure containing the top level flight
//              dynamics model parameters
//-------------------------------------------------------------------
// Documentation associated with original code below:
// net_fdm.hxx -- defines a common net I/O interface to the flight
//                dynamics model
//
// Written by Curtis Olson - http://www.flightgear.org/~curt
// Started September 2001.
//
// This file is in the Public Domain, and comes with no warranty.
//
// NOTE: this file defines an external interface structure.  Due to
// variability between platforms and architectures, we only used fixed
// length types here.  Specifically, integer types can vary in length.
// I am not aware of any platforms that don't use 4 bytes for float
// and 8 bytes for double.
//-------------------------------------------------------------------
struct FGNetFDM
{
   FGNetFDM() = default;

   enum {
      FG_MAX_ENGINES = 4,
      FG_MAX_WHEELS = 3,
      FG_MAX_TANKS = 4
   };

   std::uint32_t version{FG_NET_FDM_VERSION};
   std::uint32_t padding{};   // padding

   // positions
   double longitude{};   // geodetic (radians)
   double latitude{};    // geodetic (radians)
   double altitude{};    // above sea level (meters)
   float agl{};          // above ground level (meters)
   float phi{};          // roll (radians)
   float theta{};        // pitch (radians)
   float psi{};          // yaw or true heading (radians)
   float alpha{};        // angle of attack (radians)
   float beta{};         // side slip angle (radians)

   // velocities
   float phidot{};       // roll rate (radians/sec)
   float thetadot{};     // pitch rate (radians/sec)
   float psidot{};       // yaw rate (radians/sec)
   float vcas{};         // calibrated airspeed
   float climb_rate{};   // feet per second
   float v_north{};      // north velocity in local/body frame, fps
   float v_east{};       // east velocity in local/body frame, fps
   float v_down{};       // down/vertical velocity in local/body frame, fps
   float v_body_u{};     // ECEF velocity in body frame
   float v_body_v{};     // ECEF velocity in body frame
   float v_body_w{};     // ECEF velocity in body frame

   // accelerations
   float A_X_pilot{};    // x accel in body frame ft/sec^2
   float A_Y_pilot{};    // y accel in body frame ft/sec^2
   float A_Z_pilot{};    // z accel in body frame ft/sec^2

   // stall
   float stall_warning{}; // 0.0 - 1.0 indicating the amount of stall
   float slip_deg{};      // slip ball deflection

   // engine status
   std::uint32_t num_engines{};               // number of valid engines
   std::uint32_t eng_state[FG_MAX_ENGINES]{}; // engine state (off, cranking, running)
   float rpm[FG_MAX_ENGINES]{};               // engine RPM rev/min
   float fuel_flow[FG_MAX_ENGINES]{};         // fuel flow gallons/hr
   float fuel_px[FG_MAX_ENGINES]{};           // fuel pressure psi
   float egt[FG_MAX_ENGINES]{};               // exhuast gas temp deg F
   float cht[FG_MAX_ENGINES]{};               // cylinder head temp deg F
   float mp_osi[FG_MAX_ENGINES]{};            // manifold pressure
   float tit[FG_MAX_ENGINES]{};               // turbine Inlet Temperature
   float oil_temp[FG_MAX_ENGINES]{};          // oil temp deg F
   float oil_px[FG_MAX_ENGINES]{};            // oil pressure psi

   // consumables
   std::uint32_t num_tanks{};                 // max number of fuel tanks
   float fuel_quantity[FG_MAX_TANKS]{};

   // gear status
   std::uint32_t num_wheels{};
   std::uint32_t wow[FG_MAX_WHEELS]{};
   float gear_pos[FG_MAX_WHEELS]{};
   float gear_steer[FG_MAX_WHEELS]{};
   float gear_compression[FG_MAX_WHEELS]{};

   // environment
   std::uint32_t cur_time{};    // current unix time
                                // FIXME: make this uint64_t before 2038
   std::int32_t warp{};         // offset in seconds to unix time
   float visibility{};          // visibility in meters (for env. effects)

   // control surface positions (normalized values)
   float elevator{};
   float elevator_trim_tab{};
   float left_flap{};
   float right_flap{};
   float left_aileron{};
   float right_aileron{};
   float rudder{};
   float nose_wheel{};
   float speedbrake{};
   float spoilers{};
};

}
}
}

#endif
