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
 *  FILENAME:   CigiCompCtrlV2.cpp
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
 *  Defined _EXPORT_CCL_ for exporting the class in a Windows DLL.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Removed the inline defnitions for the PackedPointer union.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Changed native C++ types to use CCL-defined types instead where appropriate.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the class constructor to initialize the MinorVersion member 
 *  variable.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  
 *  02/11/2008 Greg Basler                       Version 2.0.0
 *  Changed the conversion process.
 *  
 *  05/14/2008 Greg Basler                       Version 2.2.0
 *  Fixed the conversion process.
 *  
 *  05/15/2008 Greg Basler                       Version 2.2.0
 *  Changed the Component class Conversion table sizing to a unified
 *   constant.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiCompCtrlV2.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"

#include <string.h>


#ifdef CIGI_LITTLE_ENDIAN
   #define CIGI_SCOPY2 CigiSwap2
   #define CIGI_SCOPY4 CigiSwap4
   #define CIGI_SCOPY8 CigiSwap8
#else
   #define CIGI_SCOPY2 CigiCopy2
   #define CIGI_SCOPY4 CigiCopy4
   #define CIGI_SCOPY8 CigiCopy8
#endif



// Component Class conversion Tables
const CigiBaseCompCtrl::CompAssocGrp CigiCompCtrlV2::CompClassV2xV1[CigiCompCtrlV2::CompClassCnvtSz] =
{
   Entity,
   Environment,
   View,
   NoCnvtV1,
   NoCnvtV1,
   NoCnvtV1
};

const CigiBaseCompCtrl::CompClassV3Grp CigiCompCtrlV2::CompClassV2xV3[CigiCompCtrlV2::CompClassCnvtSz] =
{
   EntityV3,
   AtmosphereV3,
   ViewV3,
   ViewGrpV3,
   SensorV3,
   SystemV3
};


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiCompCtrlV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCompCtrlV2::CigiCompCtrlV2()
{

   PacketID = CIGI_COMP_CTRL_PACKET_ID_V2;
   PacketSize = CIGI_COMP_CTRL_PACKET_SIZE_V2;
   Version = 2;
   MinorVersion = 0;

   CompID = 0;
   InstanceID = 0;
   EntityID = 0;
   ViewID = 0;
   CompState = 0;
   CompAssoc = CigiBaseCompCtrl::Entity;
   CompClassV2 = CigiBaseCompCtrl::EntityV2;
   CompClassV3 = CigiBaseCompCtrl::EntityV3;
   CompData[0] = 0;
   CompData[1] = 0;
   CompData[2] = 0;
   CompData[3] = 0;
   CompData[4] = 0;
   CompData[5] = 0;

}

