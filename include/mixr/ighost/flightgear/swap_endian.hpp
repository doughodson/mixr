
#ifndef __mixr_ighost_flightgear_swap_endian_HPP__
#define __mixr_ighost_flightgear_swap_endian_HPP__

namespace mixr {
namespace flightgear {

struct FGNetFDM;
struct FGNetCtrls;

void swap_endian(FGNetFDM* const);
void swap_endian(FGNetCtrls* const);

}
}

#endif
