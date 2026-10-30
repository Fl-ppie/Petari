#include "Archipelago/Network.hpp"
#include <revolution/os.h>
#include <cstring>

Network::Network() : mDeviceFd(-1), mSocketFd(-1), mAddress() {
}

void Network::init() {
    mDeviceFd = open();

    if (mDeviceFd >= 0) {
        mSocketFd = socket(AF_INET, SOCK_STREAM);
    }
}

IOSError Network::open(u32 flags) {
    return IOS_Open(IOS_PATH, flags);
}

IOSError Network::socket(Family family, Type type, Protocol protocol) {
    if (mDeviceFd < 0) {
        return IOS_ERROR_NOEXISTS;
    }

    struct {
        Family family;
        Type type;
        Protocol protocol;
    } params ATTRIBUTE_ALIGN(32);

    params.family = family;
    params.type = type;
    params.protocol = protocol;

    IOSError error = IOS_Ioctl(mDeviceFd, IOCTL_SOCKET, &params, sizeof(params), nullptr, 0);

    if (error < 0) {
        return error;
    }

    mSocketFd = error;

    return error;
}

IOSError Network::setNonBlocking(bool isNonBlocking) {
    if (mDeviceFd < 0) {
        return IOS_ERROR_NOEXISTS;
    }

    if (mSocketFd < 0) {
        return IOS_ERROR_NOEXISTS;
    }

    struct {
        IOSFd fd;
        s32 cmd;
        s32 value;
    } params ATTRIBUTE_ALIGN(32);
    params.fd = mSocketFd;
    params.cmd = 4;
    params.value = isNonBlocking ? 4 : 0;

    return IOS_Ioctl(mDeviceFd, IOCTL_FCNTL, &params, sizeof(params), nullptr, 0);
}

IOSError Network::connect() {
    if (mDeviceFd < 0) {
        return IOS_ERROR_NOEXISTS;
    }

    if (mSocketFd < 0) {
        return IOS_ERROR_NOEXISTS;
    }

    struct {
        IOSFd fd;
        s32 hasAddress;
        SocketAddress address;
    } params ATTRIBUTE_ALIGN(32);

    params.fd = mSocketFd;
    params.hasAddress = true;
    memcpy(&params.address, mAddress, sizeof(params.address));

    return IOS_Ioctl(mDeviceFd, IOCTL_CONNECT, &params, sizeof(params), nullptr, 0);
}

IOSError Network::send(const void* pData, s32 size, u32 flags) {
    if (mDeviceFd < 0) {
        return IOS_ERROR_NOEXISTS;
    }

    if (mSocketFd < 0) {
        return IOS_ERROR_NOEXISTS;
    }

    if (mAddress == nullptr) {
        return IOS_ERROR_NOEXISTS;
    }

    struct {
        IOSFd fd;
        u32 flags;
        s32 hasAddress;
        SocketAddress address;
    } params ATTRIBUTE_ALIGN(32);

    params.fd = mSocketFd;
    params.flags = flags;
    params.hasAddress = true;
    memcpy(&params.address, mAddress, sizeof(params.address));

    IOSIoVector vectors[2] ATTRIBUTE_ALIGN(32);
    vectors[0].base = const_cast< u8* >(static_cast< const u8* >(pData));
    vectors[0].length = size;
    vectors[1].base = reinterpret_cast< u8* >(&params);
    vectors[1].length = sizeof(params);

    IOSError error = IOS_Ioctlv(mDeviceFd, IOCTL_SEND, 2, 0, vectors);

    return error;
}

IOSError Network::recv(void* pDst, s32 size, u32 flags) {
    if (mDeviceFd < 0) {
        return IOS_ERROR_NOEXISTS;
    }

    if (mSocketFd < 0) {
        return IOS_ERROR_NOEXISTS;
    }

    struct {
        IOSFd fd;
        u32 flags;
    } params ATTRIBUTE_ALIGN(32);

    params.fd = mSocketFd;
    params.flags = flags;

    IOSIoVector vectors[2] ATTRIBUTE_ALIGN(32);
    vectors[0].base = reinterpret_cast< u8* >(&params);
    vectors[0].length = sizeof(params);
    vectors[1].base = static_cast< u8* >(pDst);
    vectors[1].length = size;

    return IOS_Ioctlv(mDeviceFd, IOCTL_RECV, 1, 1, vectors);
}

IOSError Network::close() {
    if (mDeviceFd < 0) {
        return IOS_ERROR_NOEXISTS;
    }

    if (mSocketFd < 0) {
        return IOS_ERROR_NOEXISTS;
    }

    struct {
        IOSFd fd;
    } params ATTRIBUTE_ALIGN(32);

    params.fd = mSocketFd;

    IOSError error = IOS_Ioctl(mDeviceFd, IOCTL_CLOSE, &params, sizeof(params), nullptr, 0);

    if (error < 0) {
        return error;
    }

    error = IOS_Close(mDeviceFd);

    if (error >= 0) {
        mDeviceFd = -1;
        mSocketFd = -1;
        mAddress = nullptr;
    }

    return error;
}

Network::~Network() {
    close();
}
