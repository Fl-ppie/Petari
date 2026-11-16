#include "Archipelago/PatchHandler.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    const char* sPatchFile = "Patch.arc";
};

PatchHandler::PatchHandler() : mPatchInfo(), mNetworkInfo() {
    mPatchInfo = new JMapInfo();
    mNetworkInfo = new JMapInfo();
}

void PatchHandler::init() {
    mPatchInfo->attach(MR::loadResourceFromArc(::sPatchFile, "Patch.bcsv"));
    mNetworkInfo->attach(MR::loadResourceFromArc(::sPatchFile, "Network.bcsv"));
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

u32 PatchHandler::getAddress() {
    u32 address;
    if (mNetworkInfo->getValue(0, "address", &address)) {
        return address;
    }

    return 0;
}

u32 PatchHandler::getPort() {
    u32 port;
    if (mNetworkInfo->getValue(0, "port", &port)) {
        return port;
    }

    return 0;
}
