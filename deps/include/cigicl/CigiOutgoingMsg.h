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
 *  FILENAME:   CigiOutgoingMsg.h
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
 *  Added the CIGI_SPEC modifier to the class declaration for exporting the 
 *  class in a Windows DLL.
 *  
 *  02/20/2006 Greg Basler                       Version 1.6.0
 *  Added the PackageMsg, GetCurrentlyPackagedMsg, and FreeMsg methods to
 *  better update the frame the frame counter in the IG_Ctrl packet for hosts
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Granted friend access to the CigiVersionJumpTable class.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added an overloaded UpdateSOF method without the LastIGCtrl parameter to 
 *  update the the start of frame packet's frame counter.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Deprecated the existing UpdateSOF method and modified it to call the new 
 *  implementation.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added a protected Reset method to reset the buffer system.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added the CurrentIGCtrl member variable to store the last IGCtrl packed 
 *  into a message.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added the CurrentSOF member variable to store the last SOF packed into a 
 *  message.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 * </pre>
 *  Author: The Boeing Company
 *
 */

#if !defined(_CIGI_OUTGOING_MESSAGE_INCLUDED_)
#define _CIGI_OUTGOING_MESSAGE_INCLUDED_

#include <string>
#include <list>

#include "CigiMessage.h"
#include "CigiBasePacket.h"
#include "CigiBaseEntityCtrl.h"
#include "CigiBaseIGCtrl.h"
#include "CigiBaseSOF.h"
#include "CigiMessageBuffer.h"
#include "CigiBaseVariableSizePckt.h"
#include "CigiDefaultPacket.h"

class CigiSession;

//=========================================================
//! The class for the outgoing message
//!
class CIGI_SPEC CigiOutgoingMsg : public CigiMessage  
{

   friend class CigiSession;

public:
   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiOutgoingMsg();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiOutgoingMsg();





   //==> Buffer Packing Operators with References

   //=========================================================
   //! Packs the buffer with the specifed IG Control packet.  
   //! \param refPacket - A reference to an IG Control packet.
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & operator <<(CigiBaseIGCtrl &refPacket);

   //=========================================================
   //! Non operator-overloaded pack method for scripting.
   //! \param refPacket - A reference to an IG Control packet.
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   inline CigiOutgoingMsg & pack(CigiBaseIGCtrl &refPacket)
   {
     return (*this) << refPacket;
   }

   //=========================================================
   //! Packs the buffer with the specifed Start Of Frame packet.  
   //! \param refPacket - A reference to an Start Of Frame packet.
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & operator <<(CigiBaseSOF &refPacket);

   //=========================================================
   //! Non operator-overloaded pack method for scripting.
   //! \param refPacket - A reference to an Start Of Frame packet.
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & pack(CigiBaseSOF &refPacket)
   {
     return (*this) << refPacket;
   }

   //=========================================================
   //! Packs the buffer with the specifed Entity Control packet.  
   //! \param refPacket - A reference to an Entity Control packet.
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & operator <<(CigiBaseEntityCtrl &refPacket);

   //=========================================================
   //! Non operator-overloaded pack method for scripting.
   //! \param refPacket - A reference to an Entity Control packet.
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & pack(CigiBaseEntityCtrl &refPacket)
   {
     return (*this) << refPacket;
   }

   //=========================================================
   //! Packs the buffer with the specifed packet.  
   //! \param refPacket - A reference to an Enviroment type
   //!    Control packet.  This includes Celestial and Atmosphere Ctrl
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & operator <<(CigiBaseEnvCtrl &refPacket);

   //=========================================================
   //! Non operator-overloaded pack method for scripting.
   //! \param refPacket - A reference to an Enviroment type
   //!    Control packet.  This includes Celestial and Atmosphere Ctrl
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & pack(CigiBaseEnvCtrl &refPacket)
   {
     return (*this) << refPacket;
   }

   //=========================================================
   //! Packs the buffer with the specifed packet.  
   //! \param refPacket - A reference to a variable size packet
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & operator <<(CigiBaseVariableSizePckt &refPacket);

   //=========================================================
   //! Non operator-overloaded pack method for scripting.
   //! \param refPacket - A reference to a variable size packet
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & pack(CigiBaseVariableSizePckt &refPacket)
   {
     return (*this) << refPacket;
   }

   //=========================================================
   //! Packs the buffer with the pointer to the specifed packet.  
   //! \param refBasePacket - A reference to a base packet.
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & operator <<(CigiBasePacket &refBasePacket);

   //=========================================================
   //! Non operator-overloaded pack method for scripting.
   //! \param refBasePacket - A reference to a base packet.
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & pack(CigiBasePacket &refBasePacket)
   {
     return (*this) << refBasePacket;
   }


