
#ifndef __mixr_base_Identifier_HPP__
#define __mixr_base_Identifier_HPP__

#include "mixr/base/Object.hpp"

#include <string>
#include <algorithm>
#include <cctype>

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Identifier
// Description: A single unquoted word (usually from the input file), which
//              can contain upper and lower case letters, numbers, special
//              characters, but not spaces.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Identifier
// Slots: none
//------------------------------------------------------------------------------
// Notes: Identifier is a 'native' type associated with the EDL parser.  It is
//        created when a non-quoted sequence of characters are encountered.
//------------------------------------------------------------------------------
class Identifier final: public Object
{
   DECLARE_SUBCLASS(Identifier, Object)

public:
   Identifier();
   Identifier(const std::string&);

   void setStr(const std::string& x)         { id = x;  replaceSpaces(); }

   // convenience methods
   const std::string& asString() const       { return id; }          // returns a standard string (std::string)
   const char* c_str() const                 { return id.c_str(); }  // returns a pointer to valid a C-style string

   // operators
   Identifier& operator=(const char*);            // assignment operator =
   Identifier& operator=(const std::string&);
   void operator+=(const char*);
   void operator+=(const std::string&);

   void empty()                              { id = "";   }          // empty our string
   bool isEmpty() const                      { return id == ""; }    // returns true if our string is empty

private:
   std::string id;
   void replaceSpaces();
};

inline Identifier& Identifier::operator=(const char* s)
{
   id = s;
   return *this;
}

inline Identifier& Identifier::operator=(const std::string& s)
{
   id = s;
   return *this;
}

inline void Identifier::operator+=(const char* s)
{
   id = id + std::string(s);
}

inline void Identifier::operator+=(const std::string& s)
{
   id = id + s;
}

inline std::ostream& operator<<(std::ostream& sout, const Identifier& s)
{
   sout << s.c_str();
   return sout;
}

// replace spaces with underscore
inline void Identifier::replaceSpaces()
{
   std::replace(id.begin(), id.end(), ' ', '_');
}

// comparison operator: ==
inline bool operator==(const Identifier& s1, const Identifier& s2)
{
   if (s1.asString() == s2.asString()) {
      return true;
   }
   return false;
}

inline bool operator==(const char* s1, const Identifier& s2)
{
   if (s2.asString() == std::string(s1)) {
      return true;
   }
   return false;
}

inline bool operator==(const Identifier& s1, const char* s2)
{
   if (s1.asString() == std::string(s2)) {
      return true;
   }
   return false;
}

}
}

#endif
