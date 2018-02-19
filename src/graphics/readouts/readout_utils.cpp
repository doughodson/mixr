

#include "mixr/graphics/readouts/readout_utils.hpp"

#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// processInteger() -- process a integer number string
//    Take in a format such as "+0##" and return "%+04d".
//------------------------------------------------------------------------------
std::string processInteger(const DataType dataType, const std::string& example, bool* postSign)
{
   // copy example to text buffer for processing (this mimics older interface)
   char text[256]{};
   std::strncpy(text, example.c_str(), 255);
   text[255] = '\0';
   const int len{static_cast<int>(example.length())};
   // storage for output format specification
   char format[256]{};

   // ---
   // Scan the example string
   // ---

   // Check for the sign
   int i{};
   int s{};

   if (text[i] == '+') {
      s = 1;
      i++;
   }

   // Check for an leading zeros
   int lz{};
   while (text[i] == '0' && i < len) {
      lz++;
      i++;
   }

   // Check for an leading numbers
   int ln{};
   while (text[i] == '#' && i < len) {
      ln++;
      i++;
   }

   // Check for the post sign
   if (text[i] == '+') {
      *postSign = true;
      s = 1;
      i++;
   }

   // ---
   // Build the format statement
   // ---

   // Computer total number of characters in numberic field:
   //    sign, leading zeros, and leading numbers.
   int fw{s + lz + ln};

   // Create the sprintf() format string
   int j{};

   format[j++] = '%';				// Start with %

   if (s)
      format[j++] = '+';			// Add a '+' if signed

   if (lz > 0)
      format[j++] = '0';			// Add a '0' if leading zeros

   j += std::sprintf(&format[j], "%d", fw);		// Add total field size

   if (dataType == DataType::number) {
      format[j++] = '.';
      format[j++] = '0';
      format[j++] = 'f';
   }
   else if (dataType == DataType::octal) format[j++] = 'o';
   else if (dataType == DataType::hex) format[j++] = 'X';

   format[j] = '\0';

   return std::string(format);
}

//------------------------------------------------------------------------------
// processFloat() -- process a floating point number string
//    Take in a format such as "+0###.###" and return "%+09.3f".
//------------------------------------------------------------------------------
std::string processFloat(const DataType dataType, const std::string& example, bool* postSign)
{
   // copy example to text buffer for processing (this mimics older interface)
   char text[256]{};
   std::strncpy(text, example.c_str(), 255);
   text[255] = '\0';
   const int len{static_cast<int>(example.length())};
   // storage for output format specification
   char format[256]{};

   // ---
   // Scan the example string
   // ---

   // Check for the sign
   int i{};
   int s{};

   if (text[i] == '+') {
      s = 1;
      i++;
   }

   // Check for an leading zeros
   int lz{};
   while (text[i] == '0' && i < len) {
      lz++;
      i++;
   }

   // Check for an leading numbers
   int ln{};
   while (text[i] == '#' && i < len) {
      ln++;
      i++;
   }

   // Skip the decimal point
   i++;

   // Check for the trailing numbers
   int nr{};
   while (text[i] == '#' && i < len) {
      nr++;
      i++;
   }

   // Check for the post sign
   if (text[i] == '+') {
      *postSign = true;
      s = 1;
      i++;
   }

   // ---
   // Build the format statement
   // ---

   // Computer total number of characters in numberic field:
   //    sign, leading zeros, leading numbers, decimal point, and
   //    trailing numbers.
   int fw{s + lz + ln + 1 + nr};

   // Create the sprintf() format string
   int j{};

   format[j++] = '%';                           // Start with %

   if (s)
      format[j++] = '+';                        // Add a '+' if signed

   if (lz > 0)
      format[j++] = '0';                        // Add a '0' if leading zeros

   j += std::sprintf(&format[j], "%d", fw);     // Add total field size
   j += std::sprintf(&format[j], ".%d", nr);    // Add trailing numbers

   format[j++] = 'f';                           // Add the data type

   format[j] = '\0';

   return std::string(format);
}

