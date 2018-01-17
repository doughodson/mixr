
#ifndef __mixr_base_Identifier_H__
#define __mixr_base_Identifier_H__

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
//
// Note: When setting the identifier, any spaces will be replaced
//       with underscores.
//------------------------------------------------------------------------------
class Identifier final: public Object
{
   DECLARE_SUBCLASS(Identifier, Object)

public:
   Identifier();
   Identifier(const char*);

   // operators
   Identifier& operator=(const char*);            // assignment operator =
   Identifier& operator=(const std::string&);
   void operator+=(const char*);
   void operator+=(const std::string&);

   const char* c_str() const                 { return id.c_str(); }  // returns a pointer to valid a C-style string
   const std::string& str() const            { return id; }          // returns a standard string (std::string)

   void empty()                              { id = "";   }          // empty our string
   void setStr(const char* x)                { id = x;  replaceSpaces(); }
   bool isEmpty() const                      { return id == ""; }    // returns true if our string is empty

   // returns true if our string is the ascii form of a number
   bool isNumber() const;

   // returns the value converted to a number or zero if isNumber() is false
   double getNumber() const;

   // returns true if our string is the ascii form of an integer number
   bool isInteger() const;

   // returns the value of this string converted to an integer value or zero if isInteger() is false
   int getInteger() const;

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


//------------------------------------------------------------------------------
// isNumber() -- is this an a ascii number
//------------------------------------------------------------------------------
inline bool Identifier::isNumber() const
{
   if (isEmpty()) {
      return false;
   }

   for (int i = 0; id[i] != '\0'; i++) {
      if ( !std::isdigit(id[i]) && id[i] != '.' ) {
         return false;
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// getNumber() -- convert a ascii number string to a number
//------------------------------------------------------------------------------
inline double Identifier::getNumber() const
{
   if (isNumber()) {
      return std::atof(id.c_str());
   }
   return 0.0;
}

//------------------------------------------------------------------------------
// isInteger() -- is this an a ascii integer
//------------------------------------------------------------------------------
inline bool Identifier::isInteger() const
{
   if (isEmpty()) {
      return false;
   }

   for (int i = 0; id[i] != '\0'; i++) {
      if ( !std::isdigit(id[i]) ) {
         return false;
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// getInteger() -- convert a ascii number string to an integer
//------------------------------------------------------------------------------
inline int Identifier::getInteger() const
{
   if (isInteger()) {
      return std::atoi(id.c_str());
   }
   return 0;
}

// replace spaces with underscore
inline void Identifier::replaceSpaces()
{
   std::replace(id.begin(), id.end(), ' ', '_');
}

// comparison operator: ==
inline bool operator==(const Identifier& s1, const Identifier& s2)
{
   if (s1.str() == s2.str()) {
      return true;
   }
   return false;
}

inline bool operator==(const char* s1, const Identifier& s2)
{
   if (s2.str() == std::string(s1)) {
      return true;
   }
   return false;
}

inline bool operator==(const Identifier& s1, const char* s2)
{
   if (s1.str() == std::string(s2)) {
      return true;
   }
   return false;
}

}
}

#endif
