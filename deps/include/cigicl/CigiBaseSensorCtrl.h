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
 *  FILENAME:   CigiBaseSensorCtrl.h
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


#if !defined(_CIGI_BASE_SENSOR_CTRL_INCLUDED_)
#define _CIGI_BASE_SENSOR_CTRL_INCLUDED_

#include "CigiBasePacket.h"


// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_SENSOR_CTRL_PACKET_ID_V1 9
#define CIGI_SENSOR_CTRL_PACKET_SIZE_V1 24

#define CIGI_SENSOR_CTRL_PACKET_ID_V2 9
#define CIGI_SENSOR_CTRL_PACKET_SIZE_V2 24

#define CIGI_SENSOR_CTRL_PACKET_ID_V3 17
#define CIGI_SENSOR_CTRL_PACKET_SIZE_V3 24


class CigiSensorCtrlV1;
class CigiSensorCtrlV2;
class CigiSensorCtrlV3;


class CIGI_SPEC CigiBaseSensorCtrl : public CigiBasePacket
{

friend class CigiSensorCtrlV1;
friend class CigiSensorCtrlV2;
friend class CigiSensorCtrlV3;

public:

   //=========================================================
   //! The enumeration for the CigiBaseSensorCtrl Group
   //!
   enum PolarityGrp
   {
      WhiteHot=0,
      BlackHot=1
   };

   //=========================================================
   //! The enumeration for the CigiBaseSensorCtrl Group
   //!
   enum TrackModeGrp
   {
      TrackOff=0,
      ForceCorrelate=1,
      Scene=2,
      Target=3,
      Ship=4,
      TMDefA=5,
      TMDefB=6,
      TMDefC=7
   };

   //=========================================================
   //! The enumeration for the CigiBaseSensorCtrl Group
   //!
   enum TrackPolarityGrp
   {
      TrackWhite=0,
      TrackBlack=1
   };

   //=========================================================
   //! The enumeration for the CigiBaseSensorCtrl Group
   //!
   enum ResponseTypeGrp
   {
      GatePos=0,
      GateAndTargetPos=1
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseSensorCtrl();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseSensorCtrl();


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
   //! \param SensorIDIn - The unique id identifying the specific
   //!   sensor that this packet is being applied.
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


   //+> SensorOn

