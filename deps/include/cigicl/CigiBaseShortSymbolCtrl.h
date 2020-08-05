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
 *  FILENAME:   CigiBaseShortSymbolCtrl.h
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
 *  03/11/2008 Greg Basler                       CIGI_SYM_1
 *  Initial Release.
 *  
 *  04/03/2008 Greg Basler                       2.1.0
 *  Updated packet id to reflect the ICD.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#if !defined(_CIGI_BASE_SHORT_SYMBOL_CTRL_INCLUDED_)
#define _CIGI_BASE_SHORT_SYMBOL_CTRL_INCLUDED_


#include "CigiBaseSymbolCtrl.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_SHORT_SYMBOL_CONTROL_PACKET_ID_V3_3 35
#define CIGI_SHORT_SYMBOL_CONTROL_PACKET_SIZE_V3_3 16


class CigiShortSymbolCtrlV3_3;


//=========================================================
//! The base class for all packets.
//!
class CIGI_SPEC CigiBaseShortSymbolCtrl :
   public CigiBasePacket
{

   friend class CigiShortSymbolCtrlV3_3;

public:


   //=========================================================
   //! The enumeration for the CigiBaseShortSymbolCtrl
   //!   Datum type Group
   //!
   enum DatumTypeGrp
   {
      None=0,
      SurfaceIdDatumType=1,
      ParentIdDatumType=2,
      LayerDatumType=3,
      FlashDutyCycleDatumType=4,
      FlashPeriodDatumType=5,
      UPositionDatumType=6,
      VPositionDatumType=7,
      RotationDatumType=8,
      ColorDatumType=9,
      ScaleUDatumType=10,
      ScaleVDatumType=11
   };

   //=========================================================
   //! The union of the types of values that can be used in
   //!   datum parameter
   //!
   typedef union DatumUnion {
      Cigi_uint32 UIntValue;
      float FloatValue;
   } DatumUnionType;



   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseShortSymbolCtrl(void);

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseShortSymbolCtrl(void);


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
   //! Gets the SymbolID with bound checking control
   //! \return The ID number of this symbol.
   //!
   Cigi_uint16 GetSymbolID(void) const { return(SymbolID); }
   

   //+> SymbolState

   //=========================================================
   //! Sets the SymbolState with bound checking control
   //! \param SymbolStateIn - Specifies the commanded state of
   //!   this symbol.  (Hidden, Visible, Destroyed)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSymbolState(const CigiBaseSymbolCtrl::SymbolStateGrp SymbolStateIn, bool bndchk=true);

   //=========================================================
   //! Gets the SymbolState with bound checking control
   //! \return The State of this symbol.
   //!
   CigiBaseSymbolCtrl::SymbolStateGrp GetSymbolState(void) const { return(SymbolState); }
   

   //+> AttachState

   //=========================================================
   //! Sets the AttachState with bound checking control
   //! \param AttachStateIn - Specifies whether this symbol
   //!   is attached to another symbol or is detached.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetAttachState(const CigiBaseSymbolCtrl::AttachStateGrp AttachStateIn, bool bndchk=true);

   //=========================================================
   //! Gets the AttachState with bound checking control
   //! \return The Attach State of this symbol.
   //!
   CigiBaseSymbolCtrl::AttachStateGrp GetAttachState(void) const { return(AttachState); }
   

   //+> FlashCtrl

   //=========================================================
   //! Sets the FlashCtrl with bound checking control
   //! \param FlashCtrlIn - Specifies whether the flashing pattern
   //!   of this symbol is continued or is reset.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetFlashCtrl(const CigiBaseSymbolCtrl::FlashCtrlGrp FlashCtrlIn, bool bndchk=true);

   //=========================================================
   //! Gets the FlashCtrl with bound checking control
   //! \return The Flash Ctrl of this symbol.
   //!
   CigiBaseSymbolCtrl::FlashCtrlGrp GetFlashCtrl(void) const { return(FlashCtrl); }
   

   //+> InheritColor

   //=========================================================
   //! Sets the InheritColor with bound checking control
   //! \param InheritColorIn - Specifies whether this symbol
   //!   inherits its color from its parent or uses its own color.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetInheritColor(const CigiBaseSymbolCtrl::InheritColorGrp InheritColorIn, bool bndchk=true);

   //=========================================================
   //! Gets the InheritColor with bound checking control
   //! \return Whether this symbol inherits its color
   //!   from its parent or uses its own color.
   //!
   CigiBaseSymbolCtrl::InheritColorGrp GetInheritColor(void) const { return(InheritColor); }


   //+> Getting Datum Type

   //=========================================================
   //! Gets the ID specifying the datum contained in Datum 0
   //! \param DatumId - The ID of the Datum being checked (0 or 1)
   //!
   //! \return ParentSymbolID
   DatumTypeGrp GetDatumType(int DatumId) const;


   //+> Getting Datum

   //=========================================================
   //! Gets the unsigned integer value of the specified datum.
   //! \param DatumId - The ID of the Datum to read (0 or 1)
   //!
   //! \return ParentSymbolID
   Cigi_uint32 GetUIntDatum(int DatumId) const;

   //=========================================================
   //! Gets the color value of the specified datum
   //! \param DatumId - The ID of the Datum to read (0 or 1)
   //! \param RedValue - The reference to the variable in which
   //!    the value of the red component will be placed
   //! \param GreenValue - The reference to the variable in which
   //!    the value of the Green component will be placed
   //! \param BlueValue - The reference to the variable in which
   //!    the value of the Blue component will be placed
   //! \param AlphaValue - The reference to the variable in which
   //!    the value of the Alpha component will be placed
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int GetColorDatum(int DatumId,
                     Cigi_uint8 &RedValue,
                     Cigi_uint8 &GreenValue,
                     Cigi_uint8 &BlueValue,
                     Cigi_uint8 &AlphaValue,
                     bool bndchk=true);

   //=========================================================
   //! Gets thefloating point value of the specified datum.
   //! \param DatumId - The ID or the Datum to read (0 or 1)
   //!
   //! \return ParentSymbolID
   float GetFloatDatum(int DatumId) const;


   //+> Datum Entries

   //=========================================================
   //! Sets the Datum type to "NONE" and zeros the Datum
   //!   with bound checking control
   //! \param DatumId - The ID number of the datum to be set.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDatumNone(int DatumId, bool bndchk=true);

   //=========================================================
   //! Sets the SurfaceID with bound checking control
   //! \param DatumId - The ID number of the datum to be set.
   //! \param SurfaceIDIn - The ID number of the surface to which
   //!   this symbol should be attached.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSurfaceID(int DatumId, const Cigi_uint16 SurfaceIDIn, bool bndchk=true);

   //=========================================================
   //! Sets the ParentSymbolID with bound checking control
   //! \param DatumId - The ID number of the datum to be set.
   //! \param ParentSymbolIDIn - Specifies the ID number of
   //!   this symbol's parent symbol.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetParentSymbolID(int DatumId, const Cigi_uint16 ParentSymbolIDIn, bool bndchk=true);

   //=========================================================
   //! Sets the Layer with bound checking control
   //! \param DatumId - The ID number of the datum to be set.
   //! \param LayerIn - Specifies the ID number of the layer that
   //!   this symbol occupies.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetLayer(int DatumId, const Cigi_uint8 LayerIn, bool bndchk=true);

   //=========================================================
   //! Sets the FlashDutyCycle with bound checking control
   //! \param DatumId - The ID number of the datum to be set.
   //! \param FlashDutyCycleIn - Specifies the percentage of time
   //!   that the symbol is visible.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetFlashDutyCycle(int DatumId, const Cigi_uint8 FlashDutyCycleIn, bool bndchk=true);

   //=========================================================
   //! Sets the FlashPeriod with bound checking control
   //! \param DatumId - The ID number of the datum to be set.
   //! \param FlashPeriodIn - Specifies the amount of time in
   //!   seconds of a full flash cycle.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetFlashPeriod(int DatumId, const float FlashPeriodIn, bool bndchk=true);

   //=========================================================
   //! Sets the UPosition with bound checking control
   //! \param DatumId - The ID number of the datum to be set.
   //! \param UPositionIn - Specifies the position of the symbol along
   //!   the U axis of the surface's or symbol parent's coordinate system.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetUPosition(int DatumId, const float UPositionIn, bool bndchk=true);

   //=========================================================
   //! Sets the VPosition with bound checking control
   //! \param DatumId - The ID number of the datum to be set.
   //! \param VPositionIn - Specifies the position of the symbol along
   //!   the V axis of the surface's or symbol parent's coordinate system.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetVPosition(int DatumId, const float VPositionIn, bool bndchk=true);

   //=========================================================
   //! Sets the Rotation with bound checking control
   //! \param DatumId - The ID number of the datum to be set.
   //! \param RotationIn - Specifies how much in degrees that the
   //!   symbol is rotated.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRotation(int DatumId, const float RotationIn, bool bndchk=true);

   //=========================================================
   //! Sets the ScaleU with bound checking control
   //! \param DatumId - The ID number of the datum to be set.
   //! \param ScaleUIn - Specifies the scale to apply to the symbol's
   //!   local coordinate system's U axis.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetScaleU(int DatumId, const float ScaleUIn, bool bndchk=true);

   //=========================================================
   //! Sets the ScaleV with bound checking control
   //! \param DatumId - The ID number of the datum to be set.
   //! \param ScaleVIn - Specifies the scale to apply to the symbol's
   //!   local coordinate system's V axis.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetScaleV(int DatumId, const float ScaleVIn, bool bndchk=true);



   //+> Color

   //=========================================================
   //! Sets the color with bound checking control
   //! \param DatumId - The ID number of the datum to be set.
   //! \param RedValue - The reference to the variable in which
   //!    the value of the red component will be placed
   //! \param GreenValue - The reference to the variable in which
   //!    the value of the Green component will be placed
   //! \param BlueValue - The reference to the variable in which
   //!    the value of the Blue component will be placed
   //! \param AlphaValue - The reference to the variable in which
   //!    the value of the Alpha component will be placed
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetColor(int DatumId,
                const Cigi_uint8 RedIn,
                const Cigi_uint8 GreenIn,
                const Cigi_uint8 BlueIn,
                const Cigi_uint8 AlphaIn,
                bool bndchk=true);



protected:

   //==> Member variables

   //=========================================================
   //! SymbolID<br>
   //! The ID number of this symbol
   //!
   Cigi_uint16 SymbolID;

   //=========================================================
   //! SymbolState<br>
   //! The commanded state of this symbol.<br>
   //!   Hidden<br>
   //!   Visible<br>
   //!   Destroyed
   //!
   CigiBaseSymbolCtrl::SymbolStateGrp SymbolState;

   //=========================================================
   //! AttachState<br>
   //! Whether this symbol is attached to another symbol.
   //!
   CigiBaseSymbolCtrl::AttachStateGrp AttachState;

   //=========================================================
   //! FlashCtrl<br>
   //! Whether the current flash pattern is continued or restarted.
   //!
   CigiBaseSymbolCtrl::FlashCtrlGrp FlashCtrl;

   //=========================================================
   //! InheritColor<br>
   //! Whether this symbol inherits its color from its parent or
   //!   uses its own color.
   //!
   CigiBaseSymbolCtrl::InheritColorGrp InheritColor;

   //=========================================================
   //! Datum1Type<br>
   //! The type of data in each of the Datum slots
   //!
   DatumTypeGrp DatumType[2];

   //=========================================================
   //! ParentSymbolID<br>
   //! The variables containing the specific data to be used.
   //!
   DatumUnionType Datum[2];

};



#endif   //  #if !defined(_CIGI_BASE_SHORT_SYMBOL_CTRL_INCLUDED_)

