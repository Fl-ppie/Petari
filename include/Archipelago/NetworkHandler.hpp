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

class APPacket {
public:
    APPacket();

    virtual ~APPacket();

    /* 0x4 */ u16 mSize;
    /* 0x8 */ PacketType mType;
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

class SyncPacket : public APPacket {
public:
    SyncPacket();

    virtual ~SyncPacket();
};

class StarPacket : public APPacket {
public:
    StarPacket(StarType starType, u8 galaxyNo, u8 scenarioNo) : mStarType(starType), mGalaxyNo(galaxyNo), mScenarioNo(scenarioNo) {
    }

    virtual ~StarPacket();

    /* 0x0C */ StarType mStarType;
    /* 0x10 */ u8 mGalaxyNo;
    /* 0x11 */ u8 mScenarioNo;
};

class DeathPacket : public APPacket {
public:
    DeathPacket();

    virtual ~DeathPacket();
};

class LivesPacket : public APPacket {
public:
    LivesPacket();

    virtual ~LivesPacket();
};
