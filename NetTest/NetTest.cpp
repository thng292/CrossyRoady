#include <queue>

#include "ConsoleGame.h"

using namespace ConsoleGame;

constexpr int hostAddressLength = 256;

class Square {
    Color color;

   public:
    Vec2 coord;

    Square(Color col, Vec2 coord) : color(col), coord(coord){};

    void SetCoord(Vec2 newCoord) { coord = newCoord; }

    void Draw(AbstractCanvas* canvas) const
    {
        for (int i = std::max(0, coord.y);
             i < std::min(_CanvasSize.height, coord.y + 20);
             i++) {
            for (int j = std::max(0, coord.x);
                 j < std::min(_CanvasSize.width, coord.x + 20);
                 j++) {
                (*canvas)[i][j] = color;
            }
        }
    }
};

class TestScreenServer : public AbstractScreen {
    char hostAddress[hostAddressLength] = {0};
    Square local, net;
    std::mutex queueMutex;
    std::vector<int> inputQueue;
    int queueCursor = 0;
    std::jthread worker;
    SOCKET clientSocket;

   public:
    TestScreenServer()
        : local(Color::RED, {30, 30}),
          net(Color::BLUE, {200, 30}),
          inputQueue(512){};

    static const std::wstring_view ScreenName() { return L"TestScreen"; }

    std::wstring_view getName() override { return ScreenName(); }

    void GetLocalIP()
    {
        addrinfo hints = {0};
        addrinfo* res = nullptr;
        gethostname(hostAddress, sizeof(hostAddress));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        int err = getaddrinfo(hostAddress, PORT_STR.data(), &hints, &res);
        if (err != 0) {
            exit(err);
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
    }

    void Init(const std::any& args) override
    {
        GetLocalIP();
        worker = std::jthread([&](std::stop_token stoken) {
            addrinfo hints = {0};
            addrinfo* res;

            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_UDP;
            hints.ai_flags = AI_PASSIVE;
            getaddrinfo(NULL, PORT_STR.data(), &hints, &res);

            SOCKET listenSocket =
                socket(res->ai_family, res->ai_socktype, res->ai_protocol);
            freeaddrinfo(res);
            bind(listenSocket, res->ai_addr, (int)res->ai_addrlen);

            listen(listenSocket, SOMAXCONN);

            clientSocket = accept(listenSocket, NULL, NULL);
            closesocket(listenSocket);

            int err = 0;
            char buff[512] = {0};
            WSABUF wsaBuff{.len = sizeof(buff), .buf = buff};
            DWORD numberOfBytesRecv = 0;
            DWORD flags = 0;
            WSAOVERLAPPED overlapped = {0};
            overlapped.hEvent = WSACreateEvent();
            do {
                err = WSARecvFrom(
                    clientSocket,
                    &wsaBuff,
                    1,
                    &numberOfBytesRecv,
                    &flags,
                    0,
                    0,
                    &overlapped,
                    NULL
                );

                if (err != 0) {
                    err = WSAGetLastError();
                    if (err != WSA_IO_PENDING) {
                        break;
                    } else {
                        err = WSAWaitForMultipleEvents(
                            1, &overlapped.hEvent, TRUE, INFINITE, TRUE
                        );
                        if (err == WSA_WAIT_FAILED) {
                            break;
                        }
                        if (stoken.stop_requested()) {
                            break;
                        }
                        err = WSAGetOverlappedResult(
                            clientSocket,
                            &overlapped,
                            &numberOfBytesRecv,
                            FALSE,
                            &flags
                        );
                        if (err == false) {
                            break;
                        }
                        if (stoken.stop_requested()) {
                            break;
                        }

                        // OK now
                        std::lock_guard lock(queueMutex);
                        int key = 0;
                        int queueCursor = wsaBuff.len / 4;
                        int* arrInput = (int*)wsaBuff.buf;
                        std::copy(
                            inputQueue.begin(),
                            inputQueue.begin() + queueCursor,
                            arrInput
                        );
                    }
                }
            } while (err > 0 && !stoken.stop_requested());
        });
    }

    AbstractScreen* Clone() const override { return new TestScreenServer; }

    AbstractNavigation::NavigationRes Update(
        float deltaTime, const AbstractNavigation* navigation
    ) override
    {
        constexpr int speed = 300;
        int buff[4] = {0};
        DWORD flags = 0;
        WSAOVERLAPPED overlapped = {0};
        overlapped.hEvent = WSACreateEvent();
        WSABUF wsaBuff = {.len = 0, .buf = (char*)buff};
        if (IsKeyDown('W')) {
            local.coord.y -= speed * deltaTime;
            buff[wsaBuff.len++] = 'W';
            wsaBuff.len++;
        }
        if (IsKeyDown('S')) {
            local.coord.y += speed * deltaTime;
            buff[wsaBuff.len++] = 'S';
            wsaBuff.len++;
        }
        if (IsKeyDown('A')) {
            local.coord.x -= speed * deltaTime;
            buff[wsaBuff.len++] = 'A';
            wsaBuff.len++;
        }
        if (IsKeyDown('D')) {
            local.coord.x += speed * deltaTime;
            buff[wsaBuff.len++] = 'D';
            wsaBuff.len++;
        }
        wsaBuff.len *= sizeof(int);
        WSASend(clientSocket, &wsaBuff, 1, NULL, 0, &overlapped, NULL);
        std::lock_guard lock(queueMutex);
        while (--queueCursor >= 0) {
            if (inputQueue[queueCursor] == 'W') {
                net.coord.y -= speed * deltaTime;
            }
            if (inputQueue[queueCursor] == 'S') {
                net.coord.y += speed * deltaTime;
            }
            if (inputQueue[queueCursor] == 'A') {
                net.coord.x -= speed * deltaTime;
            }
            if (inputQueue[queueCursor] == 'D') {
                net.coord.x += speed * deltaTime;
            }
        }
        return navigation->NoChange();
    }

    void Draw(AbstractCanvas* canvas) const override
    {
        Font::DrawString(canvas, hostAddress, {10, 10});
        local.Draw(canvas);
        net.Draw(canvas);
    }
};

class TestScreenClient : public AbstractScreen {
    SOCKET clientSocket;
    Square local, net;
    std::mutex queueMutex;
    std::vector<int> inputQueue;
    int queueCursor = 0;
    std::jthread worker;

