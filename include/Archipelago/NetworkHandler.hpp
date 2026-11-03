#pragma once

#include "Archipelago/NetworkWrapper.hpp"
#include "Game/Util/Array.hpp"

#define PACKET_BUFFER 32

enum PacketType {
    PacketType_PowerStar = 0x1,
    PacketType_GrandStar = 0x2,
    PacketType_GreenStar = 0x3,
};

struct APPacket {
    APPacket() : mType(), mValue() {
    }

    APPacket(PacketType type, u16 value) : mType(type), mValue(value) {
    }

    /* 0x0 */ u8 mType;
    /* 0x2 */ u16 mValue;
};

class NetworkHandler {
public:
    NetworkHandler();

    virtual ~NetworkHandler();
    
    void init(Family family, u32 port, u8 ipv41, u8 ipv42, u8 ipv43, u8 ipv44);
    void update();
    void recvPacket(APPacket*);
    void sendPacket(const APPacket&);

    /* 0x4 */ NetworkWrapper* mWrapper;
    /* 0x8 */ MR::Vector< MR::FixedArray<APPacket, PACKET_BUFFER> > mSendQueue;
};
