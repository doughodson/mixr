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
 *  FILENAME:   CigiShortCompCtrlV3_3.cpp
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
 *  02/11/2008 Greg Basler                       Version 2.0.0
 *  Initial Release.
 *  Converted from CigiShortCompCtrlV3.cpp.
 *  Fixed the conversion process.
 *  
 *  05/15/2008 Greg Basler                       Version 2.2.0
 *  Corrected problem with conversion from V3_3 to V2 & V1
 *   involving Component Classes of SymbolSurfaceV3_3 &
 *   SymbolV3_3.
 *  Corrected Version Number.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiShortCompCtrlV3_3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"




// Component Class conversion Table
const CigiBaseCompCtrl::CompAssocGrp CigiShortCompCtrlV3_3::CompClassV3xV1[CigiShortCompCtrlV3_3::CompClassCnvtSz] =
{
   Entity,  // EntityV3
   View,    // ViewV3
   NoCnvtV1, // ViewGrpV3
   NoCnvtV1,  // SensorV3
   NoCnvtV1,  // RegionalSeaSurfaceV3
   NoCnvtV1,  // RegionalTerrainSurfaceV3
   NoCnvtV1,  // RegionalLayeredWeatherV3
   NoCnvtV1,  // GlobalSeaSurfaceV3
   NoCnvtV1,  // GlobalTerrainSurfaceV3
   NoCnvtV1,  // GlobalLayeredWeatherV3
   Environment,  // AtmosphereV3
   Environment,  // CelestialSphereV3
   NoCnvtV1,  // EventV3
   NoCnvtV1,  // SystemV3
   NoCnvtV1,  // SymbolSurfaceV3_3
   NoCnvtV1   // SymbolV3_3
};

const CigiBaseCompCtrl::CompClassV2Grp CigiShortCompCtrlV3_3::CompClassV3xV2[CigiShortCompCtrlV3_3::CompClassCnvtSz] =
{
   EntityV2,  // EntityV3
   ViewV2,    // ViewV3
   ViewGrpV2, // ViewGrpV3
   SensorV2,  // SensorV3
   NoCnvtV2,  // RegionalSeaSurfaceV3
   NoCnvtV2,  // RegionalTerrainSurfaceV3
   NoCnvtV2,  // RegionalLayeredWeatherV3
   NoCnvtV2,  // GlobalSeaSurfaceV3
   NoCnvtV2,  // GlobalTerrainSurfaceV3
   NoCnvtV2,  // GlobalLayeredWeatherV3
   EnvironmentV2,  // AtmosphereV3
   EnvironmentV2,  // CelestialSphereV3
   NoCnvtV2,  // EventV3
   SystemV2,  // SystemV3
   NoCnvtV2,  // SymbolSurfaceV3_3
   NoCnvtV2   // SymbolV3_3
};



// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiShortCompCtrlV3_3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiShortCompCtrlV3_3::CigiShortCompCtrlV3_3()
{

   PacketID = CIGI_SHORT_COMP_CTRL_PACKET_ID_V3_3;
   PacketSize = CIGI_SHORT_COMP_CTRL_PACKET_SIZE_V3_3;
   Version = 3;
   MinorVersion = 3;

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
// ~CigiShortCompCtrlV3_3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiShortCompCtrlV3_3::~CigiShortCompCtrlV3_3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiShortCompCtrlV3_3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseCompCtrl * Data = ( CigiBaseCompCtrl *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->CompID;
   *CDta.s++ = Data->InstanceID;

   *CDta.c++ = Data->CompClassV3 & 0x3f;

   *CDta.c++ = (Cigi_uint8)((Data->CompState > 0x00ff) ?
                                 0x00ff : Data->CompState);

   *CDta.l++ = Data->CompData[0];
   *CDta.l++ = Data->CompData[1];

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiShortCompCtrlV3_3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {
      CompID = *CDta.s++;
      InstanceID = *CDta.s++;

      CompClassV3 = (CompClassV3Grp)(*CDta.c++ & 0x3f);

      CompState = (Cigi_uint16)*CDta.c++;

      CompData[0] = *CDta.l++;
      CompData[1] = *CDta.l++;
   }
   else
   {
      CigiSwap2(&CompID, CDta.s++);
      CigiSwap2(&InstanceID, CDta.s++);

      CompClassV3 = (CompClassV3Grp)(*CDta.c++ & 0x3f);

      CompState = (Cigi_uint16)*CDta.c++;

      CigiSwap4(&CompData[0], CDta.l++);
      CigiSwap4(&CompData[1], CDta.l++);
   }


   if((CompClassV3 >= CigiBaseCompCtrl::EntityV3) &&
      (CompClassV3 <= CigiBaseCompCtrl::SymbolV3_3))
   {
      CompAssoc = CompClassV3xV1[CompClassV3];
      CompClassV2 = CompClassV3xV2[CompClassV3];
   }
   else
   {
      CompAssoc = NoCnvtV1;
      CompClassV2 = NoCnvtV2;
   }

   return(PacketSize);

}


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiShortCompCtrlV3_3::GetCnvt(CigiVersionID &CnvtVersion,
                                 CigiCnvtInfoType::Type &CnvtInfo)
{
   // Do not convert unless a conversion is found
   CnvtInfo.ProcID = CigiProcessType::TwoPassCnvtProcNone;
   CnvtInfo.CnvtPacketID = 0;

   if(CnvtVersion.CigiMajorVersion == 3)
   {
      // All Component control packets from version 3 and above
      //  use the same packet id number

      if(CnvtVersion.CigiMinorVersion < 3)
      {
         if((CompClassV3 >= EntityV3)&&(CompClassV3 <= SystemV3))
         {
            CnvtInfo.ProcID = CigiProcessType::TwoPassCnvtProcStd;
            CnvtInfo.CnvtPacketID = CIGI_SHORT_COMP_CTRL_PACKET_ID_V3;
         }
      }
      else
      {
         CnvtInfo.ProcID = CigiProcessType::TwoPassCnvtProcStd;
         CnvtInfo.CnvtPacketID = CIGI_SHORT_COMP_CTRL_PACKET_ID_V3_3;
      }
   }
   else if(CnvtVersion.CigiMajorVersion == 2)
   {
      if((CompClassV2 >= EntityV2)&&(CompClassV2 <= SystemV2))
      {
         CnvtInfo.ProcID = CigiProcessType::TwoPassCnvtProcStd;
         CnvtInfo.CnvtPacketID = CIGI_COMP_CTRL_PACKET_ID_V2;
      }
   }
   else if(CnvtVersion.CigiMajorVersion == 1)
   {
      if((CompAssoc >= Entity)&&(CompAssoc <= View))
      {
         CnvtInfo.ProcID = CigiProcessType::TwoPassCnvtProcStd;
         CnvtInfo.CnvtPacketID = CIGI_COMP_CTRL_PACKET_ID_V1;
      }
   }
   else
   {
      // All Component control packets from version 3 and above
      //  use the same packet id number
      CnvtInfo.ProcID = CigiProcessType::TwoPassCnvtProcStd;
      CnvtInfo.CnvtPacketID = CIGI_SHORT_COMP_CTRL_PACKET_ID_V3_3;
   }

   return(CIGI_SUCCESS);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// CompClassV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiShortCompCtrlV3_3::SetCompClassV3(const CompClassV3Grp CompClassV3In, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((CompClassV3In < 0)||(CompClassV3In > 15)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("CompClassV3",(CompClassV3Grp)CompClassV3In,0,15);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   CompClassV3 = CompClassV3In;

   CompAssoc = CompClassV3xV1[CompClassV3];
   CompClassV2 = CompClassV3xV2[CompClassV3];

   return(CIGI_SUCCESS);

}



