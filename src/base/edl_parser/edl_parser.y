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

#include "mxrp/base/edl_parser.hpp"
#include "mxrp/base/Object.hpp"
#include "mxrp/base/String.hpp"
#include "mxrp/base/Identifier.hpp"
#include "mxrp/base/Integer.hpp"
#include "mxrp/base/Float.hpp"
#include "mxrp/base/Boolean.hpp"
#include "mxrp/base/Pair.hpp"
#include "mxrp/base/PairStream.hpp"
#include "mxrp/base/List.hpp"
#include "EdlScanner.hpp"

static mxrp::base::Object* result;             // result of all our work (i.e., an Object)
static mxrp::base::EdlScanner* scanner;        // edl scanner
static mxrp::base::factory_func factory;       // factory function 
static unsigned int err_count;                 // error count

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
static mxrp::base::Object* parse(const std::string& name, mxrp::base::PairStream* arg_list)
{
    mxrp::base::Object* obj {};

    if (factory != nullptr) {

        // call user provided factory() to construct an object
        obj = factory(name);

        // set slots in our new object
        if (obj != nullptr && arg_list != nullptr) {
            mxrp::base::List::Item* item = arg_list->getFirstItem();
            while (item != nullptr) {
                mxrp::base::Pair* p = static_cast<mxrp::base::Pair*>(item->getValue());
                bool ok = obj->setSlotByName(*p->slot(), p->object());
                if (!ok) {
                    std::string msg = "error while setting slot name: " + std::string(*p->slot());
                    yyerror(msg.c_str());
                }
                item = item->getNext();
            }
            bool ok = obj->isValid();
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
   mxrp::base::Object*        ovalp;
   mxrp::base::Pair*          pvalp;
   mxrp::base::PairStream*    svalp;
   mxrp::base::List*          lvalp;
   mxrp::base::Number*        nvalp;
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
        | SLOT_ID form              { if ($2 != 0) { result = new mxrp::base::Pair($1, $2); delete[] $1; $2->unref(); } }
        ;

arglist :                           { $$ = new mxrp::base::PairStream(); }

        | arglist form              { if ($2 != 0) {
                                        int i = $1->entries();
                                        char cbuf[20];
                                        std::sprintf(cbuf, "%i", i+1);
                                        mxrp::base::Pair* p = new mxrp::base::Pair(cbuf, $2);
                                        $2->unref();
                                        $1->put(p);
                                        p->unref();
                                        $$ = $1;
                                      }
                                    }

        | arglist prim              {
                                    int i = $1->entries();
                                    char cbuf[20];
                                    std::sprintf(cbuf,"%i", i+1);
                                    mxrp::base::Pair* p = new mxrp::base::Pair(cbuf, $2);
                                    $2->unref();
                                    $1->put(p);
                                    p->unref();
                                    $$ = $1;
                                    }

        | arglist slot_value        { $1->put($2); $2->unref(); $$ = $1; }
        ;


form    : '(' IDENT arglist ')'     { $$ = parse($2, $3); delete[] $2; $3->unref(); }

        | '{' arglist '}'           { $$ = (mxrp::base::Object*) $2; }
        ;


slot_value  : SLOT_ID prim          { $$ = new mxrp::base::Pair($1, $2); delete[] $1; $2->unref(); }
        | SLOT_ID form              { $$ = new mxrp::base::Pair($1, $2); delete[] $1; $2->unref(); }
        ;

prim    : STRING_LITERAL            { $$ = new mxrp::base::String($1); delete[] $1; }
        | IDENT                     { $$ = new mxrp::base::Identifier($1); delete[] $1; }
        | BOOLconstant              { $$ = new mxrp::base::Boolean($1); }
        | '[' numlist ']'           { $$ = $2; }
        | number                    { $$ = $1; }
        ;

numlist : number                    { $$ = new mxrp::base::List(); $$->put($1); $1->unref(); }
        | numlist number            { $$ = $1; $$->put($2); $2->unref(); }
        ;

number  : INTEGERconstant           { $$ = new mxrp::base::Integer($1); }
        | FLOATINGconstant          { $$ = new mxrp::base::Float($1); }
        ;
%%

namespace mxrp {
namespace base {

//------------------------------------------------------------------------------
// Returns an Object* that was constructed from parsing an EDL file.
// factory is the name of the Object creation function  
//------------------------------------------------------------------------------
Object* edl_parser(const std::string& filename, factory_func f, unsigned int* num_errors)
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

    // if we have a good pointer, set the number of errors encountered
    if (num_errors != nullptr) {
        *num_errors = err_count;
    }
    return obj;
}

}
}

