/** <pre>
 *  The SDK is used to create and format CIGI compliant messages.
 *  Copyright (c) 2001-2005 The Boeing Company
 *  
 *  This library is free software; you can redistribute it and/or modify it 
 *  under the terms of the GNU Lesser General Public License as published by 
 *  the Free Software Foundation; either version 2.1 of the License, or (at 
 *  your option) any later version.
 *  
 *  This library is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser Public License for more 
 *  details.
 *  
 *  You should have received a copy of the GNU Lesser General Public License 
 *  along with this library; if not, write to the Free Software Foundation, 
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *  
 *  FILENAME:   CigiBaseSymbolClone.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Common Image Generator Interface (CIGI) SDK
 *  
 *  PROGRAM DESCRIPTION: 
 *  ...
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/03/2008 Greg Basler                       CIGI_SYM_1
 *  Initial Release.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#if !defined(_CIGI_BASE_SYMBOL_CLONE_INCLUDED_)
#define _CIGI_BASE_SYMBOL_CLONE_INCLUDED_


#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_SYMBOL_CLONE_PACKET_ID_V3_3 33
#define CIGI_SYMBOL_CLONE_PACKET_SIZE_V3_3 8


class CigiSymbolCloneV3_3;


//=========================================================
//! The base class for all packets.
//!
class CIGI_SPEC CigiBaseSymbolClone :
   public CigiBasePacket
{

   friend class CigiSymbolCloneV3_3;

public:


   //=========================================================
   //! The enumeration for the CigiBaseSymbolClone
   //!   Symbol Source Group
   //!
   enum SymbolSourceGrp
   {
      SymbolSrc=0,
      SymbolTemplateSrc=1
   };



   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseSymbolClone(void);

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseSymbolClone(void);


   //==> Buffer Packing/Unpacking

   //=========================================================
   //! A pure virtual Pack function.  
   //! This function is not implemented in this class.
   //! \param Base - A pointer to the instance of the packet 
   //!          to be packed. (Downcast to CigiBasePacket)
   //! \param Buff - A pointer to the current pack point.
   //! \param Spec - A pointer to special data.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const=0;

   //=========================================================
   //! A pure virtual Unpack function.
   //! This function is not implemented in this class.
   //! \param Buff - A pointer to the current pack point.
   //! \param Swap - N/A for V1 & V2
   //! \param Spec - A pointer to special data.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec) =0;

   //=========================================================
   //! A virtual Conversion Information function.
   //! This function provides conversion information for this
   //!  packet.
   //! \param CnvtVersion - The CIGI version to which this packet
   //!    is being converted.
   //! \param CnvtInfo - The information needed for conversion
   //!    
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	virtual int GetCnvt(CigiVersionID &CnvtVersion,
                       CigiCnvtInfoType::Type &CnvtInfo);



   //==> Accessing Member Variable Values functions

   //+> SymbolID

   //=========================================================
   //! Sets the SymbolID with bound checking control
   //! \param SymbolIDIn - Specifies the ID number of this symbol.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSymbolID(const Cigi_uint16 SymbolIDIn, bool bndchk=true)
   {
      SymbolID = SymbolIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the SymbolID
   //! \return The ID number of this symbol.
   //!
   Cigi_uint16 GetSymbolID(void) const { return(SymbolID); }
   

   //+> SourceType

   //=========================================================
   //! Sets the SourceType with bound checking control
   //! \param SourceTypeIn - Specifies the type of object
   //!   from which this symbol is being cloned.
   //!   (SymbolSrc, SymbolTemplateSrc)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSourceType(const SymbolSourceGrp SourceTypeIn, bool bndchk=true);

   //=========================================================
   //! Gets the SourceType with bound checking control
   //! \return The State of this symbol.
   //!
   SymbolSourceGrp GetSourceType(void) const { return(SourceType); }


   //+> SourceID

   //=========================================================
   //! Sets the SourceID with bound checking control
   //! \param SourceIDIn - Specifies the ID number of the source
   //!   to be cloned.  Either another symbol or a symbol template.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSourceID(const Cigi_uint16 SourceIDIn, bool bndchk=true)
   {
      SourceID = SourceIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the SourceID with bound checking control
   //! \return The ID number of the source symbol
   //!   or symbol template.
   //!
   Cigi_uint16 GetSourceID(void) const { return(SourceID); }
   


protected:

   //==> Member variables

   //=========================================================
   //! SymbolID<br>
   //! The ID number of this symbol
   //!
   Cigi_uint16 SymbolID;

   //=========================================================
   //! SymbolState<br>
   //! Specifies the type of object from which this symbol
   //!   is being cloned.<br>
   //!   SymbolSrc<br>
   //!   SymbolTemplateSrc<br>
   //!
   SymbolSourceGrp SourceType;

   //=========================================================
   //! SourceID<br>
   //! The ID number of the Source symbol or symbol template.
   //!
   Cigi_uint16 SourceID;


};



#endif   //  #if !defined(_CIGI_BASE_SYMBOL_CLONE_INCLUDED_)

