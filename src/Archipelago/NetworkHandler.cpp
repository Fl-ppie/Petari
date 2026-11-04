#include "Archipelago/NetworkHandler.hpp"
#include "Archipelago/ArchipelagoStorage.hpp"
#include "Archipelago/Deathlink.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/Util/SingletonHolder.hpp"

NetworkHandler::NetworkHandler() : mWrapper(), mSendQueue() {
}

void NetworkHandler::init(u8 ipv41, u8 ipv42, u8 ipv43, u8 ipv44, u32 port) {
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
        GameDataFunction::getArchipelagoStorage()->addPowerStarCount();
        break;
    case PacketType_GrandStar:
        GameDataFunction::getArchipelagoStorage()->addGrandStarCount();
        break;
    case PacketType_GreenStar:
        GameDataFunction::getArchipelagoStorage()->addGreenStarCount();
        break;
    case PacketType_Deathlink:
        SingletonHolder< Deathlink >::get()->recvDeath();
        break;
    case PacketType_Lives:
        GameDataFunction::addPlayerLeft(1);
    }
}

void NetworkHandler::sendPacket(const APPacket& rPacket) {
    mSendQueue.push_back(rPacket);
}

NetworkHandler::~NetworkHandler() {
    delete mWrapper;
}
