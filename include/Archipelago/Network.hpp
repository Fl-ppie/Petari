#pragma once

#include <revolution/ipc/ipcclt.h>

#define IOS_PATH "/dev/net/ip/top"

enum Family {
    AF_UNSPEC = 0x0,
    AF_LOCAL = 0x1,
    AF_INET = 0x2,
};

enum Type {
    SOCK_STREAM = 0x1,
    SOCK_DGRAM = 0x2,
    SOCK_RAW = 0x3,
    SOCK_RDM = 0x4,
    SOCK_SEQPACKET = 0x5,
};

enum Protocol {
    IPPROTO_IP = 0x0,
};

enum IoctlCmd {
    IOCTL_ACCEPT = 0x1,
    IOCTL_BIND = 0x2,
    IOCTL_CLOSE = 0x3,
    IOCTL_CONNECT = 0x4,
    IOCTL_FCNTL = 0x5,
    IOCTL_GET_SOCK_NAME = 0x7,
    IOCTL_GET_SOCK_OPT = 0x8,
    IOCTL_SET_SOCK_OPT = 0x9,
    IOCTL_LISTEN = 0xA,
    IOCTL_POLL = 0xB,
    IOCTL_RECV = 0xC,
    IOCTL_SEND = 0xD,
    IOCTL_SHUTDOWN = 0xE,
    IOCTL_SOCKET = 0xF,
    IOCTL_GET_HOST_IP = 0x10,
    IOCTL_GET_HOST_BY_NAME = 0x11,
};

enum MsgFlag {
    MSG_NULL = 0x0,
    MSG_OOB = 0x1,
    MSG_PEEK = 0x2,
    MSG_DONTROUTE = 0x4,
    MSG_CTRUNC = 0x8,
    MSG_TRUNC = 0x20,
    MSG_WAITALL = 0x100,
};

struct SocketAddress {
    SocketAddress() : mLength(sizeof(SocketAddress)), mFamily(), mPort(), mAddress() {
    }

    SocketAddress(Family family, u16 port, u32 address) : mLength(sizeof(SocketAddress)), mFamily(family), mPort(port), mAddress(address) {
    }

    SocketAddress(Family family, u16 port, u8 octet1, u8 octet2, u8 octet3, u8 octet4) : mLength(sizeof(SocketAddress)), mFamily(family), mPort(port) {
        mOctets[0] = octet1;
        mOctets[1] = octet2;
        mOctets[2] = octet3;
        mOctets[3] = octet4;
    }

    /* 0x0 */ u8 mLength;
    /* 0x1 */ u8 mFamily;
    /* 0x2 */ u16 mPort;
    /* 0x4 */ union {
        u32 mAddress;
        u8 mOctets[4];
    };
};

class Network {
public:
    Network();

    virtual ~Network();

    void init();
    IOSError open(u32 flags = 0);
    IOSError socket(Family family, Type type, Protocol protocol = IPPROTO_IP);
    IOSError setNonBlocking(bool isNonBlocking);
    IOSError connect();
    IOSError send(const void* pData, s32 size, u32 flags = 0);
    IOSError recv(void* pDst, s32 size, u32 flags = 0);
    IOSError close();

    /* 0x04 */ IOSFd mDeviceFd;
    /* 0x08 */ IOSFd mSocketFd;
    /* 0x0C */ SocketAddress* mAddress;
};
