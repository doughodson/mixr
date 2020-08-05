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
 *  FILENAME:   CigiBaseSensorResp.h
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
 *  Changed native C++ types in function declarations to CCL-defined types.
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


#if !defined(_CIGI_BASE_SENSOR_RESP_INCLUDED_)
#define _CIGI_BASE_SENSOR_RESP_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_SENSOR_RESP_PACKET_ID_V1 105
#define CIGI_SENSOR_RESP_PACKET_SIZE_V1 12

#define CIGI_SENSOR_RESP_PACKET_ID_V2 105
#define CIGI_SENSOR_RESP_PACKET_SIZE_V2 12

#define CIGI_SENSOR_RESP_PACKET_ID_V3 106
#define CIGI_SENSOR_RESP_PACKET_SIZE_V3 24

#define CIGI_SENSOR_XRESP_PACKET_ID_V3 107
#define CIGI_SENSOR_XRESP_PACKET_SIZE_V3 48


class CigiSensorRespV1;
class CigiSensorRespV2;
class CigiSensorRespV3;
class CigiSensorXRespV3;


class CIGI_SPEC CigiBaseSensorResp : public CigiBasePacket
{

friend class CigiSensorRespV1;
friend class CigiSensorRespV2;
friend class CigiSensorRespV3;
friend class CigiSensorXRespV3;

public:

   //=========================================================
   //! The enumeration for the CigiBaseSensorResp Group
   //!
   enum SensorStatGrp
   {
      Searching=0,
      Tracking=1,
      NearBrakeLock=2,
      BrakeLock=3
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseSensorResp();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseSensorResp();


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

   //+> SensorID

   //=========================================================
   //! Sets the SensorID with bound checking control
   //! \param SensorIDIn - Sensor ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSensorID(const Cigi_uint8 SensorIDIn, bool bndchk=true)
   {
      SensorID = SensorIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the SensorID value.
   //! \return the current SensorID.
   Cigi_uint8 GetSensorID(void) const { return(SensorID); }


   //+> SensorStat

   //=========================================================
   //! Sets the SensorStat with bound checking control
   //! \param SensorStatIn - Sensor State/Status<br>
   //!   Searching=0<br>
   //!   Tracking=1<br>
   //!   NearBrakeLock=2<br>
   //!   BrakeLock=3<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSensorStat(const SensorStatGrp SensorStatIn, bool bndchk=true);

   //=========================================================
   //! Gets the SensorStat value.
   //! \return the current SensorStat.
   SensorStatGrp GetSensorStat(void) const { return(SensorStat); }


   //+> GateSzX

   //=========================================================
   //! Sets the GateSzX with bound checking control
   //! \param GateSzXIn - Gate Size along the X axis
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetGateSzX(const Cigi_uint16 GateSzXIn, bool bndchk=true)
   {
      GateSzX = GateSzXIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the GateSzX value.
   //! \return the current GateSzX.
   Cigi_uint16 GetGateSzX(void) const { return(GateSzX); }


   //+> GateSzY

   //=========================================================
   //! Sets the GateSzY with bound checking control
   //! \param GateSzYIn - Gate Size along the Y axis
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetGateSzY(const Cigi_uint16 GateSzYIn, bool bndchk=true)
   {
      GateSzY = GateSzYIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the GateSzY value.
   //! \return the current GateSzY.
   Cigi_uint16 GetGateSzY(void) const { return(GateSzY); }


   //+> GateXoff

   //=========================================================
   //! Sets the GateXoff with bound checking control
   //! \param GateXoffIn - The Gate offset angle along the X axis.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetGateXoff(const float GateXoffIn, bool bndchk=true);

   //=========================================================
   //! Gets the GateXoff value.
   //! \return the current GateXoff.
   float GetGateXoff(void) const { return(GateXoff); }


   //+> GateYoff

   //=========================================================
   //! Sets the GateYoff with bound checking control
   //! \param GateYoffIn - The Gate offset angle along the Y axis.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetGateYoff(const float GateYoffIn, bool bndchk=true);

   //=========================================================
   //! Gets the GateYoff value.
   //! \return the current GateYoff.
   float GetGateYoff(void) const { return(GateYoff); }



protected:

   //==> Member variables

   //=========================================================
   //! ViewID<br>
   //! The ID of the view currently displaying the sensor scene.
   //!
   Cigi_uint16 ViewID;

   //=========================================================
   //! SensorID<br>
   //! The ID of the sensor being reported.
   //!
   Cigi_uint8 SensorID;

   //=========================================================
   //! SensorStat<br>
   //! Sensor State or Status<br>
   //!  0-Searching<br>
   //!  1-Tracking<br>
   //!  2-NearBrakeLock<br>
   //!  3-BrakeLock
   //!
   //!
   SensorStatGrp SensorStat;

   //=========================================================
   //! EntityTgt<br>
   //! True if an entity is being tracked
   //!
   bool EntityTgt;

   //=========================================================
   //! EntityID<br>
   //! The ID of the entity being tracked
   //!
   Cigi_uint16 EntityID;

   //=========================================================
   //! GateSzX<br>
   //! The size of the gate in pixels along the horizontal
   //!   display axis.
   //!
   Cigi_uint16 GateSzX;

   //=========================================================
   //! GateSzY<br>
   //! The size of the gate in pixels along the vertical
   //!   display axis.
   //!
   Cigi_uint16 GateSzY;

   //=========================================================
   //! GateXoff<br>
   //! The horizontal angle between the view normal and the line
   //!   from the view projection center and the center of the
   //!   gate position.
   //!
   float GateXoff;

   //=========================================================
   //! GateYoff<br>
   //! The horizontal angle between the view normal and the line
   //!   from the view projection center and the center of the
   //!   gate position.
   //!
   float GateYoff;

   //=========================================================
   //! FrameCntr<br>
   //! Frame count
   //!
   Cigi_uint32 FrameCntr;

   //=========================================================
   //! TrackPntLat<br>
   //! The tracked point's Latitude.
   //!
   double TrackPntLat;

   //=========================================================
   //! TrackPntLon<br>
   //! The tracked point's Longitude.
   //!
   double TrackPntLon;

   //=========================================================
   //! TrackPntAlt<br>
   //! The tracked point's altitude.
   //!
   double TrackPntAlt;


};

#endif // #if !defined(_CIGI_BASE_SENSOR_RESP_INCLUDED_)