   public:
    TestScreenClient()
        : local(Color::BLUE, {200, 30}), net(Color::RED, {30, 30})
    {
    }

    static const std::wstring_view ScreenName() { return L"TestScreenClient"; }

    std::wstring_view getName() override { return ScreenName(); }

    void Init(const std::any& args) override
    {
        addrinfo hints = {0};
        addrinfo* res;
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_UDP;

        int err = getaddrinfo("00000", PORT_STR.data(), &hints, &res);
        defer { freeaddrinfo(res); };
        // Attempt to connect to an address until one succeeds
        for (auto ptr = res; ptr != NULL; ptr = ptr->ai_next) {
            // Create a SOCKET for connecting to server
            clientSocket =
                socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
            if (clientSocket == INVALID_SOCKET) {
                exit(1);
            }

            // Connect to server.
            err = connect(clientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (err == SOCKET_ERROR) {
                closesocket(clientSocket);
                clientSocket = INVALID_SOCKET;
                continue;
            }
            break;
        }

        worker = std::jthread([&](std::stop_token stoken) {
            addrinfo hints = {0};
            addrinfo* res;

            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_UDP;
            hints.ai_flags = AI_PASSIVE;
            getaddrinfo(NULL, PORT_STR.data(), &hints, &res);

            SOCKET listenSocket =
                socket(res->ai_family, res->ai_socktype, res->ai_protocol);
            freeaddrinfo(res);
            bind(listenSocket, res->ai_addr, (int)res->ai_addrlen);

            listen(listenSocket, SOMAXCONN);

            clientSocket = accept(listenSocket, NULL, NULL);
            closesocket(listenSocket);

            int err = 0;
            char buff[512] = {0};
            WSABUF wsaBuff{.len = sizeof(buff), .buf = buff};
            DWORD numberOfBytesRecv = 0;
            DWORD flags = 0;
            WSAOVERLAPPED overlapped = {0};
            overlapped.hEvent = WSACreateEvent();
            do {
                err = WSARecvFrom(
                    clientSocket,
                    &wsaBuff,
                    1,
                    &numberOfBytesRecv,
                    &flags,
                    0,
                    0,
                    &overlapped,
                    NULL
                );

                if (err != 0) {
                    err = WSAGetLastError();
                    if (err != WSA_IO_PENDING) {
                        break;
                    } else {
                        err = WSAWaitForMultipleEvents(
                            1, &overlapped.hEvent, TRUE, INFINITE, TRUE
                        );
                        if (err == WSA_WAIT_FAILED) {
                            break;
                        }
                        if (stoken.stop_requested()) {
                            break;
                        }
                        err = WSAGetOverlappedResult(
                            clientSocket,
                            &overlapped,
                            &numberOfBytesRecv,
                            FALSE,
                            &flags
                        );
                        if (err == false) {
                            break;
                        }
                        if (stoken.stop_requested()) {
                            break;
                        }

                        // OK now
                        std::lock_guard lock(queueMutex);
                        int key = 0;
                        int queueCursor = wsaBuff.len / 4;
                        int* arrInput = (int*)wsaBuff.buf;
                        std::copy(
                            inputQueue.begin(),
                            inputQueue.begin() + queueCursor,
                            arrInput
                        );
                    }
                }
            } while (err > 0 && !stoken.stop_requested());
        });
    }

    AbstractScreen* Clone() const override { return new TestScreenClient; }

    AbstractNavigation::NavigationRes Update(
        float deltaTime, const AbstractNavigation* navigation
    ) override
    {
        constexpr int speed = 300;
        int buff[4] = {0};
        DWORD flags = 0;
        WSAOVERLAPPED overlapped = {0};
        overlapped.hEvent = WSACreateEvent();
        WSABUF wsaBuff = {.len = 0, .buf = (char*)buff};
        if (IsKeyDown('W')) {
            local.coord.y -= speed * deltaTime;
            buff[wsaBuff.len++] = 'W';
            wsaBuff.len++;
        }
        if (IsKeyDown('S')) {
            local.coord.y += speed * deltaTime;
            buff[wsaBuff.len++] = 'S';
            wsaBuff.len++;
        }
        if (IsKeyDown('A')) {
            local.coord.x -= speed * deltaTime;
            buff[wsaBuff.len++] = 'A';
            wsaBuff.len++;
        }
        if (IsKeyDown('D')) {
            local.coord.x += speed * deltaTime;
            buff[wsaBuff.len++] = 'D';
            wsaBuff.len++;
        }
        wsaBuff.len *= sizeof(int);
        WSASend(clientSocket, &wsaBuff, 1, NULL, 0, &overlapped, NULL);
        std::lock_guard lock(queueMutex);
        while (--queueCursor >= 0) {
            if (inputQueue[queueCursor] == 'W') {
                net.coord.y -= speed * deltaTime;
            }
            if (inputQueue[queueCursor] == 'S') {
                net.coord.y += speed * deltaTime;
            }
            if (inputQueue[queueCursor] == 'A') {
                net.coord.x -= speed * deltaTime;
            }
            if (inputQueue[queueCursor] == 'D') {
                net.coord.x += speed * deltaTime;
            }
        }
        return navigation->NoChange();
    }

    void Draw(AbstractCanvas* canvas) const override {}
};

auto main() -> int
{
    WSADATA wsadata;
    int err = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (err != 0) {
        return err;
    }
    defer { WSACleanup(); };
    Font::Load("../CrossyRoady/test.font");
    auto game = std::make_unique<Game>(GetDisplayRefreshRate());
    game->AddScreen(std::make_unique<TestScreenServer>());
    game->Run(TestScreenServer::ScreenName());
    return 0;
}