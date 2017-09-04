
#ifndef __mixr_base_edl_parser_H__
#define __mixr_base_edl_parser_H__

#include <string>

namespace mixr {
namespace base {

class Object;

//
// factory function signature (e.g., factory(const std::string& name); )
// -- the user defines this function
//
typedef Object* (*factory_func)(const std::string& name);

//
// edl_parser( text filename to parse, user supplied factory function to create objects,
//             pointer to variable for num of errors found )
//
extern Object* edl_parser(const std::string& filename, factory_func f, int* num_errors = nullptr);

}
}

#endif

