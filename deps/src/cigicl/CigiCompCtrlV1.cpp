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
 *  FILENAME:   CigiCompCtrlV1.cpp
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
 *  05/15/2008 Greg Basler                       Version 2.2.0
 *  Changed the Component class Conversion table sizing to a unified
 *   constant.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiCompCtrlV1.h"
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



const CigiBaseCompCtrl::CompClassV3Grp CigiCompCtrlV1::CompClassV1xV3[CigiCompCtrlV1::CompClassCnvtSz] =
{
   EntityV3,
   AtmosphereV3,
   ViewV3
};


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiCompCtrlV1
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCompCtrlV1::CigiCompCtrlV1()
{

   PacketID = CIGI_COMP_CTRL_PACKET_ID_V1;
   PacketSize = CIGI_COMP_CTRL_PACKET_SIZE_V1;
   Version = 1;
   MinorVersion = 0;

   CompID = 0;
   InstanceID = 0;
   EntityID = 0;
   ViewID = 0;
   CompState = 0;
   CompAssoc = Entity;
   CompClassV2 = EntityV2;
   CompClassV3 = EntityV3;
   CompData[0] = 0;
   CompData[1] = 0;
   CompData[2] = 0;
   CompData[3] = 0;
   CompData[4] = 0;
   CompData[5] = 0;

}

// ================================================
// ~CigiCompCtrlV1
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCompCtrlV1::~CigiCompCtrlV1()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV1::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseCompCtrl * Data = ( CigiBaseCompCtrl *)Base;

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   CIGI_SCOPY2(CDta.s++, &Data->EntityID);

   Cigi_uint8 HDta = (Data->ViewID << 3) & 0xf8;
   HDta |= (Data->CompAssoc << 1) & 0x06;
   *CDta.c++ = HDta;

   *CDta.c++ = 0;
   *CDta.s++ = 0;

   CIGI_SCOPY2(CDta.s++, &Data->CompID);
   CIGI_SCOPY2(CDta.s++, &Data->CompState);
   CIGI_SCOPY4(CDta.l++, &Data->CompData[0]);
   CIGI_SCOPY4(CDta.l++, &Data->CompData[1]);

   memcpy(Buff,tBuf,CIGI_COMP_CTRL_PACKET_SIZE_V1);

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV1::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{

   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   memcpy(tBuf,Buff,CIGI_COMP_CTRL_PACKET_SIZE_V1);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size

   CIGI_SCOPY2(&EntityID, CDta.s++);

   Cigi_uint8 HDta = *CDta.c++;
   ViewID = (HDta >> 3) & 0x1f;
   CompAssoc = (CompAssocGrp)((HDta >> 1) & 0x03);

   CDta.c += 3;
   
   CIGI_SCOPY2(&CompID, CDta.s++);
   CIGI_SCOPY2(&CompState, CDta.s++);
   CIGI_SCOPY4(&CompData[0], CDta.l++);
   CIGI_SCOPY4(&CompData[1], CDta.l++);


   if((CompAssoc >= CigiBaseCompCtrl::Entity) &&
      (CompAssoc <= CigiBaseCompCtrl::View))
   {
      // The Component classes in V2 valued at 0, 1, & 2
      //   match the classes in V1 in value and purpose.
      CompClassV2 = (CompClassV2Grp)CompAssoc;
      CompClassV3 = CompClassV1xV3[(int)CompAssoc];
   }
   else
   {
      CompClassV2 = CigiBaseCompCtrl::NoCnvtV2;
      CompClassV3 = CigiBaseCompCtrl::NoCnvtV3;
   }


   return(PacketSize);

}


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV1::GetCnvt(CigiVersionID &CnvtVersion,
                            CigiCnvtInfoType::Type &CnvtInfo)
{
   CnvtInfo.ProcID = CigiProcessType::ProcStd;

   // Note: CIGI_ART_PART_CTRL_PACKET_ID_V1 &
   // CIGI_ART_PART_CTRL_PACKET_ID_V2 are the same
   if(CnvtVersion.CigiMajorVersion < 3)
      CnvtInfo.CnvtPacketID = CIGI_COMP_CTRL_PACKET_ID_V2;
   else
      CnvtInfo.CnvtPacketID = CIGI_SHORT_COMP_CTRL_PACKET_ID_V3;

   return(CIGI_SUCCESS);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// ViewID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV1::SetViewID(const Cigi_uint8 ViewIDIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && (ViewIDIn > 31))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("ViewID",(Cigi_uint8)ViewIDIn,0,31);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   ViewID = ViewIDIn;
   if(CompAssoc == CigiBaseCompCtrl::View)
      InstanceID = ViewID;

   return(CIGI_SUCCESS);

}