// ================================================
// ~CigiCompCtrlV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCompCtrlV2::~CigiCompCtrlV2()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseCompCtrl * Data = ( CigiBaseCompCtrl *)Base;

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   CIGI_SCOPY2(CDta.s++, &Data->InstanceID);
   *CDta.c++ = Data->CompClassV2;

   *CDta.c++ = 0;
   *CDta.s++ = 0;

   CIGI_SCOPY2(CDta.s++, &Data->CompID);
   CIGI_SCOPY2(CDta.s++, &Data->CompState);
   CIGI_SCOPY4(CDta.l++, &Data->CompData[0]);
   CIGI_SCOPY4(CDta.l++, &Data->CompData[1]);


   memcpy(Buff,tBuf,CIGI_COMP_CTRL_PACKET_SIZE_V2);

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   memcpy(tBuf,Buff,CIGI_COMP_CTRL_PACKET_SIZE_V2);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size


   CIGI_SCOPY2(&InstanceID, CDta.s++);
   CompClassV2 = (CompClassV2Grp)*CDta.c++;

   CDta.c += 3;

   CIGI_SCOPY2(&CompID, CDta.s++);
   CIGI_SCOPY2(&CompState, CDta.s++);
   CIGI_SCOPY4(&CompData[0], CDta.l++);
   CIGI_SCOPY4(&CompData[1], CDta.l++);


   if((CompClassV2 >= CigiBaseCompCtrl::EntityV2) &&
      (CompClassV2 <= CigiBaseCompCtrl::SystemV2))
   {
      CompAssoc = CompClassV2xV1[CompClassV2];
      CompClassV3 = CompClassV2xV3[CompClassV2];
   }
   else
   {
      CompAssoc = CigiBaseCompCtrl::NoCnvtV1;
      CompClassV3 = CigiBaseCompCtrl::NoCnvtV3;
   }


   return(PacketSize);

}


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV2::GetCnvt(CigiVersionID &CnvtVersion,
                            CigiCnvtInfoType::Type &CnvtInfo)
{

   if(CnvtVersion.CigiMajorVersion == 2)
   {
      CnvtInfo.ProcID = CigiProcessType::TwoPassCnvtProcStd;
      CnvtInfo.CnvtPacketID = CIGI_COMP_CTRL_PACKET_ID_V2;
   }
   else if(CnvtVersion.CigiMajorVersion == 1)
   {
      if((CompAssoc >= Entity)&&(CompAssoc <= View))
      {
         CnvtInfo.ProcID = CigiProcessType::TwoPassCnvtProcStd;
         CnvtInfo.CnvtPacketID = CIGI_COMP_CTRL_PACKET_ID_V1;
      }
      else
      {
         CnvtInfo.ProcID = CigiProcessType::TwoPassCnvtProcNone;
         CnvtInfo.CnvtPacketID = 0;
      }
   }
   else
   {
      CnvtInfo.ProcID = CigiProcessType::TwoPassCnvtProcStd;
      CnvtInfo.CnvtPacketID = CIGI_SHORT_COMP_CTRL_PACKET_ID_V3;
   }

   return(CIGI_SUCCESS);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// CompClassV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV2::SetCompClassV2(const CompClassV2Grp CompClassV2In, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((CompClassV2In < 0)||(CompClassV2In > 5)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("CompClassV2",(CompClassV2Grp)CompClassV2In,0,5);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   CompClassV2 = CompClassV2In;


   CompAssoc = CompClassV2xV1[CompClassV2];
   CompClassV3 = CompClassV2xV3[CompClassV2];


   return(CIGI_SUCCESS);

}



// ================================================
// SetCompData - Cigi_uint8
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV2::SetCompData(const Cigi_uint8 CompDataIn,
                    const unsigned int Word,
                    const BytePos Pos,
                    bool bndchk)
{

   int Err = 0;

   if(Word > 1)
      Err = 1;
   else if((Pos < 0) || (Pos > 3))
      Err = 2;

   if(Err != 0)
   {
#ifndef CIGI_NO_BND_CHK
      if(bndchk)
      {
#ifndef CIGI_NO_EXCEPT
         if(Err == 1)
            throw CigiValueOutOfRangeException("Word",Word,0,1);
         else
            throw CigiValueOutOfRangeException("Pos",Pos,0,3);
#endif
      }
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }


   int bitPos = Pos * 8;

   Cigi_uint32 tDta = (Cigi_uint32)CompDataIn;

   CompData[Word] &= ~(0x000000ff << bitPos);
   CompData[Word] |= ((tDta & 0x000000ff) << bitPos);

   return(CIGI_SUCCESS);
}


// ================================================
// SetCompData - Cigi_int8
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV2::SetCompData(const Cigi_int8 CompDataIn,
                    const unsigned int Word,
                    const BytePos Pos,
                    bool bndchk)
{

   union {
      Cigi_int8 sc;
      Cigi_uint8 uc;
   } cxfer;

   int Err = 0;

   if(Word > 1)
      Err = 1;
   else if((Pos < 0) || (Pos > 3))
      Err = 2;

   if(Err != 0)
   {
#ifndef CIGI_NO_BND_CHK
      if(bndchk)
      {
#ifndef CIGI_NO_EXCEPT
         if(Err == 1)
            throw CigiValueOutOfRangeException("Word",Word,0,1);
         else
            throw CigiValueOutOfRangeException("Pos",Pos,0,3);
#endif
      }
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }


   int bitPos = Pos * 8;

   cxfer.sc = CompDataIn;
   Cigi_uint32 tDta = (Cigi_uint32)cxfer.uc;

   CompData[Word] &= ~(0x000000ff << bitPos);
   CompData[Word] |= ((tDta & 0x000000ff) << bitPos);

   return(CIGI_SUCCESS);
}