   //=========================================================
   //! Sets the SensorOn with bound checking control
   //! \param SensorOnIn - Specifies whether the sensor is on(true) or off(false).
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSensorOn(const bool SensorOnIn, bool bndchk=true)
   {
      SensorOn = SensorOnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the SensorOn value.
   //! \return the current SensorOn.
   bool GetSensorOn(void) const { return(SensorOn); }


   //+> Polarity

   //=========================================================
   //! Sets the Polarity with bound checking control
   //! \param PolarityIn - Specifies what intesity pole by which 
   //!    hot and cold are represented.<br>
   //!   WhiteHot=0<br>
   //!   BlackHot=1<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetPolarity(const PolarityGrp PolarityIn, bool bndchk=true);

   //=========================================================
   //! Gets the Polarity value.
   //! \return the current Polarity.
   PolarityGrp GetPolarity(void) const { return(Polarity); }


   //+> LineDropEn

   //=========================================================
   //! Sets the LineDropEn with bound checking control
   //! \param LineDropEnIn - Specifies whether the line-by-line 
   //!   dropout feature is enabled (true) or disabled (false).
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetLineDropEn(const bool LineDropEnIn, bool bndchk=true)
   {
      LineDropEn = LineDropEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the LineDropEn value.
   //! \return the current LineDropEn.
   bool GetLineDropEn(void) const { return(LineDropEn); }


   //+> TrackMode

   //=========================================================
   //! Sets the TrackMode with bound checking control
   //! \param TrackModeIn - Specifies in which mode the tracker 
   //!    should be currently operating.<br>
   //!   TrackOff=0<br>
   //!   ForceCorrelate=1<br>
   //!   Scene=2<br>
   //!   Target=3<br>
   //!   Ship=4<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetTrackMode(const TrackModeGrp TrackModeIn, bool bndchk=true);

   //=========================================================
   //! Gets the TrackMode value.
   //! \return the current TrackMode.
   TrackModeGrp GetTrackMode(void) const { return(TrackMode); }


   //+> AutoGainEn

   //=========================================================
   //! Sets the AutoGainEn with bound checking control
   //! \param AutoGainEnIn - Specifies whether the auto gain feature
   //!   is enabled (true) or disabled (false).
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetAutoGainEn(const bool AutoGainEnIn, bool bndchk=true)
   {
      AutoGainEn = AutoGainEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the AutoGainEn value.
   //! \return the current AutoGainEn.
   bool GetAutoGainEn(void) const { return(AutoGainEn); }


   //+> TrackPolarity

   //=========================================================
   //! Sets the TrackPolarity with bound checking control
   //! \param TrackPolarityIn - Specifies whether the sensor should
   //!    track a white or black area.<br>
   //!   TrackWhite=0<br>
   //!   TrackBlack=1<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetTrackPolarity(const TrackPolarityGrp TrackPolarityIn, bool bndchk=true);

   //=========================================================
   //! Gets the TrackPolarity value.
   //! \return the current TrackPolarity.
   TrackPolarityGrp GetTrackPolarity(void) const { return(TrackPolarity); }


   //+> Level

   //=========================================================
   //! Sets the Level with bound checking control
   //! \param LevelIn - This specifies the level control of the seeker.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetLevel(const float LevelIn, bool bndchk=true);

   //=========================================================
   //! Gets the Level value.
   //! \return the current Level.
   float GetLevel(void) const { return(Level); }


   //+> ACCoupling

   //=========================================================
   //! Sets the ACCoupling with bound checking control
   //! \param ACCouplingIn - This specifies the AC Coupling control
   //!   of the seeker.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetACCoupling(const float ACCouplingIn, bool bndchk=true);

   //=========================================================
   //! Gets the ACCoupling value.
   //! \return the current ACCoupling.
   float GetACCoupling(void) const { return(ACCoupling); }


   //+> Noise

   //=========================================================
   //! Sets the Noise with bound checking control
   //! \param NoiseIn - This specifies the noise to show in the
   //!   sensor display.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetNoise(const float NoiseIn, bool bndchk=true);

   //=========================================================
   //! Gets the Noise value.
   //! \return the current Noise.
   float GetNoise(void) const { return(Noise); }



protected:

   //==> Member variables

   //=========================================================
   //! ViewID<br>
   //! Uniquely identifies the view to which this packet is applied.
   //!
   Cigi_uint16 ViewID;

   //=========================================================
   //! SensorID<br>
   //! Uniquely identifies the sensor to which this packet is applied.
   //!
   Cigi_uint8 SensorID;

   //=========================================================
   //! SensorOn<br>
   //! Specifies whether the Sensor is on (true) or off (false).
   //!
   bool SensorOn;

   //=========================================================
   //! Polarity<br>
   //! Specifies what intesity pole by which hot and cold 
   //!  are represented.<br>
   //!  0-WhiteHot<br>
   //!  1-BlackHot
   //!
   PolarityGrp Polarity;

   //=========================================================
   //! LineDropEn<br>
   //! Specifies whether the Line-by-line dropout feature is
   //!  enabled (true) or disabled (false) in the sensor.
   //!
   bool LineDropEn;

   //=========================================================
   //! TrackMode<br>
   //!  Specifies the operating mode of the sensor.
   //!  0-TrackOff<br>
   //!  1-ForceCorrelate<br>
   //!  2-Scene<br>
   //!  3-Target<br>
   //!  4-Ship
   //!
   TrackModeGrp TrackMode;

   //=========================================================
   //! AutoGainEn<br>
   //! Specifies whether the autogain feature of the sensor is
   //!  enabled (true) or disabled (false).
   //!
   bool AutoGainEn;

   //=========================================================
   //! TrackPolarity<br>
   //!  Specifies whether the sensor is to track a white or black area.
   //!  0-TrackWhite<br>
   //!  1-TrackBlack
   //!
   TrackPolarityGrp TrackPolarity;

   //=========================================================
   //! ResponseType<br>
   //!  Specifies the type of response that the IG should send
   //!   back to the host in CIGI 3.
   //!  0-GatePos<br>
   //!  1-GateAndTargetPos
   //!
   ResponseTypeGrp ResponseType;

   //=========================================================
   //! Gain<br>
   //! Specifies the gain control for the sensor.
   //!  (0.0 - 1.0)
   //!  (Note: CIGI 1 values are 0.0 - 100.0: this is converted
   //!     to 0.0 - 1.0 before storing in Gain.)
   //!
   float Gain;

   //=========================================================
   //! Level<br>
   //! Specifies the level control for the sensor.
   //!  (0.0 - 1.0)
   //!
   float Level;

   //=========================================================
   //! ACCoupling<br>
   //! Specifies the AC Coupling control for the sensor.
   //!  (0.0 - 1.0)
   //!
   float ACCoupling;

   //=========================================================
   //! Noise<br>
   //! Specifies the amount of noise that the sensor is receiving.
   //!  (0.0 - 1.0)
   //!
   float Noise;


};

#endif // #if !defined(_CIGI_BASE_SENSOR_CTRL_INCLUDED_)
