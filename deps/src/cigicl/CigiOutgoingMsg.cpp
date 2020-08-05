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
 *  FILENAME:   CigiOutgoingMsg.cpp
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
 *  09/15/2003 James Durtschy                    CIGI_CR_DR_1
 *  Initial Release.
 *
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Defined _EXPORT_CCL_ for exporting the class in a Windows DLL.
 *
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Changed native C++ types to use CCL-defined types instead where appropriate.
 *
 *  02/20/2006 Greg Basler                       Version 1.6.0
 *  Added the PackageMsg and FreeMsg methods to better update the frame
 *  the frame counter in the IG_Ctrl packet for hosts
 *
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the class constructor and destructor methods to initialize
 *  (allocate) and destroy the CurrentIGCtrl and CurrentSOF member variables.
 *
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Updated the overloaded << operators for the CigiBaseIGCtrl parameter to set
 *  the CurrentIGCtrl member variable, and for the CigiBaseSOF parameter to set
 *  the CurrentSOF member variable.
 *
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Updated the UpdateFrameCntr methods to use minor version numbers and to add
 *  additional frame synchronization functionality.
 *
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the UpdateIGCtrl and UpdateSOF methods to use minor version
 *  numbers and to add additional frame synchronization functionality.
 *
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Implemented the Reset method.
 *
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and
 *  Cigi_uint8.
 *
 *  11/27/2006 Greg Basler                       Version 1.7.4
 *  Corrected UpdateIGCtrl so that it zeros the outgoing database id when the
 *    outgoing database id is negative or incoming database id is negative
 *    unless it is -128 or the outgoing database id is the same as the
 *    incoming database id.
 *    It does not zero the id if the incoming id is -128 because an incoming
 *    database id of -128 signifies a database not found condition.
 *
 *  04/24/2007 Greg Basler                       Version 1.7.5
 *  Corrected a problem with the database id adjustment code in UpdateIGCtrl.
 *    The user can disable database id adjustment by declaring
 *    _NO_DATABASE_ADJUSTMENT_. (note: the system will still 0 a negative
 *    database request.) The database adjustment code now will 0 the database
 *    request if the incoming database number is the same or the negative of
 *    database request. It does not 0 the database request if the incoming
 *    database number is -128.
 *
 *  03/11/2008 Greg Basler                       Version 2.0.0
 *  Completely rewrote the way conversions are handled.  Also, rewrote
 *    the message buffers and how they are handled.  Also, removed
 *    the VersionJmpTbl.
 *
 *  04/03/2008 Greg Basler                       Version 2.1.0
 *  Added CigiSymbolCloneV3_3
 *
 *  05/09/2008 Greg Basler                       Version 2.2.0
 *  Added CigiIGCtrlV3_3
 *  Added CigiEntityCtrlV3_3
 *  Fixed the conversion process
 *
 *  05/16/2008 Greg Basler                       Version 2.2.0
 *  Fixed the EnvCtrl conversion process
 *
 *  12/7/200 Greg Basler                         Version 3.3.2
 *  Removed automatic zeroing of the database id based on the SOF declared
 *  database id.  Now the UpdateIGCtrl method just bounds checks the
 *  database id and corrects it to zero if needed.
 *
 * </pre>
 *  Author: The Boeing Company
 *
 */

#include <stdlib.h>

#define _EXPORT_CCL_

#include "CigiOutgoingMsg.h"

#include "CigiVersionID.h"
#include "CigiAllPackets.h"
#include "CigiExceptions.h"
#include "CigiSwapping.h"
#include "CigiSession.h"


#ifdef CIGI_LITTLE_ENDIAN
   #define CIGI_SCOPY2 CigiSwap2
   #define CIGI_SCOPY4 CigiSwap4
   #define CIGI_SCOPY8 CigiSwap8
#else
   #define CIGI_SCOPY2 CigiCopy2
   #define CIGI_SCOPY4 CigiCopy4
   #define CIGI_SCOPY8 CigiCopy8
#endif

using namespace std;


// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiOutgoingMsg
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiOutgoingMsg::CigiOutgoingMsg()
{

   pIGCtrlPck[0] = NULL;
   pIGCtrlPck[1] = new CigiIGCtrlV1;
   pIGCtrlPck[2] = new CigiIGCtrlV2;
   pIGCtrlPck[3] = new CigiIGCtrlV3;
   pIGCtrlPck[4] = new CigiIGCtrlV3_2;
   pIGCtrlPck[5] = new CigiIGCtrlV3_3;

   pSOFPck[0] = NULL;
   pSOFPck[1] = new CigiSOFV1;
   pSOFPck[2] = new CigiSOFV2;
   pSOFPck[3] = new CigiSOFV3;
   pSOFPck[4] = new CigiSOFV3_2;

   for(int ndx=0;ndx<256;ndx++)
   {
      OutgoingHandlerTbl[ndx] = (CigiBasePacket *)&DefaultPacket;
      VldSnd[ndx] = false;
   }

   CmdVersionChng = false;
   CmdVersion.SetCigiVersion(3,3);
   MostMatureVersionReceived.SetCigiVersion(0,0);

   FrameCnt = 0;
   LastRcvdFrame = 0;

}

// ================================================
// ~CigiOutgoingMsg
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiOutgoingMsg::~CigiOutgoingMsg()
{
   int ndx;
   for(ndx=1;ndx<6;ndx++)
   {
      delete pIGCtrlPck[ndx];
   }
   for(ndx=1;ndx<5;ndx++)
   {
      delete pSOFPck[ndx];
   }

   ClearHandlerTable();

}



// ====================================================================
// Processing
// ====================================================================


// ================================================
// IsSynchronous
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool CigiOutgoingMsg::IsSynchronous(void) const
{
   if(Session != NULL)
      return(Session->IsSynchronous());
   else
      return(true);  // default response
}


// ================================================
// AdvanceBuffer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CigiOutgoingMsg::AdvanceBuffer(void)
{
   if(AvailBuff.empty())
   {
      // Create a new buffer
      CrntFillBuf = new CigiMessageBuffer(BufferSize);
   }
   else
   {
      // Get the first buffer in the Available Buffers list
      CrntFillBuf = (*AvailBuff.begin());
      AvailBuff.pop_front();
   }

   // Add the buffer to the Active Buffers list
   Buffers.push_back(CrntFillBuf);

   // Initialize the buffer
   CrntFillBuf->Active = true;
   CrntFillBuf->BufferFillCnt = 0;
   CrntFillBuf->DataPresent = false;
   CrntFillBuf->FillBufferPos = CrntFillBuf->Buffer;
   CrntFillBuf->Locked = false;
   CrntFillBuf->ValidIGCtrlSOF = false;

   if(CmdVersionChng)
   {
      MostMatureVersionReceived.SetCigiVersion(0,0);
      ChangeOutgoingCigiVersion(CmdVersion);
      CmdVersionChng = false;
   }
   else if((OutgoingVersion != MostMatureVersionReceived) &&
           (MostMatureVersionReceived.CigiMajorVersion > 0))
      ChangeOutgoingCigiVersion(MostMatureVersionReceived);

   // Set the buffer's Cigi Version
   ChangeBufferCigiVersion();

}