// ================================================
// SetCompData - Cigi_uint16
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV2::SetCompData(const Cigi_uint16 CompDataIn,
                    const unsigned int Word,
                    const HalfWordPos Pos,
                    bool bndchk)
{

   int Err = 0;

   if(Word > 1)
      Err = 1;
   else if((Pos < 0) || (Pos > 1))
      Err = 2;

   if(Err != 0)
   {
#ifndef CIGI_NO_BND_CHK
      if(bndchk)
      {
#ifndef CIGI_NO_EXCEPT
         if(Err == 1)
            throw CigiValueOutOfRangeException("Word",Word,0,1);
         else
            throw CigiValueOutOfRangeException("Pos",Pos,0,3);
#endif
      }
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }


   int bitPos = Pos * 16;

   Cigi_uint32 tDta = (Cigi_uint32)CompDataIn;

   CompData[Word] &= ~(0x0000ffff << bitPos);
   CompData[Word] |= ((tDta & 0x0000ffff) << bitPos);

   return(CIGI_SUCCESS);
}


// ================================================
// SetCompData - short
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV2::SetCompData(const Cigi_int16 CompDataIn,
                    const unsigned int Word,
                    const HalfWordPos Pos,
                    bool bndchk)
{

   union {
      Cigi_int16 s16;
      Cigi_uint16 u16;
   } sxfer;

   int Err = 0;

   if(Word > 1)
      Err = 1;
   else if((Pos < 0) || (Pos > 1))
      Err = 2;

   if(Err != 0)
   {
#ifndef CIGI_NO_BND_CHK
      if(bndchk)
      {
#ifndef CIGI_NO_EXCEPT
         if(Err == 1)
            throw CigiValueOutOfRangeException("Word",Word,0,1);
         else
            throw CigiValueOutOfRangeException("Pos",Pos,0,3);
#endif
      }
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }


   int bitPos = Pos * 16;

   sxfer.s16 = CompDataIn;
   Cigi_uint32 tDta = (Cigi_uint32)sxfer.u16;

   CompData[Word] &= ~(0x0000ffff << bitPos);
   CompData[Word] |= ((tDta & 0x0000ffff) << bitPos);

   return(CIGI_SUCCESS);
}


// ================================================
// SetCompData - Cigi_uint32
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV2::SetCompData(const Cigi_uint32 CompDataIn,
                const unsigned int Word,
                bool bndchk)
{

   if(Word > 1)
   {
#ifndef CIGI_NO_BND_CHK
      if(bndchk)
      {
#ifndef CIGI_NO_EXCEPT
         throw CigiValueOutOfRangeException("Word",Word,0,1);
#endif
      }
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }


   CompData[Word] = CompDataIn;

   return(CIGI_SUCCESS);
}


// ================================================
// SetCompData - long
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV2::SetCompData(const Cigi_int32 CompDataIn,
                const unsigned int Word,
                bool bndchk)
{

   union {
      Cigi_uint32 u32;
      Cigi_int32 s32;
   } tDta;

   if(Word > 1)
   {
#ifndef CIGI_NO_BND_CHK
      if(bndchk)
      {
#ifndef CIGI_NO_EXCEPT
         throw CigiValueOutOfRangeException("Word",Word,0,1);
#endif
      }
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }


   tDta.s32 = CompDataIn;
   CompData[Word] = (Cigi_uint32)tDta.u32;

   return(CIGI_SUCCESS);
}


