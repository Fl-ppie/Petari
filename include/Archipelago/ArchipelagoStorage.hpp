#pragma once

#include "Game/System/BinaryDataChunkHolder.hpp"

class ArchipelagoStorage : public BinaryDataChunkBase {
public:
    ArchipelagoStorage();

    virtual u32 makeHeaderHashCode() const;
    virtual u32 getSignature() const;
    virtual s32 serialize(u8*, u32) const;
    virtual s32 deserialize(const u8*, u32);
    virtual void initializeData();

    u32 getPowerStarCount();
    void addPowerStarCount();
    void setPowerStarCount(u32);
    u32 getGrandStarCount();
    void addGrandStarCount();
    void setGrandStarCount(u32);
    u32 getGreenStarCount();
    void addGreenStarCount();
    void setGreenStarCount(u32);
    u32 getLastRecvIndex();
    void addLastRecvIndex();
    void setLastRecvIndex(u32);

    /* 0x04 */ u32 mPowerStarCount;
    /* 0x08 */ u32 mGrandStarCount;
    /* 0x0C */ u32 mGreenStarCount;
    /* 0x10 */ u32 mLastRecvIndex;
};
