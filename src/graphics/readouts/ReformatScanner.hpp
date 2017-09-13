
#ifndef __mixr_graphics_ReformatScanner_H__
#define __mixr_graphics_ReformatScanner_H__

#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer rfFlexLexer
#include "mixr/base/util/FlexLexer.h"
#endif

#include "mixr/graphics/readouts/TimeReadout.hpp"
#include "mixr/graphics/readouts/DirectionReadout.hpp"

namespace mixr {
namespace graphics {

//---------------------------------------------
// Scanner for the Readout reformatter
//---------------------------------------------
// Reformat converts example strings into C/C++ format specifiers for
// sprintf, printf, etc.  These examples will be later used as templates
// for data entry.
class ReformatScanner : public rfFlexLexer
{
public:
   enum class DataType { invalid, number, octal, hex, time, dir };

   ReformatScanner() : rfFlexLexer()        { }
   virtual ~ReformatScanner()               { }

   const char* getFormat() const            { return dataType != DataType::invalid ? format : nullptr; }

   DataType convertNumber(const char*);                       // called by NumericReadout
   DataType convertOctal(const char*);                        // called by OctalReadout
   DataType convertHex(const char*);                          // called by HexReadout
   TimeReadout::TimeMode convertTime(const char*);            // called by TimeReadout
   DirectionReadout::DirMode convertDirection(const char*);   // called by DirectionReadout

   bool isPostSign()                        { return postSign; }

protected:
   virtual int yylex();
   virtual int yylex(const DataType);

   int processInteger(const char* text, const int len);
   int processFloat(const char* text, const int len);
   int processTime(const TimeReadout::TimeMode tm, const char* text, const int len);
   int processDirection(const DirectionReadout::DirMode dm, const char* text, const int len);

   int formatError(const char* text);

private:
   DataType dataType {DataType::invalid};
   char format[256] {};
   bool postSign {};
};

}
}

#endif
