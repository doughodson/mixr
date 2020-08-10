#
include ../makedefs

LIB = $(DEPS_LIB_DIR)/libcigicl.a

CPPFLAGS += -std=c++11
CPPFLAGS += -I$(MIXR_ROOT)/deps/include/cigicl
CPPFLAGS += -DCIGI_LITTLE_ENDIAN

OBJS =  \
	CigiAerosolRespV3.o \
	CigiAnimationStopV3.o \
	CigiAnimationTable.o \
	CigiArtPartCtrlV1.o \
	CigiArtPartCtrlV2.o \
	CigiArtPartCtrlV3.o \
	CigiAtmosCtrl.o \
	CigiBaseAerosolResp.o \
	CigiBaseArtPartCtrl.o \
	CigiBaseCircleSymbolData.o \
	CigiBaseCollDetSegResp.o \
	CigiBaseCollDetVolDef.o \
	CigiBaseCollDetVolResp.o \
	CigiBaseConfClampEntityCtrl.o\
	CigiBaseEntityCtrl.o\
	CigiBaseEnvCondReq.o\
	CigiBaseEnvCtrl.o\
	CigiBaseEnvRgnCtrl.o\
	CigiBaseEventNotification.o\
	CigiBaseHatHotReq.o\
	CigiBaseIGCtrl.o\
	CigiBaseLosResp.o\
	CigiBaseLosSegReq.o\
	CigiBaseLosVectReq.o\
	CigiBaseMaritimeSurfaceCtrl.o\
	CigiBaseMaritimeSurfaceResp.o\
	CigiBaseMotionTrackCtrl.o\
	CigiBasePositionReq.o\
	CigiBasePositionResp.o\
	CigiBaseSOF.o\
	CigiBaseSensorCtrl.o\
	CigiBaseSensorResp.o\
	CigiBaseShortArtPartCtrl.o\
	CigiBaseShortSymbolCtrl.o\
	CigiBaseSpecEffDef.o\
	CigiBaseSymbolCircleDef.o\
	CigiBaseSymbolClone.o\
	CigiBaseSymbolCtrl.o\
	CigiBaseSymbolLineDef.o\
	CigiBaseSymbolSurfaceDef.o\
	CigiBaseSymbolTextDef.o\
	CigiBaseTerrestrialSurfaceCtrl.o\
	CigiBaseTrajectoryDef.o\
	CigiBaseViewCtrl.o\
	CigiBaseViewDef.o\
	CigiBaseWaveCtrl.o\
	CigiBaseWeatherCondResp.o\
	CigiBaseWeatherCtrl.o\
	CigiCelestialCtrl.o\
	CigiCircleSymbolDataV3_3.o\
	CigiCollDetSegDefV1.o\
	CigiCollDetSegDefV2.o\
	CigiCollDetSegDefV3.o\
	CigiCollDetSegRespV1.o\
	CigiCollDetSegRespV2.o\
	CigiCollDetSegRespV3.o\
	CigiCollDetVolDefV2.o\
	CigiCollDetVolDefV3.o\
	CigiCollDetVolRespV2.o\
	CigiCollDetVolRespV3.o\
	CigiCompCtrlV1.o\
	CigiCompCtrlV2.o\
	CigiCompCtrlV3.o\
	CigiCompCtrlV3_3.o\
	CigiConfClampEntityCtrlV3.o\
	CigiDefaultPacket.o\
	CigiEarthModelDefV3.o\
	CigiEntityCtrlV1.o\
	CigiEntityCtrlV2.o\
	CigiEntityCtrlV3.o\
	CigiEntityCtrlV3_3.o\
	CigiEnvCondReqV3.o\
	CigiEnvCtrlV1.o\
	CigiEnvCtrlV2.o\
	CigiEnvRgnCtrlV3.o\
	CigiEventNotificationV3.o\
	CigiHatHotReqV3.o\
	CigiHatHotReqV3_2.o\
	CigiHatHotRespV3.o\
	CigiHatHotRespV3_2.o\
	CigiHatHotXRespV3.o\
	CigiHatHotXRespV3_2.o\
	CigiHatReqV1.o\
	CigiHatReqV2.o\
	CigiHatRespV1.o\
	CigiHatRespV2.o\
	CigiHostSession.o\
	CigiHotReqV2.o\
	CigiHotRespV2.o\
	CigiIGCtrlV1.o\
	CigiIGCtrlV2.o\
	CigiIGCtrlV3.o\
	CigiIGCtrlV3_2.o\
	CigiIGCtrlV3_3.o\
	CigiIGMsgV2.o\
	CigiIGMsgV3.o\
	CigiIGSession.o\
	CigiIO.o\
	CigiIncomingMsg.o\
	CigiLosRespV1.o\
	CigiLosRespV2.o\
	CigiLosRespV3.o\
	CigiLosRespV3_2.o\
	CigiLosSegReqV1.o\
	CigiLosSegReqV2.o\
	CigiLosSegReqV3.o\
	CigiLosSegReqV3_2.o\
	CigiLosVectReqV1.o\
	CigiLosVectReqV2.o\
	CigiLosVectReqV3.o\
	CigiLosVectReqV3_2.o\
	CigiLosXRespV3.o\
	CigiLosXRespV3_2.o\
	CigiMaritimeSurfaceCtrlV3.o\
	CigiMaritimeSurfaceRespV3.o\
	CigiMessage.o\
	CigiMotionTrackCtrlV3.o\
	CigiOutgoingMsg.o\
	CigiPositionReqV3.o\
	CigiPositionRespV3.o\
	CigiRateCtrlV1.o\
	CigiRateCtrlV2.o\
	CigiRateCtrlV3.o\
	CigiRateCtrlV3_2.o\
	CigiSOFV1.o\
	CigiSOFV2.o\
	CigiSOFV3.o\
	CigiSOFV3_2.o\
	CigiSensorCtrlV1.o\
	CigiSensorCtrlV2.o\
	CigiSensorCtrlV3.o\
	CigiSensorRespV1.o\
	CigiSensorRespV2.o\
	CigiSensorRespV3.o\
	CigiSensorXRespV3.o\
	CigiSession.o\
	CigiShortArtPartCtrlV3.o\
	CigiShortCompCtrlV3.o\
	CigiShortCompCtrlV3_3.o\
	CigiShortSymbolCtrlV3_3.o\
	CigiSpecEffDefV1.o\
	CigiSpecEffDefV2.o\
	CigiSwapping.o\
	CigiSymbolCircleDefV3_3.o\
	CigiSymbolCloneV3_3.o\
	CigiSymbolCtrlV3_3.o\
	CigiSymbolLineDefV3_3.o\
	CigiSymbolSurfaceDefV3_3.o\
	CigiSymbolTextDefV3_3.o\
	CigiTerrestrialSurfaceCtrlV3.o\
	CigiTerrestrialSurfaceRespV3.o\
	CigiTrajectoryDefV1.o\
	CigiTrajectoryDefV2.o\
	CigiTrajectoryDefV3.o\
	CigiVertexSymbolDataV3_3.o\
	CigiViewCtrlV1.o\
	CigiViewCtrlV2.o\
	CigiViewCtrlV3.o\
	CigiViewDefV1.o\
	CigiViewDefV2.o\
	CigiViewDefV3.o\
	CigiWaveCtrlV3.o\
	CigiWeatherCondRespV3.o\
	CigiWeatherCtrlV1.o\
	CigiWeatherCtrlV2.o\
	CigiWeatherCtrlV3.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)

