
#include "mixr/graphics/readouts/HexReadout.hpp"

#include "ReformatScanner.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(HexReadout, "HexReadout")
EMPTY_SLOTTABLE(HexReadout)
EMPTY_COPYDATA(HexReadout)
EMPTY_DELETEDATA(HexReadout)

HexReadout::HexReadout()
{
   STANDARD_CONSTRUCTOR()
   base::utStrcpy(format, FORMAT_LENGTH, "%X");
}

//------------------------------------------------------------------------------
// input mode function --
//   filterInputEvent() -- Filter input events using a template character (tc)
//------------------------------------------------------------------------------
char HexReadout::filterInputEvent(const int event, const int tc)
{
   if (tc == '0' || tc == '#') {
      // Default numeric keys
      if ( (event >= '0' && event <= '9') || (event >= 'A' && event <= 'F') )
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
double HexReadout::getInputValue() const
{
   int value {};

   // copy string to buffer with correct sign character
   const std::size_t CBUFLOCAL_LEN {100};
   char cbuf[CBUFLOCAL_LEN] {};
   const char* p {*this};
   base::utStrcpy(cbuf, CBUFLOCAL_LEN, p);
   if (cbuf[0] == plusChar)  cbuf[0] = '+';
   if (cbuf[0] == minusChar) cbuf[0] = '-';

   std::sscanf(cbuf, format, &value);
   return static_cast<double>(value);
}

//------------------------------------------------------------------------------
//  makeText() -- make the text string use the current value and formats
//------------------------------------------------------------------------------
void HexReadout::makeText()
{
   std::sprintf(cbuf, format, getInt());
}

//------------------------------------------------------------------------------
// reformat() -- convert the numerical value into an ascii character string
//------------------------------------------------------------------------------
void HexReadout::reformat(const char* const example)
{
   if (reformatter->convertHex(example) != ReformatScanner::DataType::invalid) {
      setExample(example);
      base::utStrcpy(format, FORMAT_LENGTH, reformatter->getFormat());
      postSign = reformatter->isPostSign();
      redisplay();
   }
}

}
}
