
#ifndef __mixr_base_String_H__
#define __mixr_base_String_H__

#include "mixr/base/Object.hpp"

#include "mixr/base/util/str_utils.hpp"

#include <cstdlib>
#include <cctype>
#include <cstring>
#include <iostream>
#include <string>

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: String
// Description: Character string class
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: String
// Slots: none
//------------------------------------------------------------------------------
// Notes:
//      Assignment Operators:     =
//      Concatenation Operators:  +  +=
//      Comparison Operators:     <  <=  ==  >=  >  !=
//      iostream Operations:      <<  >>
//
// EDL syntax: "strings are enclosed in quotes"
//------------------------------------------------------------------------------
class String final: public Object
{
    DECLARE_SUBCLASS(String, Object)

public:
    enum class Justify { NONE, LEFT, RIGHT, CENTER };
    static const int MAX_STRING_LENGTH{512};       // only by setString()

public:
    String(const char*);
    String(const char* s1, const char* s2);
    String();

    String& operator=(const char*);
    void operator+=(const char*);

    std::size_t len() const;                       // Returns the length of this string
    bool isEmpty() const;                          // Returns true if this string is empty

    const char* c_str() const;                     // Returns a pointer to a valid C-style string

    char getChar(const unsigned int index) const;  // Gets a specific character

    void empty();                                            // Empty this string.
    void setStr(const char*);                                // Sets us to a copy of 'string'
    void catStr(const char*);                                // Append a copy of 'string' to our text string
    void setChar(const unsigned int index, const char c);    // Sets a specific character

    // Returns true if this string is the ascii form of a number
    bool isNumber() const;

    // Returns the value of this string converted to a number or zero if isNumber() is false.
    double getNumber() const;

    // Returns true if this string is the ascii form of an integer number.
    bool isInteger() const;

    // Returns the value of this string converted to an integer value or zero if isInteger() is false.
    int getInteger() const;

    // Returns in "subStr" a "numChars" character sub-string of 'this' string that starts a "startIndex"
    bool getSubString(String& subStr, const unsigned int startIndex, const std::size_t numChars) const;

    // Set to the first 'w' characters of 'str'.
    // If the length of 'str' is less than 'w', than justify as 'j'.
    void setString(const String& str, const std::size_t w, const Justify j = Justify::NONE);

private:
    char* cstr{};         // the character string
    std::size_t n{};      // length of this string
    std::size_t nn{};     // length of the memory allocated for this string
};

//------------------------------------------------------------------------------
// String append operators: + and +=
//------------------------------------------------------------------------------
inline void String::operator+=(const char* s)
{
    catStr(s);
}

inline String operator+(const String& s1, const String& s2)
{
    return String(s1.c_str(), s2.c_str());
}

inline String operator+(const char* s1, const String& s2)
{
    return String(s1, s2.c_str());
}

inline String operator+(const String& s1, const char* s2)
{
    return String(s1.c_str(), s2);
}

//------------------------------------------------------------------------------
// len() -- returns the length of this string.
//------------------------------------------------------------------------------
inline std::size_t String::len() const
{
    return n;
}

//------------------------------------------------------------------------------
// isEmpty() -- returns true if this is an empty string.
//------------------------------------------------------------------------------
inline bool String::isEmpty() const
{
    return len() == 0;
}


//------------------------------------------------------------------------------
// getChar() -- returns a character from the string.
//------------------------------------------------------------------------------
inline char String::getChar(const unsigned int index) const
{
   if (cstr != nullptr && index <= len()) {
      return cstr[index];
   } else {
      return '\0';
   }
}


inline const char* String::c_str() const
{
    return cstr;
}

//------------------------------------------------------------------------------
// isInteger() -- is this an a ascii integer
//------------------------------------------------------------------------------
inline bool String::isInteger() const
{
    if (isEmpty()) return false;
    for (int i = 0; cstr[i] != '\0'; i++) {
        if ( !std::isdigit(cstr[i]) ) return false;
    }
    return true;
}


//------------------------------------------------------------------------------
// isNumber() -- is this an a ascii number
//------------------------------------------------------------------------------
inline bool String::isNumber() const
{
    if (isEmpty()) return false;
    for (int i = 0; cstr[i] != '\0'; i++) {
        if ( !std::isdigit(cstr[i]) && cstr[i] != '.' ) return false;
    }
    return true;
}


//------------------------------------------------------------------------------
// getInteger() -- convert a ascii number string to an integer
//------------------------------------------------------------------------------
inline int String::getInteger() const
{
    if (isInteger()) return std::atoi(cstr);
    else return 0;
}


//------------------------------------------------------------------------------
// getNumber() -- convert a ascii number string to a number
//------------------------------------------------------------------------------
inline double String::getNumber() const
{
    if (isNumber()) return  std::atof(cstr);
    else return 0.0;
}

//------------------------------------------------------------------------------
// Comparison operators: ==, !=, <, <=, >, and >=
//------------------------------------------------------------------------------
inline bool operator==(const String& s1, const String& s2)
{
   bool ok {};
   if (s1.isEmpty() || s2.isEmpty()) {
      // if a string is null, then both must be null to be equal
      if (s1.isEmpty() && s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1.c_str(), s2.c_str()) == 0);
   return ok;
}

inline bool operator==(const char* s1, const String& s2)
{
   bool ok {};
   if (s1 == nullptr || s2.isEmpty()) {
      // if a string is null, then both must be null to be equal
      if (s1 == nullptr && s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2.c_str()) == 0);
   return ok;
}