   //==> Buffer Packing Operators with Pointers

   //=========================================================
   //! Packs the buffer with the pointer to the specifed IG Control packet.  
   //! \param refPacket - A pointer to an IG Control packet.
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & operator <<(CigiBaseIGCtrl *refPacket)
   {
      return(operator<<(*refPacket));
   }

   //=========================================================
   //! Non operator-overloaded pack method for scripting.
   //! \param refPacket - A pointer to an IG Control packet.
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & pack(CigiBaseIGCtrl *refPacket)
   {
     return this->pack(*refPacket);
   }

   //=========================================================
   //! Packs the buffer with the pointer to the specifed Start Of Frame packet.  
   //! \param refPacket - A pointer to an Start Of Frame packet.
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & operator <<(CigiBaseSOF *refPacket)
   {
      return(operator<<(*refPacket));
   }

   //=========================================================
   //! Non operator-overloaded pack method for scripting.
   //! \param refPacket - A pointer to an Start Of Frame packet.
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & pack(CigiBaseSOF *refPacket)
   {
     return this->pack(*refPacket);
   }

   //=========================================================
   //! Packs the buffer with the pointer to the specifed Entity Control packet.  
   //! \param refPacket - A pointer to an Entity Control packet.
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & operator <<(CigiBaseEntityCtrl *refPacket)
   {
      return(operator<<(*refPacket));
   }

   //=========================================================
   //! Non operator-overloaded pack method for scripting.
   //! \param refPacket - A pointer to an Entity Control packet.
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & pack(CigiBaseEntityCtrl *refPacket)
   {
     return this->pack(*refPacket);
   }

   //=========================================================
   //! Packs the buffer with the pointer to the specifed packet.  
   //! \param refPacket - A pointer to an Enviroment type
   //!    Control packet.  This includes Celestial and Atmosphere Ctrl
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & operator <<(CigiBaseEnvCtrl *refPacket)
   {
      return(operator<<(*refPacket));
   }

   //=========================================================
   //! Non operator-overloaded pack method for scripting.
   //! \param refPacket - A pointer to an Enviroment type
   //!    Control packet.  This includes Celestial and Atmosphere Ctrl
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & pack(CigiBaseEnvCtrl *refPacket)
   {
     return this->pack(*refPacket);
   }

   //=========================================================
   //! Packs the buffer with the specifed packet.  
   //! \param refPacket - A pointer to a variable size packet
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & operator <<(CigiBaseVariableSizePckt *refPacket)
   {
      return(operator<<(*refPacket));
   }

   //=========================================================
   //! Non operator-overloaded pack method for scripting.
   //! \param refPacket - A pointer to a variable size packet
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & pack(CigiBaseVariableSizePckt *refPacket)
   {
     return this->pack(*refPacket);
   }

   //=========================================================
   //! Packs the buffer with the pointer to the specifed packet.  
   //! \param refBasePacket - A pointer to a base packet.
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & operator <<(CigiBasePacket *refBasePacket)
   {
      return(operator<<(*refBasePacket));
   }

   //=========================================================
   //! Non operator-overloaded pack method for scripting.
   //! \param refBasePacket - A pointer to a base packet.
   //!
   //! \return This returns a reference to this CigiOutgoingMsg object
   CigiOutgoingMsg & pack(CigiBasePacket *refBasePacket)
   {
     return this->pack(*refBasePacket);
   }



   //==> Buffer Packing Managers

