#----------------------------------------------------------------------------------------
# To build scanner:
#
# cd readouts
# dos2unix reformat_scanner.l
# flex -f -oReformatScanner.cpp reformat_scanner.l   // creates ReformatScanner.cpp
#   (ReformatScanner.hpp already exists)
#
# search for "FlexLexer.h" in ReformatScanner.cpp - replace:
#   #include <FlexLexer.h>
# with
#   #include "FlexLexer.h"
#----------------------------------------------------------------------------------------

