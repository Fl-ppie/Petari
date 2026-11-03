#include "Archipelago/ArchipelagoStorage.hpp"
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>

ArchipelagoStorage::ArchipelagoStorage() {
}

u32 ArchipelagoStorage::makeHeaderHashCode() const {
    return 'APSG';
}

u32 ArchipelagoStorage::getSignature() const {
    return 'AP  ';
}

s32 ArchipelagoStorage::serialize(u8* pBuffer, u32 size) const {
    JSUMemoryOutputStream stream = JSUMemoryOutputStream(pBuffer, size);

    stream.write(&mPowerStarCount, sizeof(mPowerStarCount));
    stream.write(&mGrandStarCount, sizeof(mGrandStarCount));
    stream.write(&mGreenStarCount, sizeof(mGreenStarCount));
    stream.write(&mLastRecvIndex, sizeof(mLastRecvIndex));

    return stream.mPosition;
}

s32 ArchipelagoStorage::deserialize(const u8* pBuffer, u32 size) {
    initializeData();

    JSUMemoryInputStream stream = JSUMemoryInputStream(pBuffer, size);

    stream.read(&mPowerStarCount, sizeof(mPowerStarCount));
    stream.read(&mGrandStarCount, sizeof(mGrandStarCount));
    stream.read(&mGreenStarCount, sizeof(mGreenStarCount));
    stream.read(&mLastRecvIndex, sizeof(mLastRecvIndex));

    return 0;
}

void ArchipelagoStorage::initializeData() {
    mPowerStarCount = 0;
    mGrandStarCount = 0;
    mGreenStarCount = 0;
    mLastRecvIndex = 0;
}

u32 ArchipelagoStorage::getPowerStarCount() {
    return mPowerStarCount;
}

void ArchipelagoStorage::addPowerStarCount() {
    mPowerStarCount++;
}

void ArchipelagoStorage::setPowerStarCount(u32 num) {
    mPowerStarCount = num;
}

u32 ArchipelagoStorage::getGrandStarCount() {
    return mGrandStarCount;
}

void ArchipelagoStorage::addGrandStarCount() {
    mGrandStarCount++;
}

void ArchipelagoStorage::setGrandStarCount(u32 num) {
    mGrandStarCount = num;
}

u32 ArchipelagoStorage::getGreenStarCount() {
    return mGreenStarCount;
}

void ArchipelagoStorage::addGreenStarCount() {
    mGreenStarCount++;
}

void ArchipelagoStorage::setGreenStarCount(u32 num) {
    mGreenStarCount = num;
}

u32 ArchipelagoStorage::getLastRecvIndex() {
    return mLastRecvIndex;
}

void ArchipelagoStorage::addLastRecvIndex() {
    mLastRecvIndex++;
}

void ArchipelagoStorage::setLastRecvIndex(u32 num) {
    mLastRecvIndex = num;
}
