#include "Archipelago/PatchHandler.hpp"
#include "Game/Util/ObjUtil.hpp"

PatchHandler::PatchHandler() : mInfo() {
    mInfo = new JMapInfo();
}

void PatchHandler::init() {
    mInfo->attach(MR::loadResourceFromArc("Patch.arc", "Patch.bcsv"));
}

const char* PatchHandler::getNewGalaxy(const char* pGalaxyName) {
    const char* pNewName = nullptr;
    getIter(pGalaxyName).getValue("new_name", &pNewName);
    return pNewName;
}

s32 PatchHandler::getGalaxyType(const char* pGalaxyName) {
    s32 type = -1;
    if (getIter(pGalaxyName).getValue("type", &type)) {
        return type;
    }

    return -1;
}

s32 PatchHandler::getGalaxyUnlock(const char* pGalaxyName) {
    char buf[64] = "Mini";
    strcpy(buf + 4, pGalaxyName);

    s32 unlock = -1;
    if (getIter(buf).getValue("unlock", &unlock)) {
        return unlock;
    }

    return -1;
}

f32 PatchHandler::getGalaxyOrbitRadius(const char* pGalaxyName) {
    f32 radius = 0.0f;
    if (getIter(pGalaxyName).getValue("radius", &radius)) {
        return radius;
    }

    return 0.0f;
}

f32 PatchHandler::getGalaxyAngle(const char* pGalaxyName) {
    f32 angle = 0.0f;
    if (getIter(pGalaxyName).getValue("angle", &angle)) {
        return angle;
    }

    return 0.0f;
}

u32 PatchHandler::getReturnDome(const char* pNewGalaxyName) {
    s32 returnDome = 0;
    if (getNewIter(pNewGalaxyName).getValue("dome", &returnDome)) {
        return returnDome;
    }

    return 0;
}