   //=========================================================
   //! Alerts the system that a message is being started.  
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int BeginMsg(void)
   {
      if(Buffers.empty())
         AdvanceBuffer();
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Updates the Frame Counter in the IG Control or SOF packet
   //!   of the current message.  
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int UpdateFrameCntr(void);

   //=========================================================
   //! Increments the Frame Counter in the IG Control or SOF packet
   //!   of the specified Outgoing message.  If the IncomingMsg
   //!   is NULL, data from the Version Jump Table is used.
   //! \param OutgoingMsg - A pointer to the outgoing msg buffer
   //! \param IncomingMsg - A pointer to the incoming msg buffer
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int UpdateFrameCntr(Cigi_uint8 *OutgoingMsg, Cigi_uint8 *IncomingMsg = NULL);

   //=========================================================
   //! Updates the IG Control's Frame Counter and Database ID if needed
   //! \param OutgoingMsg - A pointer to the outgoing msg buffer
   //! \param IncomingMsg - A pointer to the incoming msg buffer
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int UpdateIGCtrl(Cigi_uint8 *OutgoingMsg, Cigi_uint8 *IncomingMsg);

   //=========================================================
   //! Updates SOF's Frame Counter
   //! \param OutgoingMsg - A pointer to the outgoing msg buffer
   //! \param IncomingMsg - A pointer to the incoming msg buffer
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int UpdateSOF(Cigi_uint8 *OutgoingMsg, Cigi_uint8 *IncomingMsg = NULL)
   {
      return(UpdateFrameCntr(OutgoingMsg,IncomingMsg));
   }

   //=========================================================
   //! Locks the message for sending
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   int LockMsg(void);

   //=========================================================
   //! Gets a pointer to the beginning of the message and
   //!   the length of the message
   //! \param length - A reference to place the length of
   //!    the message.

   //! \return Message - A pointer to the beginning of the message.
   //!
	Cigi_uint8 * GetMsg(int &length);

   //=========================================================
   //! Gets a pointer to the beginning of the buffer.
   //! \return Buffer - A pointer to the beginning of the buffer.
   //!
   Cigi_uint8 * GetBuffer(void);

   //=========================================================
   //! Gets the length of the message in bytes. 
   //!
   //! \return The length of the message in bytes.
   //!
   int GetMsgLength(void);

   //=========================================================
   //! Gets a std::string of the buffer.  Note - not really the 
   //! most efficient way to retrieve the buffer contents, as it 
   //! makes a copy of the buffer.  Used by the scripting language 
   //! wrapper.
   //! \return Buffer - A std::string copy of the buffer.
   //!
   std::string GetBufferString(void);

   //=========================================================
   //! Unlocks the message after sending
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   int UnlockMsg(void);

   //=========================================================
   //! Packages the current message.
   //! Sets up the next buffer for packing.
   //! and returns a pointer to the message to be sent and the length.
   //! \param Msg - A pointer to a pointer to the beginning of the message.
   //! \param length - A reference to place the length of
   //!    the message.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int PackageMsg(Cigi_uint8 **Msg, int &length);

   //=========================================================
   //! Gets the currently Packaged message.
   //! Sets up the next buffer for packing.
   //! and returns a pointer to the message to be sent and the length.
   //! \param Msg - A pointer to a pointer to the beginning of the message.
   //! \param length - A reference to place the length of
   //!    the message.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int GetCurrentlyPackagedMsg(Cigi_uint8 **Msg, int &length)
   {
      if(PackagedMsg != NULL)
      {
         *Msg = PackagedMsg->Buffer;
         length = PackagedMsg->BufferFillCnt;
         return(CIGI_SUCCESS);
      }
      else
      {
         *Msg = (Cigi_uint8 *)0;  // equivalent to NULL
         length = 0;
         return(CIGI_ERROR_UNEXPECTED_NULL);
      }
   }

   //=========================================================
   //! Frees the last packaged message buffer for use.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   int FreeMsg(void);

   //+> Frame Count

   //=========================================================
   //! Sets the current Frame Count
   //! \param FrameCntIn - Sets the frame count of the session.
   //!
   void SetFrameCnt(const Cigi_uint32 FrameCntIn) { FrameCnt = FrameCntIn; }

   //=========================================================
   //! Gets the current Frame Count.
   //! \return the current frame count of the session.
   //!
   Cigi_uint32 GetFrameCnt(void) const { return(FrameCnt); }

   //+> Synchronous Operation

   //=========================================================
   //! Deprecated:<br>
   //!   This member function nolonger does anything.
   //! \param Sync - Sets the synchronous/asynchronous mode 
   //!   of operation.<br>
   //!   true - Synchronous operation.
   //!   false - Asynchronous operation.
   //!
   void SetSynchronous(const bool Sync) { };

   //=========================================================
   //! Gets the synchronous/asynchronous mode of operation.
   //! \return the synchronous/asynchronous mode of operation.
   //!   true - Synchronous operation.
   //!   false - Asynchronous operation.
   //!
   bool IsSynchronous(void) const;


   //+> Registering

   //=========================================================
   //! Register a user packet for use.
   //! \param Packet - A pointer to the packet manager object
   //! \param PacketID - The packet id
   //! \param HostSend - A flag specifying whether the host
   //!   can send this packet.
   //! \param IGSend - A flag specifying whether the IG
   //!   can send this packet.
   //!
   //! \return the a flag specifying whether the specified
   //!   packet is valid to send.
   //!
	int RegisterUserPacket(CigiBasePacket *Packet,
                          Cigi_uint8 PacketID,
                          bool HostSend,
                          bool IGSend);




protected:


   //=========================================================
   //! Resets the buffer system.  
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int Reset(void);

   //=========================================================
   //! Prepairs a message buffer for packet insertion.  
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int InitMsg(void)
   {
      AdvanceBuffer();
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Sets the complete outgoing external interface Cigi
   //!  Version with bound checking control
   //! \param CigiVersionIn - Sets the Cigi Version of
   //!   the outgoing message interface
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetOutgoingCigiVersion(CigiVersionID &Version,
                              bool bndchk=true);

   //=========================================================
   //! Sets the complete outgoing external interface Cigi
   //!  Version with bound checking control
   //! \param CigiVersionIn - Sets the Cigi Version of
   //!   the outgoing message interface
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	CigiVersionID GetOutgoingCigiVersion(void)
   {
      return(OutgoingVersion);
   }

   //=========================================================
   //! Sets the complete Cigi Version value of the most mature
   //    Cigi Version of an incoming message received.
   //! \param CigiVersionIn - The most mature Cigi Version of
   //!   the incoming message interface
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	void SetMostMatureReceivedCigiVersion(CigiVersionID &Version);

   //=========================================================
   //! Sets the count of the last frame received.
   //! \param LastRcvdFrame - The count of the last frame received.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   void SetLastRcvdFrame(Cigi_uint32 LastRcvdFrameIn)
   {
      LastRcvdFrame = LastRcvdFrameIn;
   }

   //=========================================================
   //! Sets the buffer's complete CIGI version.
   //! Also, changes which version of the IG Control packet
   //! gets packed into the buffer.
   //! It uses the version of CIGI that is currently set as
   //! the version being used for the Outgoing Message.
   //!
	void ChangeBufferCigiVersion(void);

   //=========================================================
   //! Deletes the contents of OutgoingHandlerTbl and sets entries in 
   //! VldSnd accordingly.
   //!
   void ClearHandlerTable(void);

   //=========================================================
   //! Sets the external interface tables to Host output with
   //!   Cigi Version 1 packets
   //!
	void SetOutgoingHostV1Tbls(void);

   //=========================================================
   //! Sets the external interface tables to IG output with
   //!   Cigi Version 1 packets
   //!
	void SetOutgoingIGV1Tbls(void);

   //=========================================================
   //! Sets the external interface tables to Host output with
   //!   Cigi Version 2 packets
   //!
	void SetOutgoingHostV2Tbls(void);

   //=========================================================
   //! Sets the external interface tables to IG output with
   //!   Cigi Version 2 packets
   //!
	void SetOutgoingIGV2Tbls(void);

   //=========================================================
   //! Sets the external interface tables to Host output with
   //!   Cigi Version 3 packets
   //!
	void SetOutgoingHostV3Tbls(void);

   //=========================================================
   //! Sets the external interface tables to IG output with
   //!   Cigi Version 3 packets
   //!
	void SetOutgoingIGV3Tbls(void);

   //=========================================================
   //! Inserts a new buffer into the active buffer list
   //!  and initiates it as the buffer currently being filled.
   //!
	void AdvanceBuffer(void);

   //=========================================================
   //! Packs the buffer with the pointer to the specifed packet.  
   //! \param DataPacket - A packet object containing the data.
   //! \param PackingPacket - A packet object that will pack
   //!   the data from the DataPacket.
   //! \param Spec - A pointer to special data
   //!
   CigiOutgoingMsg &  PackObj(CigiBasePacket &DataPacket,
                              CigiBasePacket &PackingPacket,
                              void *Spec);

   //=========================================================
   //! Changes the outgoing cigi version.  
   //! \param Version - The CIGI version to which to change.
   //!
   void ChangeOutgoingCigiVersion(CigiVersionID &Version);

   //=========================================================
   //! The Version of CIGI being sent out
   //!
   CigiVersionID OutgoingVersion;

   //=========================================================
   //! There is a change in the Commanded Version of Cigi
   //!
   bool CmdVersionChng;

   //=========================================================
   //! The Commanded Version of Cigi
   //!
   CigiVersionID CmdVersion;

   //=========================================================
   //! The most mature version of CIGI received
   //!
   CigiVersionID MostMatureVersionReceived;

   //=========================================================
   //! A default packet manager
   //!
   CigiDefaultPacket DefaultPacket;

   //=========================================================
   //! The Conversion Table for managers of outgoing packets
   //!
   CigiBasePacket *OutgoingHandlerTbl[256];

   //=========================================================
   //! The Packet valid to send Table
   //!
   bool VldSnd[256];

   //=========================================================
   //! The current frame count
   //!
   Cigi_uint32 FrameCnt;

   //=========================================================
   //! The count of the last received frame
   //!
   Cigi_uint32 LastRcvdFrame;

   //=========================================================
   //! An Array of IGCtrl packers
   //!
   CigiBaseIGCtrl *pIGCtrlPck[6];

   //=========================================================
   //! An Array of SOF packers
   //!
   CigiBaseSOF *pSOFPck[5];




};

#endif // !defined(_CIGI_OUTGOING_MESSAGE_INCLUDED_)
