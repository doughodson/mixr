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
 *  FILENAME:   CigiBaseMaritimeSurfaceCtrl.h
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
 *  09/17/2003 Greg Basler                       CIGI_CR_DR_1
 *  Initial Release.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Added the CIGI_SPEC modifier to the class declaration for exporting the 
 *  class in a Windows DLL.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Changed native C++ types in function declarations to CCL-defined types.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  Moved Packet information to base packet.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#if !defined(_CIGI_BASE_MARITIME_SURFACE_CTRL_INCLUDED_)
#define _CIGI_BASE_MARITIME_SURFACE_CTRL_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_MARITIME_SURFACE_CTRL_PACKET_ID_V3 13
#define CIGI_MARITIME_SURFACE_CTRL_PACKET_SIZE_V3 24


class CigiMaritimeSurfaceCtrlV3;


class CIGI_SPEC CigiBaseMaritimeSurfaceCtrl : public CigiBasePacket
{

friend class CigiMaritimeSurfaceCtrlV3;

public:

   //=========================================================
   //! The enumeration for the CigiBaseMaritimeSurfaceCtrl Group
   //!
   enum ScopeGrp
   {
      Global=0,
      Regional=1,
      Entity=2
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseMaritimeSurfaceCtrl();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseMaritimeSurfaceCtrl();


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

   //+> EntityRgnID

   //=========================================================
   //! Sets the EntityRgnID with bound checking control
   //! \param EntityRgnIDIn - Entity or Region ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetEntityRgnID(const Cigi_uint16 EntityRgnIDIn, bool bndchk=true)
   {
      EntityRgnID = EntityRgnIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the EntityRgnID value.
   //! \return the current EntityRgnID.
   Cigi_uint16 GetEntityRgnID(void) const { return(EntityRgnID); }


   //+> SurfaceCondEn

   //=========================================================
   //! Sets the SurfaceCondEn with bound checking control
   //! \param SurfaceCondEnIn - Surface conditions enable
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSurfaceCondEn(const bool SurfaceCondEnIn, bool bndchk=true)
   {
      SurfaceCondEn = SurfaceCondEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the SurfaceCondEn value.
   //! \return the current SurfaceCondEn.
   bool GetSurfaceCondEn(void) const { return(SurfaceCondEn); }


   //+> WhitecapEn

   //=========================================================
   //! Sets the WhitecapEn with bound checking control
   //! \param WhitecapEnIn - Whitecap enable
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetWhitecapEn(const bool WhitecapEnIn, bool bndchk=true)
   {
      WhitecapEn = WhitecapEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the WhitecapEn value.
   //! \return the current WhitecapEn.
   bool GetWhitecapEn(void) const { return(WhitecapEn); }


   //+> Scope

   //=========================================================
   //! Sets the Scope with bound checking control
   //! \param ScopeIn - Scope of effect<br>
   //!   Global=0<br>
   //!   Regional=1<br>
   //!   Entity=2<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetScope(const ScopeGrp ScopeIn, bool bndchk=true);

   //=========================================================
   //! Gets the Scope value.
   //! \return the current Scope.
   ScopeGrp GetScope(void) const { return(Scope); }


   //+> SurfaceHeight

   //=========================================================
   //! Sets the SurfaceHeight with bound checking control
   //! \param SurfaceHeightIn - Sea surface height
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSurfaceHeight(const float SurfaceHeightIn, bool bndchk=true)
   {
      SurfaceHeight = SurfaceHeightIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the SurfaceHeight value.
   //! \return the current SurfaceHeight.
   float GetSurfaceHeight(void) const { return(SurfaceHeight); }


   //+> WaterTemp

   //=========================================================
   //! Sets the WaterTemp with bound checking control
   //! \param WaterTempIn - Sea surface water temperature
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetWaterTemp(const float WaterTempIn, bool bndchk=true)
   {
      WaterTemp = WaterTempIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the WaterTemp value.
   //! \return the current WaterTemp.
   float GetWaterTemp(void) const { return(WaterTemp); }


   //+> Clarity

   //=========================================================
   //! Sets the Clarity with bound checking control
   //! \param ClarityIn - Sea surface clarity
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetClarity(const float ClarityIn, bool bndchk=true);

   //=========================================================
   //! Gets the Clarity value.
   //! \return the current Clarity.
   float GetClarity(void) const { return(Clarity); }



protected:

   //==> Member variables

   //=========================================================
   //! EntityRgnID<br>
   //! Entity or Region ID
   //!
   Cigi_uint16 EntityRgnID;

   //=========================================================
   //! SurfaceCondEn<br>
   //! Sea Surface Conditions enable
   //!
   bool SurfaceCondEn;

   //=========================================================
   //! WhitecapEn<br>
   //! Whitecaps enable
   //!
   bool WhitecapEn;

   //=========================================================
   //! Scope<br>
   //! Scope of effect
   //!  0-Global<br>
   //!  1-Regional<br>
   //!  2-Entity
   //!
   //!
   ScopeGrp Scope;

   //=========================================================
   //! SurfaceHeight<br>
   //! Sea surface height
   //!
   float SurfaceHeight;

   //=========================================================
   //! WaterTemp<br>
   //! Sea surface water temperature
   //!
   float WaterTemp;

   //=========================================================
   //! Clarity<br>
   //! Sea surface water percentage clarity
   //!
   float Clarity;


};

#endif // #if !defined(_CIGI_BASE_MARITIME_SURFACE_CTRL_INCLUDED_)
