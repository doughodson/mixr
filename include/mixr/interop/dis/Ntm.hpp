
#ifndef __mixr_interop_dis_Ntm_HPP__
#define __mixr_interop_dis_Ntm_HPP__

#include "mixr/interop/INtm.hpp"
#include "mixr/interop/dis/NetIO.hpp"

namespace mixr {
namespace base { class List; }
namespace dis {

//------------------------------------------------------------------------------
// Class: Ntm
//
// Description: DIS Network Type Mapper
//
//    This class, which is derived from the simulation::Ntm, maps MIXR
//    player types to/from DIS entity type codes.
//
//
// Factory name: DisNtm
// Slots:
//    disEntityType  <base::List>  ! DIS Entity type vector:
//                                 !  [ kind domain country category subcategory specific extra ]
//                                 !  example: [ 1 2 333 4 5 6 7 ]
//
//------------------------------------------------------------------------------
class Ntm : public interop::INtm
{
    DECLARE_SUBCLASS(Ntm, interop::INtm)

public:
   Ntm();

   unsigned char getEntityKind() const         { return disKind;        }       // DIS kind type code (or 255 if not valid)
   unsigned char getEntityDomain() const       { return disDomain;      }       // DIS domain type code
   unsigned short getEntityCountry() const     { return disCountry;     }       // DIS country type code
   unsigned char getEntityCategory() const     { return disCategory;    }       // DIS category type code
   unsigned char getEntitySubcategory() const  { return disSubcategory; }       // DIS subcategory type code
   unsigned char getEntitySpecific() const     { return disSpecific;    }       // DIS specific type code
   unsigned char getEntityExtra() const        { return disExtra;       }       // DIS extra type code

   // Sets the player's entity type codes
   virtual bool setEntityType(
      const unsigned char  kind,
      const unsigned char  domain,
      const unsigned short countryCode,
      const unsigned char  category,
      const unsigned char  subcategory = 0,
      const unsigned char  specific = 0,
      const unsigned char  extra = 0
   );

   bool copyEntityType(interop::INib* const targetNib) const override;

private:
    // Standard (DIS based) type codes
    unsigned char  disKind {};      // DIS kind code
    unsigned char  disDomain {};    // DIS domain code
    unsigned short disCountry {};   // DIS country code
    unsigned char  disCategory {};  // DIS category code
    unsigned char  disSubcategory {}; // DIS subcategory code
    unsigned char  disSpecific {};  // DIS specific code
    unsigned char  disExtra {};     // DIS extra code

private:
   // slot table helper methods
   virtual bool setSlotDisEntityType(const base::List* const);         // Sets DIS entity type enumerations
};

}
}

#endif