//------------------------------------------------------------------------------
// processTime() -- process a time format string
//    Take in a format such as "0MSS.S" and return "%02d%04.1f".
//------------------------------------------------------------------------------
std::string processTime(const TimeMode tm, const std::string& example, bool* postSign)
{
   // copy example to text buffer for processing (this mimics older interface)
   char text[256]{};
   std::strncpy(text, example.c_str(), 255);
   text[255] = '\0';
   const int len{static_cast<int>(example.length())};
   // storage for output format specification
   char format[256]{};

   // Check sign
   int i{};
   int s{};

   if (text[i] == '+') {
      s = 1;
      i++;
   }

   // Check leading zeros
   int lz {};
   if (text[i] == '0') {
      lz = 1;
      i++;
   }

   // Handle hours
   int hh{};
   while (text[i] == 'H' && i < len) {
      hh++;
      i++;
   }

   int hc{};
   int hd{};
   int hr{};

   // A colon or a decimal point is optional after hours
   if (text[i] == ':') {
      hc = 1;
      i++;
   }
   else if (text[i] == '.') {
      hd = 1;
      i++;

      while (text[i] == 'H' && i < len) {
         hr++;
         i++;
      }
   }

   // Handle minutes
   int mm{};
   while (text[i] == 'M' && i < len) {
      mm++;
      i++;
   }

   int mc{};
   int md{};
   int mr{};

   // A colon or a decimal point is optional after minutes
   if (text[i] == ':') {
      mc = 1;
      i++;
   }
   else if (text[i] == '.') {
      md = 1;
      i++;

      while (text[i] == 'M' && i < len) {
         mr++;
         i++;
      }
   }

   // Handle seconds
   int ss{};
   while (text[i] == 'S' && i < len) {
      ss++;
      i++;
   }

   int sr{};

   // A decimal point is optional with seconds
   if (text[i] == '.') {
      i++;

      while (text[i] == 'S' && i < len) {
         sr++;
         i++;
      }
   }

   // Check for the post sign
   if (text[i] == '+') {
      *postSign = true;
      s = 1;
      i++;
   }

   // Build format string
   int j{};

   format[j++] = '%';

   if (s)
      format[j++] = '+';

   if (lz)
      format[j++] = '0';

   // Build the format for "HH[:.]".  Add in s and lz into fw.
   if (hh > 0) {
      if (s) hh++;
      if (lz) hh++;

      if (hd || mm == 0)
         j+= std::sprintf(&format[j], "%d.%df", hh+hr+(hr>0), hr);
      else
         j+= std::sprintf(&format[j], "%dd", hh);

      if (hc)
         format[j++] = ':';

      if (mm > 0) {
         format[j++] = '%';
         format[j++] = '0';
      }
   }

   // Build the format for "MM[:.]".  Add in s and lz into fw if
   // there are NO hours.
   if (mm > 0) {
      if (s  && hh <= 0) mm++;
      if (lz && hh <= 0) mm++;

      if (md || ss == 0)
         j+= std::sprintf(&format[j], "%d.%df", mm+mr+(mr>0), mr);
      else
         j+= std::sprintf(&format[j], "%dd", mm);

      if (mc)
         format[j++] = ':';

      if (ss > 0) {
         format[j++] = '%';
         format[j++] = '0';
      }
   }

   // Build the format for "SS[.]".  Add in s and lz into fw if
   // there are NO hours or minutes.
   if (ss > 0) {
      if (s  && hh <= 0 && mm <= 0) ss++;
      if (lz && hh <= 0 && mm <= 0) ss++;

      j+= std::sprintf(&format[j], "%d.%df", ss+sr+(sr>0), sr);
   }

   format[j] = '\0';

   return std::string(format);
}


