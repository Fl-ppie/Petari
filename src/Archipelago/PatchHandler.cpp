#include "Archipelago/PatchHandler.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/ObjUtil.hpp"

PatchHandler::PatchHandler() : mInfo() {
    mInfo = new JMapInfo();
}

void PatchHandler::init() {
    mInfo->attach(MR::loadResourceFromArc("Patch.arc", "Patch.bcsv"));
}