// ================================================
// CompAssoc
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV1::SetCompAssoc(const CompAssocGrp CompAssocIn, bool bndchk)
{
   // V1 to V3 Component Class conversion table
   const CompClassV3Grp Assoc2ClassV3[3] =
   {
      EntityV3,
      AtmosphereV3,
      ViewV3
   };


#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((CompAssocIn < 0)||(CompAssocIn > 3)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("CompAssoc",(CompAssocGrp)CompAssocIn,0,3);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   CompAssoc = CompAssocIn;

   CompClassV2 = (CompClassV2Grp)CompAssoc;
   CompClassV3 = CompClassV1xV3[CompAssocIn];

   if(CompAssoc == CigiBaseCompCtrl::Entity)
      InstanceID = EntityID;
   else if(CompAssoc == CigiBaseCompCtrl::View)
      InstanceID = ViewID;
   else
      InstanceID = 0;

   return(CIGI_SUCCESS);

}


// ================================================
// SetCompData - Cigi_uint8
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV1::SetCompData(const Cigi_uint8 CompDataIn,
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
int CigiCompCtrlV1::SetCompData(const Cigi_int8 CompDataIn,
                    const unsigned int Word,
                    const BytePos Pos,
                    bool bndchk)
{

   union {
      Cigi_uint8 uc;
      Cigi_int8 sc;
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
int CigiCompCtrlV1::SetCompData(const Cigi_uint16 CompDataIn,
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
int CigiCompCtrlV1::SetCompData(const Cigi_int16 CompDataIn,
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
int CigiCompCtrlV1::SetCompData(const Cigi_uint32 CompDataIn,
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
int CigiCompCtrlV1::SetCompData(const Cigi_int32 CompDataIn,
                const unsigned int Word,
                bool bndchk)
{

   union {
      Cigi_int32 s32;
      Cigi_uint32 u32;
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
int CigiCompCtrlV1::SetCompData(const float CompDataIn,
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
int CigiCompCtrlV1::SetCompData(const Cigi_uint64 CompDataIn,
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

   CompData[Word++] = (Cigi_uint32)((CompDataIn >> 32) & _I64_CONST_(0x00000000ffffffff));
   CompData[Word] = (Cigi_uint32)(CompDataIn & _I64_CONST_(0x00000000ffffffff));

   return(CIGI_SUCCESS);
}


// ================================================
// SetCompData - double
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCompCtrlV1::SetCompData(const double CompDataIn,
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

   CompData[Word++] = (Cigi_uint32)((XDta.i >> 32) & _I64_CONST_(0x00000000ffffffff));
   CompData[Word] = (Cigi_uint32)(XDta.i & _I64_CONST_(0x00000000ffffffff));


   return(CIGI_SUCCESS);
}







// ================================================
// GetUCharCompData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Cigi_uint8 CigiCompCtrlV1::GetUCharCompData(const unsigned int Word,
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
Cigi_int8 CigiCompCtrlV1::GetCharCompData(const unsigned int Word,
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
Cigi_uint16 CigiCompCtrlV1::GetUShortCompData(const unsigned int Word,
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
Cigi_int16 CigiCompCtrlV1::GetShortCompData(const unsigned int Word,
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
Cigi_uint32 CigiCompCtrlV1::GetULongCompData(const unsigned int Word)
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
Cigi_int32 CigiCompCtrlV1::GetLongCompData(const unsigned int Word)
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
float CigiCompCtrlV1::GetFloatCompData(const unsigned int Word)
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
Cigi_uint64 CigiCompCtrlV1::GetI64CompData(const unsigned int Pos)
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
double CigiCompCtrlV1::GetDoubleCompData(const unsigned int Pos)
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

      XDta.d = 0.0;
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