//------------------------------------------------------------------------------
// processDirection() -- process a directional format string
//    Take in a format such as "+120@23'43.2"" and return
//    "+%03d@%02d'%4.1f""
//------------------------------------------------------------------------------
std::string processDirection(const DirMode dm, const std::string& example, bool* postSign)
{
   // copy example to text buffer for processing (this mimics older interface)
   char text[256]{};
   std::strncpy(text, example.c_str(), 255);
   text[255] = '\0';
   const int len{static_cast<int>(example.length())};
   // storage for output format specification
   char format[256]{};

   // Check sign
   int i{};
   int s{};

   if (text[i] == '+') {
      s = 1;
      i++;
   }

   // Check leading zeros
   int lz{};
   if (text[i] == '0') {
      lz = 1;
      i++;
   }

   // Handle degrees
   int dd{};
   while (text[i] == 'D' && i < len) {
      dd++;
      i++;
   }

   char dc{'\0'};
   int  ddc{};
   int  dr{};

   if (text[i] == '.' && i < len) {
      ddc = 1;
      i++;

      while (text[i] == 'D' && i < len) {
         dr++;
         i++;
      }
   }
   if (text[i] != 'M' && text[i] != '+' && i < len)
      dc = text[i++];

   // Handle minutes
   int mm{};
   while (text[i] == 'M' && i < len) {
      mm++;
      i++;
   }

   char mc{'\0'};
   int md{};
   int mr{};

   if (text[i] == '.' && i < len) {
      md = 1;
      i++;

      while (text[i] == 'M' && i < len) {
         mr++;
         i++;
      }
   }
   if (text[i] != 'S' && text[i] != '+' && i < len)
      mc = text[i++];

   // Handle seconds
   int ss{};
   while (text[i] == 'S' && i < len) {
      ss++;
      i++;
   }

   char sc{'\0'};
   int  sr{};

   if (text[i] == '.' && i < len) {
      i++;

      while (text[i] == 'S' && i < len) {
         sr++;
         i++;
      }
   }
   if (i < len && text[i] != '+')
      sc = text[i++];

   // Check for the post sign
   if (text[i] == '+') {
      *postSign = true;
      s = 1;
      i++;
   }

   // Build format string
   int j{};

   format[j++] = '%';

   if (s)
      format[j++] = '+';

   if (lz)
      format[j++] = '0';

   // Build the format for "DDD[.]".  Add in s and lz into fw.
   if (dd > 0) {
      if (s) dd++;
      if (lz) dd++;
      if (ddc || mm == 0)
         j+= std::sprintf(&format[j], "%d.%df", dd+dr+(dr>0), dr);
      else
         j+= std::sprintf(&format[j], "%dd", dd);

      if (dc)
         format[j++] = dc;

      if (mm > 0) {
         format[j++] = '%';
         format[j++] = '0';
      }
   }

   // Build the format for "MM[.]".  Add in s and lz into fw if
   // there are NO hours.
   if (mm > 0) {
      if (s  && dd <= 0) mm++;
      if (lz && dd <= 0) mm++;

      if (md || ss == 0)
         j+= std::sprintf(&format[j], "%d.%df", mm+mr+(mr>0), mr);
      else
         j+= std::sprintf(&format[j], "%dd", mm);

      if (mc)
         format[j++] = mc;

      if (ss > 0) {
         format[j++] = '%';
         format[j++] = '0';
      }
   }

   // Build the format for "SS[.]".  Add in s and lz into fw if
   // there are NO hours or minutes.
   if (ss > 0) {
      if (s  && dd <= 0 && mm <= 0) ss++;
      if (lz && dd <= 0 && mm <= 0) ss++;

      j+= std::sprintf(&format[j], "%d.%df", ss+sr+(sr>0), sr);

      if (sc)
         format[j++] = sc;
   }

   format[j] = '\0';

   return std::string(format);
}

}
}
