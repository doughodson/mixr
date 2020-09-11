
#ifndef __mixr_interop_dis_utils_HPP__
#define __mixr_interop_dis_utils_HPP__

namespace mixr {
namespace dis {

// Generate a federate name from the site and application numbers:
//  "SnnAmm" -- where nn and mm are the site and app numbers.
bool makeFederateName(char* const fedName, const unsigned int len, const unsigned short site, const unsigned short app);

// Generate a federation name from the exercise numbers:
//  "Ennn" -- where nnn is the exercise number, which must be greater than zero
bool makeFederationName(char* const fedName, const unsigned int len, const unsigned short exercise);

}
}

#endif
