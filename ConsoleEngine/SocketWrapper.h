#pragma once
#include <mutex>
#include <thread>
#include <vector>

#include "Common.h"

namespace ConsoleGame {
    // Remember to init WSA
    struct SendData {
        char* data;
        size_t length;
    };

    class SocketWrapper {
        static std::atomic_int instanceCount;
        SOCKET inSock;
        SOCKET outSock;
        std::jthread recvWorker;
        std::vector<char> recvBuffer;
        std::mutex recvBufferMutex;
        int recvError = 0;
        int recvBufferCursor = 0;

        // From a MSdoc's example:
        // https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsarecv#example-code
        static constexpr size_t wsaBuffSize = 4096;

       public:
        SocketWrapper();

        static std::string GetHostLocalIP();

        int ListenAndAccept(const std::string& port);
        int Connect(const std::string& addr, const std::string& port);
        void Send(const SendData& sendData);
        void StartRecive();
        // 0 means no error.
        // If the return value is not 0
        // Then go here:
        // https://learn.microsoft.com/en-us/windows/win32/winsock/windows-sockets-error-codes-2
        int GetReciveError();
        // remember to reuse out
        void ReceiveFromBuffer(std::vector<uint8_t>& out);

        ~SocketWrapper();
    };
}  // namespace ConsoleGame
