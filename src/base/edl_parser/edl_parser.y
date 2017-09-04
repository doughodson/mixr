//------------------------------------------------------------------------------
// Description: Parses EDL
//
// Factory function:
//
//    Object* (*factory_func)(const std::string& name)
//
//       This is a user supplied function, via our constructor, that is
//       used by the parser to create objects using their 'factory' names
//       (see Object.hpp).  The object's factory name is passed to
//       the function.  The function will construct a default object and
//       return a pointer to the new object.  If the name is not
//       recognized then no object is created and nullptr is returned.
//--------------------------------------------------------------------------

%token	IDENT SLOT_ID
%token	INTEGERconstant  FLOATINGconstant  BOOLconstant STRING_LITERAL

%{

#include <cstdio>
#include <string>
#include <fstream>

#include "mixr/base/edl_parser.hpp"
#include "mixr/base/Object.hpp"
#include "mixr/base/String.hpp"
#include "mixr/base/Identifier.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/Float.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/List.hpp"
#include "EdlScanner.hpp"

static mixr::base::Object* result {};          // result of all our work (i.e., an Object)
static mixr::base::EdlScanner* scanner {};     // edl scanner
static mixr::base::factory_func factory {};    // factory function 
static int err_count {};                       // error count

//------------------------------------------------------------------------------
// yylex() -- user defined; used by the parser to call the lexical generator
//------------------------------------------------------------------------------
inline int yylex()
{
   return scanner->yylex();
}

//------------------------------------------------------------------------------
// yyerror() -- user defined; use by the parser to report errors.
//------------------------------------------------------------------------------
inline void yyerror(const char* s)
{
   std::string filename(scanner->getFilename());
   if (filename.empty()) {
      std::cerr << "At line ";
   } else {
      std::cerr << "In " << filename << ", line ";
   }
   std::cerr << scanner->getLineNumber() << ": ";
   std::cerr << s << std::endl;
   err_count++;
}

//------------------------------------------------------------------------------
// parse() -- returns an object with factory 'name' with its slots set to
//            values in 'arg_list'
//------------------------------------------------------------------------------
static mixr::base::Object* parse(const std::string& name, mixr::base::PairStream* arg_list)
{
    mixr::base::Object* obj {};

    if (factory != nullptr) {

        // call user provided factory() to construct an object
        obj = factory(name);

        // set slots in our new object
        if (obj != nullptr && arg_list != nullptr) {
            mixr::base::List::Item* item {arg_list->getFirstItem()};
            while (item != nullptr) {
                mixr::base::Pair* p {static_cast<mixr::base::Pair*>(item->getValue())};
                bool ok {obj->setSlotByName(*p->slot(), p->object())};
                if (!ok) {
                    std::string msg = "error while setting slot name: " + std::string(*p->slot());
                    yyerror(msg.c_str());
                }
                item = item->getNext();
            }
            bool ok {obj->isValid()};
            if (!ok) {
                std::string msg = "error: invalid object: " + name;
                yyerror(msg.c_str());
            }
        }
        else if (obj == nullptr) {
            std::string msg = "undefined factory name: " + name;
            yyerror(msg.c_str());
        }
    }
    return obj;
}

%}

// Defines types that our values can be, yylval.
%union {
   double                     dval;
   long                       lval;
   bool                       bval;
   char*                      cvalp;
   mixr::base::Object*        ovalp;
   mixr::base::Pair*          pvalp;
   mixr::base::PairStream*    svalp;
   mixr::base::List*          lvalp;
   mixr::base::Number*        nvalp;
}

// Add types to some for our tokens and rules
%type  <lval>   INTEGERconstant
%type  <bval>   BOOLconstant
%type  <dval>   FLOATINGconstant
%type  <cvalp>  STRING_LITERAL
%type  <cvalp>  IDENT
%type  <cvalp>  SLOT_ID
%type  <svalp>  arglist
%type  <ovalp>  form
%type  <ovalp>  prim
%type  <pvalp>  slot_value
%type  <lvalp>  numlist
%type  <nvalp>  number


//--------------------------------------------------------------------------
// The grammar rules ---
//--------------------------------------------------------------------------
%%
file    : form                      { result = $1; }
        | SLOT_ID form              { if ($2 != 0) { result = new mixr::base::Pair($1, $2); delete[] $1; $2->unref(); } }
        ;

arglist :                           { $$ = new mixr::base::PairStream(); }

        | arglist form              { if ($2 != 0) {
                                        int i = $1->entries();
                                        char cbuf[20] {};
                                        std::sprintf(cbuf, "%i", i+1);
                                        mixr::base::Pair* p {new mixr::base::Pair(cbuf, $2)};
                                        $2->unref();
                                        $1->put(p);
                                        p->unref();
                                        $$ = $1;
                                      }
                                    }

        | arglist prim              {
                                    int i = $1->entries();
                                    char cbuf[20] {};
                                    std::sprintf(cbuf, "%i", i+1);
                                    mixr::base::Pair* p {new mixr::base::Pair(cbuf, $2)};
                                    $2->unref();
                                    $1->put(p);
                                    p->unref();
                                    $$ = $1;
                                    }

        | arglist slot_value        { $1->put($2); $2->unref(); $$ = $1; }
        ;


form    : '(' IDENT arglist ')'     { $$ = parse($2, $3); delete[] $2; $3->unref(); }

        | '{' arglist '}'           { $$ = (mixr::base::Object*) $2; }
        ;


slot_value  : SLOT_ID prim          { $$ = new mixr::base::Pair($1, $2); delete[] $1; $2->unref(); }
        | SLOT_ID form              { $$ = new mixr::base::Pair($1, $2); delete[] $1; $2->unref(); }
        ;

prim    : STRING_LITERAL            { $$ = new mixr::base::String($1); delete[] $1; }
        | IDENT                     { $$ = new mixr::base::Identifier($1); delete[] $1; }
        | BOOLconstant              { $$ = new mixr::base::Boolean($1); }
        | '[' numlist ']'           { $$ = $2; }
        | number                    { $$ = $1; }
        ;

numlist : number                    { $$ = new mixr::base::List(); $$->put($1); $1->unref(); }
        | numlist number            { $$ = $1; $$->put($2); $2->unref(); }
        ;

number  : INTEGERconstant           { $$ = new mixr::base::Integer($1); }
        | FLOATINGconstant          { $$ = new mixr::base::Float($1); }
        ;
%%

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Returns an Object* that was constructed from parsing an EDL file.
// factory is the name of the Object creation function  
//------------------------------------------------------------------------------
Object* edl_parser(const std::string& filename, factory_func f, int* num_errors)
{
    // set the global file scope static variables
    factory = f;
    result = nullptr;
    err_count = 0;

    // open the text file and create the scanner
    std::fstream fin;
    fin.open(filename, std::ios::in);
    scanner = new EdlScanner(&fin);

    //yydebug = 1;
    Object* obj {};
    if (yyparse() == 0) {    // returns 0 on success
        obj = result;
    }

    // close the text file and delete the scanner
    fin.close();
    delete scanner;
    scanner = nullptr;

    // if we have a good pointer, set the number of errors encountered
    if (num_errors != nullptr) {
        *num_errors = err_count;
    }
    return obj;
}

}
}

