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
 *  FILENAME:   CigiBaseSpecEffDef.h
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


#if !defined(_CIGI_BASE_SPEC_EFF_DEF_INCLUDED_)
#define _CIGI_BASE_SPEC_EFF_DEF_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_SPEC_EFF_DEF_PACKET_ID_V1 22
#define CIGI_SPEC_EFF_DEF_PACKET_SIZE_V1 32

#define CIGI_SPEC_EFF_DEF_PACKET_ID_V2 22
#define CIGI_SPEC_EFF_DEF_PACKET_SIZE_V2 32


class CigiSpecEffDefV1;
class CigiSpecEffDefV2;


class CIGI_SPEC CigiBaseSpecEffDef : public CigiBasePacket
{

friend class CigiSpecEffDefV1;
friend class CigiSpecEffDefV2;

public:

   //=========================================================
   //! The enumeration for the CigiBaseSpecEffDef Group
   //!
   enum SeqDirGrp
   {
      Forward=0,
      Backward=1
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseSpecEffDef();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseSpecEffDef();


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

   //+> EntityID

   //=========================================================
   //! Sets the EntityID with bound checking control
   //! \param EntityIDIn - Entity ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetEntityID(const Cigi_uint16 EntityIDIn, bool bndchk=true)
   {
      EntityID = EntityIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the EntityID value.
   //! \return the current EntityID.
   Cigi_uint16 GetEntityID(void) const { return(EntityID); }


   //+> SeqDir

   //=========================================================
   //! Sets the SeqDir with bound checking control
   //! \param SeqDirIn - Special Effect Sequence direction<br>
   //!   Forward=0<br>
   //!   Backward=1<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSeqDir(const SeqDirGrp SeqDirIn, bool bndchk=true);

   //=========================================================
   //! Gets the SeqDir value.
   //! \return the current SeqDir.
   SeqDirGrp GetSeqDir(void) const { return(SeqDir); }


   //+> ColorEn

   //=========================================================
   //! Sets the ColorEn with bound checking control
   //! \param ColorEnIn - Color enable
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetColorEn(const bool ColorEnIn, bool bndchk=true)
   {
      ColorEn = ColorEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ColorEn value.
   //! \return the current ColorEn.
   bool GetColorEn(void) const { return(ColorEn); }


   //+> Red

   //=========================================================
   //! Sets the Red with bound checking control
   //! \param RedIn - Red color component
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRed(const Cigi_uint8 RedIn, bool bndchk=true)
   {
      Red = RedIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Red value.
   //! \return the current Red.
   Cigi_uint8 GetRed(void) const { return(Red); }


   //+> Green

   //=========================================================
   //! Sets the Green with bound checking control
   //! \param GreenIn - Green color component
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetGreen(const Cigi_uint8 GreenIn, bool bndchk=true)
   {
      Green = GreenIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Green value.
   //! \return the current Green.
   Cigi_uint8 GetGreen(void) const { return(Green); }


   //+> Blue

   //=========================================================
   //! Sets the Blue with bound checking control
   //! \param BlueIn - Blue color component
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetBlue(const Cigi_uint8 BlueIn, bool bndchk=true)
   {
      Blue = BlueIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Blue value.
   //! \return the current Blue.
   Cigi_uint8 GetBlue(void) const { return(Blue); }


   //+> XScale

   //=========================================================
   //! Sets the XScale with bound checking control
   //! \param XScaleIn - Scaling factor along the X axis for the effect.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetXScale(const float XScaleIn, bool bndchk=true);

   //=========================================================
   //! Gets the XScale value.
   //! \return the current XScale.
   float GetXScale(void) const { return(XScale); }


   //+> YScale

   //=========================================================
   //! Sets the YScale with bound checking control
   //! \param YScaleIn - Scaling factor along the Y axis for the effect.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetYScale(const float YScaleIn, bool bndchk=true);

   //=========================================================
   //! Gets the YScale value.
   //! \return the current YScale.
   float GetYScale(void) const { return(YScale); }


   //+> ZScale

   //=========================================================
   //! Sets the ZScale with bound checking control
   //! \param ZScaleIn - Scaling factor along the Z axis for the effect.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetZScale(const float ZScaleIn, bool bndchk=true);

   //=========================================================
   //! Gets the ZScale value.
   //! \return the current ZScale.
   float GetZScale(void) const { return(ZScale); }


   //+> TimeScale

   //=========================================================
   //! Sets the TimeScale with bound checking control
   //! \param TimeScaleIn - Scaling factor for the effect's time of play.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetTimeScale(const float TimeScaleIn, bool bndchk=true);

   //=========================================================
   //! Gets the TimeScale value.
   //! \return the current TimeScale.
   float GetTimeScale(void) const { return(TimeScale); }


   //+> EffectCnt

   //=========================================================
   //! Sets the EffectCnt with bound checking control
   //! \param EffectCntIn - Number of effects to control
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetEffectCnt(const Cigi_uint16 EffectCntIn, bool bndchk=true)
   {
      EffectCnt = EffectCntIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the EffectCnt value.
   //! \return the current EffectCnt.
   Cigi_uint16 GetEffectCnt(void) const { return(EffectCnt); }


   //+> Separation

   //=========================================================
   //! Sets the Separation with bound checking control
   //! \param SeparationIn - Distance between effects
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSeparation(const float SeparationIn, bool bndchk=true)
   {
      Separation = SeparationIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Separation value.
   //! \return the current Separation.
   float GetSeparation(void) const { return(Separation); }


   //+> BurstRate

   //=========================================================
   //! Sets the BurstRate with bound checking control
   //! \param BurstRateIn - The Rate at which the effects occur
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetBurstRate(const float BurstRateIn, bool bndchk=true)
   {
      BurstRate = BurstRateIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the BurstRate value.
   //! \return the current BurstRate.
   float GetBurstRate(void) const { return(BurstRate); }


   //+> Duration

   //=========================================================
   //! Sets the Duration with bound checking control
   //! \param DurationIn - Duration of the effect
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDuration(const float DurationIn, bool bndchk=true)
   {
      Duration = DurationIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Duration value.
   //! \return the current Duration.
   float GetDuration(void) const { return(Duration); }



protected:

   //==> Member variables

   //=========================================================
   //! EntityID<br>
   //! Entity ID
   //!
   Cigi_uint16 EntityID;

   //=========================================================
   //! SeqDir<br>
   //! Special Effect Sequence direction of play.
   //!  0-Forward<br>
   //!  1-Backward
   //!
   SeqDirGrp SeqDir;

   //=========================================================
   //! ColorEn<br>
   //! Color enable
   //!
   bool ColorEn;

   //=========================================================
   //! Red<br>
   //! Red color component
   //!
   Cigi_uint8 Red;

   //=========================================================
   //! Green<br>
   //! Green color component
   //!
   Cigi_uint8 Green;

   //=========================================================
   //! Blue<br>
   //! Blue color component
   //!
   Cigi_uint8 Blue;

   //=========================================================
   //! XScale<br>
   //! Scaling factor along the X axis for the effect.
   //!
   float XScale;

   //=========================================================
   //! YScale<br>
   //! Scaling factor along the Y axis for the effect.
   //!
   float YScale;

   //=========================================================
   //! ZScale<br>
   //! Scaling factor along the Z axis for the effect.
   //!
   float ZScale;

   //=========================================================
   //! TimeScale<br>
   //! Scaling factor for the effect's time of play.
   //!
   float TimeScale;

   //=========================================================
   //! EffectCnt<br>
   //! Number of effects to control
   //!
   Cigi_uint16 EffectCnt;

   //=========================================================
   //! Separation<br>
   //! Distance between effects
   //!
   float Separation;

   //=========================================================
   //! BurstRate<br>
   //! The Rate at which the effects occur
   //!
   float BurstRate;

   //=========================================================
   //! Duration<br>
   //! Duration of the effect
   //!
   float Duration;


};

#endif // #if !defined(_CIGI_BASE_SPEC_EFF_DEF_INCLUDED_)
