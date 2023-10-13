#include "Common.h"

#include <bit>

namespace ConsoleGame {

    bool ConsoleGame::IsKeyDown(int key)
    {
        return (GetAsyncKeyState(key) & (1 << 16));
    }

    Vec2 getCanvasPixelSize(HWND hConsoleWindow)
    {
        RECT windowRect;
        GetWindowRect(hConsoleWindow, &windowRect);
        return Vec2{
            .width = (windowRect.right - windowRect.left) / _CanvasSize.width,
            .height =
                (windowRect.bottom - windowRect.top) / _CanvasSize.height};
    }

    Vec2 ConsoleGame::GetMousePos()
    {
        static const HWND hConsoleWindow = GetConsoleWindow();
        static const Vec2 pixSize = getCanvasPixelSize(hConsoleWindow);
        POINT pos{0};
        GetCursorPos(&pos);
        ScreenToClient(hConsoleWindow, &pos);
        return Vec2{.x = pos.x / pixSize.width, .y = pos.y / pixSize.height};
    }

    uint16_t ToHost(uint16_t num) { return ((num >> 8)) | ((num << 8)); }

    ScaleFactor GetScalingFactor()
    {
        auto activeWindow = GetActiveWindow();
        HMONITOR monitor =
            MonitorFromWindow(activeWindow, MONITOR_DEFAULTTONEAREST);

        // Get the logical width and height of the monitor
        MONITORINFOEX monitorInfoEx;
        monitorInfoEx.cbSize = sizeof(monitorInfoEx);
        GetMonitorInfo(monitor, &monitorInfoEx);
        auto cxLogical =
            monitorInfoEx.rcMonitor.right - monitorInfoEx.rcMonitor.left;
        auto cyLogical =
            monitorInfoEx.rcMonitor.bottom - monitorInfoEx.rcMonitor.top;

        // Get the physical width and height of the monitor
        DEVMODE devMode;
        devMode.dmSize = sizeof(devMode);
        devMode.dmDriverExtra = 0;
        EnumDisplaySettings(
            monitorInfoEx.szDevice, ENUM_CURRENT_SETTINGS, &devMode
        );
        auto cxPhysical = devMode.dmPelsWidth;
        auto cyPhysical = devMode.dmPelsHeight;

        // Calculate the scaling factor
        auto horizontalScale = ((double)cxPhysical / (double)cxLogical);
        auto verticalScale = ((double)cyPhysical / (double)cyLogical);
        return ScaleFactor{horizontalScale, verticalScale};
    }

    uint16_t ToBigEndian(uint16_t num)
    {
        static constexpr bool isBigEndian =
            std::endian::native == std::endian::big;
        if (isBigEndian) {
            return num;
        } else {
            return (num << 8) | (num >> 8);
        }
    }

}  // namespace ConsoleGame
