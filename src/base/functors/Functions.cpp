//------------------------------------------------------------------------------
// Classes ---
//    Base function:                      Function
//    Polynomial function:                Polynomial
//    Generic multi-variable functions:   Func1, Func2, Func3, Func4 and Func5
//    Function storage:                   FStorage
//------------------------------------------------------------------------------

#include "mixr/base/functors/Functions.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/functors/Tables.hpp"
#include <iostream>

namespace mixr {
namespace base {

//==============================================================================
// Class: Func1
//==============================================================================
IMPLEMENT_SUBCLASS(Func1, "Func1")
EMPTY_SLOTTABLE(Func1)
EMPTY_CONSTRUCTOR(Func1)
EMPTY_COPYDATA(Func1)
EMPTY_DELETEDATA(Func1)

double Func1::f(const double iv1, FStorage* const s) const
{
   double value {};

   // No derived class handled this ...

   const auto p = static_cast<const Table1*>(getTable());
   if (p != nullptr) {
      // But we do have an optional table that'll handle it.
      value = p->lfi(static_cast<double>(iv1), s);
   }

   return value;
}

bool Func1::setSlotLfiTable(const Table* const msg)
{
   bool ok {};
   if (msg == nullptr) {
      ok = BaseClass::setSlotLfiTable(nullptr);
   }
   else if ( msg->isClassType(typeid(Table1)) ) {
      ok = BaseClass::setSlotLfiTable(msg); // We have a 1-D table.
   }
   else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Func1::setSlotLfiTable(): ERROR -- table must use a Table1 or derived class" << std::endl;
   }
   return ok;
}

//==============================================================================
// Class: Func2
//==============================================================================
IMPLEMENT_SUBCLASS(Func2, "Func2")
EMPTY_SLOTTABLE(Func2)
EMPTY_CONSTRUCTOR(Func2)
EMPTY_COPYDATA(Func2)
EMPTY_DELETEDATA(Func2)

double Func2::f(const double iv1, const double iv2, FStorage* const s) const
{
   double value {};

   // No derived class handled this ...

   const auto p = static_cast<const Table2*>(getTable());
   if (p != nullptr) {
      // But we do have an optional table that'll handle it.
      value = p->lfi(static_cast<double>(iv1), static_cast<double>(iv2), s);
   }

   return value;
}

bool Func2::setSlotLfiTable(const Table* const msg)
{
   bool ok {};
   if (msg == nullptr) {
      ok = BaseClass::setSlotLfiTable(nullptr);
   }
   else if ( msg->isClassType(typeid(Table2)) ) {
      ok = BaseClass::setSlotLfiTable(msg);  // We have a 2-D table.
   }
   else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Func2::setSlotLfiTable(): ERROR -- table must use a Table2 or derived class" << std::endl;
   }
   return ok;
}

//==============================================================================
// Class: Func3
//==============================================================================
IMPLEMENT_SUBCLASS(Func3, "Func3")
EMPTY_SLOTTABLE(Func3)
EMPTY_CONSTRUCTOR(Func3)
EMPTY_COPYDATA(Func3)
EMPTY_DELETEDATA(Func3)

double Func3::f(const double iv1, const double iv2, const double iv3, FStorage* const s) const
{
   double value {};

   // No derived class handled this ...

   const auto p = static_cast<const Table3*>(getTable());
   if (p != nullptr) {
      // But we do have an optional table that'll handle it.
      value = p->lfi(static_cast<double>(iv1), static_cast<double>(iv2), static_cast<double>(iv3), s);
   }

   return value;
}

bool Func3::setSlotLfiTable(const Table* const msg)
{
   bool ok {};
   if (msg == nullptr) {
      ok = BaseClass::setSlotLfiTable(nullptr);
   }
   else if ( msg->isClassType(typeid(Table3)) ) {
      ok = BaseClass::setSlotLfiTable(msg);  // We have a 3-D table.
   }
   else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Func3::setSlotLfiTable(): ERROR -- table must use a Table3 or derived class" << std::endl;
   }
   return ok;
}

//==============================================================================
// Class: Func4
//==============================================================================
IMPLEMENT_SUBCLASS(Func4, "Func4")
EMPTY_SLOTTABLE(Func4)
EMPTY_CONSTRUCTOR(Func4)
EMPTY_COPYDATA(Func4)
EMPTY_DELETEDATA(Func4)

double Func4::f(const double iv1, const double iv2, const double iv3, const double iv4, FStorage* const s) const
{
   double value {};

   // No derived class handled this ...

   const auto p = static_cast<const Table4*>(getTable());
   if (p != nullptr) {
      // But we do have an optional table that'll handle it.
      value = p->lfi(static_cast<double>(iv1), static_cast<double>(iv2),
                     static_cast<double>(iv3), static_cast<double>(iv4), s);
   }

   return value;
}

