
#ifndef __mixr_map_vpf_VMap0FeatureClass_H__
#define __mixr_map_vpf_VMap0FeatureClass_H__

#include "mixr/base/Object.hpp"

#include <string>
#include <array>

namespace mixr {
namespace vpf {

// ------------------------------------------------------------
// Class: VMap0FeatureClass
//
// Description: Defines relationships between feature tables and 
// primitives.
// ------------------------------------------------------------
class VMap0FeatureClass : public base::Object
{
    DECLARE_SUBCLASS(VMap0FeatureClass, base::Object)

public:
    VMap0FeatureClass();

    const char* getName()                        { return feature_class_name.c_str(); }
    virtual void setName(const std::string& x)   { feature_class_name = x;            }

    struct Relation {
        char table1[255];
        char table2[255];
        char table1key[255];
        char table2key[255];
        int table1Type;
        int table2Type;
    };

    virtual void addRelation(const char* table1, const char* table2, const char* table1key, const char* table2key);

private:
    static const int MAX_RELATIONS {50};
    std::string feature_class_name;
    std::array<Relation*, MAX_RELATIONS> relations {};
    int numRelations {};
};

}
}

#endif