// ================================================
// ChangeOutgoingCigiVersion
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CigiOutgoingMsg::ChangeOutgoingCigiVersion(CigiVersionID &Version)
{
   // Determine the buffer's Cigi Version
   if((Session != NULL) &&
      (Version.CigiMajorVersion > 0))
   {
      OutgoingVersion = Version;

      ClearHandlerTable();

      if(Session->IsHost())
      {
         if(OutgoingVersion.CigiMajorVersion >= 3)
            SetOutgoingHostV3Tbls();
         else if(OutgoingVersion.CigiMajorVersion == 2)
            SetOutgoingHostV2Tbls();
         else if(OutgoingVersion.CigiMajorVersion == 1)
            SetOutgoingHostV1Tbls();
      }
      else
      {
         if(OutgoingVersion.CigiMajorVersion >= 3)
            SetOutgoingIGV3Tbls();
         else if(OutgoingVersion.CigiMajorVersion == 2)
            SetOutgoingIGV2Tbls();
         else if(OutgoingVersion.CigiMajorVersion == 1)
            SetOutgoingIGV1Tbls();
      }
   }
}


// ================================================
// SetMostMatureReceivedCigiVersion
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CigiOutgoingMsg::SetMostMatureReceivedCigiVersion(CigiVersionID &Version)
{
   if(Version.IsKnownCigiVersion())
   {
      if(Version.GetCombinedCigiVersion() >
         MostMatureVersionReceived.GetCombinedCigiVersion())
      {
         MostMatureVersionReceived = Version;

         if(CrntFillBuf != NULL)
         {
            if((CrntFillBuf->Active) &&
               (!CrntFillBuf->DataPresent))
            {
               // Set the buffer's Cigi Version
               ChangeBufferCigiVersion();
            }
         }
      }
   }
}


// ================================================
// SetMostMatureReceivedCigiVersion
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiOutgoingMsg::SetOutgoingCigiVersion(CigiVersionID &Version,
                                            bool bndchk)
{
   int stat = CIGI_ERROR_WRONG_VERSION;

   if(Version.IsKnownCigiVersion())
   {
      MostMatureVersionReceived.SetCigiVersion(0,0);
      CmdVersionChng = true;
      CmdVersion = Version;
      stat = CIGI_SUCCESS;

      if(CrntFillBuf != NULL)
      {
         if((CrntFillBuf->Active) &&
            (!CrntFillBuf->DataPresent))
         {
            ChangeOutgoingCigiVersion(Version);

            // Set the buffer's Cigi Version
            ChangeBufferCigiVersion();

            CmdVersionChng = false;  // Already changed
         }
      }
   }

   return(stat);
}


// ================================================
// SetMostMatureReceivedCigiVersion
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CigiOutgoingMsg::ChangeBufferCigiVersion(void)
{

   // Set the buffer's Cigi Version
   CrntFillBuf->CigiVersion = OutgoingVersion;

   if(Session->IsHost())
   {
      if((OutgoingVersion.GetCombinedCigiVersion() >= 0x303))
         CrntFillBuf->PackIGCtrl = pIGCtrlPck[5];
      else if((OutgoingVersion.GetCombinedCigiVersion() == 0x302))
         CrntFillBuf->PackIGCtrl = pIGCtrlPck[4];
      else
         CrntFillBuf->PackIGCtrl =
            pIGCtrlPck[OutgoingVersion.CigiMajorVersion];

      int pSize = CrntFillBuf->PackIGCtrl->GetPacketSize();
      CrntFillBuf->BufferFillCnt = pSize;
      CrntFillBuf->FillBufferPos = CrntFillBuf->Buffer + pSize;

   }
   else
   {
      CrntFillBuf->PackSOF =
         pSOFPck[OutgoingVersion.CigiMajorVersion];
      if((OutgoingVersion.GetCombinedCigiVersion() >= 0x302))
         CrntFillBuf->PackSOF = pSOFPck[4];

      int pSize = CrntFillBuf->PackSOF->GetPacketSize();
      CrntFillBuf->BufferFillCnt = pSize;
      CrntFillBuf->FillBufferPos = CrntFillBuf->Buffer + pSize;

   }
}


// ================================================
// operator << - IG Control
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiOutgoingMsg & CigiOutgoingMsg::operator <<(CigiBaseIGCtrl &refPacket)
{
   CigiMessageBuffer *HostMsgBuf = NULL;

   if(Buffers.empty())
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(*this);
   }

   HostMsgBuf = *(Buffers.begin());
   if(HostMsgBuf == NULL)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(*this);
   }

   if(!HostMsgBuf->Active || HostMsgBuf->Locked)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(*this);
   }

   if(Session->IsHost())
   {
      if(HostMsgBuf->PackIGCtrl != NULL)
      {
         HostMsgBuf->PackIGCtrl->Pack(&refPacket,
                                    CrntFillBuf->Buffer,
                                    (void *)&OutgoingVersion);
         HostMsgBuf->ValidIGCtrlSOF = true;
         HostMsgBuf->DataPresent = true;
      }
   }

   return(*this);

}


// ================================================
// operator << - Start Of Frame
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiOutgoingMsg & CigiOutgoingMsg::operator <<(CigiBaseSOF &refPacket)
{
   CigiMessageBuffer *IgMsgBuf = NULL;

   if(Buffers.empty())
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(*this);
   }

   IgMsgBuf = *(Buffers.begin());

   if(IgMsgBuf == NULL)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(*this);
   }


   if(!IgMsgBuf->Active || IgMsgBuf->Locked)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(*this);
   }

   if(Session->IsIG())
   {
      if(IgMsgBuf->PackSOF != NULL)
      {
         IgMsgBuf->PackSOF->Pack(&refPacket,
                                 CrntFillBuf->Buffer,
                                 (void *)&OutgoingVersion);
         IgMsgBuf->ValidIGCtrlSOF = true;
         IgMsgBuf->DataPresent = true;
      }
   }

   return(*this);

}


// ================================================
// operator << - Entity Control
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiOutgoingMsg & CigiOutgoingMsg::operator <<(CigiBaseEntityCtrl &refPacket)
{
   CigiCnvtInfoType::Type Cnvt;
   refPacket.GetCnvt(OutgoingVersion,Cnvt);

   if(VldSnd[Cnvt.CnvtPacketID])
      PackObj(refPacket,*OutgoingHandlerTbl[Cnvt.CnvtPacketID],ATbl);

   return(*this);

}



