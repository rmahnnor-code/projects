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
                }
                break;
            }
        }

        // delay
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

Texture2D Set_Texture(Image &img, Texture2D &texture, int height, int width, const char *filename)
{
    img = LoadImage(filename);
    ImageResize(&img, width, height);
    texture = LoadTextureFromImage(img);
    return texture;
}

int main()
{
    std::thread window_monitor(WindowMonitorPro);
    InitWindow(800, 600, "LOCK IN");
    SetTargetFPS(60);
    Font customFont = LoadFontEx("Comfortaa-Bold.ttf", 20, 0, 0);
    GuiSetFont(customFont);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    GuiSetStyle(DEFAULT, TEXT_PADDING, 4);
    Image Img[7];
    Texture2D Tx[7];
    Set_Texture(Img[0], Tx[0], 600, 800, "BG3.png");
    Set_Texture(Img[1], Tx[1], 100, 220, "button3.png");
    Set_Texture(Img[2], Tx[2], 20, 20, "CheckBox2.png");
    Set_Texture(Img[3], Tx[3], 20, 20, "CheckBoxChecked2.png");
    Set_Texture(Img[4], Tx[4], 90, 148, "button3.png");
    Set_Texture(Img[5], Tx[5], 34, 74, "button3.png");
    Set_Texture(Img[6], Tx[6], 90, 148, "button3.png");
    Rectangle selectB = {321, 363, 172.5, 70};
    Rectangle BackB = {44, 498, 114, 62};
    Rectangle startB = {644, 498, 114, 62};
    Rectangle stopB = {330, 363, 170, 70};
    Rectangle MMB = {20, 20, 60, 60};
    Rectangle CheckBoxB = {50, 100, 20, 20};
    Rectangle CheckBoxS = {50, 150, 20, 20};
    Rectangle CheckBoxD = {50, 200, 20, 20};
    Rectangle CheckBoxC = {50, 250, 20, 20};
    Rectangle CheckBoxI = {50, 300, 20, 20};
    Rectangle CheckBoxY = {600, 100, 20, 20};
    Rectangle CheckBoxT = {600, 150, 20, 20};
    Rectangle CheckBoxN = {600, 200, 20, 20};
    Rectangle CheckBoxE = {600, 250, 20, 20};
    Rectangle CheckBoxR = {600, 300, 20, 20};
    Rectangle CATextBox = {300, 400, 150, 25};
    Rectangle ADDB = {450, 400, 60, 25};
    Rectangle CLB = {350, 498, 114, 62};
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
    bool CheckBoxState[10] = {false, false, false, false, false, false, false, false, false, false};
    char TextMem[69] = "\0";
    bool TextBoxEditMode = false;
    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(Tx[0], 0, 0, WHITE);
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
                std::lock_guard<std::mutex> lock(mtxApps);
                if (!TargetApps.empty())
                {
                    usc = LOCK_INITIALIZED;
                    IsRunning = false;
                    lsc = LOCKED;
                }
            }
            if (GuiCheckBox(CheckBoxB, "Brave(browser)", &CheckBoxState[0]))
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
            if (GuiCheckBox(CheckBoxT, "Twitch", &CheckBoxState[6]))
            {
                std::lock_guard<std::mutex> lock(mtxApps);
                if (CheckBoxState[6])
                {
                    if (std::find(TargetApps.begin(), TargetApps.end(), L"Twitch") == TargetApps.end())
                    {
                        TargetApps.push_back(L"Twitch");
                    }
                }
                else if (!CheckBoxState[6])
                {
                    TargetApps.erase(std::remove(TargetApps.begin(), TargetApps.end(), L"Twitch"), TargetApps.end());
                }
            }
            if (GuiCheckBox(CheckBoxN, "Netflix", &CheckBoxState[7]))
            {
                std::lock_guard<std::mutex> lock(mtxApps);
                if (CheckBoxState[7])
                {
                    if (std::find(TargetApps.begin(), TargetApps.end(), L"Netflix") == TargetApps.end())
                    {
                        TargetApps.push_back(L"Netflix");
                    }
                }
                else if (!CheckBoxState[7])
                {
                    TargetApps.erase(std::remove(TargetApps.begin(), TargetApps.end(), L"Netflix"), TargetApps.end());
                }
            }
            if (GuiCheckBox(CheckBoxE, "Epic Games", &CheckBoxState[8]))
            {
                std::lock_guard<std::mutex> lock(mtxApps);
                if (CheckBoxState[8])
                {
                    if (std::find(TargetApps.begin(), TargetApps.end(), L"Epic Games") == TargetApps.end())
                    {
                        TargetApps.push_back(L"Epic Games");
                    }
                }
                else if (!CheckBoxState[8])
                {
                    TargetApps.erase(std::remove(TargetApps.begin(), TargetApps.end(), L"EA"), TargetApps.end());
                }
            }
            if (GuiCheckBox(CheckBoxR, "Riot Client", &CheckBoxState[9]))
            {
                std::lock_guard<std::mutex> lock(mtxApps);
                if (CheckBoxState[9])
                {
                    if (std::find(TargetApps.begin(), TargetApps.end(), L"Riot Client") == TargetApps.end())
                    {
                        TargetApps.push_back(L"Riot Client");
                    }
                }
                else if (!CheckBoxState[9])
                {
                    TargetApps.erase(std::remove(TargetApps.begin(), TargetApps.end(), L"Riot Client"), TargetApps.end());
                }
            }
            if (GuiButton(ADDB, "ADD"))
            {
                if (TextMem[0] != '\0')
                {
                    std::string tempstr(TextMem);
                    std::wstring wstrCA(tempstr.begin(), tempstr.end());
                    std::lock_guard<std::mutex> lock(mtxApps);
                    if (std::find(TargetApps.begin(), TargetApps.end(), wstrCA) == TargetApps.end())
                    {
                        TargetApps.push_back(wstrCA);
                    }
                }
                TextMem[0] = '\0';
            }
            if(GuiButton(CLB, "Clear"))
            {
                std::lock_guard<std::mutex> lock(mtxApps);
                if(!TargetApps.empty())
                {
                    TargetApps.clear();
                    for(int i = 0; i < 10; i++)
                    {
                        CheckBoxState[i] = false;
                    }
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
            if (GuiButton(MMB, "Main Menu"))
            {
                if (lsc == UNLOCKED)
                {
                    usc = FIRST_SCREEN;
                }
            }
        }
        if (usc == FIRST_SCREEN)
        {
            DrawTexture(Tx[1], 300, 345, WHITE);
        }
        if (usc == APP_SELECTION)
        {
            DrawTexture(CheckBoxState[0] ? Tx[3] : Tx[2], 50, 100, WHITE);
            DrawTexture(CheckBoxState[1] ? Tx[3] : Tx[2], 50, 150, WHITE);
            DrawTexture(CheckBoxState[2] ? Tx[3] : Tx[2], 50, 200, WHITE);
            DrawTexture(CheckBoxState[3] ? Tx[3] : Tx[2], 50, 250, WHITE);
            DrawTexture(CheckBoxState[4] ? Tx[3] : Tx[2], 50, 300, WHITE);
            DrawTexture(CheckBoxState[5] ? Tx[3] : Tx[2], 600, 100, WHITE);
            DrawTexture(CheckBoxState[6] ? Tx[3] : Tx[2], 600, 150, WHITE);
            DrawTexture(CheckBoxState[7] ? Tx[3] : Tx[2], 600, 200, WHITE);
            DrawTexture(CheckBoxState[8] ? Tx[3] : Tx[2], 600, 250, WHITE);
            DrawTexture(CheckBoxState[9] ? Tx[3] : Tx[2], 600, 300, WHITE);
            DrawTexture(Tx[4], 29, 482, WHITE);
            DrawTexture(Tx[4], 629, 482, WHITE);
            DrawTexture(Tx[4], 335, 482, WHITE);
            if (GuiTextBox(CATextBox, TextMem, 64, TextBoxEditMode))
            {
                TextBoxEditMode = !TextBoxEditMode;
            }
            DrawTexture(Tx[5], 444, 395, WHITE);
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