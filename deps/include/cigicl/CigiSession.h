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
 *  FILENAME:   CigiSession.h
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
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added the GetCigiMinorVersion accessor method.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added an overloaded SetCigiVersion method to accept a CigiMinorVersionIn 
 *  parameter.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the existing SetCigiVersion method to use the new SetCigiVersion 
 *  method with a default minor version of 0.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 * </pre>
 *  Author: The Boeing Company
 *
 */

#if !defined(_CIGI_SESSION_INCLUDED_)
#define _CIGI_SESSION_INCLUDED_

#include "CigiOutgoingMsg.h"
#include "CigiIncomingMsg.h"
#include "CigiAnimationTable.h"



//=========================================================
//! The base class for the Session<br>
//!
class CIGI_SPEC CigiSession  
{
public:

   //=========================================================
   //! The enumeration for the Session Type
   //!
   enum SessionTypeEnum
   {
      Host,
      IG
   };

   //==> Management

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiSession();



   //==> Member functions


   //+> Cigi Version

   //=========================================================
   //! Sets the Cigi version of the external interface
   //!   with bound checking control
   //! \param CigiVersionIn - The Cigi Version being used.
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetCigiVersion(const int CigiVersionIn, bool bndchk=true)
   {
      return( SetCigiVersion(CigiVersionIn, 0, bndchk));
   }

   //=========================================================
   //! Sets the Cigi version of the external interface
   //!   with bound checking control
   //! \param CigiVersionIn - The Cigi Version being used.
   //! \param CigiMinorVersionIn - Sets the Cigi Minor Version of
   //!   the external interface
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetCigiVersion(const int CigiVersionIn,
                      const int CigiMinorVersionIn,
                      bool bndchk=true)
   {
      CigiVersionID tCigiVer;
      tCigiVer.CigiMajorVersion = CigiVersionIn;
      tCigiVer.CigiMinorVersion = CigiMinorVersionIn;
      return(OutMsg.SetOutgoingCigiVersion(tCigiVer,bndchk));
   }