// ================================================
// operator << - Environmental Control
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiOutgoingMsg & CigiOutgoingMsg::operator <<(CigiBaseEnvCtrl &refPacket)
{
   // Store data
   refPacket.FillHold(&EnvHoldObj);

   Cigi_uint8 FillVer = refPacket.GetVersion();

   if(FillVer >= 3)
   {
      // From V3 or above
      if(OutgoingVersion.CigiMajorVersion >= 3)
      {
         // To V3 and above
         if(VldSnd[refPacket.GetPacketID()])
         {
            PackObj(refPacket,
                  *OutgoingHandlerTbl[refPacket.GetPacketID()],
                  NULL);
         }
      }
      else
      {
         // To V1 or V2
         // Note: V1 & V2 use the same packet id number
         if(VldSnd[CIGI_ENV_CTRL_PACKET_ID_V2])
         {
            PackObj(EnvHoldObj,
                  *OutgoingHandlerTbl[CIGI_ENV_CTRL_PACKET_ID_V2],
                  NULL);
         }
      }
   }
   else
   {
      // From V1 or V2
      if(OutgoingVersion.CigiMajorVersion >= 3)
      {
         // To V3 or above
         // If CIGI_ATMOS_CTRL_PACKET_ID_V3 is valid to send
         //   CIGI_CELESTIAL_CTRL_PACKET_ID_V3 is also valid to send.
         if(VldSnd[CIGI_ATMOS_CTRL_PACKET_ID_V3])
         {
            PackObj(EnvHoldObj,
               *OutgoingHandlerTbl[CIGI_ATMOS_CTRL_PACKET_ID_V3],
               NULL);
            PackObj(EnvHoldObj,
               *OutgoingHandlerTbl[CIGI_CELESTIAL_CTRL_PACKET_ID_V3],
               NULL);
         }
      }
      else
      {
         // To V1 or V2
         // Note: V1 & V2 use the same packet id number
         if(VldSnd[refPacket.GetPacketID()])
         {
            PackObj(refPacket,
               *OutgoingHandlerTbl[refPacket.GetPacketID()],
               NULL);
         }
      }
   }

   return(*this);

}


// ================================================
// operator << - Variable Sized Packets
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiOutgoingMsg & CigiOutgoingMsg::operator <<(CigiBaseVariableSizePckt &refPacket)
{
   if(CrntFillBuf == NULL)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(*this);
   }


   if(!CrntFillBuf->Active || CrntFillBuf->Locked)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(*this);
   }

   CigiCnvtInfoType::Type Cnvt;
   refPacket.GetCnvt(OutgoingVersion,Cnvt);
   CigiBaseVariableSizePckt *PackingPacket =
      (CigiBaseVariableSizePckt *)OutgoingHandlerTbl[Cnvt.CnvtPacketID];

   if(VldSnd[Cnvt.CnvtPacketID])
   {
      // This gets the size of this variable sized packet
      int Size = PackingPacket->GetTruePacketSize(refPacket);

      if( Size > 0 )
      {
         // Check room left in current buffer
         if((BufferSize - CrntFillBuf->BufferFillCnt) < Size)
            AdvanceBuffer();

         int pSize = PackingPacket->Pack(&refPacket, CrntFillBuf->FillBufferPos, NULL);
         CrntFillBuf->BufferFillCnt += pSize;
         CrntFillBuf->FillBufferPos += pSize;
         CrntFillBuf->DataPresent = true;

      }
   }

   return(*this);

}



// ================================================
// operator << - Most Packets
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiOutgoingMsg & CigiOutgoingMsg::operator <<(CigiBasePacket &refBasePacket)
{
   CigiCnvtInfoType::Type Cnvt;
   refBasePacket.GetCnvt(OutgoingVersion,Cnvt);

   if((Cnvt.ProcID == CigiProcessType::ProcStd) ||
      (Cnvt.ProcID == CigiProcessType::TwoPassCnvtProcStd))
   {
      if(VldSnd[Cnvt.CnvtPacketID])
         PackObj(refBasePacket,*OutgoingHandlerTbl[Cnvt.CnvtPacketID],NULL);
   }
   else
   {
      CigiBaseEntityCtrl *pEnt;
      CigiBaseEnvCtrl *pEnv;
      CigiBaseIGCtrl *pIG;
      CigiBaseSOF *pSof;
      CigiBaseVariableSizePckt *pVSz;

      switch(Cnvt.ProcID)
      {
      case CigiProcessType::ProcShortArtPartToArtPart:
         if(VldSnd[Cnvt.CnvtPacketID])
         {
            CigiBaseShortArtPartCtrl *pSArtPart =
               (CigiBaseShortArtPartCtrl *)&refBasePacket;
            Cigi_uint8 ArtPartID1 = pSArtPart->GetArtPart1();
            Cigi_uint8 ArtPartID2 = pSArtPart->GetArtPart2();
            CigiArtPartCtrlV3 tArtPart;
            pSArtPart->SpecialConversion(OutgoingVersion,ArtPartID1,&tArtPart);
            PackObj(tArtPart,*OutgoingHandlerTbl[Cnvt.CnvtPacketID],NULL);
            if(ArtPartID2 != ArtPartID1)
            {
               CigiArtPartCtrlV3 tArtPart2;
               pSArtPart->SpecialConversion(OutgoingVersion,ArtPartID2,&tArtPart2);
               PackObj(tArtPart2,*OutgoingHandlerTbl[Cnvt.CnvtPacketID],NULL);
            }
         }
         break;
      case CigiProcessType::ProcEntity:
         pEnt = (CigiBaseEntityCtrl *)&refBasePacket;
         operator<<(*pEnt);
         break;
      case CigiProcessType::ProcEnvCtrl:
         pEnv = (CigiBaseEnvCtrl *)&refBasePacket;
         operator<<(*pEnv);
         break;
      case CigiProcessType::ProcIGCtrl:
         pIG = (CigiBaseIGCtrl *)&refBasePacket;
         operator<<(*pIG);
         break;
      case CigiProcessType::ProcSOF:
         pSof = (CigiBaseSOF *)&refBasePacket;
         operator<<(*pSof);
         break;
      case CigiProcessType::ProcVarSize:
         pVSz = (CigiBaseVariableSizePckt *)&refBasePacket;
         operator<<(*pVSz);
         break;
      default:
         break;
      }
   }

   return(*this);

}



// ================================================
// PackObj
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiOutgoingMsg &  CigiOutgoingMsg::PackObj(CigiBasePacket &DataPacket,
   CigiBasePacket &PackingPacket, void *Spec)
{
   if(CrntFillBuf == NULL)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(*this);
   }


   if(!CrntFillBuf->Active || CrntFillBuf->Locked)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(*this);
   }

   int Size = PackingPacket.GetPacketSize();

   if( Size > 0 )
   {
      // Check room left in current buffer
      if((BufferSize - CrntFillBuf->BufferFillCnt) < Size)
         AdvanceBuffer();

      int pSize = PackingPacket.Pack(&DataPacket, CrntFillBuf->FillBufferPos, Spec);
      CrntFillBuf->BufferFillCnt += pSize;
      CrntFillBuf->FillBufferPos += pSize;
      CrntFillBuf->DataPresent = true;

   }

   return(*this);

}


// ================================================
// UpdateFrameCntr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiOutgoingMsg::UpdateFrameCntr(void)
{
   int stat = CIGI_ERROR_UNEXPECTED_NULL;

   if(CrntMsgBuf == NULL)
   {
      if(!Buffers.empty())
      {
         CigiMessageBuffer *tBuf = *(Buffers.begin());
         if(tBuf->IsValidIGCtrlSOF())
         {
            Cigi_uint32 *tFrm = (Cigi_uint32 *)(tBuf->Buffer + 8);
            *tFrm = FrameCnt++;
            stat = CIGI_SUCCESS;
         }
      }
   }
   else
      stat = UpdateFrameCntr(CrntMsgBuf->Buffer);

   return(stat);
}