// ================================================
// SetCompData - float
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV2::SetCompData(const float CompDataIn,
                const unsigned int Word,
                bool bndchk)
{

   if(Word > 1)
   {
#ifndef CIGI_NO_BND_CHK
      if(bndchk)
      {
#ifndef CIGI_NO_EXCEPT
         throw CigiValueOutOfRangeException("Word",Word,0,1);
#endif
      }
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }


   union DtaXfer {
      Cigi_uint32 lDta;
      float fDta;
   } XDta;

   XDta.fDta = CompDataIn;
   CompData[Word] = XDta.lDta;

   return(CIGI_SUCCESS);
}


// ================================================
// SetCompData  - Cigi_uint64
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV2::SetCompData(const Cigi_uint64 CompDataIn,
                const unsigned int Pos,
                bool bndchk)
{

   if(Pos > 0)
   {
#ifndef CIGI_NO_BND_CHK
      if(bndchk)
      {
#ifndef CIGI_NO_EXCEPT
         throw CigiValueOutOfRangeException("Pos",Pos,0,0);
#endif
      }
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }


   int Word = Pos * 2;

   CompData[Word++] = (Cigi_uint32)((CompDataIn >> 32) & 0x00000000ffffffff);
   CompData[Word] = (Cigi_uint32)(CompDataIn & 0x00000000ffffffff);

   return(CIGI_SUCCESS);
}


// ================================================
// SetCompData - double
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV2::SetCompData(const double CompDataIn,
                const unsigned int Pos,
                bool bndchk)
{

   if(Pos > 0)
   {
#ifndef CIGI_NO_BND_CHK
      if(bndchk)
      {
#ifndef CIGI_NO_EXCEPT
         throw CigiValueOutOfRangeException("Pos",Pos,0,0);
#endif
      }
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

   union DtaXfer {
      Cigi_uint64 i;
      double d;
   } XDta;

   XDta.d = CompDataIn;

   int Word = Pos * 2;

   CompData[Word++] = (Cigi_uint32)((XDta.i >> 32) & 0x00000000ffffffff);
   CompData[Word] = (Cigi_uint32)(XDta.i & 0x00000000ffffffff);


   return(CIGI_SUCCESS);
}







// ================================================
// GetUCharCompData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Cigi_uint8 CigiCompCtrlV2::GetUCharCompData(const unsigned int Word,
                               const BytePos Pos)
{

   Cigi_uint8 tDta;

   int Err = 0;

   if(Word > 1)
      Err = 1;
   else if((Pos < 0) || (Pos > 3))
      Err = 2;

   if(Err != 0)
   {
#ifndef CIGI_NO_EXCEPT
      if(Err == 1)
         throw CigiValueOutOfRangeException("Word",Word,0,1);
      else
         throw CigiValueOutOfRangeException("Pos",Pos,0,3);
#endif

      tDta = 0;
   }
   else
      tDta = (Cigi_uint8)((CompData[Word] >> (Pos * 8)) & 0x000000ff);

   return(tDta);
}


// ================================================
// GetCharCompData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Cigi_int8 CigiCompCtrlV2::GetCharCompData(const unsigned int Word,
                     const BytePos Pos)
{
   union {
      Cigi_int8 sc;
      Cigi_uint8 uc;
   } tDta;

   int Err = 0;

   if(Word > 1)
      Err = 1;
   else if((Pos < 0) || (Pos > 3))
      Err = 2;

   if(Err != 0)
   {
#ifndef CIGI_NO_EXCEPT
      if(Err == 1)
         throw CigiValueOutOfRangeException("Word",Word,0,1);
      else
         throw CigiValueOutOfRangeException("Pos",Pos,0,3);
#endif

      tDta.uc = 0;
   }
   else
      tDta.uc = (Cigi_uint8)((CompData[Word] >> (Pos * 8)) & 0x000000ff);

   return(tDta.sc);
}


