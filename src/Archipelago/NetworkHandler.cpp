#include "Archipelago/NetworkHandler.hpp"
#include "Archipelago/ArchipelagoStorage.hpp"
#include "Archipelago/Deathlink.hpp"
#include "Archipelago/NetworkWrapper.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"

namespace {
    void printPacketData(APPacket* pPacket) {
        for (s32 i = 0; i < pPacket->mSize; i++) {
            OSReport("%02X", *(reinterpret_cast< u8* >(pPacket) + i));
        }
    }
};  // namespace

NetworkHandler::NetworkHandler() : mReconnectTimer(RECONNECT_TIMEOUT), mWrapper(), mSendQueue() {
    mSendQueue.init(32);
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

    OSReport("Starting connection...\n");

    mWrapper->connect();
}

void NetworkHandler::update() {
    if (mWrapper == nullptr) {
        return;
    }

    IOSError poll = mWrapper->poll();

    if (poll < 0) {
        OSReport("Network not initialised.\n");
        return;
    } else if (poll & POLL_UNK1 || poll & POLL_UNK2) {
        reconnect();
        return;
    }

    if (!mWrapper->mIsConnected) {
        OSReport("Connected!\n");
    }

    mWrapper->mIsConnected = true;

    if (poll & POLL_IN) {
        u8 recvbuf[32];
        MR::zeroMemory(&recvbuf, sizeof(recvbuf));
        mWrapper->recv(&recvbuf, sizeof(APPacket));

        APPacket* packet = reinterpret_cast< APPacket* >(&recvbuf);
        if (packet->mType != PacketType_NULL) {
            mWrapper->recv(packet + 1, packet->mSize - sizeof(APPacket));

            OSReport("Received packet data: ");
            ::printPacketData(packet);
            OSReport("\n");

            recvPacket(packet);
        }
    }

    if (mSendQueue.size() != 0) {
        for (APPacket** packet = mSendQueue.begin(); packet != mSendQueue.end(); packet++) {
            mWrapper->send(*packet, (*packet)->mSize);

            OSReport("Sent packet data: ");
            ::printPacketData(*packet);
            OSReport("\n");

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
            OSReport("Received Powerstar!\n");
            GameDataFunction::getArchipelagoStorage()->addPowerStarCount();
            break;
        case StarType_GrandStar:
            OSReport("Received Grandstar!\n");
            GameDataFunction::getArchipelagoStorage()->addPowerStarCount();
            GameDataFunction::getArchipelagoStorage()->addGrandStarCount();
            break;
        case StarType_GreenStar:
            OSReport("Received Greenstar!\n");
            GameDataFunction::getArchipelagoStorage()->addPowerStarCount();
            GameDataFunction::getArchipelagoStorage()->addGreenStarCount();
            break;
        }
        break;
    case PacketType_Deathlink:
        OSReport("Received deathlink :-(\n");
        SingletonHolder< Deathlink >::get()->recvDeath();
        break;
    case PacketType_Lives:
        OSReport("Received a life! :-)\n");
        GameDataFunction::addPlayerLeft(1);
        break;
    default:
        OSReport("Unknown packet :/\n");
        break;
    }
}

void NetworkHandler::sendPacket(APPacket* pPacket) {
    mSendQueue.push_back(pPacket);
}

NetworkHandler::~NetworkHandler() {
    delete mWrapper;
}