// ================================================
// UpdateFrameCntr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiOutgoingMsg::UpdateFrameCntr(Cigi_uint8 *OutgoingMsg, Cigi_uint8 *IncomingMsg)
{
   if((OutgoingMsg == NULL) ||
      (CrntMsgBuf == NULL) ||
      (Session == NULL))
      return(CIGI_ERROR_UNEXPECTED_NULL);

   if(!CrntMsgBuf->ValidIGCtrlSOF)
   {
      if(Session->IsHost())
      {
#ifndef CIGI_NO_EXCEPT
         throw CigiMissingIgControlException();
#endif
         return(CIGI_ERROR_MISSING_IG_CONTROL_PACKET);
      }
      else
      {
#ifndef CIGI_NO_EXCEPT
         throw CigiMissingStartOfFrameException();
#endif
         return(CIGI_ERROR_MISSING_SOF_PACKET);
      }
   }

   // Note: For all current CIGI Versions in the
   //  IG Control and Start-Of-Frame packets:
   //  The Packet ID is in the same location in the packets.
   //  The version ID is in the same location in the packets.
   //  The database ID is in the same location in the packets.
   //  The outgoing frame counter is in the same location in the packets.
   //  Unfortunately The minor version ID is in different locations

   // How the frame counter fields are filled is dependant
   //  on the cigi version of the outgoing message and the
   //  whether the outgoing message is a host or ig message
   //  and whether the system is running synchronized

   Cigi_uint32 *OutgoingMsgWord = (Cigi_uint32 *)OutgoingMsg;
   Cigi_uint32 *IncomingMsgWord = (Cigi_uint32 *)IncomingMsg;

   // Determine Outgoing CigiVersion
   //  and frame adjustment method
   bool FrameIncr = false;
   bool FrameRcvd = false;
   CigiVersionID OutVer;
   OutVer.CigiMajorVersion = (int) *(OutgoingMsg + 2);
   if(OutVer.CigiMajorVersion < 3)
   {
      if((Session->IsHost()) && (Session->IsSynchronous()))
         FrameRcvd = true;
      else
         FrameIncr = true;
   }
   else
   {
      int tVer = 0;
      if(Session->IsHost())
      {
         tVer = (int) *(OutgoingMsg + 4);
         if(Session->IsSynchronous())
            FrameRcvd = true;
         else
            FrameIncr = true;
      }
      else
      {
         tVer = (int) *(OutgoingMsg + 5);
         FrameIncr = true;
      }

      OutVer.CigiMinorVersion = (tVer >> 4) & 0x0f;

      if(OutVer.GetCombinedCigiVersion() >= 0x0302)
      {
         FrameIncr = true;
         FrameRcvd = true;
      }
   }

   int NewFrame = FrameCnt;
   int RcvdFrame = -1;
   if(FrameRcvd)
   {
      if(IncomingMsg == NULL)
         RcvdFrame = LastRcvdFrame;
      else
      {
         int InVer = (int) *(IncomingMsg + 2);
         if(InVer >= 3)
         {
            // Get Byte Swap
            Cigi_uint16 *IncomingMsgShort = (Cigi_uint16 *)IncomingMsg;
            if(*(IncomingMsgShort + 3) == 0x8000)
               RcvdFrame = *(IncomingMsgWord + 2);
            else
               CigiSwap4(&RcvdFrame,(IncomingMsgWord + 2));  // Swap RcvFrame
         }
         else
         {
            CIGI_SCOPY4(&RcvdFrame,(IncomingMsgWord + 2));
         }
      }

      if(!FrameIncr)
         NewFrame = RcvdFrame;
   }

   if(OutVer.CigiMajorVersion < 3)
      CIGI_SCOPY4((OutgoingMsgWord + 2),&NewFrame);
   else
   {
      *(OutgoingMsgWord + 2) = NewFrame;
      if(FrameIncr && FrameRcvd)
         *(OutgoingMsgWord + 4) = RcvdFrame;
   }

   FrameCnt++;  // increment frame count

   return(CIGI_SUCCESS);

}


// ================================================
// UpdateIGCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiOutgoingMsg::UpdateIGCtrl(Cigi_uint8 *OutgoingMsg, Cigi_uint8 *IncomingMsg)
{

   Cigi_int8 *OBufr = NULL;
   Cigi_int8 *IBufr = NULL;

   if(OutgoingMsg == NULL)
      return(CIGI_ERROR_UNEXPECTED_NULL);


   // Note: For all current CIGI Versions in the
   //  IG Control and Start-Of-Frame packets:
   //  The Packet ID is in the same location in the packets.
   //  The version ID is in the same location in the packets.
   //  The database ID is in the same location in the packets.
   //  The outgoing frame counter is in the same location in the packets.


   // Update the Frame Counter and possibly the Recieved Frame Counter
   UpdateFrameCntr(OutgoingMsg,IncomingMsg);


   // bounds check the Database ID
   OBufr = (Cigi_int8 *)(OutgoingMsg + 3);
   if(*OBufr < 0)
   {
      *OBufr = 0;  // Host sent Database IDs should never be negative
   }

   return(CIGI_SUCCESS);

}


// ================================================
// LockMsg
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiOutgoingMsg::LockMsg()
{
   // Check for a buffer in the Active Buffers list
   if(Buffers.empty())
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(CIGI_ERROR_CALLED_OUT_OF_SEQUENCE);
   }

   // Verify there are no buffers locked for transmission
   if(CrntMsgBuf != NULL)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(CIGI_ERROR_CALLED_OUT_OF_SEQUENCE);
   }

   // Get the next buffer for transmission
   //  And verify the Session is valid
   CigiMessageBuffer *ChkMsgBuf = *(Buffers.begin());
   if((ChkMsgBuf == NULL) || (Session == NULL))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiNullPointerException();
#endif
      return(CIGI_ERROR_UNEXPECTED_NULL);
   }

   if(!ChkMsgBuf->ValidIGCtrlSOF)
   {
      if(Session->IsHost())
      {
#ifndef CIGI_NO_EXCEPT
         throw CigiMissingIgControlException();
#endif
         return(CIGI_ERROR_MISSING_IG_CONTROL_PACKET);
      }
      else
      {
#ifndef CIGI_NO_EXCEPT
         throw CigiMissingStartOfFrameException();
#endif
         return(CIGI_ERROR_MISSING_SOF_PACKET);
      }
   }

   if( !ChkMsgBuf->Active || ChkMsgBuf->Locked )
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(CIGI_ERROR_CALLED_OUT_OF_SEQUENCE);
   }

   CrntMsgBuf = ChkMsgBuf;
   CrntMsgBuf->Locked = true;

   // Set up next fill buffer the CrntFillBuf and the
   //  CrntMsgBuf are the same.
   if(CrntFillBuf == CrntMsgBuf)
      AdvanceBuffer();

   return(CIGI_SUCCESS);

}


