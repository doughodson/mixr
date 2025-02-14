
#ifndef __mixr_base_Object_HPP__
#define __mixr_base_Object_HPP__

// defines reference counting system for objects
#include "mixr/base/IReferenced.hpp"

// platform configuration file
#include "mixr/config.hpp"
// keep this include to correct a bug in vs2012 compiler intrinsics
#include "mixr/base/util/platform_api.hpp"

#include <typeinfo>

#include "mixr/base/macros.hpp"
#include "mixr/base/SlotTable.hpp"

#include "mixr/base/MetaObject.hpp"

#include <iosfwd>

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Object
// Description: Top-level base class for all framework objects.
//
//    Provides a common base class for type checking and support for factory names,
//    slot tables, exceptions and reference counting.  Most of this needs to
//    be implemented by each derived class, and the macros in 'macros.hpp' provide
//    an easy way to do this.
//
//------------------------------------------------------------------------------
// Macros
//
//    Macros are provided to generate much of the boilerplate code that is required
//    for each derived class (see macros.hpp).  The macro DECLARE_SUBCLASS() must be
//    in every derived class declaration, and the macro IMPLEMENT_SUBCLASS() must
//    be included with each class implementation.  Use IMPLEMENT_ABSTRACT_SUBCLASS()
//    for the implementation of abstract classes, and IMPLEMENT_PARTIAL_SUBCLASS()
//    if you need to write your own destructor, copy constructor, assignment operator
//    and clone function.
//
// Constructors and general class functions:
//
//    Each derived class must provide a standard constructor that requires no
//    parameters (e.g., Foo()).  This is required by the class factories (i.e., the
//    factory functions) to constructor instances of derived classes.
//
//    The copy constructors, assignment operators (=), virtual destructors,
//    and clone functions are provided for all derived classes using the
//    macros and the user written functions, copyData and deleteData().
//
//    All constructors must include the STANDARD_CONSTRUCTOR() macro.
//
//    All newly constructed objects start with a reference count of one.
//
//       Foo::Foo()
//          Constructor; must be written for each derived class, or you can
//          use the EMPTY_CONSTRUCTOR() macro.
//
//       void copyData(const Foo& orig, const bool cc)
//
//          Declared in DECLARE_SUBCLASS(), and must be written for each derived
//          class, or use the EMPTY_COPYDATA() macro.
//
//          Copies data from the original object, 'orig', to our member variables
//          (e.g., copying the data, subcomponents, etc.).  Old, unused member
//          components need to be disposed of, as required.  The base class'
//          copyData() should be called BEFORE we copy any of our data.  The
//          copy constructor flag, 'cc', is true if we're being called by our
//          copy constructor, which indicates that all of our member data and
//          pointers are still undefined.  This flag should NOT be passed to
//          our base class's copyData().
//
//       void deleteData()
//
//          Declared in DECLARE_SUBCLASS(), and must be written for each derived
//          class, or use the EMPTY_COPYDATA() macro.
//
//          Disposes of our member data and components, as required.  Our
//          base class's deleteData() should be called AFTER we have disposed
//          of our data.  Warning: this function can  be called several times,
//          so data should be disposed of cleanly (e.g., test pointers for
//          zero before unreferencing objects and setting the pointers to zero
//          after the object has been unreferenced).
//
//       Foo::Foo(Foo& orig)
//          Copy constructor provided by the macros and uses copyData().
//
//       Foo::~Foo()
//          Class destructor provided by the macros and uses deleteData().
//
//       Object* clone()
//          Returns a copy of this object.  Provided by the macros and uses
//          the copy constructor.
//
//       "="
//          Assignment Operator is provided by the macros and requires copyData()
//
//       bool isClassType(const type_info& type)
//          Returns true if this object's class type is 'type' or if it is
//          derived from class 'type'. <defined by the macros>

