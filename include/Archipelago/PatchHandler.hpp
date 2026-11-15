#pragma once

#include "Game/Util/JMapInfo.hpp"

class PatchHandler {
public:
    PatchHandler();

    void init();
    const char* getNewGalaxy(const char*);
    s32 getGalaxyType(const char*);
    s32 getGalaxyUnlock(const char*);
    f32 getGalaxyOrbitRadius(const char*);
    f32 getGalaxyAngle(const char*);
    u32 getReturnDome(const char*);

    u32 getAddress();
    u32 getPort();

    JMapInfoIter getIter(const char* pName) {
        return mPatchInfo->findElement("name", pName, 0);
    }

    JMapInfoIter getNewIter(const char* pNewName) {
        return mPatchInfo->findElement("new_name", pNewName, 0);
    }

    /* 0x0 */ JMapInfo* mPatchInfo;
    /* 0x4 */ JMapInfo* mNetworkInfo;
};