// ================================================
// GetMsg
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Cigi_uint8 * CigiOutgoingMsg::GetMsg(int &length)
{
   Cigi_uint8 * Buff = NULL;
   length = 0;

   // Verify that a buffer is ready for transmission
   if(CrntMsgBuf != NULL)
   {
      if((CrntMsgBuf->Active) && (CrntMsgBuf->Locked))
      {
         Buff = CrntMsgBuf->Buffer;
         length = CrntMsgBuf->BufferFillCnt;
      }
   }

   return(Buff);
}


// ================================================
// GetBuffer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Cigi_uint8 * CigiOutgoingMsg::GetBuffer()
{
   Cigi_uint8 * Buff = NULL;

   // Verify that a buffer is ready for transmission
   if(CrntMsgBuf != NULL)
   {
      if((CrntMsgBuf->Active) && (CrntMsgBuf->Locked))
         Buff = CrntMsgBuf->Buffer;
   }

   return(Buff);
}


// ================================================
// GetMsgLength
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiOutgoingMsg::GetMsgLength()
{
   int length = 0;

   // Verify that a buffer is ready for transmission
   if(CrntMsgBuf != NULL)
   {
      if((CrntMsgBuf->Active) && (CrntMsgBuf->Locked))
         length = CrntMsgBuf->BufferFillCnt;
   }

   return(length);
}


// ================================================
// GetBufferString
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
std::string CigiOutgoingMsg::GetBufferString(void)
{
   Cigi_uint8 *buffer = this->GetBuffer();
   int size = this->GetMsgLength();
   return std::string(buffer,buffer+size);
}


// ================================================
// UnlockMsg
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiOutgoingMsg::UnlockMsg()
{
   // Verify there is a current message buffer
   if(CrntMsgBuf == NULL)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(CIGI_ERROR_CALLED_OUT_OF_SEQUENCE);
   }

   // Verify that the buffer is active and locked
   if(!CrntMsgBuf->Active || !CrntMsgBuf->Locked)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(CIGI_ERROR_CALLED_OUT_OF_SEQUENCE);
   }

   // Reset and clear the current buffer
   CrntMsgBuf->Active = false;
   CrntMsgBuf->BufferFillCnt = 0;
   CrntMsgBuf->DataPresent = false;
   CrntMsgBuf->FillBufferPos = CrntMsgBuf->Buffer;
   CrntMsgBuf->Locked = false;
   CrntMsgBuf->ValidIGCtrlSOF = false;

   // Clear this buffer from the Active Buffers list
   if(!Buffers.empty())
   {
      if(CrntMsgBuf == *(Buffers.begin()))
         Buffers.pop_front();
      else
      {
         list<CigiMessageBuffer *>::iterator iBuf;
         for(iBuf=Buffers.begin();iBuf!=Buffers.end();iBuf++)
         {
            if(*iBuf == CrntMsgBuf)
            {
               iBuf = Buffers.erase(iBuf);
               break;
            }
         }
      }
   }

   // Add this buffer to the Availible Buffers list
   AvailBuff.push_back(CrntMsgBuf);

   // Clear the Current Message Buffer
   CrntMsgBuf = NULL;

   return(CIGI_SUCCESS);

}


// ================================================
// PackageMsg
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiOutgoingMsg::PackageMsg(Cigi_uint8 **Msg, int &length)
{
   *Msg = NULL;
   length = 0;

   if(PackagedMsg != NULL)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(CIGI_ERROR_CALLED_OUT_OF_SEQUENCE);
   }

   int stat = LockMsg();

   if(stat == CIGI_SUCCESS)
   {
      *Msg = GetMsg(length);
      if(*Msg != NULL)
         PackagedMsg = CrntMsgBuf;
      else
         stat = CIGI_ERROR_CALLED_OUT_OF_SEQUENCE;
   }

   return(stat);

}

// ================================================
// FreeMsg
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiOutgoingMsg::FreeMsg(void)
{
   if(PackagedMsg == NULL)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiCalledOutOfSequenceException();
#endif
      return(CIGI_ERROR_CALLED_OUT_OF_SEQUENCE);
   }

   int stat = UnlockMsg();

   PackagedMsg = NULL;

   return(stat);
}



// ================================================
// Reset
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiOutgoingMsg::Reset(void)
{
   CrntMsgBuf = NULL;
   PackagedMsg = NULL;
   CrntFillBuf = NULL;

   list<CigiMessageBuffer *>::iterator iBuf;
   for(iBuf=Buffers.begin();iBuf!=Buffers.end();iBuf++)
   {
      (*iBuf)->Active = false;
      (*iBuf)->BufferFillCnt = 0;
      (*iBuf)->DataPresent = false;
      (*iBuf)->FillBufferPos = (*iBuf)->Buffer;
      (*iBuf)->Locked = false;
      (*iBuf)->ValidIGCtrlSOF = false;

      AvailBuff.push_back(*iBuf);
   }

   Buffers.clear();

   return(CIGI_SUCCESS);

}


// ================================================
// RegisterUserPacket
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiOutgoingMsg::RegisterUserPacket(CigiBasePacket *Packet,
                                        Cigi_uint8 PacketID,
                                        bool HostSend,
                                        bool IGSend)
{
   int stat = CIGI_ERROR_INVALID_USER_PACKET;
   if(((PacketID > 199) && (PacketID <= 255)) &&
      (Packet != NULL) &&
      ((HostSend && Session->IsHost()) ||
       (IGSend && Session->IsIG())))
   {
      OutgoingHandlerTbl[PacketID] = Packet;
      VldSnd[PacketID] = true;
      stat = CIGI_SUCCESS;
   }

   return(stat);
}


// ================================================
// ClearHandlerTable
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CigiOutgoingMsg::ClearHandlerTable()
{
      for(int ndx=0;ndx<200;ndx++)
      {
         if(OutgoingHandlerTbl[ndx] != (CigiBasePacket *)&DefaultPacket)
         {
            delete OutgoingHandlerTbl[ndx];
            OutgoingHandlerTbl[ndx] = &DefaultPacket;
         }
         VldSnd[ndx] = false;
      }
}