bool Func4::setSlotLfiTable(const Table* const msg)
{
   bool ok {};
   if (msg == nullptr) {
      ok = BaseClass::setSlotLfiTable(nullptr);    // Setting the table to null.
   }
   else if ( msg->isClassType(typeid(Table4)) ) {
      ok = BaseClass::setSlotLfiTable(msg);  // We have a 4-D table.
   }
   else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Func4::setSlotLfiTable(): ERROR -- table must use a Table4 or derived class" << std::endl;
   }
   return ok;
}

//==============================================================================
// Class: Func5
//==============================================================================
IMPLEMENT_SUBCLASS(Func5, "Func5")
EMPTY_SLOTTABLE(Func5)
EMPTY_CONSTRUCTOR(Func5)
EMPTY_COPYDATA(Func5)
EMPTY_DELETEDATA(Func5)

double Func5::f(const double iv1, const double iv2, const double iv3, const double iv4, const double iv5, FStorage* const s) const
{
   double value {};

   // No derived class handled this ...

   const auto p = static_cast<const Table5*>(getTable());
   if (p != nullptr) {
      // But we do have an optional table that'll handle it.
      value = p->lfi(static_cast<double>(iv1), static_cast<double>(iv2), static_cast<double>(iv3),
                     static_cast<double>(iv4), static_cast<double>(iv5) ,s);
   }

   return value;
}

bool Func5::setSlotLfiTable(const Table* const msg)
{
   bool ok {};
   if (msg == nullptr) {
      ok = BaseClass::setSlotLfiTable(nullptr);
   }
   else if ( msg->isClassType(typeid(Table5)) ) {
      ok = BaseClass::setSlotLfiTable(msg);  // We have a 5-D table.
   }
   else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Func5::setSlotLfiTable(): ERROR -- table must use a Table5 or derived class" << std::endl;
   }
   return ok;
}

//==============================================================================
// Class: Polynomial
//==============================================================================
IMPLEMENT_SUBCLASS(Polynomial, "Polynomial")

BEGIN_SLOTTABLE(Polynomial)
    "coefficients"   // 1) Constant coefficients vector: [ a0 a1 a2 ... aN ]
END_SLOTTABLE(Polynomial)

BEGIN_SLOT_MAP(Polynomial)
    ON_SLOT( 1, setSlotCoefficients, List)
END_SLOT_MAP()

Polynomial::Polynomial()
{
   STANDARD_CONSTRUCTOR()
}

void Polynomial::copyData(const Polynomial& org, const bool)
{
   BaseClass::copyData(org);

   setCoefficients(org.a, org.m);
}

void Polynomial::deleteData()
{
   setCoefficients(nullptr, 0);
}

//------------------------------------------------------------------------------
// The polynomial function
//------------------------------------------------------------------------------
double Polynomial::f(const double x, FStorage* const) const
{
   double result {};
   if (m > 0) {
      const int n {m-1};
      double xx {1.0};
      for (int i = 0; i <= n; i++) {
         result += (a[i] * xx);
         xx *= x;
      }
   }
   return result;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool Polynomial::setCoefficients(const double* const aa, const int mm)
{
   bool ok {};

   // Clear the coefficients
   if (aa == nullptr || mm == 0) {
      for (int i = 0; i < mm; i++) {
         a[i] = 0;
      }
      m = 0;
      ok = true;
   }

   // Copy the coefficients
   else if (mm <= MAX_COEFF) {
      for (int i = 0; i < mm; i++) {
         a[i] = aa[i];
      }
      m = mm;
      ok = true;
   }

   else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Polynomial::setSlotCoefficients(): ERROR; too many coefficients; max is " << (MAX_DEGREE+1) << std::endl;
   }

   return ok;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool Polynomial::setSlotCoefficients(const List* const msg)
{
   bool ok {};

   if (msg != nullptr) {
      unsigned int entries {msg->entries()};
      if ( entries <= MAX_COEFF ) {
         double  aa[MAX_COEFF] {};   // Constant coefficients vector
         unsigned int mm {msg->getNumberList(aa, MAX_COEFF)};
         ok = setCoefficients( aa, mm );
      }

      else if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Polynomial::setSlotCoefficients(): ERROR; too many coefficients; max is " << MAX_COEFF << std::endl;
      }

   }
   else {
      // Just remove the old ...
      ok = setCoefficients(nullptr, 0);
   }

   return ok;
}

}
}
