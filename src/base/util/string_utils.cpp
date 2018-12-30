
#include "mixr/base/util/string_utils.hpp"

#include <algorithm>
#include <cctype>
#include <string>

namespace mixr {
namespace base {

//------------
// return a standard string aligned within a given width and justification
//------------
// Set to the first 'w' characters of 'str'.
// If the length of 'str' is less than 'w', than justify as 'j'.
//void String::setString(const String& origStr, const std::size_t w, const Justify j)
std::string align(const std::string& str, const std::size_t w, const Justify j)
{
   const int MAX_STRING_LENGTH{512};

   char sbuf[MAX_STRING_LENGTH+1] {};  // source buffer
   char dbuf[MAX_STRING_LENGTH+1] {};  // destination buffer
   const char* ss{sbuf};               // pointer to source buffer

/*
   // ---
   // when w is zero or str is empty
   // ---

   if (w == 0 || str.len() == 0) {
      dbuf[0] = '\0';
      *this = dbuf;
      return;
   }

   // ---
   // When justified (left, right, center), make the copy without leading
   // or trailing spaces.
   // ---

   if (j != Justify::None) {
      // Justified:  copy without leading or trailing spaces
      const char* p {str.c_str()};
      char* q {sbuf};
      while (*p != '\0' && *p == ' ') { p++; }
      while (*p != '\0' && q <= &sbuf[MAX_STRING_LENGTH-1]) { *q++ = *p++; }
      *q-- = '\0';
      while (*q == ' ' && q >= sbuf) { *q-- = ' '; }
   } else {
      // Not justified:  change our source buffer pointer to the orig string
      ss = str.c_str();
   }


   // ---
   // Set this to the new string justified
   // ---

   int i1 {};                                          // Source index
   int i2 {};                                          // Destination index
   int l1 {static_cast<int>(std::strlen(ss))};         // Source string length
   if (l1 > MAX_STRING_LENGTH) l1 = MAX_STRING_LENGTH;
   int l2 {static_cast<int>(w)};                       // Destination string length
   if (l2 > MAX_STRING_LENGTH) l2 = MAX_STRING_LENGTH;
   int d {l2 - l1};                                    // Difference in lengths

   switch (j) {

      // none or left justified
      case Justify::None :
      case Justify::Left :
      {
         if (d < 0) l1 += d;
         while (i1 < l1) { dbuf[i2++] = ss[i1++]; }
         while (i2 < l2) { dbuf[i2++] = ' '; }
      }
      break;

      // right justified
      case Justify::Right : {
         if (d < 0) i1 = -d;
         while (i2 < d)  { dbuf[i2++] = ' '; }
         while (i1 < l1) { dbuf[i2++] = ss[i1++]; }
      }
      break;

      // center justified
      case Justify::Center : {
         int n1 = d/2;
         if (d < 0) { i1 = -n1; l1 += (d-n1); }
         while (i2 < n1) { dbuf[i2++] = ' '; }
         while (i1 < l1) { dbuf[i2++] = ss[i1++]; }
         while (i2 < l2) { dbuf[i2++] = ' '; }
      }
      break;
   }

   dbuf[i2] = '\0';
   *this = dbuf;
*/
   return "";
}

// is this an a ascii number?
bool isNumber(const std::string& s)
{
   if (s.empty()) {
      return false;
   }

   for (int i = 0; s[i] != '\0'; i++) {
      if ( !std::isdigit(s[i]) && s[i] != '.' ) {
         return false;
      }
   }
   return true;
}

// convert a ascii number string to a number
double getNumber(const std::string& s)
{
   if (isNumber(s)) {
      return std::atof(s.c_str());
   }
   return 0.0;
}

// is this an a ascii integer
bool isInteger(const std::string& s)
{
   if (s.empty()) {
      return false;
   }

   for (int i = 0; s[i] != '\0'; i++) {
      if ( !std::isdigit(s[i]) ) {
         return false;
      }
   }
   return true;
}

// convert a ascii number string to an integer
int getInteger(const std::string& s)
{
   if (isInteger(s)) {
      return std::atoi(s.c_str());
   }
   return 0;
}

}
}
