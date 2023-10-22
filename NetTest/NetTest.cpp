#include <thread>

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
    Square local, net;
    Color idid = Color::BLACK;
    std::string hostAddress;
    SocketWrapper sockWrap;
    std::jthread listenThrd;

    std::vector<uint8_t> netInputBuff;

   public:
    TestScreenServer()
        : local(Color::RED, {30, 30}), net(Color::BLUE, {200, 30})
    {
    }

    static const std::wstring_view ScreenName() { return L"TestScreenServer"; }

    std::wstring_view getName() override { return ScreenName(); }

    void Init(const std::any& args) override
    {
        hostAddress = sockWrap.GetHostLocalIP();
        listenThrd = std::jthread([&] {
            int err = sockWrap.ListenAndAccept(std::string(PORT_STR));
            if (err != 0) {
                exit(err);
            }
            idid = Color::GREEN;
            sockWrap.StartRecive();
        });
        listenThrd.detach();
    }

    AbstractScreen* Clone() const override { return new TestScreenServer; }

    AbstractNavigation::NavigationRes Update(
        float deltaTime, const AbstractNavigation* navigation
    ) override
    {
        constexpr int speed = 300;
        size_t cursor = 0;
        if (IsKeyDown('W')) {
            local.coord.y -= speed * deltaTime;
        }
        if (IsKeyDown('S')) {
            local.coord.y += speed * deltaTime;
        }
        if (IsKeyDown('A')) {
            local.coord.x -= speed * deltaTime;
        }
        if (IsKeyDown('D')) {
            local.coord.x += speed * deltaTime;
        }
        sockWrap.Send(
            {.data = (char*)&local.coord, .length = sizeof(local.coord)}
        );
        sockWrap.ReceiveFromBuffer(netInputBuff);
        auto data = (Vec2*)netInputBuff.data();
        size_t length = netInputBuff.size() / sizeof(Vec2);
        if (length > 0) {
            net.coord = data[length - 1];
        }
        return navigation->NoChange();
    }

    void Draw(AbstractCanvas* canvas) const override
    {
        Font::DrawString(canvas, hostAddress, {10, 10}, 1, idid);
        local.Draw(canvas);
        net.Draw(canvas);
    }
};

class TestScreenClient : public AbstractScreen {
    Square local, net;
    SocketWrapper sockWrap;

    std::vector<uint8_t> netInputBuff;

   public:
    TestScreenClient()
        : net(Color::RED, {30, 30}), local(Color::BLUE, {200, 30})
    {
    }

    static const std::wstring_view ScreenName() { return L"TestScreenClient"; }

    std::wstring_view getName() override { return ScreenName(); }

    void Init(const std::any& args) override
    {
        sockWrap.Connect("127.0.0.1", std::string(PORT_STR));
        sockWrap.StartRecive();
    }

    AbstractScreen* Clone() const override { return new TestScreenClient; }

    AbstractNavigation::NavigationRes Update(
        float deltaTime, const AbstractNavigation* navigation
    ) override
    {
        constexpr int speed = 300;
        size_t cursor = 0;
        if (IsKeyDown('W')) {
            local.coord.y -= speed * deltaTime;
        }
        if (IsKeyDown('S')) {
            local.coord.y += speed * deltaTime;
        }
        if (IsKeyDown('A')) {
            local.coord.x -= speed * deltaTime;
        }
        if (IsKeyDown('D')) {
            local.coord.x += speed * deltaTime;
        }
        sockWrap.Send(
            {.data = (char*)&local.coord, .length = sizeof(local.coord)}
        );
        sockWrap.ReceiveFromBuffer(netInputBuff);
        auto data = (Vec2*)netInputBuff.data();
        size_t length = netInputBuff.size() / sizeof(Vec2);
        if (length > 0) {
            net.coord = data[length - 1];
        }
        return navigation->NoChange();
    }

    void Draw(AbstractCanvas* canvas) const override
    {
        local.Draw(canvas);
        net.Draw(canvas);
    }
};

class BeginTest : public AbstractScreen {
   public:
    static const std::wstring_view ScreenName() { return L"BeginTest"; }

    std::wstring_view getName() override { return ScreenName(); }

    void Init(const std::any& args) override {}

    AbstractScreen* Clone() const override { return new BeginTest; }

    AbstractNavigation::NavigationRes Update(
        float deltaTime, const AbstractNavigation* navigation
    ) override
    {
        if (IsKeyDown('1')) {
            return navigation->Navigate(TestScreenClient::ScreenName());
        }
        if (IsKeyDown('2')) {
            return navigation->Navigate(TestScreenServer::ScreenName());
        }
        return navigation->NoChange();
    }

    void Draw(AbstractCanvas* canvas) const override
    {
        Font::DrawString(canvas, "1. Client", {10, 10});
        Font::DrawString(canvas, "2. Server", {10, 30});
    }
};

auto main() -> int
{
    Font::Load("../CrossyRoady/test.font");
    auto game = std::make_unique<Game>(L"Net test", GetDisplayRefreshRate());
    game->AddScreen(std::make_unique<TestScreenServer>())
        ->AddScreen(std::make_unique<TestScreenClient>())
        ->AddScreen(std::make_unique<BeginTest>());
    game->Run(BeginTest::ScreenName());
    return 0;
}