//       const char* Foo::getClassName()
//          Static function that returns the full class name of class Foo.
//
//       bool isValid()
//          Returns true if this object is valid (i.e., all required slots/attributes
//          have been properly set).  This function is called by the parser to inform
//          the object that all calls to setSlotByName() have been completed.
//
// 'BaseClass' type
//
//    The DECLARE_SUBCLASS macro defines the type 'BaseClass', which is the class'
//    base class, and which should be used when calling base class member functions
//    instead of calling the functions with explicit class names.
//
// Factory names:
//
//    The user written factory functions will create an object
//    given its 'factory' name and will return a pointer to the new object or zero if the
//    object wasn't found.  These new objects have a reference count of one.  The
//    object's factory name, which is defined by a parameter to the IMPLEMENT_SUBCLASS()
//    macro, is usually the same or similar to the class name.
//
//    The factory function is passed to the parser (see edl_parser.y); which servers
//    as a helper function to build an object tree.  The function constructs objects
//    using factory names parsed from the input file.
//
//    Typically an application will build its own application factory function,
//    which in turn will call the various factory methods for the model libraries and
//    framework libraries that the application requires.  By ordering these factory
//    calls at within the application, the factory can search a library containing
//    a more specific models prior to searching a library of other generic models,
//    and if a more specific model exists for a given factory name then it is used,
//    otherwise the library of generic models is searched and its model, if found,
//    is used.
//
//    Factory name support functions --
//
//       bool isFactoryName(char* name)
//          Returns true if this object's factory name is 'name' or if it is
//          derived from a factory named 'name'.
//
//       const char* Foo::getFactoryName()
//          Static function that returns the factory name of class Foo.
//
//    (For examples of factory functions, see base/factory.cpp, graphics/factory.cpp,
//     simulation/factory.cpp, as well as the various MIXR examples)
//
//
// Slots and the Slot table:
//
//    Slot tables define the names of the slots (i.e., attributes) accepted by
//    the class of objects and maps these slot names to local slot index numbers.
//
//    Slot tables are defined by BEGIN_SLOTTABLE() and END_SLOTTABLE(),
//    or for an empty slot table, use EMPTY_SLOTTABLE().
//
//       // Class 'Foo' slot table
//       BEGIN_SLOTTABLE(Foo)
//          "slotA",             // index #1
//          "slotB",             // index #2
//          "slotC",             // index #3
//       END_SLOTTABLE(Foo)
//
//    The slot map, which is defined by the macros BEGIN_SLOT_MAP(), ON_SLOT()
//    and END_SLOT_MAP(), provides a mapping by local slot index number and
//    argument type to class member functions, 'set slot functions', that will
//    handle setting the class attribute.
//
//       // Class 'Foo' slot map
//       BEGIN_SLOT_MAP(Foo)
//           ON_SLOT( 1, setSlotA, List)     // setSlotA() handles "slotA" with a List type argument
//           ON_SLOT( 1, setSlotA, String)   // setSlotA() handles "slotA" with a String type argument
//           ON_SLOT( 2, setSlotB, Number)   // setSlotA() handles "slotB" with a Number type argument
//           ON_SLOT( 3, setSlotC, String)   // setSlotA() handles "slotC" with a String type argument
//       END_SLOT_MAP()
//
//    Each object instance has a slot table pointer that points to the class
//    slot table, and each class constructor must include the STANDARD_CONSTRUCTOR()
//    macro, which will set this slot table pointer.
//
//    Each slot table contains a pointer to its base class' slot table, and
//    slot names that are not found in a class slot table are passed up to
//    the base class' slot table.  Therefore, slot names are inherited by
//    derived classes and derived classes can override base class slot names.
//
//    Note: there are the SlotTable's index numbers and our local slot index
//    numbers.  As defined by the SlotTable class (see SlotTable.hpp), the first
//    slot of the first base class that contains a slot table is slot number one.
//    Whereas BEGIN_SLOT_MAP() remaps these to local slot numbers, which are
//    relative to our class (i.e. a local slot one), and these local number are
//    used by the ON_SLOT() macro.
//
//
// Standard message types (see definitions below):
//
//    By default, error and warning messages are enabled.
//
//    And by default, the other message types (Informational, Debug, Data,
//    User) are implicitly disabled (i.e., neither enabled or disabled
//    explicitly)(use std::cout).
//
//    Use isMessageEnabled() to test if a message type is enabled before
//    writing messages; except in 'slot' functions, which are usually
//    called by the parser and need to be written.
//
//       bool isMessageEnabled(const unsigned int msgTypeBit)
//          Returns true if this message type is enabled.
//
//       bool isMessageDisabled(const unsigned int msgTypeBit)
//          Returns true if this message type is disabled.
//
//       bool enableMessageTypes(const unsigned int msgTypeBits)
//          Enables these message types (bitwise OR of the message type bits).
//
//       bool disableMessageTypes(const unsigned int msgTypeBits)
//          Disables these message types (bitwise OR of the message type bits).
//          Note: the MSG_ERROR type message can not be disabled.
//
//
// Exception:
//    Exception
//       Object's general exception class which returns a description.
//       The default description is "Unknown".
//
//
// Variables:
//    protected: const SlotTable* slotTable
//       Object slot table.  The Object constructor sets this variable to zero.
//       Class constructors must set this variable to their <class>::slottable.
//       (See macro STANDARD_CONSTRUCTOR())
//
//
// MetaObject
//
//    Each class has an associated MetaObject instance which is used
//    to store data related to class and object instances.  For example,
//    the meta-object contains the number of instances of a particular class in
//    existance, as well as the total number of objects that have ever been
//    created and the maximum number that has existed at the same time.  This
//    capability was added to Object to serves as a debugging tool to monitor
//    the creation and deletion of large numbers of particular classes of interest
//    to spot potential memory leaks.
//
//------------------------------------------------------------------------------
class Object : public IReferenced
{
   // -------------------------------------------------------------------------
   // Standard object stuff --
   //    derived classes will use the macro DECLARE_SUBCLASS(); see macros.hpp
   // -------------------------------------------------------------------------
   public: Object();
   public: Object(const Object& org);
   public: Object& operator=(const Object& org);
   public: virtual Object* clone() const =0;
   public: virtual ~Object();

