
#include "mixr/map/vpf/VMap0FeatureClass.hpp"
#include "mixr/base/String.hpp"

#include <cstdio>
#include <cstring>

namespace mixr {
namespace vpf {
    
IMPLEMENT_SUBCLASS(VMap0FeatureClass, "VMap0FeatureClass")
EMPTY_SLOTTABLE(VMap0FeatureClass)
EMPTY_COPYDATA(VMap0FeatureClass)

VMap0FeatureClass::VMap0FeatureClass()
{
    STANDARD_CONSTRUCTOR()
}

void VMap0FeatureClass::deleteData()
{
    relations.fill(nullptr);
}

void VMap0FeatureClass::addRelation(const char* table1, const char* table2, const char *table1key, const char *table2key)
{   
    char* tempTable1 {const_cast<char*>(table1)};
    char* tempTable2 {const_cast<char*>(table2)};
    char* tempTable1Key {const_cast<char*>(table1key)};
    char* tempTable2Key {const_cast<char*>(table2key)};

    // truncate our values
    std::size_t size {std::strlen(tempTable1)};
    std::size_t count {};
    while (tempTable1[count] != ' ' && count < size) count++;
    tempTable1[count] = 0;

    size = std::strlen(tempTable2);
    count = 0;
    while (tempTable2[count] != ' ' && count < size) count++;
    tempTable2[count] = 0;            

    size = std::strlen(tempTable1Key);
    count = 0;
    while (tempTable1Key[count] != ' ' && count < size) count++;
    tempTable1Key[count] = 0;            

    size = std::strlen(tempTable2Key);
    count = 0;
    while (tempTable2Key[count] != ' ' && count < size) count++;
    tempTable2Key[count] = 0;            

    relations[numRelations] = new Relation();

    std::strcpy(relations[numRelations]->table1, tempTable1);
    //std::sprintf(relations[numRelations]->table1, "%s", tempTable1);
    //lcStrcpy(relations[numRelations]->table1, sizeof(relations[numRelations]->table1), tempTable1);
    std::strcpy(relations[numRelations]->table2, tempTable2);
    //std::sprintf(relations[numRelations]->table2, "%s", tempTable2);
    //lcStrcpy(relations[numRelations]->table2, sizeof(relations[numRelations]->table2), tempTable2);
    std::strcpy(relations[numRelations]->table1key, tempTable1Key);
    //std::sprintf(relations[numRelations]->table1key, "%s", tempTable1Key);
    //lcStrcpy(relations[numRelations]->table1key, sizeof(relations[numRelations]->table1key), tempTable1Key);
    std::strcpy(relations[numRelations]->table2key, tempTable2Key);
    //std::sprintf(relations[numRelations]->table2key, "%s", tempTable2Key);
    //lcStrcpy(relations[numRelations]->table2key, sizeof(relations[numRelations]->table2key), tempTable2Key);
    relations[numRelations]->table1Type = -1;
    relations[numRelations]->table2Type = -1;

    #if defined DEBUG_OUTPUT
    std::cout << "RELATION #" << numRelations+1 << " = " << relations[numRelations]->table1 << ", " << relations[numRelations]->table1key \
        << ", " << relations[numRelations]->table2 << ", " << relations[numRelations]->table2key << std::endl;
#endif
    numRelations++;
}

}
}
