
#ifndef __mixr_base_MetaObject_HPP__
#define __mixr_base_MetaObject_HPP__

#include <string>

namespace mixr {
namespace base {
class SlotTable;

//------------------------------------------------------------------------------
// Class: MetaObject
// Description: Concrete class that defines attributes and object instances.
//              This includes its name, slot table, and even the number of them
//              in existance.
//------------------------------------------------------------------------------
class MetaObject final
{
public:
   MetaObject() = default;
   MetaObject(const std::string&, const std::string&, const SlotTable* const, const MetaObject* const);
   MetaObject(const MetaObject&) = delete;
   MetaObject& operator=(const MetaObject&) = delete;

   const std::string& getClassName() const             { return className; }
   const std::string& getFactoryName() const           { return factoryName; }

   const SlotTable* const slottable {};         // pointer to the SlotTable
   const MetaObject* const baseMetaObject {};   // pointer to the base object's MetaObject
   int count {};                                // current number of object instances
   int mc {};                                   // max number of instances in existance at the same time
   int tc {};                                   // total number of instances created

private:
   const std::string className;                 // class name from 'type_info'
   const std::string factoryName;               // factory name
};

}
}

#endif

