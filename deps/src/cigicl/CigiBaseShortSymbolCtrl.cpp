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
 *  FILENAME:   CigiBaseShortSymbolCtrl.cpp
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
 * </pre>
 *  Author: The Boeing Company
 *
 */


#define _EXPORT_CCL_


#include "CigiBaseShortSymbolCtrl.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseShortSymbolCtrl::CigiBaseShortSymbolCtrl(void)
{
}

// ================================================
// ~CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseShortSymbolCtrl::~CigiBaseShortSymbolCtrl(void)
{
}


// ====================================================================
// Conversion Control
// ====================================================================
int CigiBaseShortSymbolCtrl::GetCnvt(CigiVersionID &CnvtVersion,
                                CigiCnvtInfoType::Type &CnvtInfo)
{
   if(CnvtVersion.GetCombinedCigiVersion() < 0x303)
   {
      CnvtInfo.ProcID = CigiProcessType::ProcNone;
      CnvtInfo.CnvtPacketID = 0;
   }
   else
   {
      CnvtInfo.ProcID = CigiProcessType::ProcStd;
      CnvtInfo.CnvtPacketID = CIGI_SHORT_SYMBOL_CONTROL_PACKET_ID_V3_3;
   }

   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================

// ================================================
// SetSymbolState
// ================================================
int CigiBaseShortSymbolCtrl::SetSymbolState(const CigiBaseSymbolCtrl::SymbolStateGrp SymbolStateIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((SymbolStateIn < 0)||(SymbolStateIn > 2)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("SymbolState", (int)SymbolStateIn,0,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   SymbolState = SymbolStateIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetAttachState
// ================================================
int CigiBaseShortSymbolCtrl::SetAttachState(const CigiBaseSymbolCtrl::AttachStateGrp AttachStateIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((AttachStateIn < 0)||(AttachStateIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("AttachState", (int)AttachStateIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   AttachState = AttachStateIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetFlashCtrl
// ================================================
int CigiBaseShortSymbolCtrl::SetFlashCtrl(const CigiBaseSymbolCtrl::FlashCtrlGrp FlashCtrlIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((FlashCtrlIn < 0)||(FlashCtrlIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("FlashCtrl", (int)FlashCtrlIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   FlashCtrl = FlashCtrlIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetInheritColor
// ================================================
int CigiBaseShortSymbolCtrl::SetInheritColor(const CigiBaseSymbolCtrl::InheritColorGrp InheritColorIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((InheritColorIn < 0)||(InheritColorIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("InheritColor", (int)InheritColorIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   InheritColor = InheritColorIn;

   return(CIGI_SUCCESS);

}



// ================================================
// GetDatumType
// ================================================
CigiBaseShortSymbolCtrl::DatumTypeGrp CigiBaseShortSymbolCtrl::GetDatumType(int DatumId) const
{

   //This must be bound checked!
   if((DatumId < 0)||(DatumId > 1))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DatumId", (int)DatumId,1,2);
#endif
      return(None);
   }

   return(DatumType[(DatumId)]);

}



// ================================================
// GetUIntDatum
// ================================================
Cigi_uint32 CigiBaseShortSymbolCtrl::GetUIntDatum(int DatumId) const
{

   //This must be bound checked!
   if((DatumId < 0)||(DatumId > 1))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DatumId", (int)DatumId,1,2);
#endif
      return(0);
   }

   return(Datum[(DatumId)].UIntValue);

}



// ================================================
// GetColorDatum
// ================================================
int CigiBaseShortSymbolCtrl::GetColorDatum(int DatumId,
                                           Cigi_uint8 &RedValue,
                                           Cigi_uint8 &GreenValue,
                                           Cigi_uint8 &BlueValue,
                                           Cigi_uint8 &AlphaValue,
                                           bool bndchk)
{

   //This must be bound checked!
   if((DatumId < 0)||(DatumId > 1))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DatumId", (int)DatumId,1,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

   RedValue = (Cigi_uint8)((Datum[(DatumId)].UIntValue >> 24) & 0xff);
   GreenValue = (Cigi_uint8)((Datum[(DatumId)].UIntValue >> 16) & 0xff);
   BlueValue = (Cigi_uint8)((Datum[(DatumId)].UIntValue >> 8) & 0xff);
   AlphaValue = (Cigi_uint8)((Datum[(DatumId)].UIntValue) & 0xff);

   return(CIGI_SUCCESS);

}



// ================================================
// GetFloatDatum
// ================================================
float CigiBaseShortSymbolCtrl::GetFloatDatum(int DatumId) const
{

   //This must be bound checked!
   if((DatumId < 0)||(DatumId > 1))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DatumId", (int)DatumId,1,2);
#endif
      return(0.0);
   }

   return(Datum[(DatumId)].FloatValue);

}



// ================================================
// SetDatumNone
// ================================================
int CigiBaseShortSymbolCtrl::SetDatumNone(int DatumId, bool bndchk)
{

   //This must be bound checked!
   if((DatumId < 0)||(DatumId > 1))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DatumId", (int)DatumId,1,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

   DatumType[(DatumId)] = None;
   Datum[(DatumId)].UIntValue = 0;

   return(CIGI_SUCCESS);

}



// ================================================
// SetSurfaceID
// ================================================
int CigiBaseShortSymbolCtrl::SetSurfaceID(int DatumId,
            const Cigi_uint16 SurfaceIDIn, bool bndchk)
{

   //This must be bound checked!
   if((DatumId < 0)||(DatumId > 1))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DatumId", (int)DatumId,1,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

   DatumType[(DatumId)] = SurfaceIdDatumType;
   Datum[(DatumId)].UIntValue = (Cigi_uint32)SurfaceIDIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetParentSymbolID
// ================================================
int CigiBaseShortSymbolCtrl::SetParentSymbolID(int DatumId,
            const Cigi_uint16 ParentSymbolIDIn, bool bndchk)
{

   //This must be bound checked!
   if((DatumId < 0)||(DatumId > 1))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DatumId", (int)DatumId,1,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

   DatumType[(DatumId)] = ParentIdDatumType;
   Datum[(DatumId)].UIntValue = (Cigi_uint32)ParentSymbolIDIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetLayer
// ================================================
int CigiBaseShortSymbolCtrl::SetLayer(int DatumId,
             const Cigi_uint8 LayerIn, bool bndchk)
{

   //This must be bound checked!
   if((DatumId < 0)||(DatumId > 1))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DatumId", (int)DatumId,1,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

   DatumType[(DatumId)] = LayerDatumType;
   Datum[(DatumId)].UIntValue = (Cigi_uint32)LayerIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetFlashDutyCycle
// ================================================
int CigiBaseShortSymbolCtrl::SetFlashDutyCycle(int DatumId,
             const Cigi_uint8 FlashDutyCycleIn, bool bndchk)
{

   //This must be bound checked!
   if((DatumId < 0)||(DatumId > 1))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DatumId", (int)DatumId,1,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((FlashDutyCycleIn < 0)||(FlashDutyCycleIn > 100)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("FlashDutyCycle", (int)FlashDutyCycleIn,0,100);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   DatumType[(DatumId)] = FlashDutyCycleDatumType;
   Datum[(DatumId)].UIntValue = (Cigi_uint32)FlashDutyCycleIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetFlashDutyCycle
// ================================================
int CigiBaseShortSymbolCtrl::SetFlashPeriod(int DatumId,
                     const float FlashPeriodIn, bool bndchk)
{

   //This must be bound checked!
   if((DatumId < 0)||(DatumId > 1))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DatumId", (int)DatumId,1,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

   DatumType[(DatumId)] = FlashPeriodDatumType;
   Datum[(DatumId)].FloatValue = FlashPeriodIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetUPosition
// ================================================
int CigiBaseShortSymbolCtrl::SetUPosition(int DatumId,
                     const float UPositionIn, bool bndchk)
{

   //This must be bound checked!
   if((DatumId < 0)||(DatumId > 1))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DatumId", (int)DatumId,1,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

   DatumType[(DatumId)] = UPositionDatumType;
   Datum[(DatumId)].FloatValue = UPositionIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetVPosition
// ================================================
int CigiBaseShortSymbolCtrl::SetVPosition(int DatumId,
                     const float VPositionIn, bool bndchk)
{

   //This must be bound checked!
   if((DatumId < 0)||(DatumId > 1))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DatumId", (int)DatumId,1,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

   DatumType[(DatumId)] = VPositionDatumType;
   Datum[(DatumId)].FloatValue = VPositionIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetRotation
// ================================================
int CigiBaseShortSymbolCtrl::SetRotation(int DatumId,
                     const float RotationIn, bool bndchk)
{

   //This must be bound checked!
   if((DatumId < 0)||(DatumId > 1))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DatumId", (int)DatumId,1,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

   DatumType[(DatumId)] = RotationDatumType;
   Datum[(DatumId)].FloatValue = RotationIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetScaleU
// ================================================
int CigiBaseShortSymbolCtrl::SetScaleU(int DatumId,
                     const float ScaleUIn, bool bndchk)
{

   //This must be bound checked!
   if((DatumId < 0)||(DatumId > 1))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DatumId", (int)DatumId,1,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

   DatumType[(DatumId)] = ScaleUDatumType;
   Datum[(DatumId)].FloatValue = ScaleUIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetScaleV
// ================================================
int CigiBaseShortSymbolCtrl::SetScaleV(int DatumId,
                     const float ScaleVIn, bool bndchk)
{

   //This must be bound checked!
   if((DatumId < 0)||(DatumId > 1))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DatumId", (int)DatumId,1,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

   DatumType[(DatumId)] = ScaleVDatumType;
   Datum[(DatumId)].FloatValue = ScaleVIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetColor
// ================================================
int CigiBaseShortSymbolCtrl::SetColor(int DatumId,
                                      const Cigi_uint8 RedIn,
                                      const Cigi_uint8 GreenIn,
                                      const Cigi_uint8 BlueIn,
                                      const Cigi_uint8 AlphaIn,
                                      bool bndchk)
{

   //This must be bound checked!
   if((DatumId < 0)||(DatumId > 1))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DatumId", (int)DatumId,1,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

   DatumType[(DatumId)] = ColorDatumType;

   Cigi_uint32 cColor = (Cigi_uint32)AlphaIn;
   Cigi_uint32 tColor =  (Cigi_uint32)RedIn;
   cColor |= ((tColor << 24) & 0xff000000);
   tColor =  (Cigi_uint32)GreenIn;
   cColor |= ((tColor << 16) & 0x00ff0000);
   tColor =  (Cigi_uint32)BlueIn;
   cColor |= ((tColor << 8) & 0x0000ff00);
   Datum[(DatumId)].UIntValue = cColor;

   return(CIGI_SUCCESS);

}


