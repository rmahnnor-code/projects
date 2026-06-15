#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define CloseWindow Win32CloseWindow
#define ShowCursor Win32ShowCursor
#define LoadImage Win32LoadImage
#define DrawText Win32DrawText
#define DrawTextEx Win32DrawTextEx
#endif
#include <windows.h>
#if defined(_WIN32)
#undef CloseWindow
#undef ShowCursor
#undef LoadImage
#undef DrawText
#undef DrawTextEx
#undef near
#undef far
#endif
#include <atomic>
#include <thread>
#include <chrono>
#include <vector>
#include <stdexcept>
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <mutex>
#include <algorithm>

std::atomic<bool> IsRunning = true;
std::atomic<bool> IsActive = true;
std::atomic<bool> IsChecked = false;
std::vector<std::wstring> TargetApps;
std::mutex mtxApps;

void WindowMonitorPro()
{
    while (IsActive)
    {
        if (IsRunning)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }
        HWND w_h = GetForegroundWindow();
        if (w_h == NULL)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }
        auto titlelength = GetWindowTextLengthW(w_h);

        if (titlelength < 0)
        {
            continue;
        }
        // window title memory space
        std::wstring windowtitle(titlelength + 1, L'\0');
        GetWindowTextW(w_h, &windowtitle[0], titlelength + 1);

        // force kill
        DWORD ProcessID = 0;
        GetWindowThreadProcessId(w_h, &ProcessID);
        {
            std::lock_guard<std::mutex> lock(mtxApps);
            for (auto &BApp : TargetApps)
            {
                if (windowtitle.find(BApp) != std::wstring::npos)
                {
                    if (ProcessID != 0)
                    {
                        HANDLE HndlProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessID);
                        if (HndlProcess != NULL)
                        {
                            TerminateProcess(HndlProcess, 0);
                            CloseHandle(HndlProcess);
                        }
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    continue;
                }
                break;
            }
        }

        // delay
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main()
{
    std::thread window_monitor(WindowMonitorPro);
    InitWindow(800, 600, "LOCK IN");
    SetTargetFPS(60);
    Font customFont = LoadFontEx("Pricedown.otf", 100, 0, 0);
    GuiSetFont(customFont);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 32);
    Rectangle selectB = {300, 350, 200, 50};
    Rectangle BackB = {50, 530, 100, 50};
    Rectangle startB = {300, 470, 150, 50};
    Rectangle stopB = {300, 350, 200, 50};
    Rectangle MMB = {300, 250, 200, 50};
    Rectangle CheckBoxS = {100, 100, 20, 20};
    Rectangle CheckBoxB = {100, 150, 20, 20};
    Rectangle CheckBoxD = {100, 200, 20, 20};
    Rectangle CheckBoxC = {100, 250, 20, 20};
    Rectangle CheckBoxI = {100, 300, 20, 20};
    Rectangle CheckBoxY = {600, 100, 20, 20};
    enum BUTTON_STATE
    {
        STATE_START,
        STATE_STOP
    };
    enum UI_STATE
    {
        FIRST_SCREEN,
        APP_SELECTION,
        LOCK_INITIALIZED
    };
    enum LOCK_STATE
    {
        LOCKED,
        UNLOCKED
    };
    BUTTON_STATE state_cuurent;
    state_cuurent = STATE_START;
    UI_STATE usc;
    usc = FIRST_SCREEN;
    LOCK_STATE lsc;
    lsc = UNLOCKED;
    bool CheckBoxState[7] = {false, false, false, false, false, false, false};
    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(BLACK);
        if (usc == FIRST_SCREEN)
        {
            if (GuiButton(selectB, "Select APPs"))
            {
                usc = APP_SELECTION;
            }
        }
        else if (usc == APP_SELECTION)
        {
            if (GuiButton(BackB, "Back"))
            {
                usc = FIRST_SCREEN;
            }
            if (GuiButton(startB, "LOCK"))
            {
                usc = LOCK_INITIALIZED;
                IsRunning = false;
                lsc = LOCKED;
            }
            if (GuiCheckBox(CheckBoxB, "Brave", &CheckBoxState[0]))
            {
                std::lock_guard<std::mutex> lock(mtxApps);
                if (CheckBoxState[0])
                {
                    if (std::find(TargetApps.begin(), TargetApps.end(), L"Brave") == TargetApps.end())
                    {
                        TargetApps.push_back(L"Brave");
                    }
                }
                else if (!CheckBoxState[0])
                {
                    TargetApps.erase(std::remove(TargetApps.begin(), TargetApps.end(), L"Brave"), TargetApps.end());
                }
            }
            if (GuiCheckBox(CheckBoxS, "Steam", &CheckBoxState[1]))
            {
                std::lock_guard<std::mutex> lock(mtxApps);
                if (CheckBoxState[1])
                {
                    if (std::find(TargetApps.begin(), TargetApps.end(), L"Steam") == TargetApps.end())
                    {
                        TargetApps.push_back(L"Steam");
                    }
                }
                else if (!CheckBoxState[1])
                {
                    TargetApps.erase(std::remove(TargetApps.begin(), TargetApps.end(), L"Steam"), TargetApps.end());
                }
            }
            if (GuiCheckBox(CheckBoxD, "Discord", &CheckBoxState[2]))
            {
                std::lock_guard<std::mutex> lock(mtxApps);
                if (CheckBoxState[2])
                {
                    if (std::find(TargetApps.begin(), TargetApps.end(), L"Discord") == TargetApps.end())
                    {
                        TargetApps.push_back(L"Discord");
                    }
                }
                else if (!CheckBoxState[2])
                {
                    TargetApps.erase(std::remove(TargetApps.begin(), TargetApps.end(), L"Discord"), TargetApps.end());
                }
            }
            if (GuiCheckBox(CheckBoxC, "Google Chrome", &CheckBoxState[3]))
            {
                std::lock_guard<std::mutex> lock(mtxApps);
                if (CheckBoxState[3])
                {
                    if (std::find(TargetApps.begin(), TargetApps.end(), L"Google Chrome") == TargetApps.end())
                    {
                        TargetApps.push_back(L"Google Chrome");
                    }
                }
                else if (!CheckBoxState[3])
                {
                    TargetApps.erase(std::remove(TargetApps.begin(), TargetApps.end(), L"Google Chrome"), TargetApps.end());
                }
            }
            if (GuiCheckBox(CheckBoxI, "Instagram", &CheckBoxState[4]))
            {
                std::lock_guard<std::mutex> lock(mtxApps);
                if (CheckBoxState[4])
                {
                    if (std::find(TargetApps.begin(), TargetApps.end(), L"Instagram") == TargetApps.end())
                    {
                        TargetApps.push_back(L"Instagram");
                    }
                }
                else if (!CheckBoxState[4])
                {
                    TargetApps.erase(std::remove(TargetApps.begin(), TargetApps.end(), L"Instagram"), TargetApps.end());
                }
            }
            if (GuiCheckBox(CheckBoxY, "YouTube", &CheckBoxState[5]))
            {
                std::lock_guard<std::mutex> lock(mtxApps);
                if (CheckBoxState[5])
                {
                    if (std::find(TargetApps.begin(), TargetApps.end(), L"YouTube") == TargetApps.end())
                    {
                        TargetApps.push_back(L"YouTube");
                    }
                }
                else if (!CheckBoxState[5])
                {
                    TargetApps.erase(std::remove(TargetApps.begin(), TargetApps.end(), L"YouTube"), TargetApps.end());
                }
            }
        }
        else if (usc == LOCK_INITIALIZED)
        {
            if (lsc == LOCKED)
            {
                if (GuiButton(stopB, "UNLOCK"))
                {
                    IsRunning = true;
                    lsc = UNLOCKED;
                }
            }
            if (lsc == UNLOCKED)
            {
                if (GuiButton(MMB, "Main Menu"))
                {
                    usc = FIRST_SCREEN;
                }
            }
        }
        EndDrawing();
    }
    IsActive = false;
    if (window_monitor.joinable())
    {
        window_monitor.join();
    }
    CloseWindow();
}