// ================================================
// SetOutgoingV1Tbls
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CigiOutgoingMsg::SetOutgoingHostV1Tbls(void)
{

   OutgoingHandlerTbl[CIGI_IG_CTRL_PACKET_ID_V1] = (CigiBasePacket *) new CigiIGCtrlV1;
   VldSnd[CIGI_IG_CTRL_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_ENTITY_CTRL_PACKET_ID_V1] = (CigiBasePacket *) new CigiEntityCtrlV1;
   VldSnd[CIGI_ENTITY_CTRL_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_VIEW_DEF_PACKET_ID_V1] = (CigiBasePacket *) new CigiViewDefV1;
   VldSnd[CIGI_VIEW_DEF_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_VIEW_CTRL_PACKET_ID_V1] = (CigiBasePacket *) new CigiViewCtrlV1;
   VldSnd[CIGI_VIEW_CTRL_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_RATE_CTRL_PACKET_ID_V1] = (CigiBasePacket *) new CigiRateCtrlV1;
   VldSnd[CIGI_RATE_CTRL_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_SENSOR_CTRL_PACKET_ID_V1] = (CigiBasePacket *) new CigiSensorCtrlV1;
   VldSnd[CIGI_SENSOR_CTRL_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_TRAJECTORY_DEF_PACKET_ID_V1] = (CigiBasePacket *) new CigiTrajectoryDefV1;
   VldSnd[CIGI_TRAJECTORY_DEF_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_WEATHER_CTRL_PACKET_ID_V1] = (CigiBasePacket *) new CigiWeatherCtrlV1;
   VldSnd[CIGI_WEATHER_CTRL_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_COLL_DET_SEG_DEF_PACKET_ID_V1] = (CigiBasePacket *) new CigiCollDetSegDefV1;
   VldSnd[CIGI_COLL_DET_SEG_DEF_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_LOS_SEG_REQ_PACKET_ID_V1] = (CigiBasePacket *) new CigiLosSegReqV1;
   VldSnd[CIGI_LOS_SEG_REQ_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_LOS_VECT_REQ_PACKET_ID_V1] = (CigiBasePacket *) new CigiLosVectReqV1;
   VldSnd[CIGI_LOS_VECT_REQ_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_HAT_REQ_PACKET_ID_V1] = (CigiBasePacket *) new CigiHatReqV1;
   VldSnd[CIGI_HAT_REQ_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_ENV_CTRL_PACKET_ID_V1] = (CigiBasePacket *) new CigiEnvCtrlV1;
   VldSnd[CIGI_ENV_CTRL_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_SPEC_EFF_DEF_PACKET_ID_V1] = (CigiBasePacket *) new CigiSpecEffDefV1;
   VldSnd[CIGI_SPEC_EFF_DEF_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_ART_PART_CTRL_PACKET_ID_V1] = (CigiBasePacket *) new CigiArtPartCtrlV1;
   VldSnd[CIGI_ART_PART_CTRL_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_COMP_CTRL_PACKET_ID_V1] = (CigiBasePacket *) new CigiCompCtrlV1;
   VldSnd[CIGI_COMP_CTRL_PACKET_ID_V1] = true;

}


// ================================================
// SetOutgoingV1Tbls
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CigiOutgoingMsg::SetOutgoingIGV1Tbls(void)
{

   OutgoingHandlerTbl[CIGI_SOF_PACKET_ID_V1] = (CigiBasePacket *) new CigiSOFV1;
   VldSnd[CIGI_SOF_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_COLL_DET_SEG_RESP_PACKET_ID_V1] = (CigiBasePacket *) new CigiCollDetSegRespV1;
   VldSnd[CIGI_COLL_DET_SEG_RESP_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_SENSOR_RESP_PACKET_ID_V1] = (CigiBasePacket *) new CigiSensorRespV1;
   VldSnd[CIGI_SENSOR_RESP_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_LOS_RESP_PACKET_ID_V1] = (CigiBasePacket *) new CigiLosRespV1;
   VldSnd[CIGI_LOS_RESP_PACKET_ID_V1] = true;
   OutgoingHandlerTbl[CIGI_HAT_RESP_PACKET_ID_V1] = (CigiBasePacket *) new CigiHatRespV1;
   VldSnd[CIGI_HAT_RESP_PACKET_ID_V1] = true;

}


// ================================================
// SetOutgoingV2Tbls
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CigiOutgoingMsg::SetOutgoingHostV2Tbls(void)
{

   OutgoingHandlerTbl[CIGI_IG_CTRL_PACKET_ID_V2] = (CigiBasePacket *) new CigiIGCtrlV2;
   VldSnd[CIGI_IG_CTRL_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_ENTITY_CTRL_PACKET_ID_V2] = (CigiBasePacket *) new CigiEntityCtrlV2;
   VldSnd[CIGI_ENTITY_CTRL_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_VIEW_DEF_PACKET_ID_V2] = (CigiBasePacket *) new CigiViewDefV2;
   VldSnd[CIGI_VIEW_DEF_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_VIEW_CTRL_PACKET_ID_V2] = (CigiBasePacket *) new CigiViewCtrlV2;
   VldSnd[CIGI_VIEW_CTRL_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_RATE_CTRL_PACKET_ID_V2] = (CigiBasePacket *) new CigiRateCtrlV2;
   VldSnd[CIGI_RATE_CTRL_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_SENSOR_CTRL_PACKET_ID_V2] = (CigiBasePacket *) new CigiSensorCtrlV2;
   VldSnd[CIGI_SENSOR_CTRL_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_TRAJECTORY_DEF_PACKET_ID_V2] = (CigiBasePacket *) new CigiTrajectoryDefV2;
   VldSnd[CIGI_TRAJECTORY_DEF_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_WEATHER_CTRL_PACKET_ID_V2] = (CigiBasePacket *) new CigiWeatherCtrlV2;
   VldSnd[CIGI_WEATHER_CTRL_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_COLL_DET_SEG_DEF_PACKET_ID_V2] = (CigiBasePacket *) new CigiCollDetSegDefV2;
   VldSnd[CIGI_COLL_DET_SEG_DEF_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_LOS_SEG_REQ_PACKET_ID_V2] = (CigiBasePacket *) new CigiLosSegReqV2;
   VldSnd[CIGI_LOS_SEG_REQ_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_LOS_VECT_REQ_PACKET_ID_V2] = (CigiBasePacket *) new CigiLosVectReqV2;
   VldSnd[CIGI_LOS_VECT_REQ_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_HAT_REQ_PACKET_ID_V2] = (CigiBasePacket *) new CigiHatReqV2;
   VldSnd[CIGI_HAT_REQ_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_HOT_REQ_PACKET_ID_V2] = (CigiBasePacket *) new CigiHotReqV2;
   VldSnd[CIGI_HOT_REQ_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_ENV_CTRL_PACKET_ID_V2] = (CigiBasePacket *) new CigiEnvCtrlV2;
   VldSnd[CIGI_ENV_CTRL_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_SPEC_EFF_DEF_PACKET_ID_V2] = (CigiBasePacket *) new CigiSpecEffDefV2;
   VldSnd[CIGI_SPEC_EFF_DEF_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_ART_PART_CTRL_PACKET_ID_V2] = (CigiBasePacket *) new CigiArtPartCtrlV2;
   VldSnd[CIGI_ART_PART_CTRL_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_COLL_DET_VOL_DEF_PACKET_ID_V2] = (CigiBasePacket *) new CigiCollDetVolDefV2;
   VldSnd[CIGI_COLL_DET_VOL_DEF_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_COMP_CTRL_PACKET_ID_V2] = (CigiBasePacket *) new CigiCompCtrlV2;
   VldSnd[CIGI_COMP_CTRL_PACKET_ID_V2] = true;

}


