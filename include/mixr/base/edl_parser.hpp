
#ifndef __mixr_base_edl_parser_HPP__
#define __mixr_base_edl_parser_HPP__

#include <string>

namespace mixr {
namespace base {

class IObject;

//
// factory function signature (e.g., factory(const std::string& name); )
// -- the user defines this function
//
typedef IObject* (*factory_func)(const std::string& name);

//
// edl_parser( text filename to parse, user supplied factory function to create objects,
//             pointer to variable for num of errors found )
//
extern IObject* edl_parser(const std::string& filename, factory_func f, int* num_errors = nullptr);

}
}

#endif