// ================================================
// SetCompData - Cigi_uint8
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiShortCompCtrlV3_3::SetCompData(const Cigi_uint8 CompDataIn,
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
int CigiShortCompCtrlV3_3::SetCompData(const Cigi_int8 CompDataIn,
                    const unsigned int Word,
                    const BytePos Pos,
                    bool bndchk)
{

   union {
      Cigi_int8 sc;
      Cigi_uint8 uc;
   } bxfer;

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

   bxfer.sc = CompDataIn;
   Cigi_uint32 tDta = (Cigi_uint32)bxfer.uc;

   CompData[Word] &= ~(0x000000ff << bitPos);
   CompData[Word] |= ((tDta & 0x000000ff) << bitPos);

   return(CIGI_SUCCESS);
}


// ================================================
// SetCompData - Cigi_uint16
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiShortCompCtrlV3_3::SetCompData(const Cigi_uint16 CompDataIn,
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
int CigiShortCompCtrlV3_3::SetCompData(const Cigi_int16 CompDataIn,
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
int CigiShortCompCtrlV3_3::SetCompData(const Cigi_uint32 CompDataIn,
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
int CigiShortCompCtrlV3_3::SetCompData(const Cigi_int32 CompDataIn,
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
   CompData[Word] = tDta.u32;


   return(CIGI_SUCCESS);
}


// ================================================
// SetCompData - float
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiShortCompCtrlV3_3::SetCompData(const float CompDataIn,
                const unsigned int Word,
                bool bndchk)
{

   union {
      Cigi_uint32 u32;
      float f32;
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

   tDta.f32 = CompDataIn;
   CompData[Word] = tDta.u32;

   return(CIGI_SUCCESS);
}


// ================================================
// SetCompData  - Cigi_uint64
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiShortCompCtrlV3_3::SetCompData(const Cigi_uint64 CompDataIn,
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
int CigiShortCompCtrlV3_3::SetCompData(const double CompDataIn,
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
Cigi_uint8 CigiShortCompCtrlV3_3::GetUCharCompData(const unsigned int Word,
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
Cigi_int8 CigiShortCompCtrlV3_3::GetCharCompData(const unsigned int Word,
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
Cigi_uint16 CigiShortCompCtrlV3_3::GetUShortCompData(const unsigned int Word,
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
Cigi_int16 CigiShortCompCtrlV3_3::GetShortCompData(const unsigned int Word,
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
Cigi_uint32 CigiShortCompCtrlV3_3::GetULongCompData(const unsigned int Word)
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
Cigi_int32 CigiShortCompCtrlV3_3::GetLongCompData(const unsigned int Word)
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
float CigiShortCompCtrlV3_3::GetFloatCompData(const unsigned int Word)
{

   union {
      Cigi_uint32 u32;
      float f32;
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
Cigi_uint64 CigiShortCompCtrlV3_3::GetI64CompData(const unsigned int Pos)
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
double CigiShortCompCtrlV3_3::GetDoubleCompData(const unsigned int Pos)
{

   union DtaXfer {
      Cigi_uint64 i;
      double d;
   } XDta;

   if(Pos > 0)
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




