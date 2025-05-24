
#include "mixr/interop/dis/Ntm.hpp"
#include "mixr/interop/dis/Nib.hpp"

#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"

namespace mixr {

namespace dis {

IMPLEMENT_SUBCLASS(Ntm, "DisNtm")
EMPTY_DELETEDATA(Ntm)

BEGIN_SLOTTABLE(Ntm)
    "disEntityType", // 1) DIS Entity type (base::List -- number vector)  [ kind domain country category ... ]
END_SLOTTABLE(Ntm)

BEGIN_SLOT_MAP(Ntm)
    ON_SLOT(1, setSlotDisEntityType, base::IList)
END_SLOT_MAP()

Ntm::Ntm()
{
   STANDARD_CONSTRUCTOR()
}

void Ntm::copyData(const Ntm& org, const bool)
{
   BaseClass::copyData(org);

   disKind = org.disKind;
   disDomain = org.disDomain;
   disCountry = org.disCountry;
   disCategory = org.disCategory;
   disSubcategory = org.disSubcategory;
   disSpecific = org.disSpecific;
   disExtra = org.disExtra;
}

//------------------------------------------------------------------------------
// This function will copy our DIS type codes to the target NIB, 'targetNib', object.
//------------------------------------------------------------------------------
bool Ntm::copyEntityType(interop::INib* const targetNib) const
{
   bool ok {};
   const auto tgtNib = dynamic_cast<Nib*>(targetNib);
   if (tgtNib != nullptr) {
      ok = tgtNib->setEntityType(disKind, disDomain, disCountry, disCategory, disSubcategory, disSpecific, disExtra);
   }
   return ok;
}

//------------------------------------------------------------------------------
// Set (DIS) Entity type enumerations
//------------------------------------------------------------------------------
bool Ntm::setEntityType(
       const unsigned char k,       // Kind
       const unsigned char d,       // Domain
       const unsigned short cc,     // Country code
       const unsigned char c,       // Category
       const unsigned char sc,      // Subcategory
       const unsigned char sp,      // Specific
       const unsigned char e        // Extra
    )
{
   // Some simple validity checks
   bool ok {(k < NetIO::NUM_ENTITY_KINDS) || (d < NetIO::MAX_ENTITY_DOMAINS)};

   if (ok) {
      disKind = k;
      disDomain = d;
      disCountry = cc;
      disCategory = c;
      disSubcategory = sc;
      disSpecific = sp;
      disExtra = e;
   }

   return ok;
}

//------------------------------------------------------------------------------
// Set DIS entity type enumerations
//------------------------------------------------------------------------------
bool Ntm::setSlotDisEntityType(const base::IList* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      int values[7] {};
      int n {static_cast<int>(msg->getNumberList(values, 7))};
      if (n >= 4) {
         // Need at least Kind, domain, country & category
         ok = true;  // First, assume they're valid
         unsigned char k {};    // Kind
         if (values[0] >= 0x0 && values[0] <= 0xff) {
            k = static_cast<unsigned char>(values[0]);
         } else {
            std::cerr << "Ntm::setSlotDisEntityType: Invalid kind: [ 0 .. 255 ]" << std::endl;
            ok = false;
         }

         unsigned char d {};    // Domain
         if (values[1] >= 0x0 && values[1] <= 0xff) {
            d = static_cast<unsigned char>(values[1]);
         } else {
            std::cerr << "Ntm::setSlotDisEntityType: Invalid domain: [ 0 .. 255 ]" << std::endl;
            ok = false;
         }

         unsigned short cc {};  // Country code
         if (values[2] >= 0x0 && values[2] <= 0xffff) {
            cc = static_cast<unsigned char>(values[2]);
         } else {
            std::cerr << "Ntm::setSlotDisEntityType: Invalid country: [ 0 .. 65535 ]" << std::endl;
            ok = false;
         }

         unsigned char c {};    // Category
         if (values[3] >= 0x0 && values[3] <= 0xff) {
            c = static_cast<unsigned char>(values[3]);
         } else {
            std::cerr << "Ntm::setSlotDisEntityType: Invalid category: [ 0 .. 255 ]" << std::endl;
            ok = false;
         }

         // Ok so far, now for the optional stuff ...
         unsigned char sc {};   // Subcategory
         unsigned char sp {};   // Specific
         unsigned char ex {};   // Extra
         if (n >= 5) {
            if (values[4] >= 0x0 && values[4] <= 0xff) {
               sc = static_cast<unsigned char>(values[4]);
            } else {
               std::cerr << "Ntm::setSlotDisEntityType: Invalid subcategory: [ 0 .. 255 ]" << std::endl;
               ok = false;
            }
         }
         if (n >= 6) {
            if (values[5] >= 0x0 && values[5] <= 0xff) {
               sp = static_cast<unsigned char>(values[5]);
            } else {
               std::cerr << "Ntm::setSlotDisEntityType: Invalid specific: [ 0 .. 255 ]" << std::endl;
               ok = false;
            }
         }
         if (n >= 7) {
            if (values[6] >= 0x0 && values[6] <= 0xff) {
               ex = static_cast<unsigned char>(values[6]);
            } else {
               std::cerr << "Ntm::setSlotDisEntityType: Invalid extra: [ 0 .. 255 ]" << std::endl;
               ok = false;
            }
         }

         // If all's well, then set the entity type
         if (ok) {
            ok = setEntityType(k, d, cc, c, sc, sp, ex);
         }

      } else {
         std::cerr << "Ntm::setSlotDisEntityType: Invalid entity type vector: need at least [ kind domain country category ]" << std::endl;
      }
   }
   return ok;
}

}
}
