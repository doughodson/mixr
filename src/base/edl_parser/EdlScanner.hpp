
#ifndef __mixr_base_edl_parser_EdlScanner_H_
#define __mixp_base_edl_parser_EdlScanner_H_

#ifndef __FLEX_LEXER_H
#define yyFlexLexer edlFlexLexer
#include "mixr/base/util/FlexLexer.h"
#undef yyFlexLexer
#endif

#include <fstream>

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: EdlScanner
// Base class:  FlexLexer -> edlFlexLexer -> EdlScanner
//
// Description: Scanner for the input files.
//
//------------------------------------------------------------------------------
class EdlScanner : public edlFlexLexer
{
public:
    EdlScanner(std::fstream* f) : edlFlexLexer(f) {}

    virtual int yylex();
    const char* getFilename()    { return fname; }
    int getLineNumber()          { return line; }

private:
    static const unsigned int FNAME_SIZE {256};
    char fname[FNAME_SIZE] {};   // current file name
    int line {1};                // current line number
};

}
}

#endif


