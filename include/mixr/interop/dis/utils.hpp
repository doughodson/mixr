
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

// Parse federate name for the site and application numbers
//  (We're expecting "SnnAmm" where nn and mm are the site and app numbers.)
bool parseFederateName(unsigned short* const site, unsigned short* const app, const char* const fedName);

// Parse federation name for the exercise number
//  (We're expecting "Ennn" where nnn is the exercise.)
bool parseFederationName(unsigned short* const exercise, const char* const fedName);

}
}

#endif
