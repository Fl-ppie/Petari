#include "Archipelago/NetworkHandler.hpp"
#include "Archipelago/Deathlink.hpp"
#include "Game/Util/SingletonHolder.hpp"

NetworkHandler::NetworkHandler() : mWrapper(), mSendQueue() {
}

void NetworkHandler::init(Family family, u32 port, u8 ipv41, u8 ipv42, u8 ipv43, u8 ipv44) {
    mWrapper = new NetworkWrapper();
    mWrapper->init();
    mWrapper->setNonBlocking(true);

    mWrapper->mAddress = new SocketAddress(family, port, ipv41, ipv42, ipv43, ipv44);
    mWrapper->connect();
}

void NetworkHandler::update() {
    APPacket packets[PACKET_BUFFER];
    mWrapper->recv(&packets, sizeof(packets));

    for (APPacket* packet = packets; packet->mType != 0; packet++) {
        recvPacket(packet);
    }

    if (mSendQueue.size() != 0) {
        mWrapper->send(&mSendQueue, mSendQueue.size() * sizeof(*mSendQueue.begin()));
        mSendQueue.clear();
    }
}

void NetworkHandler::recvPacket(APPacket* pPacket) {
    switch (pPacket->mType) {
    case PacketType_PowerStar:
        break;
    case PacketType_GrandStar:
        break;
    case PacketType_GreenStar:
        break;
    case PacketType_Deathlink:
        SingletonHolder< Deathlink >::get()->recvDeath();
        break;
    }
}

void NetworkHandler::sendPacket(const APPacket& rPacket) {
    mSendQueue.push_back(rPacket);
}

NetworkHandler::~NetworkHandler() {
    delete mWrapper;
}
