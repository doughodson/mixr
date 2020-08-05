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
 *  FILENAME:   CigiBaseEventNotification.h
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
 *  Changed native C++ types in function and type declarations to CCL-defined 
 *  types.
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


#if !defined(_CIGI_BASE_EVENT_NOTIFICATION_INCLUDED_)
#define _CIGI_BASE_EVENT_NOTIFICATION_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_EVENT_NOTIFICATION_PACKET_ID_V3 116
#define CIGI_EVENT_NOTIFICATION_PACKET_SIZE_V3 16


class CigiEventNotificationV3;


class CIGI_SPEC CigiBaseEventNotification : public CigiBasePacket
{

friend class CigiEventNotificationV3;

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseEventNotification();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseEventNotification();


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

   //+> EventID

   //=========================================================
   //! Sets the EventID with bound checking control
   //! \param EventIDIn - Event ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetEventID(const Cigi_uint16 EventIDIn, bool bndchk=true)
   {
      EventID = EventIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the EventID value.
   //! \return the current EventID.
   Cigi_uint16 GetEventID(void) const { return(EventID); }



   //+> EventData

   //=========================================================
   //! Sets the EventData with bound checking control
   //! \param EventDataIn - Event Data
   //! \param ndx - Which of the 3 data words to access.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetEventData(const Cigi_uint32 EventDataIn, int ndx, bool bndchk=true);

   //=========================================================
   //! Sets the EventData with bound checking control
   //! \param EventDataIn - Event Data
   //! \param ndx - Which of the 3 data words to access.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetEventData(const Cigi_int32 EventDataIn, int ndx, bool bndchk=true);

   //=========================================================
   //! Sets the EventData with bound checking control
   //! \param EventDataIn - Event Data
   //! \param ndx - Which of the 3 data words to access.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetEventData(const float EventDataIn, int ndx, bool bndchk=true);

   //=========================================================
   //! Gets the EventData value.
   //! \param ndx - Which of the 3 data words to access.
   //!
   //! \return the current EventData.<br>
   //!   If the index is incorrect, it will return the nearest
   //!   correct word of the Event data.
   Cigi_uint32 GetULEventData(int ndx);

   //=========================================================
   //! Gets the EventData value.
   //! \param ndx - Which of the 3 data words to access.
   //!
   //! \return the current EventData.<br>
   //!   If the index is incorrect, it will return the nearest
   //!   correct word of the Event data.
   Cigi_int32 GetLEventData(int ndx);

   //=========================================================
   //! Gets the EventData value.
   //! \param ndx - Which of the 3 data words to access.
   //!
   //! \return the current EventData.<br>
   //!   If the index is incorrect, it will return the nearest
   //!   correct word of the Event data.
   float GetFEventData(int ndx);



protected:

   //==> Member variables

   //=========================================================
   //! EventID<br>
   //!
   //!
   Cigi_uint16 EventID;

   //=========================================================
   //! EventData<br>
   //!
   //!
   union EventDataGrp {
      Cigi_uint32 ul[3];
      Cigi_int32 l[3];
      float f[3];
   } EventData;


};

#endif // #if !defined(_CIGI_BASE_EVENT_NOTIFICATION_INCLUDED_)