// ================================================
// SetOutgoingV2Tbls
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CigiOutgoingMsg::SetOutgoingIGV2Tbls(void)
{

   OutgoingHandlerTbl[CIGI_SOF_PACKET_ID_V2] = (CigiBasePacket *) new CigiSOFV2;
   VldSnd[CIGI_SOF_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_COLL_DET_SEG_RESP_PACKET_ID_V2] = (CigiBasePacket *) new CigiCollDetSegRespV2;
   VldSnd[CIGI_COLL_DET_SEG_RESP_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_SENSOR_RESP_PACKET_ID_V2] = (CigiBasePacket *) new CigiSensorRespV2;
   VldSnd[CIGI_SENSOR_RESP_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_LOS_RESP_PACKET_ID_V2] = (CigiBasePacket *) new CigiLosRespV2;
   VldSnd[CIGI_LOS_RESP_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_HAT_RESP_PACKET_ID_V2] = (CigiBasePacket *) new CigiHatRespV2;
   VldSnd[CIGI_HAT_RESP_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_HOT_RESP_PACKET_ID_V2] = (CigiBasePacket *) new CigiHotRespV2;
   VldSnd[CIGI_HOT_RESP_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_COLL_DET_VOL_RESP_PACKET_ID_V2] = (CigiBasePacket *) new CigiCollDetVolRespV2;
   VldSnd[CIGI_COLL_DET_VOL_RESP_PACKET_ID_V2] = true;
   OutgoingHandlerTbl[CIGI_IG_MSG_PACKET_ID_V2] = (CigiBasePacket *) new CigiIGMsgV2;
   VldSnd[CIGI_IG_MSG_PACKET_ID_V2] = true;

}


