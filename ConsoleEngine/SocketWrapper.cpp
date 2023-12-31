#include "SocketWrapper.h"

namespace ConsoleGame {
    std::atomic_int SocketWrapper::instanceCount = 0;

    SocketWrapper::SocketWrapper() : recvBuffer(wsaBuffSize, 0)
    {
        if (instanceCount.load() == 0) {
            instanceCount.fetch_add(1);
            WSADATA wsaData = {0};
            int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (err != 0) {
                exit(err);
            }
        } else {
            instanceCount.fetch_add(1);
        }
        sock = INVALID_SOCKET;
    }

    std::string SocketWrapper::GetHostLocalIP()
    {
        char hostAddress[512] = {0};
        addrinfo hints = {0};
        addrinfo* res = nullptr;
        gethostname(hostAddress, sizeof(hostAddress));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        int err = getaddrinfo(hostAddress, PORT_STR.data(), &hints, &res);
        if (err != 0) {
            return "";
        }
        defer { freeaddrinfo(res); };
        void* addr;
        if (res->ai_family == AF_INET) {  // IPv4
            sockaddr_in* ipv4 = (sockaddr_in*)res->ai_addr;
            addr = &(ipv4->sin_addr);
        } else {  // IPv6
            sockaddr_in6* ipv6 = (sockaddr_in6*)res->ai_addr;
            addr = &(ipv6->sin6_addr);
        }
        inet_ntop(res->ai_family, addr, hostAddress, sizeof(hostAddress));
        return std::string(hostAddress);
    }

    int SocketWrapper::Bind(const std::string& port)
    {
        addrinfo hints = {0};
        addrinfo* res;
        int tmp = 0;

        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;
        hints.ai_flags = AI_PASSIVE;
        int err = getaddrinfo(NULL, port.c_str(), &hints, &res);
        if (err != 0) {
            tmp = WSAGetLastError();
            return tmp;
        }
        defer { freeaddrinfo(res); };

        for (auto ptr = res; ptr != NULL; ptr = ptr->ai_next) {
            sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
            if (sock == INVALID_SOCKET) {
                return WSAGetLastError();
            }
            err = bind(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (err == SOCKET_ERROR) {
                return WSAGetLastError();
            }
        }
        return 0;
    }

    int SocketWrapper::Connect(const std::string& addr, const std::string& port)
    {
        addrinfo hints = {0};
        addrinfo* res;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;

        int err = getaddrinfo(addr.c_str(), port.c_str(), &hints, &res);
        if (err == SOCKET_ERROR) {
            return WSAGetLastError();
        }
        defer { freeaddrinfo(res); };

        for (auto ptr = res; ptr != NULL; ptr = ptr->ai_next) {
            sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
            if (sock == INVALID_SOCKET) {
                continue;
            }
            dest = *(ptr->ai_addr);
            destLen = ptr->ai_addrlen;
            break;
        }
        if (sock == INVALID_SOCKET) {
            return WSAGetLastError();
        }
        return 0;
    }

    void SocketWrapper::Send(const SendData& sendData)
    {
        sendto(sock, sendData.data, (int)sendData.length, 0, &dest, destLen);
    }

    void SocketWrapper::StartRecive()
    {
        recvWorker = std::jthread([&](std::stop_token stoken) {
            int err = 0;
            WSAOVERLAPPED overlapped = {0};
            overlapped.hEvent = WSACreateEvent();
            defer { WSACloseEvent(overlapped.hEvent); };
            auto ttt = std::make_unique<char[]>(wsaBuffSize);
            WSABUF wsaBuff = {.len = wsaBuffSize, .buf = ttt.get()};
            DWORD flags = 0;
            DWORD byteTranfered = 0;
            while (!stoken.stop_requested()) {
                err = WSARecvFrom(
                    sock,
                    &wsaBuff,
                    1,
                    NULL,
                    &flags,
                    &dest,
                    &destLen,
                    &overlapped,
                    NULL
                );
                if (err == SOCKET_ERROR) {
                    int tmp = WSAGetLastError();
                    if (tmp != WSA_IO_PENDING) {
                        sError = tmp;
                        break;
                    }
                }
                err = WSAWaitForMultipleEvents(
                    1, &overlapped.hEvent, TRUE, 10, TRUE
                );
                if (err == WSA_WAIT_FAILED) {
                    sError = WSAGetLastError();
                    break;
                }
                if (err == WSA_WAIT_IO_COMPLETION || err == WSA_WAIT_EVENT_0) {
                    if (!WSAGetOverlappedResult(
                            sock, &overlapped, &byteTranfered, TRUE, &flags
                        )) {
                        sError = WSAGetLastError();
                        break;
                    } else {
                        std::lock_guard lock(recvBufferMutex);
                        auto old = recvBufferCursor;
                        recvBufferCursor += byteTranfered;
                        if (recvBufferCursor >= recvBuffer.size()) {
                            recvBuffer.resize(recvBufferCursor, 0);
                        }
                        for (int i = 0; i < byteTranfered; i++) {
                            recvBuffer[i + old] = ttt[i];
                        }
                    }
                }
                if (err == WSA_WAIT_TIMEOUT) {
                    continue;
                }
                WSAResetEvent(overlapped.hEvent);
            }
        });
    }

    int SocketWrapper::GetReciveError() { return sError; }

    void SocketWrapper::ReceiveFromBuffer(std::vector<uint8_t>& out)
    {
        std::lock_guard lock(recvBufferMutex);
        if (out.size() < recvBufferCursor) {
            out.resize(recvBufferCursor, 0);
        }
        std::copy_n(recvBuffer.begin(), recvBufferCursor, out.begin());
        recvBufferCursor = 0;
    }

    SocketWrapper::~SocketWrapper()
    {
        instanceCount.fetch_sub(1);
        if (instanceCount.load() == 0) {
            WSACleanup();
        }
        closesocket(sock);
    }

}  // namespace ConsoleGame