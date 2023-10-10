#include "ConsoleGame.h"
#include <memory>
#include <queue>

using namespace ConsoleGame;

class DrawScreen : public AbstractScreen {
    Vec2 cursorPos{ 0,0 };
    Sprite board;
    Color currentColor = Color::BLACK;

    void fill(Vec2 pos) {
        // BFS
        static constexpr Vec2 pp[] = { {1,0}, {-1,0}, {0,1}, {0, -1} };
        auto InitialColor = board[pos.y][pos.x];
        std::queue<Vec2> queue;
        queue.push(pos);
        while (!queue.empty()) {
            auto p = queue.front();
            queue.pop();
            if (board[p.y][p.x] != InitialColor || board[p.y][p.x] == currentColor) {
                continue;
            }
            board[p.y][p.x] = currentColor;
            for (const auto& d : pp) {
                auto u = p.x + d.x;
                auto v = p.y + d.y;
                if (u >= 0 && u < _CanvasSize.width && v >= 0 && v < _CanvasSize.height && board[v][u] == InitialColor) {
                    queue.push({ u, v });
                }
            }
        }
    }

public:
    DrawScreen() : board(_CanvasSize) {
    }

    static const std::wstring_view ScreenName() {
        return L"DrawScreen";
    }

    std::wstring_view getName() override
    {
        return ScreenName();
    }
    void Init(const std::any& args) override
    {
    }

    AbstractScreen* Clone() const override
    {
        return new DrawScreen;
    }

    AbstractNavigation::NavigationRes Update(float deltaTime, const AbstractNavigation* navigation) override
    {
        if (IsKeyDown(VK_RETURN)) {
            board[cursorPos.y][cursorPos.x] = currentColor;
        }

        if (IsKeyDown('W')) {
            cursorPos.y = max(cursorPos.y - 1, 0);
        }
        if (IsKeyDown('S')) {
            if (IsKeyDown(VK_CONTROL)) {
                board.Save("./out.sprite");
            }
            else {
                cursorPos.y = min(cursorPos.y + 1, _CanvasSize.y);
            }
        }
        if (IsKeyDown('D')) {
            cursorPos.x = min(cursorPos.x + 1, _CanvasSize.x);
        }
        if (IsKeyDown('A')) {
            cursorPos.x = max(cursorPos.x - 1, 0);
        }
        if (IsKeyDown(VK_TAB)) {
            currentColor = Color(((char)currentColor + 1) % 16);
        }
        if (IsKeyDown(VK_SHIFT)) {
            board[cursorPos.y][cursorPos.x] = Color::C_TRANSPARENT;
        }
        if (IsKeyDown('F')) {
            fill(cursorPos);
        }
        return navigation->NoChange();
    }

    void Draw(AbstractCanvas* canvas) const override
    {
        board.Paint(canvas, { 0,0 });
        (*canvas)[0][0] = currentColor;
        (*canvas)[0][1] = currentColor;
        (*canvas)[1][0] = currentColor;
        (*canvas)[1][1] = currentColor;
        if ((*canvas)[cursorPos.y][cursorPos.x] == Color::BLACK) {
            (*canvas)[cursorPos.y][cursorPos.x] = Color::BRIGHT_WHITE;
        }
        else {
            (*canvas)[cursorPos.y][cursorPos.x] = Color::BLACK;
        }
    }
    void DeInit() override
    {
        board.DeInit();
    }
};

auto main() -> int {
    auto game = std::make_unique<Game>();
    game->Init();
    game->AddScreen(std::make_unique<DrawScreen>());
    game->Run(DrawScreen::ScreenName());
    return 0;
}