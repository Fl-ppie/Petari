#pragma once

#include "JSystem/J3DGraphAnimator/J3DModel.hpp"
#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"
#include "JSystem/JGeometry/TBox.hpp"
#include <revolution.h>

class J3DModel;
class J3DMaterial;
class LiveActor;
struct ResTIMG;
class XanimePlayer;
class ResourceHolder;
class XanimeResourceTable;

namespace MR {
    void newXanimePlayer(const ResourceHolder*, const char*, const ResourceHolder*, J3DMdlFlag, XanimeResourceTable*);
    XanimeResourceTable* newXanimeResourceTable(ResourceHolder*);

    void invalidateMtxCalc(J3DModelData*);

    bool isExistModel(const char*);

    J3DModel* getJ3DModel(const LiveActor*);
    J3DModelData* getJ3DModelData(const LiveActor*);
    J3DModelData* getJ3DModelData(const char*);

    void initEnvelopeAndEnvMapOrProjMapModelData(J3DModelData*);

    ResTIMG* getResTIMG(const LiveActor*, int);
    ResTIMG* getResTIMG(const J3DModelData*, int);

    void downFracVtx(J3DModelData*);

    void calcJ3DModel(LiveActor*);

    u16 getMaterialNo(J3DModelData*, const char*);
    u16 getMaterialNo(J3DModel*, const char*);
    J3DMaterial* getMaterial(J3DModelData*, const char*);
    J3DMaterial* getMaterial(J3DModelData*, int);
    J3DMaterial* getMaterial(J3DModel*, int);
    J3DMaterial* getMaterial(const LiveActor*, int);
    s32 getMaterialNum(J3DModel*);
    const char* getMaterialName(const J3DModelData*, int);
    void updateModelDiffDL(LiveActor*);
    bool isEnvelope(J3DMaterial*);
    void hideMaterial(J3DModel*, const char*);
    void hideMaterial(const LiveActor*, const char*);
    void showMaterial(J3DModel*, const char*);
    void showMaterial(const LiveActor*, const char*);

    void calcFogStartEnd(TVec3f, f32, f32*, f32*);

    bool isUseTex(J3DMaterial*, u16);
    bool isNormalTexMtx(J3DMaterial*);
    bool isUseTexMtx(J3DMaterial*, u32);

    void updateMaterial(LiveActor*);

    void syncJointAnimation(LiveActor*, const LiveActor*);

    void syncMaterialAnimation(LiveActor*, const LiveActor*);

    void copyJointAnimation(LiveActor*, const LiveActor*);

    void calcModelBoundingBox(TBox3f*, const LiveActor*);
    void calcModelBoundingRadius(f32*, const LiveActor*);

    bool isExistCollisionResource(const LiveActor*, const char*);
    bool isExistModel(const char*);
    bool isExistSubModel(const char*, const char*);

    bool isExistEffectTexMtx(LiveActor*);

    void getLightNum(J3DMaterial*, s32*, s32*, s32*, s32*);
    s32 getLightNum(J3DMaterial*);
    s32 getLightNumMax(const LiveActor*);
    s32 getLightNumMax(J3DModelData*);

    bool isUseFur(const J3DModelData*);

    s16 getBckFrameMax(const LiveActor*, const char*);
    s16 getBrkFrameMax(const LiveActor*, const char*);
    s16 getBvaFrameMax(const LiveActor*, const char*);
    bool isBckPlaying(XanimePlayer*, const char*);

    bool findBckNameStringInResource(const char**, const ResourceHolder*, const char*);

    void updateModelManager(LiveActor*);
    void updateModelAnimPlayer(LiveActor*);
    void calcAnimModelManager(LiveActor*);
};  // namespace MR