inline bool operator==(const String& s1, const char* s2)
{
   bool ok {};
   if (s1.isEmpty() || s2 == nullptr) {
      // if a string is null, then both must be null to be equal
      if (s1.isEmpty() && s2 == nullptr) ok = true;
   }
   else ok = (std::strcmp(s1.c_str(), s2) == 0);
   return ok;
}

inline bool operator!=(const String& s1, const String& s2)
{
   bool ok {};
   if (s1.isEmpty() || s2.isEmpty()) {
      // if a string is null, then only one must be null to be not equal
      if (!s1.isEmpty() || !s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1.c_str(), s2.c_str()) != 0);
   return ok;
}

inline bool operator!=(const char* s1, const String& s2)
{
   bool ok {};
   if (s1 == nullptr || s2.isEmpty()) {
      // if a string is null, then only one must be null to be not equal
      if (s1 != nullptr || !s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2.c_str()) != 0);
   return ok;
}

inline bool operator!=(const String& s1, const char* s2)
{
   bool ok {};
   if (s1.isEmpty() || s2 == nullptr) {
      // if a string is null, then only one must be null to be not equal
      if (!s1.isEmpty() || s2 != nullptr) ok = true;
   }
   else ok = (std::strcmp(s1.c_str(), s2) != 0);
   return ok;
}

inline bool operator<(const String& s1, const String& s2)
{
   bool ok {};
   if (s1.isEmpty() || s2.isEmpty()) {
      // if a string is null, then only s1 must be null to be less than s2
      if (s1.isEmpty() && !s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1.c_str(), s2.c_str()) < 0);
   return ok;
}

inline bool operator<(const char* s1, const String& s2)
{
   bool ok {};
   if (s1 == nullptr || s2.isEmpty()) {
      // if a string is null, then only s1 must be null to be less than s2
      if (s1 == nullptr && !s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2.c_str()) < 0);
   return ok;
}

inline bool operator<(const String& s1, const char* s2)
{
   bool ok {};
   if (s1.isEmpty() || s2 == nullptr) {
      // if a string is null, then only s1 must be null to be less than s2
      if (s1.isEmpty() && s2 != nullptr) ok = true;
   }
   else ok = (std::strcmp(s1.c_str(), s2) < 0);
   return ok;
}

inline bool operator<=(const String& s1, const String& s2)
{
   bool ok {};
   if (s1.isEmpty() || s2.isEmpty()) {
      // if a string is null, then s1 must be null to be less than or equal s2.
      if (s1.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1.c_str(), s2.c_str()) <= 0);
   return ok;
}

inline bool operator<=(const char* s1, const String& s2)
{
   bool ok {};
   if (s1 == nullptr || s2.isEmpty()) {
      // if a string is null, then s1 must be null to be less than or equal s2.
      if (s1 == nullptr) ok = true;
   }
   else ok = (std::strcmp(s1, s2.c_str()) <= 0);
   return ok;
}

inline bool operator<=(const String& s1, const char* s2)
{
   bool ok {};
   if (s1.isEmpty() || s2 == nullptr) {
      // if a string is null, then s1 must be null to be less than or equal s2.
      if (s1.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1.c_str(), s2) <= 0);
   return ok;
}

inline bool operator>(const String& s1, const String& s2)
{
   bool ok {};
   if (s1.isEmpty() || s2.isEmpty()) {
      // if a string is null, then only s2 must be null to be less than s1
      if (!s1.isEmpty() && s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1.c_str(), s2.c_str()) > 0);
   return ok;
}

inline bool operator>(const char* s1, const String& s2)
{
   bool ok {};
   if (s1 == nullptr || s2.isEmpty()) {
      // if a string is null, then only s2 must be null to be less than s1
      if (s1 != nullptr && s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2.c_str()) > 0);
   return ok;
}

inline bool operator>(const String& s1, const char* s2)
{
   bool ok {};
   if (s1.isEmpty() || s2 == nullptr) {
      // if a string is null, then only s2 must be null to be less than s1
      if (!s1.isEmpty() && s2 == nullptr) ok = true;
   }
   else ok = (std::strcmp(s1.c_str(), s2) > 0);
   return ok;
}

inline bool operator>=(const String& s1, const String& s2)
{
   bool ok {};
   if (s1.isEmpty() || s2.isEmpty()) {
      // if a string is null, then s2 must be null to be less than or equal s1
      if (s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1.c_str(), s2.c_str()) >= 0);
   return ok;
}

inline bool operator>=(const char* s1, const String& s2)
{
   bool ok {};
   if (s1 == nullptr || s2.isEmpty()) {
      // if a string is null, then s2 must be null to be less than or equal s1
      if (s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2.c_str()) >= 0);
   return ok;
}

inline bool operator>=(const String& s1, const char* s2)
{
   bool ok {};
   if (s1.isEmpty() || s2 == nullptr) {
      // if a string is null, then s2 must be null to be less than or equal s1
      if (s2 == nullptr) ok = true;
   }
   else ok = (std::strcmp(s1.c_str(), s2) >= 0);
   return ok;
}

//------------------------------------------------------------------------------
// output stream operator <<
//------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& sout, const String& s)
{
    sout << "\"";
    if (!s.isEmpty()) {
        const char* str = s.c_str();
        sout << str;
    }
    sout << "\"";
    return sout;
}

//------------------------------------------------------------------------------
// input stream operator >>
//------------------------------------------------------------------------------
inline std::istream& operator>>(std::istream& sin, String& s)
{
    const int a {256};
    const auto buf = new char[a+1];
    int   n1 {};
    char  c {};

    sin.get(c);
    while ( (c != '\n') && !sin.eof() && n1 < a) {
        buf[n1++] = c;
        sin.get(c);
    }

    buf[n1] = '\0';

    s.setStr(buf);

    delete[] buf;

    return sin;
}

}
}

#endif
