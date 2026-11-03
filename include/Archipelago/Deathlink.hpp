#pragma once

#include <revolution/types.h>

#define DEATH_TIMEOUT 600

class Deathlink {
public:
    Deathlink();

    void update();
    void recvDeath();
    void sendDeath();

    /* 0x0 */ u32 mDeathTimer;
    /* 0x4 */ u32 mDeathBuffer;
};
