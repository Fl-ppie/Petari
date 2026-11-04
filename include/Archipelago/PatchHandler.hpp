#pragma once

class JMapInfo;

class PatchHandler {
public:
    PatchHandler();

    void init();

    /* 0x0 */ JMapInfo* mInfo;
};
