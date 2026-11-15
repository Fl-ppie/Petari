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

    JMapInfoIter getIter(const char* pName) {
        return mInfo->findElement("name", pName, 0);
    }

    JMapInfoIter getNewIter(const char* pNewName) {
        return mInfo->findElement("new_name", pNewName, 0);
    }

    /* 0x0 */ JMapInfo* mInfo;
};
