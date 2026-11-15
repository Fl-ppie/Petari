#include "Archipelago/NetworkHandler.hpp"
#include "Archipelago/ArchipelagoStorage.hpp"
#include "Archipelago/Deathlink.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include <revolution/os.h>

NetworkHandler::NetworkHandler() : mReconnectTimer(RECONNECT_TIMEOUT), mWrapper(), mSendQueue() {
}

void NetworkHandler::init(u32 address, u16 port) {
    u8* octets = reinterpret_cast< u8* >(&address);

    if (address == 0 || port == 0) {
        OSPanic(__FILE__, __LINE__, "Invalid address: %d.%d.%d.%d:%d", octets[0], octets[1], octets[2], octets[3], port);
    }

    init(octets[0], octets[1], octets[2], octets[3], port);
}

void NetworkHandler::init(u8 ipv41, u8 ipv42, u8 ipv43, u8 ipv44, u16 port) {
    mWrapper = new NetworkWrapper();
    mWrapper->init();
    mWrapper->setNonBlocking(true);

    mWrapper->mAddress = new SocketAddress(AF_INET, port, ipv41, ipv42, ipv43, ipv44);
    mWrapper->connect();
}

void NetworkHandler::update() {
    if (mWrapper == nullptr) {
        return;
    }

    IOSError poll = mWrapper->poll();

    if (poll < 0) {
        return;
    } else if (poll != POLL_ERR) {
        reconnect();
        return;
    }

    u8 recvbuf[512];
    mWrapper->recv(&recvbuf, sizeof(APPacket));

    APPacket* packet = reinterpret_cast< APPacket* >(&recvbuf);
    if (packet->mType != PacketType_NULL) {
        mWrapper->recv(packet + 1, packet->mSize - sizeof(APPacket));
        recvPacket(packet);
    }

    if (mSendQueue.size() != 0) {
        for (APPacket* packet = *mSendQueue.begin(); packet != *mSendQueue.end(); packet++) {
            mWrapper->send(packet, packet->mSize);
            delete packet;
        }

        mSendQueue.clear();
    }
}

void NetworkHandler::recvPacket(APPacket* pPacket) {
    switch (pPacket->mType) {
    case PacketType_Star:
        switch (static_cast< StarPacket* >(pPacket)->mStarType) {
        case StarType_PowerStar:
            GameDataFunction::getArchipelagoStorage()->addPowerStarCount();
            break;
        case StarType_GrandStar:
            GameDataFunction::getArchipelagoStorage()->addGrandStarCount();
            break;
        case StarType_GreenStar:
            GameDataFunction::getArchipelagoStorage()->addGreenStarCount();
            break;
        }
        break;
    case PacketType_Deathlink:
        SingletonHolder< Deathlink >::get()->recvDeath();
        break;
    case PacketType_Lives:
        GameDataFunction::addPlayerLeft(1);
        break;
    default:
        break;
    }
}

void NetworkHandler::sendPacket(APPacket* pPacket) {
    mSendQueue.push_back(pPacket);
}

NetworkHandler::~NetworkHandler() {
    delete mWrapper;
}