   //=========================================================
   //! Sets the Cigi version of the external interface
   //!   with bound checking control
   //! \param CigiVersionIn - The Cigi Version being used.
   //! \param CigiMinorVersionIn - Sets the Cigi Minor Version of
   //!   the external interface
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetCigiVersion(CigiVersionID CigiVersionIn,
                      bool bndchk=true)
   {
      return(OutMsg.SetOutgoingCigiVersion(CigiVersionIn,bndchk));
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
	void SetMostMatureReceivedCigiVersion(CigiVersionID &Version)
   {
      OutMsg.SetMostMatureReceivedCigiVersion(Version);
   }


   //=========================================================
   //! Gets the Cigi version of the external interface.
   //! \return the complete Cigi version of the external interface.
   //!
   CigiVersionID GetCompleteCigiVersion(void)
   {
      return(OutMsg.GetOutgoingCigiVersion());
   }


   //=========================================================
   //! Gets the Cigi version of the external interface.
   //! \return the Cigi version of the external interface.
   //!
   int GetCigiVersion(void)
   {
      return(OutMsg.GetOutgoingCigiVersion().CigiMajorVersion);
   }


   //=========================================================
   //! Gets the Cigi Minor Version of the external interface.
   //! \return the Cigi Minor Version of the external interface.
   //!
   int GetCigiMinorVersion(void)
   {
      return(OutMsg.GetOutgoingCigiVersion().CigiMinorVersion);
   }


   //+> Managers

   //=========================================================
   //! Gets a reference to the Outgoing Message Manager object
   //! \return a reference to the Outgoing Message Manager object.
   //!
   CigiOutgoingMsg & GetOutgoingMsgMgr(void) { return(OutMsg); }

   //=========================================================
   //! Gets a reference to the Incoming Message Manager object
   //! \return a reference to the Incoming Message Manager object.
   //!
   CigiIncomingMsg & GetIncomingMsgMgr(void) { return(InMsg); }

   //=========================================================
   //! Gets a reference to the Animation Table Manager object
   //! \return a reference to the Animation Table Manager object.
   //!
   CigiAnimationTable & GetAnimationTableMgr(void) { return(ATbl); }


   //+> Interface Timing Method

   //=========================================================
   //! Specifies whether the interface is synchronous or asynchronous.
   //! \param Sync - The synchronous flag.<br>
   //!  true - interface is synchronous.<br>
   //!  false - interface is asynchronous.<br>
   //!
   void SetSynchronous(bool Sync) { Synchronous = Sync; }

   //=========================================================
   //! Gets whether the interface is synchronous or asynchronous.
   //! \return Sync - The synchronous flag.<br>
   //!  true - interface is synchronous.<br>
   //!  false - interface is asynchronous.<br>
   //!
   bool IsSynchronous(void) { return(Synchronous); }


   //+> User Packet Management

   //=========================================================
   //! Registers an user defined packet manager
   //! \param Packet - A pointer to an user defined packet manager object.
   //! \param PacketID - The packet ID this manager processes.
   //! \param HostSend - Flag specifying that the host can send this packet.<br>
   //!  true - The host can send this packet.<br>
   //!  false - The host cannot send this packet.<br>
   //! \param IGSend - Flag specifying that the IG can send this packet.<br>
   //!  true - The IG can send this packet.<br>
   //!  false - The IG cannot send this packet.<br>
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int RegisterUserPacket(CigiBasePacket *Packet,
                          Cigi_uint8 PacketID,
                          bool HostSend, bool IGSend)
   {
      int outMsgStatus = OutMsg.RegisterUserPacket(Packet,PacketID,HostSend,IGSend);
      int inMsgStatus  =  InMsg.RegisterUserPacket(Packet,PacketID,HostSend,IGSend);
      if( outMsgStatus == CIGI_SUCCESS && inMsgStatus == CIGI_SUCCESS )
         return CIGI_SUCCESS;
      else if( outMsgStatus == CIGI_SUCCESS )
         return inMsgStatus;
      else
         return outMsgStatus;
   }


   //+> Session Type

   //=========================================================
   //! Notification as to whether the session is a Host session
   //!
   //! \return This returns true if this is a host session
   //!
	bool IsHost(void)
   {
      return((SessionType == Host) ? true : false);
   }

   //=========================================================
   //! Notification as to whether the session is a IG session
   //!
   //! \return This returns true if this is a IG session
   //!
	bool IsIG(void)
   {
      return((SessionType == IG) ? true : false);
   }



protected:

   //==> Member variables

   //=========================================================
   //! Outgoing Message Object<br>
   //!
   CigiOutgoingMsg OutMsg;

   //=========================================================
   //! Incoming Message Object<br>
   //!
   CigiIncomingMsg InMsg;

   //=========================================================
   //! Animation Table Object<br>
   //!
   CigiAnimationTable ATbl;

   //=========================================================
   //! Session Type IG or Host<br>
   //!
   SessionTypeEnum SessionType;

   //=========================================================
   //! A flag specifying whether this session is synchronous
   //!  or asynchronous.
   //!
   bool Synchronous;

   //=========================================================
   //! 
   //! 
   //!
   const CigiVersionID MostMatureKnownCigi;


   //==> Member Functions

   //=========================================================
   //! Protected Constructor
   //! \param NumInBuf - The number of incoming message buffers.
   //! \param InBufLen - The size in bytes of each of the
   //!    incoming message buffers.
   //! \param NumOutBuf - The number of outgoing message buffers.
   //! \param OutBufLen - The size in bytes of each of the
   //!    outgoing message buffers.
   //! \param JTblType - The Jump Table type. (Host or IG)
   //!
   CigiSession(const int NumInBuf, const int InBufLen,
               const int NumOutBuf, const int OutBufLen,
               SessionTypeEnum SessionTypeIn);


};

#endif // !defined(_CIGI_SESSION_INCLUDED_)
