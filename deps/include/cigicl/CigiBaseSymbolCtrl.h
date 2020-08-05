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
 *  FILENAME:   CigiBaseSymbolCtrl.h
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

#if !defined(_CIGI_BASE_SYMBOL_CTRL_INCLUDED_)
#define _CIGI_BASE_SYMBOL_CTRL_INCLUDED_


#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_SYMBOL_CONTROL_PACKET_ID_V3_3 34
#define CIGI_SYMBOL_CONTROL_PACKET_SIZE_V3_3 40


class CigiSymbolCtrlV3_3;


//=========================================================
//! The base class for all packets.
//!
class CIGI_SPEC CigiBaseSymbolCtrl :
   public CigiBasePacket
{

   friend class CigiSymbolCtrlV3_3;

public:

   //=========================================================
   //! The enumeration for the Symbol State Group
   //!
   enum SymbolStateGrp
   {
      Hidden=0,
      Visible=1,
      Destroyed=2
   };

   //=========================================================
   //! The enumeration for the Attach State Group
   //!
   enum AttachStateGrp
   {
      Detach=0,
      Attach=1
   };

   //=========================================================
   //! The enumeration for the Flash Control flag
   //!
   enum FlashCtrlGrp
   {
      Continue=0,
      Reset=1
   };

   //=========================================================
   //! The enumeration for the Inherit Color flag
   //!
   enum InheritColorGrp
   {
      NotInherit=0,
      Inherit=1
   };



   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseSymbolCtrl(void);

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseSymbolCtrl(void);


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
   //! \param SymbolIDIn - The ID number of this symbol
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
   //! \param SymbolStateIn - Specifies the commanded state of this
   //!   symbol.  (Hidden, Visible, Destroyed)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSymbolState(const SymbolStateGrp SymbolStateIn, bool bndchk=true);

   //=========================================================
   //! Gets the SymbolState with bound checking control
   //! \return The commanded state of this symbol.
   //!   (Hidden, Visible, Destroyed)
   //!
   SymbolStateGrp GetSymbolState(void) const { return(SymbolState); }
   

   //+> AttachState

   //=========================================================
   //! Sets the AttachState with bound checking control
   //! \param AttachStateIn - Specifies whether the symbol is
   //!   attached or detached.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetAttachState(const AttachStateGrp AttachStateIn, bool bndchk=true);

   //=========================================================
   //! Gets the AttachState with bound checking control
   //! \return The attach state of this symbol
   //!
   AttachStateGrp GetAttachState(void) const { return(AttachState); }
   

   //+> FlashCtrl

   //=========================================================
   //! Sets the FlashCtrl with bound checking control
   //! \param FlashCtrlIn - Specifies whether the flashing pattern
   //!   of this symbol is continued or is reset.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetFlashCtrl(const FlashCtrlGrp FlashCtrlIn, bool bndchk=true);

   //=========================================================
   //! Gets the FlashCtrl with bound checking control
   //! \return The commanded flash control of this symbol.
   //!
   FlashCtrlGrp GetFlashCtrl(void) const { return(FlashCtrl); }
   

   //+> InheritColor

   //=========================================================
   //! Sets the InheritColor with bound checking control
   //! \param InheritColorIn - Specifies whether this symbol
   //!   inherits its color from its parent or uses its own color.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetInheritColor(const InheritColorGrp InheritColorIn, bool bndchk=true);

   //=========================================================
   //! Gets the InheritColor with bound checking control
   //! \return Whether this symbol inherits its color
   //!   from its parent or uses its own color.
   //!
   InheritColorGrp GetInheritColor(void) const { return(InheritColor); }
   

   //+> ParentSymbolID

   //=========================================================
   //! Sets the ParentSymbolID with bound checking control
   //! \param ParentSymbolIDIn - Specifies the ID number of
   //!   this symbol's parent symbol.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetParentSymbolID(const Cigi_uint16 ParentSymbolIDIn, bool bndchk=true)
   {
      ParentSymbolID = ParentSymbolIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ParentSymbolID with bound checking control
   //! \return The ParentSymbol ID
   //!
   Cigi_uint16 GetParentSymbolID(void) const { return(ParentSymbolID); }
   

   //+> SurfaceID

   //=========================================================
   //! Sets the SurfaceID with bound checking control
   //! \param SurfaceIDIn - Specifies the ID number of the
   //!   surface to which this symbol is attached.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSurfaceID(const Cigi_uint16 SurfaceIDIn, bool bndchk=true)
   {
      SurfaceID = SurfaceIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the SurfaceID with bound checking control
   //! \return The Surface ID to which this symbol is attached.
   //!
   Cigi_uint16 GetSurfaceID(void) const { return(SurfaceID); }
   

   //+> Layer

   //=========================================================
   //! Sets the Layer with bound checking control
   //! \param LayerIn - Specifies the ID number of the layer that
   //!   this symbol occupies.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetLayer(const Cigi_uint8 LayerIn, bool bndchk=true)
   {
      Layer = LayerIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Layer with bound checking control
   //! \return The ID number of the layer that
   //!   this symbol occupies.
   //!
   Cigi_uint8 GetLayer(void) const { return(Layer); }
   

   //+> FlashDutyCycle

   //=========================================================
   //! Sets the FlashDutyCycle with bound checking control
   //! \param FlashDutyCycleIn - Specifies the percentage of time
   //!   that the symbol is visible.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetFlashDutyCycle(const Cigi_uint8 FlashDutyCycleIn, bool bndchk=true);

   //=========================================================
   //! Gets the FlashDutyCycle with bound checking control
   //! \return The percentage of time that the symbol
   //!   is visible.
   //!
   Cigi_uint8 GetFlashDutyCycle(void) const { return(FlashDutyCycle); }
   

   //+> FlashPeriod

   //=========================================================
   //! Sets the FlashPeriod with bound checking control
   //! \param FlashPeriodIn - Specifies the amount of time in
   //!   seconds of a full flash cycle.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetFlashPeriod(const float FlashPeriodIn, bool bndchk=true)
   {
      FlashPeriod = FlashPeriodIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the FlashPeriod with bound checking control
   //! \return The amount of time in seconds of a
   //!   full flash cycle.
   //!
   float GetFlashPeriod(void) const { return(FlashPeriod); }
   

   //+> UPosition

   //=========================================================
   //! Sets the UPosition with bound checking control
   //! \param UPositionIn - Specifies the position of the symbol along
   //!   the U axis of the surface's or symbol parent's coordinate system.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetUPosition(const float UPositionIn, bool bndchk=true)
   {
      UPosition = UPositionIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the UPosition with bound checking control
   //! \return The position of the symbol along the U axis
   //!   of the surface's or symbol parent's coordinate system.
   //!
   float GetUPosition(void) const { return(UPosition); }
   

   //+> VPosition

   //=========================================================
   //! Sets the VPosition with bound checking control
   //! \param VPositionIn - Specifies the position of the symbol along
   //!   theV axis of the surface's or symbol parent's coordinate system.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetVPosition(const float VPositionIn, bool bndchk=true)
   {
      VPosition = VPositionIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the VPosition with bound checking control
   //! \return The position of the symbol along theV axis
   //!   of the surface's or symbol parent's coordinate system.
   //!
   float GetVPosition(void) const { return(VPosition); }
   

   //+> Rotation

   //=========================================================
   //! Sets the Rotation with bound checking control
   //! \param RotationIn - Specifies how much in degrees that the
   //!   symbol is rotated.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRotation(const float RotationIn, bool bndchk=true)
   {
      Rotation = RotationIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Rotation with bound checking control
   //! \return How much in degrees that the symbol is rotated.
   //!
   float GetRotation(void) const { return(Rotation); }


   //+> Color

   //=========================================================
   //! Sets the color with bound checking control
   //! \param RedIn - The red component of the symbol's color.
   //! \param GreenIn - The green component of the symbol's color.
   //! \param BlueIn - The blue component of the symbol's color.
   //! \param AlphaIn - The alpha component of the symbol's color.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetColor(const Cigi_uint8 RedIn,
                const Cigi_uint8 GreenIn,
                const Cigi_uint8 BlueIn,
                const Cigi_uint8 AlphaIn,
                bool bndchk=true)
   {
      Red = RedIn;
      Green = GreenIn;
      Blue = BlueIn;
      Alpha = AlphaIn;

      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ParentSymbolID with bound checking control
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
   int GetColorDatum(Cigi_uint8 &RedValue,
                     Cigi_uint8 &GreenValue,
                     Cigi_uint8 &BlueValue,
                     Cigi_uint8 &AlphaValue,
                     bool bndchk=true)
   {
      RedValue = Red;
      GreenValue = Green;
      BlueValue = Blue;
      AlphaValue = Alpha;

      return(CIGI_SUCCESS);
   }


   //+> Red

   //=========================================================
   //! Sets the Red with bound checking control
   //! \param RedIn - The red component of the symbol's color.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRed(const Cigi_uint8 RedIn, bool bndchk=true)
   {
      Red = RedIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Red with bound checking control
   //! \return The red component of the symbol's color.
   //!
   Cigi_uint8 GetRed(void) const { return(Red); }
   

   //+> Green

   //=========================================================
   //! Sets the Green with bound checking control
   //! \param GreenIn - The green component of the symbol's color.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetGreen(const Cigi_uint8 GreenIn, bool bndchk=true)
   {
      Green = GreenIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Green with bound checking control
   //! \return The green component of the symbol's color.
   //!
   Cigi_uint8 GetGreen(void) const { return(Green); }
   

   //+> Blue

   //=========================================================
   //! Sets the Blue with bound checking control
   //! \param BlueIn - The blue component of the symbol's color.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetBlue(const Cigi_uint8 BlueIn, bool bndchk=true)
   {
      Blue = BlueIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Blue with bound checking control
   //! \return The blue component of the symbol's color.
   //!
   Cigi_uint8 GetBlue(void) const { return(Blue); }
   

   //+> Alpha

   //=========================================================
   //! Sets the Alpha with bound checking control
   //! \param AlphaIn - The alpha component of the symbol's color.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetAlpha(const Cigi_uint8 AlphaIn, bool bndchk=true)
   {
      Alpha = AlphaIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Alpha with bound checking control
   //! \return The alpha component of the symbol's color.
   //!
   Cigi_uint8 GetAlpha(void) const { return(Alpha); }
   

   //+> ScaleU

   //=========================================================
   //! Sets the ScaleU with bound checking control
   //! \param ScaleUIn - Specifies the scale to apply to the symbol's
   //!   local coordinate system's U axis.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetScaleU(const float ScaleUIn, bool bndchk=true)
   {
      ScaleU = ScaleUIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ScaleU with bound checking control
   //! \return The scale to apply to the symbol's local
   //!   coordinate system's U axis.
   //!
   float GetScaleU(void) const { return(ScaleU); }
   

   //+> ScaleV

   //=========================================================
   //! Sets the ScaleV with bound checking control
   //! \param ScaleVIn - Specifies the scale to apply to the symbol's
   //!   local coordinate system'sV axis.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetScaleV(const float ScaleVIn, bool bndchk=true)
   {
      ScaleV = ScaleVIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ScaleV with bound checking control
   //! \return The scale to apply to the symbol's local
   //!   coordinate system'sV axis.
   //!
   float GetScaleV(void) const { return(ScaleV); }



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
   SymbolStateGrp SymbolState;

   //=========================================================
   //! AttachState<br>
   //! Whether this symbol is attached to another symbol.
   //!
   AttachStateGrp AttachState;

   //=========================================================
   //! FlashCtrl<br>
   //! Whether the current flash pattern is continued or restarted.
   //!
   FlashCtrlGrp FlashCtrl;

   //=========================================================
   //! InheritColor<br>
   //! Whether this symbol inherits its color from its parent or
   //!   uses its own color.
   //!
   InheritColorGrp InheritColor;

   //=========================================================
   //! ParentSymbolID<br>
   //! The ID number of this symbol's parent symbol.
   //!
   Cigi_uint16 ParentSymbolID;

   //=========================================================
   //! SurfaceID<br>
   //! The ID number of the surface to which this symbol is attached.
   //!
   Cigi_uint16 SurfaceID;

   //=========================================================
   //! Layer<br>
   //! The ID number of the layer that this symbol occupies.
   //!
   Cigi_uint8 Layer;

   //=========================================================
   //! FlashDutyCycle<br>
   //! The percentage of time that this symbol is visible.
   //!
   Cigi_uint8 FlashDutyCycle;

   //=========================================================
   //! FlashPeriod<br>
   //! The amount of time that a flash cycle takes.
   //!
   float FlashPeriod;

   //=========================================================
   //! UPosition<br>
   //! The position of the symbol along the U axis
   //!   of the surface's or symbol parent's coordinate system. 
   //!
   float UPosition;

   //=========================================================
   //! VPosition<br>
   //! The position of the symbol along theV axis
   //!   of the surface's or symbol parent's coordinate system.
   //!
   float VPosition;

   //=========================================================
   //! Rotation<br>
   //! How much in degrees that the symbol is rotated.
   //!
   float Rotation;

   //=========================================================
   //! Red<br>
   //! The red component of the symbol's color.
   //!
   Cigi_uint8 Red;

   //=========================================================
   //! Green<br>
   //! The green component of the symbol's color.
   //!
   Cigi_uint8 Green;

   //=========================================================
   //! Blue<br>
   //! The blue component of the symbol's color.
   //!
   Cigi_uint8 Blue;

   //=========================================================
   //! Alpha<br>
   //! The alpha component of the symbol's color.
   //!
   Cigi_uint8 Alpha;

   //=========================================================
   //! ScaleU<br>
   //! The scale to apply to the symbol's local
   //!   coordinate system's U axis.
   //!
   float ScaleU;

   //=========================================================
   //! ScaleV<br>
   //! The scale to apply to the symbol's local
   //!   coordinate system's U axis.
   //!
   float ScaleV;




};



#endif   //  #if !defined(_CIGI_BASE_SYMBOL_CTRL_INCLUDED_)