// ================================================
// SetOutgoingV3Tbls
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CigiOutgoingMsg::SetOutgoingHostV3Tbls(void)
{

   if(OutgoingVersion.CigiMinorVersion >= 2)
   {
      if(OutgoingVersion.CigiMinorVersion >= 3)
      {
         OutgoingHandlerTbl[CIGI_IG_CTRL_PACKET_ID_V3_3] = (CigiBasePacket *) new CigiIGCtrlV3_3;
         VldSnd[CIGI_IG_CTRL_PACKET_ID_V3_3] = true;
         OutgoingHandlerTbl[CIGI_ENTITY_CTRL_PACKET_ID_V3_3] = (CigiBasePacket *) new CigiEntityCtrlV3_3;
         VldSnd[CIGI_ENTITY_CTRL_PACKET_ID_V3_3] = true;
         OutgoingHandlerTbl[CIGI_SYMBOL_SURFACE_DEF_PACKET_ID_V3_3] = (CigiBasePacket *) new CigiSymbolSurfaceDefV3_3;
         VldSnd[CIGI_SYMBOL_SURFACE_DEF_PACKET_ID_V3_3] = true;
         OutgoingHandlerTbl[CIGI_SYMBOL_CONTROL_PACKET_ID_V3_3] = (CigiBasePacket *) new CigiSymbolCtrlV3_3;
         VldSnd[CIGI_SYMBOL_CONTROL_PACKET_ID_V3_3] = true;
         OutgoingHandlerTbl[CIGI_SHORT_SYMBOL_CONTROL_PACKET_ID_V3_3] = (CigiBasePacket *) new CigiShortSymbolCtrlV3_3;
         VldSnd[CIGI_SHORT_SYMBOL_CONTROL_PACKET_ID_V3_3] = true;
         OutgoingHandlerTbl[CIGI_SYMBOL_TEXT_DEFINITION_PACKET_ID_V3_3] = (CigiBasePacket *) new CigiSymbolTextDefV3_3;
         VldSnd[CIGI_SYMBOL_TEXT_DEFINITION_PACKET_ID_V3_3] = true;
         OutgoingHandlerTbl[CIGI_SYMBOL_CIRCLE_DEFINITION_PACKET_ID_V3_3] = (CigiBasePacket *) new CigiSymbolCircleDefV3_3;
         VldSnd[CIGI_SYMBOL_CIRCLE_DEFINITION_PACKET_ID_V3_3] = true;
         OutgoingHandlerTbl[CIGI_SYMBOL_LINE_DEFINITION_PACKET_ID_V3_3] = (CigiBasePacket *) new CigiSymbolLineDefV3_3;
         VldSnd[CIGI_SYMBOL_LINE_DEFINITION_PACKET_ID_V3_3] = true;
         OutgoingHandlerTbl[CIGI_SYMBOL_CLONE_PACKET_ID_V3_3] = (CigiBasePacket *) new CigiSymbolCloneV3_3;
         VldSnd[CIGI_SYMBOL_CLONE_PACKET_ID_V3_3] = true;
         OutgoingHandlerTbl[CIGI_COMP_CTRL_PACKET_ID_V3_3] = (CigiBasePacket *) new CigiCompCtrlV3_3;
         VldSnd[CIGI_COMP_CTRL_PACKET_ID_V3_3] = true;
         OutgoingHandlerTbl[CIGI_SHORT_COMP_CTRL_PACKET_ID_V3_3] = (CigiBasePacket *) new CigiShortCompCtrlV3_3;
         VldSnd[CIGI_SHORT_COMP_CTRL_PACKET_ID_V3_3] = true;
      }
      else
      {
         OutgoingHandlerTbl[CIGI_IG_CTRL_PACKET_ID_V3_2] = (CigiBasePacket *) new CigiIGCtrlV3_2;
         VldSnd[CIGI_IG_CTRL_PACKET_ID_V3_2] = true;
         OutgoingHandlerTbl[CIGI_ENTITY_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiEntityCtrlV3;
         VldSnd[CIGI_ENTITY_CTRL_PACKET_ID_V3] = true;
         OutgoingHandlerTbl[CIGI_COMP_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiCompCtrlV3;
         VldSnd[CIGI_COMP_CTRL_PACKET_ID_V3] = true;
         OutgoingHandlerTbl[CIGI_SHORT_COMP_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiShortCompCtrlV3;
         VldSnd[CIGI_SHORT_COMP_CTRL_PACKET_ID_V3] = true;
      }
      OutgoingHandlerTbl[CIGI_RATE_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiRateCtrlV3_2;
      VldSnd[CIGI_RATE_CTRL_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_HAT_HOT_REQ_PACKET_ID_V3] = (CigiBasePacket *) new CigiHatHotReqV3_2;
      VldSnd[CIGI_HAT_HOT_REQ_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_LOS_SEG_REQ_PACKET_ID_V3] = (CigiBasePacket *) new CigiLosSegReqV3_2;
      VldSnd[CIGI_LOS_SEG_REQ_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_LOS_VECT_REQ_PACKET_ID_V3] = (CigiBasePacket *) new CigiLosVectReqV3_2;
      VldSnd[CIGI_LOS_VECT_REQ_PACKET_ID_V3] = true;
   }
   else
   {
      OutgoingHandlerTbl[CIGI_IG_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiIGCtrlV3;
      VldSnd[CIGI_IG_CTRL_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_ENTITY_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiEntityCtrlV3;
      VldSnd[CIGI_ENTITY_CTRL_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_RATE_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiRateCtrlV3;
      VldSnd[CIGI_RATE_CTRL_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_HAT_HOT_REQ_PACKET_ID_V3] = (CigiBasePacket *) new CigiHatHotReqV3;
      VldSnd[CIGI_HAT_HOT_REQ_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_LOS_SEG_REQ_PACKET_ID_V3] = (CigiBasePacket *) new CigiLosSegReqV3;
      VldSnd[CIGI_LOS_SEG_REQ_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_LOS_VECT_REQ_PACKET_ID_V3] = (CigiBasePacket *) new CigiLosVectReqV3;
      VldSnd[CIGI_LOS_VECT_REQ_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_COMP_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiCompCtrlV3;
      VldSnd[CIGI_COMP_CTRL_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_SHORT_COMP_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiShortCompCtrlV3;
      VldSnd[CIGI_SHORT_COMP_CTRL_PACKET_ID_V3] = true;
   }

   OutgoingHandlerTbl[CIGI_VIEW_DEF_PACKET_ID_V3] = (CigiBasePacket *) new CigiViewDefV3;
   VldSnd[CIGI_VIEW_DEF_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_VIEW_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiViewCtrlV3;
   VldSnd[CIGI_VIEW_CTRL_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_SENSOR_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiSensorCtrlV3;
   VldSnd[CIGI_SENSOR_CTRL_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_TRAJECTORY_DEF_PACKET_ID_V3] = (CigiBasePacket *) new CigiTrajectoryDefV3;
   VldSnd[CIGI_TRAJECTORY_DEF_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_WEATHER_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiWeatherCtrlV3;
   VldSnd[CIGI_WEATHER_CTRL_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_COLL_DET_SEG_DEF_PACKET_ID_V3] = (CigiBasePacket *) new CigiCollDetSegDefV3;
   VldSnd[CIGI_COLL_DET_SEG_DEF_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_ATMOS_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiAtmosCtrlV3;
   VldSnd[CIGI_ATMOS_CTRL_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_CELESTIAL_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiCelestialCtrlV3;
   VldSnd[CIGI_CELESTIAL_CTRL_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_ART_PART_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiArtPartCtrlV3;
   VldSnd[CIGI_ART_PART_CTRL_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_COLL_DET_VOL_DEF_PACKET_ID_V3] = (CigiBasePacket *) new CigiCollDetVolDefV3;
   VldSnd[CIGI_COLL_DET_VOL_DEF_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_SHORT_ART_PART_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiShortArtPartCtrlV3;
   VldSnd[CIGI_SHORT_ART_PART_CTRL_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_CONF_CLAMP_ENTITY_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiConfClampEntityCtrlV3;
   VldSnd[CIGI_CONF_CLAMP_ENTITY_CTRL_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_MARITIME_SURFACE_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiMaritimeSurfaceCtrlV3;
   VldSnd[CIGI_MARITIME_SURFACE_CTRL_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_ENV_RGN_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiEnvRgnCtrlV3;
   VldSnd[CIGI_ENV_RGN_CTRL_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_TERRESTRIAL_SURFACE_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiTerrestrialSurfaceCtrlV3;
   VldSnd[CIGI_TERRESTRIAL_SURFACE_CTRL_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_MOTION_TRACK_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiMotionTrackCtrlV3;
   VldSnd[CIGI_MOTION_TRACK_CTRL_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_EARTH_MODEL_DEF_PACKET_ID_V3] = (CigiBasePacket *) new CigiEarthModelDefV3;
   VldSnd[CIGI_EARTH_MODEL_DEF_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_POSITION_REQ_PACKET_ID_V3] = (CigiBasePacket *) new CigiPositionReqV3;
   VldSnd[CIGI_POSITION_REQ_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_ENV_COND_REQ_PACKET_ID_V3] = (CigiBasePacket *) new CigiEnvCondReqV3;
   VldSnd[CIGI_ENV_COND_REQ_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_WAVE_CTRL_PACKET_ID_V3] = (CigiBasePacket *) new CigiWaveCtrlV3;
   VldSnd[CIGI_WAVE_CTRL_PACKET_ID_V3] = true;

}


// ================================================
// SetOutgoingV3Tbls
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CigiOutgoingMsg::SetOutgoingIGV3Tbls(void)
{

   if(OutgoingVersion.CigiMinorVersion >= 2)
   {
      OutgoingHandlerTbl[CIGI_SOF_PACKET_ID_V3] = (CigiBasePacket *) new CigiSOFV3_2;
      VldSnd[CIGI_SOF_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_LOS_RESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiLosRespV3_2;
      VldSnd[CIGI_LOS_RESP_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_LOS_XRESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiLosXRespV3_2;
      VldSnd[CIGI_LOS_XRESP_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_HAT_HOT_RESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiHatHotRespV3_2;
      VldSnd[CIGI_HAT_HOT_RESP_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_HAT_HOT_XRESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiHatHotXRespV3_2;
      VldSnd[CIGI_HAT_HOT_XRESP_PACKET_ID_V3] = true;
   }
   else
   {
      OutgoingHandlerTbl[CIGI_SOF_PACKET_ID_V3] = (CigiBasePacket *) new CigiSOFV3;
      VldSnd[CIGI_SOF_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_LOS_RESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiLosRespV3;
      VldSnd[CIGI_LOS_RESP_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_LOS_XRESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiLosXRespV3;
      VldSnd[CIGI_LOS_XRESP_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_HAT_HOT_RESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiHatHotRespV3;
      VldSnd[CIGI_HAT_HOT_RESP_PACKET_ID_V3] = true;
      OutgoingHandlerTbl[CIGI_HAT_HOT_XRESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiHatHotXRespV3;
      VldSnd[CIGI_HAT_HOT_XRESP_PACKET_ID_V3] = true;
   }

   OutgoingHandlerTbl[CIGI_COLL_DET_SEG_RESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiCollDetSegRespV3;
   VldSnd[CIGI_COLL_DET_SEG_RESP_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_SENSOR_RESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiSensorRespV3;
   VldSnd[CIGI_SENSOR_RESP_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_SENSOR_XRESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiSensorXRespV3;
   VldSnd[CIGI_SENSOR_XRESP_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_COLL_DET_VOL_RESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiCollDetVolRespV3;
   VldSnd[CIGI_COLL_DET_VOL_RESP_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_POSITION_RESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiPositionRespV3;
   VldSnd[CIGI_POSITION_RESP_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_WEATHER_COND_RESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiWeatherCondRespV3;
   VldSnd[CIGI_WEATHER_COND_RESP_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_AEROSOL_RESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiAerosolRespV3;
   VldSnd[CIGI_AEROSOL_RESP_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_MARITIME_SURFACE_RESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiMaritimeSurfaceRespV3;
   VldSnd[CIGI_MARITIME_SURFACE_RESP_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_TERRESTRIAL_SURFACE_RESP_PACKET_ID_V3] = (CigiBasePacket *) new CigiTerrestrialSurfaceRespV3;
   VldSnd[CIGI_TERRESTRIAL_SURFACE_RESP_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_ANIMATION_STOP_PACKET_ID_V3] = (CigiBasePacket *) new CigiAnimationStopV3;
   VldSnd[CIGI_ANIMATION_STOP_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_EVENT_NOTIFICATION_PACKET_ID_V3] = (CigiBasePacket *) new CigiEventNotificationV3;
   VldSnd[CIGI_EVENT_NOTIFICATION_PACKET_ID_V3] = true;
   OutgoingHandlerTbl[CIGI_IG_MSG_PACKET_ID_V3] = (CigiBasePacket *) new CigiIGMsgV3;
   VldSnd[CIGI_IG_MSG_PACKET_ID_V3] = true;

}


