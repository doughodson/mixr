# To build parser and scanner:
# -----------------------------
# cd edl
# dos2unix edl_parser.y
# dos2unix edl_scanner.l
# bison -d edl_parser.y -o EdlParser.cpp  // creates EdlParser.cpp and EdlParser.hpp
# flex -f -oEdlScanner.cpp edl_scanner.l  // creates EdlScanner.cpp (EdlScanner.hpp already exists)
#
# search for "FlexLexer.h" in EdlScanner.cpp - replace:
#   #include <FlexLexer.h>
# with
#   #include "mixr/base/util/FlexLexer.h"
