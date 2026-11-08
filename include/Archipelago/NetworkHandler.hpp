#pragma once

#include "Archipelago/NetworkWrapper.hpp"
#include "Game/Util/Array.hpp"

#define RECONNECT_TIMEOUT 60 * 5

enum PacketType {
    /* 0x0 */ PacketType_NULL,
    /* 0x1 */ PacketType_Sync,
    /* 0x2 */ PacketType_Star,
    /* 0x3 */ PacketType_Deathlink,
    /* 0x4 */ PacketType_Lives,
};

enum StarType {
    /* 0x0 */ StarType_PowerStar,
    /* 0x1 */ StarType_GrandStar,
    /* 0x2 */ StarType_GreenStar,
};

struct APPacket {
    APPacket(u16 size, PacketType type) : mSize(size), mType(type) {
    }

    /* 0x0 */ u16 mSize;
    /* 0x4 */ PacketType mType;
};

class NetworkHandler {
public:
    NetworkHandler();

    virtual ~NetworkHandler();

    void init(u8 ipv41, u8 ipv42, u8 ipv43, u8 ipv44, u32 port);
    void update();
    void recvPacket(APPacket*);
    void sendPacket(APPacket*);

    IOSError reconnect() {
        if (mWrapper == nullptr) {
            return IOS_ERROR_NOEXISTS;
        }

        if (--mReconnectTimer != 0) {
            return IOS_ERROR_NOTREADY;
        }

        mReconnectTimer = RECONNECT_TIMEOUT;

        return mWrapper->connect();
    }

    /* 0x4 */ u32 mReconnectTimer;
    /* 0x8 */ NetworkWrapper* mWrapper;
    /* 0xC */ MR::Vector< MR::AssignableArray< APPacket* > > mSendQueue;
};

struct SyncPacket : public APPacket {
    SyncPacket() : APPacket(sizeof(SyncPacket), PacketType_Sync) {
    }
};

struct StarPacket : public APPacket {
    StarPacket(StarType starType, u8 galaxyNo, u8 scenarioNo)
        : APPacket(sizeof(StarPacket), PacketType_Star), mStarType(starType), mGalaxyNo(galaxyNo), mScenarioNo(scenarioNo) {
    }

    /* 0x8 */ StarType mStarType;
    /* 0xC */ u8 mGalaxyNo;
    /* 0xD */ u8 mScenarioNo;
};

struct DeathPacket : public APPacket {
    DeathPacket() : APPacket(sizeof(DeathPacket), PacketType_Deathlink) {
    }
};

struct LivesPacket : public APPacket {
    LivesPacket() : APPacket(sizeof(LivesPacket), PacketType_Lives) {
    }
};
