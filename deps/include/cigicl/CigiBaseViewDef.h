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
 *  FILENAME:   CigiBaseViewDef.h
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


#if !defined(_CIGI_BASE_VIEWDEF_INCLUDED_)
#define _CIGI_BASE_VIEWDEF_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_VIEW_DEF_PACKET_ID_V1 23
#define CIGI_VIEW_DEF_PACKET_SIZE_V1 32

#define CIGI_VIEW_DEF_PACKET_ID_V2 23
#define CIGI_VIEW_DEF_PACKET_SIZE_V2 32

#define CIGI_VIEW_DEF_PACKET_ID_V3 21
#define CIGI_VIEW_DEF_PACKET_SIZE_V3 32


class CigiViewDefV1;
class CigiViewDefV2;
class CigiViewDefV3;


class CIGI_SPEC CigiBaseViewDef : public CigiBasePacket
{

friend class CigiViewDefV1;
friend class CigiViewDefV2;
friend class CigiViewDefV3;

public:

   //=========================================================
   //! The enumeration for the CigiBaseViewDef Group
   //!
   enum MirrorModeGrp
   {
      MirrorNone=0,
      Horizontal=1,
      Vertical=2,
      Horiz_Vert=3
   };

   //=========================================================
   //! The enumeration for the CigiBaseViewDef Group
   //!
   enum PixelReplicateModeGrp
   {
      ReplicateNone=0,
      Replicate1x2=1,
      Replicate2x1=2,
      Replicate2x2=3,
      ReplicateDefA=4,
      ReplicateDefB=5,
      ReplicateDefC=6,
      ReplicateDefD=7
   };

   //=========================================================
   //! The enumeration for the CigiBaseViewDef Group
   //!
   enum ProjectionTypeGrp
   {
      Perspective=0,
      Orthographic=1
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseViewDef();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseViewDef();


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

   //+> MirrorMode

   //=========================================================
   //! Sets the MirrorMode with bound checking control
   //! \param MirrorModeIn - Specifies if the image is mirrored<br>
   //!   MirrorNone=0<br>
   //!   Horizontal=1<br>
   //!   Vertical=2<br>
   //!   Horiz_Vert=3<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetMirrorMode(const MirrorModeGrp MirrorModeIn, bool bndchk=true);

   //=========================================================
   //! Gets the MirrorMode value.
   //! \return the current MirrorMode.
   MirrorModeGrp GetMirrorMode(void) const { return(MirrorMode); }


   //+> PixelReplicateMode

   //=========================================================
   //! Sets the PixelReplicateMode with bound checking control
   //! \param PixelReplicateModeIn - Specifies what kind of
   //!    pixel replication (zoom) is currently being used.<br>
   //!   ReplicateNone=0<br>
   //!   Replicate1x2=1<br>
   //!   Replicate2x1=2<br>
   //!   Replicate2x2=3<br>
   //!   ReplicateDefA=4<br>
   //!   ReplicateDefB=5<br>
   //!   ReplicateDefC=6<br>
   //!   ReplicateDefD=7<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetPixelReplicateMode(const PixelReplicateModeGrp PixelReplicateModeIn, bool bndchk=true);

   //=========================================================
   //! Gets the PixelReplicateMode value.
   //! \return the current PixelReplicateMode.
   PixelReplicateModeGrp GetPixelReplicateMode(void) const { return(PixelReplicateMode); }


   //+> ViewType

   //=========================================================
   //! Sets the ViewType with bound checking control
   //! \param ViewTypeIn - Specifies view type (IG defined values)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetViewType(const Cigi_uint8 ViewTypeIn, bool bndchk=true);

   //=========================================================
   //! Gets the ViewType value.
   //! \return the current ViewType.
   Cigi_uint8 GetViewType(void) const { return(ViewType); }


   //+> FOVNear

