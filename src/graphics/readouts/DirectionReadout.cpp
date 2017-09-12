
#include "mixr/graphics/readouts/DirectionReadout.hpp"

#include "ReformatScanner.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(DirectionReadout, "DirectionReadout")
EMPTY_SLOTTABLE(DirectionReadout)
EMPTY_DELETEDATA(DirectionReadout)

DirectionReadout::DirectionReadout()
{
   STANDARD_CONSTRUCTOR()

   base::utStrcpy(format, FORMAT_LENGTH, "%+07.2f");
}

void DirectionReadout::copyData(const DirectionReadout& org, const bool)
{
   BaseClass::copyData(org);
   tmode = org.tmode;
}

//------------------------------------------------------------------------------
// input mode function --
//   filterInputEvent() -- Filter input events using a template character (tc)
//------------------------------------------------------------------------------
char DirectionReadout::filterInputEvent(const int event, const int tc)
{
   if (tc == '+') {
      // sign keys
      if (event == '4' && minusChar != '\0')
         return minusChar;
      else if (event == '7')
         return '-';
      else if (event == '6' && plusChar != '\0')
         return plusChar;
      else if (event == '9')
         return '+';
      else
         return '\0';
   }
   else if (tc == '0' || tc == 'D' || tc == 'M' || tc == 'S') {
      // Default numeric keys
      if ( event >= '0' && event <= '9' )
         return char(event);
      else
         return '\0';
   }
   else {
      return BaseClass::filterInputEvent(event,tc);
   }
}

//------------------------------------------------------------------------------
// getInputValue() -- returns the readout as a numeric value
//------------------------------------------------------------------------------
double DirectionReadout::getInputValue() const
{
   double value {};

   // copy string to buffer with correct sign character
   const std::size_t CBUFLOCAL_LEN {100};
   char cbuf[CBUFLOCAL_LEN] {};
   const char* p {*this};
   base::utStrcpy(cbuf, CBUFLOCAL_LEN, p);
   if (cbuf[0] == plusChar)  cbuf[0] = '+';
   if (cbuf[0] == minusChar) cbuf[0] = '-';

   switch (tmode) {

      case DirMode::ddmmss : {   // Degrees, Minutes, and seconds
         double degs {};
         double min {};
         double sec {};
         std::sscanf(cbuf, "%lf@%lf'%lf", &degs, &min, &sec);
         min += sec/60.0f;
         if (degs >= 0.0) value = degs + min/60.0f;
         else value = degs - min/60.0f;
      }
      break;

      case DirMode::ddmm : { // Degrees and minutes
         double degs {};
         double min {};
         std::sscanf(cbuf, "%lf@%lf", &degs, &min);
         if (degs >= 0.0) value = degs + min/60.0f;
         else value = degs - min/60.0f;
      }
      break;

      case DirMode::dd : {   // Degrees only
         std::sscanf(cbuf, "%lf", &value);
      }
      break;

      case DirMode::invalid: {   // not handled
      }
      break;
   }
   return static_cast<double>(value);
}

//------------------------------------------------------------------------------
//  makeText() -- make the text string use the current value and formats
//------------------------------------------------------------------------------
void DirectionReadout::makeText()
{
   bool neg {};
   double degrees {getFloat()};
   if (degrees < 0.0) {
      degrees = -degrees;
      neg = true;
   }

   switch (tmode) {

      case DirMode::ddmmss : { // Degrees, Minutes, and seconds
         int     ideg {static_cast<int>(degrees)};
         double  min  {(degrees - static_cast<double>(ideg))*60.0};
         int     imin {static_cast<int>(min)};
         double sec {(min - static_cast<double>(imin))*60.0};
         if (neg) ideg = -ideg;
         std::sprintf(cbuf, format, ideg, imin, sec);
      }
      break;

      case DirMode::ddmm : {   // Degrees and minutes
         int     ideg {static_cast<int>(degrees)};
         double  min  {(degrees - static_cast<double>(ideg))*60.0};
         if (neg) ideg = -ideg;
         std::sprintf(cbuf, format, ideg, min);
      }
      break;

      case DirMode::dd : { // Degrees only
         if (neg) degrees = -degrees;
         std::sprintf(cbuf, format, degrees);
      }
      break;

      case DirMode::invalid: {   // not handled
      }
      break;
   }

   // then turn any '@' characters to degree symbols.
   {
      std::size_t len {std::strlen(cbuf)};
      for (unsigned int i = 0; i < len; i++) {
         if (cbuf[i] == '@') cbuf[i] = static_cast<char>(0xB0);
      }
   }
}

//------------------------------------------------------------------------------
// reformat() -- convert the numerical value into an ascii character string
//------------------------------------------------------------------------------
void DirectionReadout::reformat(const char* const example)
{
   DirMode results {reformatter->convertDirection(example)};
   if (results != DirMode::invalid) {
      setExample(example);
      base::utStrcpy(format, FORMAT_LENGTH, reformatter->getFormat());
      tmode = results;
      postSign = reformatter->isPostSign();
      redisplay();
   }
}

}
}