// ================================================
// GetUShortCompData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Cigi_uint16 CigiCompCtrlV2::GetUShortCompData(const unsigned int Word,
                                 const HalfWordPos Pos)
{
   Cigi_uint16 tDta;

   int Err = 0;

   if(Word > 1)
      Err = 1;
   else if((Pos < 0) || (Pos > 1))
      Err = 2;

   if(Err != 0)
   {
#ifndef CIGI_NO_EXCEPT
      if(Err == 1)
         throw CigiValueOutOfRangeException("Word",Word,0,1);
      else
         throw CigiValueOutOfRangeException("Pos",Pos,0,1);
#endif

      tDta = 0;
   }
   else
      tDta = (Cigi_uint16)((CompData[Word] >> (Pos * 16)) & 0x0000ffff);

   return(tDta);
}


// ================================================
// GetShortCompData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Cigi_int16 CigiCompCtrlV2::GetShortCompData(const unsigned int Word,
                       const HalfWordPos Pos)
{
   union {
      Cigi_int16 s16;
      Cigi_uint16 u16;
   } tDta;

   int Err = 0;

   if(Word > 1)
      Err = 1;
   else if((Pos < 0) || (Pos > 1))
      Err = 2;

   if(Err != 0)
   {
#ifndef CIGI_NO_EXCEPT
      if(Err == 1)
         throw CigiValueOutOfRangeException("Word",Word,0,1);
      else
         throw CigiValueOutOfRangeException("Pos",Pos,0,1);
#endif

      tDta.u16 = 0;
   }
   else
      tDta.u16 = (Cigi_uint16)((CompData[Word] >> (Pos * 16)) & 0x0000ffff);

   return(tDta.s16);
}


// ================================================
// GetULongCompData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Cigi_uint32 CigiCompCtrlV2::GetULongCompData(const unsigned int Word)
{

   Cigi_uint32 tDta;


   if(Word > 1)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Word",Word,0,1);
#endif

      tDta = 0;
   }
   else
      tDta = CompData[Word];

   return(tDta);
}



// ================================================
// GetLongCompData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Cigi_int32 CigiCompCtrlV2::GetLongCompData(const unsigned int Word)
{

   union {
      Cigi_int32 s32;
      Cigi_uint32 u32;
   } tDta;

   if(Word > 1)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Word",Word,0,1);
#endif

      tDta.u32 = 0;
   }
   else
      tDta.u32 = (Cigi_uint32)(CompData[Word]);

   return(tDta.s32);
}



// ================================================
// GetFloatCompData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
float CigiCompCtrlV2::GetFloatCompData(const unsigned int Word)
{

   union {
      float f32;
      Cigi_uint32 u32;
   } tDta;

   if(Word > 1)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Word",Word,0,1);
#endif

      tDta.u32 = 0;
   }
   else
      tDta.u32 = (Cigi_uint32)(CompData[Word]);

   return(tDta.f32);
}



// ================================================
// GetI64CompData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Cigi_uint64 CigiCompCtrlV2::GetI64CompData(const unsigned int Pos)
{

   Cigi_uint64 tDta;

   if(Pos > 0)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Pos",Pos,0,0);
#endif

      tDta = 0;
   }
   else
   {
      int Word = Pos * 2;
      Cigi_uint64 tDtah = (Cigi_uint64)(CompData[Word++]);
      tDtah = (tDtah << 32) & _I64_CONST_(0xffffffff00000000);

      Cigi_uint64 tDtal = (Cigi_uint64)(CompData[Word]);
      tDtal &= _I64_CONST_(0x00000000ffffffff);

      tDta = tDtah | tDtal;
   }


   return(tDta);
}



// ================================================
// GetDoubleCompData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
double CigiCompCtrlV2::GetDoubleCompData(const unsigned int Pos)
{

   union DtaXfer {
      Cigi_uint64 i;
      double d;
   } XDta;

   if(Pos > 1)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Pos",Pos,0,0);
#endif

   }
   else
   {

      int Word = Pos * 2;
      Cigi_uint64 tDtah = (Cigi_uint64)(CompData[Word++]);
      tDtah = (tDtah << 32) & _I64_CONST_(0xffffffff00000000);

      Cigi_uint64 tDtal = (Cigi_uint64)(CompData[Word]);
      tDtal &= _I64_CONST_(0x00000000ffffffff);

      XDta.i = tDtah | tDtal;

   }

   return(XDta.d);
}