   //=========================================================
   //! Sets the FOVNear with bound checking control
   //! \param FOVNearIn - The position of the near clipping plane.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetFOVNear(const float FOVNearIn, bool bndchk=true)
   {
      FOVNear = FOVNearIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the FOVNear value.
   //! \return the current FOVNear.
   float GetFOVNear(void) const { return(FOVNear); }


   //+> FOVFar

   //=========================================================
   //! Sets the FOVFar with bound checking control
   //! \param FOVFarIn - The position of the far clipping plane.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetFOVFar(const float FOVFarIn, bool bndchk=true)
   {
      FOVFar = FOVFarIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the FOVFar value.
   //! \return the current FOVFar.
   float GetFOVFar(void) const { return(FOVFar); }


   //+> FOVLeft

   //=========================================================
   //! Sets the FOVLeft with bound checking control
   //! \param FOVLeftIn - The position in degrees of the left clipping plane.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetFOVLeft(const float FOVLeftIn, bool bndchk=true)
   {
      FOVLeft = FOVLeftIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the FOVLeft value.
   //! \return the current FOVLeft.
   float GetFOVLeft(void) const { return(FOVLeft); }


   //+> FOVRight

   //=========================================================
   //! Sets the FOVRight with bound checking control
   //! \param FOVRightIn - The position in degrees of the right clipping plane.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetFOVRight(const float FOVRightIn, bool bndchk=true)
   {
      FOVRight = FOVRightIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the FOVRight value.
   //! \return the current FOVRight.
   float GetFOVRight(void) const { return(FOVRight); }


   //+> FOVTop

   //=========================================================
   //! Sets the FOVTop with bound checking control
   //! \param FOVTopIn - The position in degrees of the top clipping plane.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetFOVTop(const float FOVTopIn, bool bndchk=true)
   {
      FOVTop = FOVTopIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the FOVTop value.
   //! \return the current FOVTop.
   float GetFOVTop(void) const { return(FOVTop); }


   //+> FOVBottom

   //=========================================================
   //! Sets the FOVBottom with bound checking control
   //! \param FOVBottomIn - The position in degrees of the bottom clipping plane.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetFOVBottom(const float FOVBottomIn, bool bndchk=true)
   {
      FOVBottom = FOVBottomIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the FOVBottom value.
   //! \return the current FOVBottom.
   float GetFOVBottom(void) const { return(FOVBottom); }



protected:

   //==> Member variables

   //=========================================================
   //! ViewID<br>
   //!  Specifies the view that this packet defines.
   //!
   Cigi_uint16 ViewID;

   //=========================================================
   //! GroupID<br>
   //!  Specifies which group the view is to be attached.
   //!
   Cigi_uint8 GroupID;

   //=========================================================
   //! FOVNearEn<br>
   //!  Specifies the validity of the FOVNear parameter<br>
   //!   true - valid data : false - data is not valid
   //!
   bool FOVNearEn;

   //=========================================================
   //! FOVFarEn<br>
   //!  Specifies the validity of the FOVFar parameter<br>
   //!   true - valid data : false - data is not valid
   //!
   bool FOVFarEn;

   //=========================================================
   //! FOVLeftEn<br>
   //!  Specifies the validity of the FOVLeft parameter<br>
   //!   true - valid data : false - data is not valid
   //!
   bool FOVLeftEn;

   //=========================================================
   //! FOVRightEn<br>
   //!  Specifies the validity of the FOVRight parameter<br>
   //!   true - valid data : false - data is not valid
   //!
   bool FOVRightEn;

   //=========================================================
   //! FOVTopEn<br>
   //!  Specifies the validity of the FOVTop parameter<br>
   //!   true - valid data : false - data is not valid
   //!
   bool FOVTopEn;

   //=========================================================
   //! FOVBottomEn<br>
   //!  Specifies the validity of the FOVBottom parameter<br>
   //!   true - valid data : false - data is not valid
   //!
   bool FOVBottomEn;

   //=========================================================
   //! MirrorMode<br>
   //!  Specifies the mirror mode applied to this view.<br>
   //!  This is typically used to replicate a view on a
   //!   mirrored surface.<br>
   //!  0-MirrorNone<br>
   //!  1-Horizontal<br>
   //!  2-Vertical<br>
   //!  3-Horiz_Vert
   //!
   //!
   MirrorModeGrp MirrorMode;

   //=========================================================
   //! PixelReplicateMode<br>
   //!  Specifies the pixel replication (zoom) method.<br>
   //!  0-ReplicateNone<br>
   //!  1-Replicate1x2<br>
   //!  2-Replicate2x1<br>
   //!  3-Replicate2x2<br>
   //!  4-ReplicateDefA<br>
   //!  5-ReplicateDefB<br>
   //!  6-ReplicateDefC<br>
   //!  7-ReplicateDefD
   //!
   //!
   PixelReplicateModeGrp PixelReplicateMode;

   //=========================================================
   //! ProjectionType<br>
   //!  Specifies the projection used for this view.
   //!  0-Perspective<br>
   //!  1-Orthographic
   //!
   //!
   ProjectionTypeGrp ProjectionType;

   //=========================================================
   //! Reorder<br>
   //!  Specifies if the views should be reordered.<br>
   //!  false - Do not reorder.
   //!  true  - Bring the view to the front.
   //!
   bool Reorder;

   //=========================================================
   //! ViewType<br>
   //!  Specifies the view type.<br>
   //!  The values are defined by the IG.
   //!
   Cigi_uint8 ViewType;

   //=========================================================
   //! TrackerAssigned<br>
   //!  Specifies if a tracker is assigned to this view
   //!
   bool TrackerAssigned;

   //=========================================================
   //! FOVNear<br>
   //!  Specifies the position of the near clipping plane.
   //!
   float FOVNear;

   //=========================================================
   //! FOVFar<br>
   //!  Specifies the position of the far clipping plane.
   //!
   float FOVFar;

   //=========================================================
   //! FOVLeft<br>
   //!  Specifies the position of the left clipping plane in degrees.
   //!
   float FOVLeft;

   //=========================================================
   //! FOVRight<br>
   //!  Specifies the position of the right clipping plane in degrees.
   //!
   float FOVRight;

   //=========================================================
   //! FOVTop<br>
   //!  Specifies the position of the top clipping plane in degrees.
   //!
   float FOVTop;

   //=========================================================
   //! FOVBottom<br>
   //!  Specifies the position of the bottom clipping plane in degrees.
   //!
   float FOVBottom;

};

#endif // #if !defined(_CIGI_BASE_VIEWDEF_INCLUDED_)
