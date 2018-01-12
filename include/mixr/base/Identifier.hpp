
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
   operator char*();
   operator const char*() const;
   Identifier& operator=(const char*);            // assignment operator =
   Identifier& operator=(const std::string&);
   void operator+=(const char*);
   void operator+=(const std::string&);

   void empty()                              { str = "";   }          // Empty (null) this string
   const char* getString() const             { return str.c_str(); }  // Returns a pointer to the text string
   const std::string& getStdString() const   { return str; }
   void setStr(const char* x)                { str = x;  replaceSpaces(); }

   bool isEmpty() const                      { return str == ""; }   // Returns true if this string is empty (null)

   // Returns true if this string is the ascii form of a number
   bool isNumber() const;

   // Returns the value converted to a number or zero if isNumber() is false
   double getNumber() const;

   // Returns true if this string is the ascii form of an integer number
   bool isInteger() const;

   // Returns the value of this string converted to an integer value or zero if isInteger() is false
   int getInteger() const;

private:
   std::string str;
   void replaceSpaces();
};

inline Identifier::operator char*()
{
   return const_cast<char*>(str.c_str());
}

inline Identifier::operator const char*() const
{
   return str.c_str();
}

inline Identifier& Identifier::operator=(const char* s)
{
   str = s;
   return *this;
}

inline Identifier& Identifier::operator=(const std::string& s)
{
   str = s;
   return *this;
}

inline void Identifier::operator+=(const char* s)
{
   str = str + std::string(s);
}

inline void Identifier::operator+=(const std::string& s)
{
   str = str + s;
}

inline std::ostream& operator<<(std::ostream& sout, const Identifier& s)
{
   sout << s.getString();
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

   for (int i = 0; str[i] != '\0'; i++) {
      if ( !std::isdigit(str[i]) && str[i] != '.' ) {
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
      return std::atof(str.c_str());
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

   for (int i = 0; str[i] != '\0'; i++) {
      if ( !std::isdigit(str[i]) ) {
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
      return std::atoi(str.c_str());
   }
   return 0;
}

// replace spaces with underscore
inline void Identifier::replaceSpaces()
{
   std::replace(str.begin(), str.end(), ' ', '_');
}

// comparison operator: ==
inline bool operator==(const Identifier& s1, const Identifier& s2)
{
   if (s1.getStdString() == s2.getStdString()) {
      return true;
   }
   return false;
}

inline bool operator==(const char* s1, const Identifier& s2)
{
   if (s2.getStdString() == std::string(s1)) {
      return true;
   }
   return false;
}

inline bool operator==(const Identifier& s1, const char* s2)
{
   if (s1.getStdString() == std::string(s2)) {
      return true;
   }
   return false;
}

}
}

#endif
