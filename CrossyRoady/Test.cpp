//#include <Windows.h>
//#include <fcntl.h>
//#include <io.h>
//
//#include <chrono>
//#include <iostream>
//#include <string_view>
//#include <thread>
//#include <vector>
//
//constexpr COORD BufferSize = {120, 30};
//constexpr uint32_t BufferSize1D = BufferSize.X * BufferSize.Y;
//
//class ScreenBuffer {
//    CHAR_INFO buffer[BufferSize1D] = {0};
//
//   public:
//    CHAR_INFO* getRawData() { return buffer; }
//
//    void FillBlack()
//    {
//        for (size_t i = 0; i < BufferSize1D; i++) {
//            buffer[i].Char.UnicodeChar = L'H';
//            buffer[i].Attributes = FOREGROUND_RED | FOREGROUND_BLUE |
//                                   FOREGROUND_GREEN | FOREGROUND_INTENSITY;
//        }
//    }
//
//    void FillWhite(wchar_t chr = L'H')
//    {
//        for (size_t i = 0; i < BufferSize1D; i++) {
//            buffer[i].Char.UnicodeChar = chr;
//            buffer[i].Attributes = BACKGROUND_RED | BACKGROUND_GREEN |
//                                   BACKGROUND_BLUE | BACKGROUND_INTENSITY;
//        }
//    }
//
//    CHAR_INFO* operator[](size_t index) const
//    {
//        return (CHAR_INFO*)this->buffer + index * BufferSize.X;
//    }
//};
//
//auto main() -> int
//{
//    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
//    HWND consoleWindow = GetConsoleWindow();
//    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
//    DWORD currMode;
//    CONSOLE_FONT_INFOEX fontex;
//
//    // Turn off maximize, resize, horizontal and vertical scrolling
//    style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME) & ~(WS_HSCROLL) &
//            ~(WS_VSCROLL);
//    SetWindowLong(consoleWindow, GWL_STYLE, style);
//
//    // Turn off mouse input
//    GetConsoleMode(hOut, &currMode);
//    SetConsoleMode(
//        hOut,
//        ((currMode | ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS) &
//         ~ENABLE_QUICK_EDIT_MODE & ~ENABLE_MOUSE_INPUT)
//    );
//
//    // Hide scoll bar
//    ShowScrollBar(consoleWindow, SB_BOTH, FALSE);
//
//    // Hide the cursor
//#if _DEBUG
//    ShowCursor(true);
//#else
//    ShowCursor(false);
//#endif
//    // Set font bold
//    fontex.cbSize = sizeof(CONSOLE_FONT_INFOEX);
//    GetCurrentConsoleFontEx(hOut, 0, &fontex);
//    fontex.FontWeight = 800;
//    fontex.dwFontSize.X = 24;
//    fontex.dwFontSize.Y = 24;
//    wcscpy_s(fontex.FaceName, L"Consolas");
//    SetCurrentConsoleFontEx(hOut, NULL, &fontex);
//
//    // Update console title
//    SetConsoleTitle(TEXT("Caro Game"));
//
//    // Set IO Unicode
//    _setmode(_fileno(stdout), _O_WTEXT);
//    _setmode(_fileno(stdin), _O_WTEXT);
//
//    ScreenBuffer scrBuf;
//    SMALL_RECT rect = {0, 0, 119, 29};
//
//    constexpr auto targetFps = 30;
//    constexpr auto targetFrameTime =
//        std::chrono::nanoseconds(std::chrono::seconds(1)) / targetFps;
//
//    CONSOLE_SCREEN_BUFFER_INFOEX bufferInfo{};
//    GetConsoleScreenBufferInfoEx(hOut, &bufferInfo);
//    for (int i = 0; i < 16; i++) {
//        bufferInfo.ColorTable[i] = RGB(255, 0, 0);
//    }
//    if (!SetConsoleScreenBufferInfoEx(hOut, &bufferInfo)) {
//        std::system_category().message(GetLastError());
//    }
//
//    SetConsoleTextAttribute(hOut, FOREGROUND_BLUE);
//    // FillConsoleOutputCharacterW
//    //  WriteConsoleOutputAttribute
//    std::wcout << L"This is a test";
//
//    return 0;
//
//    /*std::chrono::nanoseconds lastFrameTime(0);
//
//    for (size_t idx = 0;; idx++) {
//        const auto start = std::chrono::high_resolution_clock::now();
//        scrBuf.FillWhite(L' ');
//        for (int i = 0; i < BufferSize.Y; i++) {
//            scrBuf[i][idx % BufferSize.X].Attributes = 0;
//            if (lastFrameTime >= targetFrameTime) {
//                scrBuf[i][idx % BufferSize.X].Attributes =
//                    BACKGROUND_GREEN | BACKGROUND_BLUE;
//            }
//        }
//        WriteConsoleOutput(
//            hOut, scrBuf.getRawData(), BufferSize, {0, 0}, &rect
//        );
//
//        const auto end = std::chrono::high_resolution_clock::now();
//        lastFrameTime = end - start;
//
//        std::this_thread::sleep_until(start + targetFrameTime);
//    }*/
//    system("cls");
//    return 0;
//}
//
//class JobSignal {
//    bool jobDone = false;
//    std::mutex lock;
//    std::unique_lock<std::mutex> tmp;
//    std::condition_variable cv;
//
//   public:
//    JobSignal() { lock.lock(); }
//
//    bool JobDone() { return jobDone; }
//
//    void StartJob()
//    {
//        jobDone = false;
//        lock.unlock();
//    }
//
//    void WaitStartJobSignal() { lock.lock(); }
//
//    void DoneJob()
//    {
//        jobDone = true;
//        lock.lock();
//    }
//
//    void WaitUntilJobDone() {}
//};