   protected: void copyData(const Object& org, const bool cc = false);
   protected: void deleteData();

   // helper methods
   public: virtual bool isClassType(const std::type_info&) const;
   public: virtual bool isFactoryName(const std::string&) const;
   public: static const std::string& getFactoryName();

   // slot table
   protected: static const SlotTable slottable;    // class slot table
   private: static const char* slotnames[];        // slot names in this object's slot table
   private: static const int nslots;               // number of slots in this object's slot table

   // slot table functions
   public: static const SlotTable& getSlotTable();
   protected: virtual bool setSlotByIndex(const int slotindex, Object* const obj);
   public: bool setSlotByName(const char* const slotname, Object* const obj);
   public: const char* slotIndex2Name(const int slotindex) const;
   public: int slotName2Index(const char* const slotname) const;

public:
   // standard message types
   static const unsigned short MSG_ERROR   {0x0001};  // Error messages; ALWAYS ENABLED (use std::cerr)
   static const unsigned short MSG_WARNING {0x0002};  // Warning messages (use std::cerr)
   static const unsigned short MSG_INFO    {0x0004};  // Informational messages (use std::cout)
   static const unsigned short MSG_DEBUG   {0x0008};  // Debug/trace messages(use std::cout)
   static const unsigned short MSG_DATA    {0x0010};  // Data messages(use std::cout)
   static const unsigned short MSG_USER    {0x0020};  // User debug/trace messages(use std::cout)
   static const unsigned short MSG_STD_ALL {0x00FF};  // Standard message types mask
   // non-standard or user defined message are the high order bits (0xFF00)
   static const unsigned short MSG_ALL     {0xFFFF};  // All message types mask

public:
   virtual bool isValid() const;

   virtual bool isMessageEnabled(const unsigned short msgType) const;
   bool isMessageDisabled(const unsigned short msgType) const;
   bool enableMessageTypes(const unsigned short msgTypeBits);
   bool disableMessageTypes(const unsigned short msgTypeBits);

   static const MetaObject* getMetaObject();

protected:
   // slot table for this object (set to the object's class slot table)
   const SlotTable* slotTable{};

   unsigned short getMessageEnableBits() const  { return enbMsgBits; }
   unsigned short getMessageDisableBits() const { return disMsgBits; }

private:
   unsigned short enbMsgBits{ MSG_ERROR | MSG_WARNING };  // Enabled message bits
   unsigned short disMsgBits{};                           // Disabled message bits

   static MetaObject metaObject;
};

}
}

#endif

