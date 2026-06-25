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

    //fonts
    Font font[6];
    font[0] = LoadFontEx("assets/Font/Restful_Silent.ttf", 130, 0, 0);
    font[1] = LoadFontEx("assets/Font/AUGUSTUS.ttf", 21, 0, 0);
    font[2] = LoadFontEx("assets/Font/AUGUSTUS.ttf", 30, 0, 0);
    font[3] = LoadFontEx("assets/Font/Restful_Silent.ttf", 20, 0, 0);
    font[4] = LoadFontEx("assets/Font/AUGUSTUS.ttf", 20, 0 ,0);
    font[5] = LoadFontEx("assets/Font/AUGUSTUS.ttf", 17, 0 ,0);
    Image Img[20];
    Texture2D Tx[20];
    GuiSetFont(font[3]);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    GuiSetStyle(DEFAULT, TEXT_PADDING, 4);

    //assets textures
    Set_Texture(Img[0], Tx[0], 600, 800, "assets/Textures/BG3.png");
    Set_Texture(Img[1], Tx[1], 100, 220, "assets/Textures/button3.png");
    Set_Texture(Img[2], Tx[2], 20, 20, "assets/Textures/CheckBox2.png");
    Set_Texture(Img[3], Tx[3], 20, 20, "assets/Textures/CheckBoxChecked2.png");
    Set_Texture(Img[4], Tx[4], 90, 148, "assets/Textures/button3.png");
    Set_Texture(Img[5], Tx[5], 34, 74, "assets/Textures/button3.png");
    Set_Texture(Img[6], Tx[6], 85, 85, "assets/Textures/button3.png");
    Set_Texture(Img[7], Tx[7], 100, 218, "assets/Textures/button3.png");
    Set_Texture(Img[8], Tx[8], 40, 40, "assets/Textures/HomeIcon.png");

    //icons textures
    Set_Texture(Img[9], Tx[9], 100, 100, "assets/ICONS/BI.png");
    Set_Texture(Img[10], Tx[10], 40, 40, "assets/ICONS/SI.png");
    Set_Texture(Img[11], Tx[11], 40, 40, "assets/ICONS/DI.png");
    Set_Texture(Img[12], Tx[12], 40, 40, "assets/ICONS/GI.png");
    Set_Texture(Img[13], Tx[13], 40, 40, "assets/ICONS/II.png");
    Set_Texture(Img[14], Tx[14], 40, 40, "assets/ICONS/YI.png");
    Set_Texture(Img[15], Tx[15], 40, 40, "assets/ICONS/TI.png");
    Set_Texture(Img[16], Tx[16], 40, 40, "assets/ICONS/NI.png");
    Set_Texture(Img[17], Tx[17], 40, 40, "assets/ICONS/EI.png");
    Set_Texture(Img[18], Tx[18], 40, 40, "assets/ICONS/RI.png");

    //GUI elements
    Rectangle selectB = {321, 363, 172.5, 70};
    Rectangle BackB = {44, 498, 114, 62};
    Rectangle startB = {644, 498, 114, 62};
    Rectangle stopB = {330, 363, 170, 70};
    Rectangle MMB = {19, 20, 61, 61};
    Rectangle CheckBoxB = {50, 100, 20, 20};
    Rectangle CheckBoxS = {50, 150, 20, 20};
    Rectangle CheckBoxD = {50, 200, 20, 20};
    Rectangle CheckBoxC = {50, 250, 20, 20};
    Rectangle CheckBoxI = {50, 300, 20, 20};
    Rectangle CheckBoxY = {730, 100, 20, 20};
    Rectangle CheckBoxT = {730, 150, 20, 20};
    Rectangle CheckBoxN = {730, 200, 20, 20};
    Rectangle CheckBoxE = {730, 250, 20, 20};
    Rectangle CheckBoxR = {730, 300, 20, 20};
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
            if (GuiButton(CLB, "Clear"))
            {
                std::lock_guard<std::mutex> lock(mtxApps);
                if (!TargetApps.empty())
                {
                    TargetApps.clear();
                    for (int i = 0; i < 10; i++)
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
        DrawTexture(Tx[0], 0, 0, WHITE);
        if (usc == FIRST_SCREEN)
        {
            DrawTextEx(font[0], "LOCK TF IN", {140, 100}, 130, 5, WHITE);
            DrawTexture(Tx[1], 300, 345, WHITE);
            DrawTextEx(font[1], "SELECT APPS", {325, 385}, 21, 3, BLACK);
        }
        if (usc == APP_SELECTION)
        {
            DrawTexture(CheckBoxState[0] ? Tx[3] : Tx[2], 50, 100, WHITE);
            DrawTexture(CheckBoxState[1] ? Tx[3] : Tx[2], 50, 150, WHITE);
            DrawTexture(CheckBoxState[2] ? Tx[3] : Tx[2], 50, 200, WHITE);
            DrawTexture(CheckBoxState[3] ? Tx[3] : Tx[2], 50, 250, WHITE);
            DrawTexture(CheckBoxState[4] ? Tx[3] : Tx[2], 50, 300, WHITE);
            DrawTexture(CheckBoxState[5] ? Tx[3] : Tx[2], 730, 100, WHITE);
            DrawTexture(CheckBoxState[6] ? Tx[3] : Tx[2], 730, 150, WHITE);
            DrawTexture(CheckBoxState[7] ? Tx[3] : Tx[2], 730, 200, WHITE);
            DrawTexture(CheckBoxState[8] ? Tx[3] : Tx[2], 730, 250, WHITE);
            DrawTexture(CheckBoxState[9] ? Tx[3] : Tx[2], 730, 300, WHITE);
            DrawTexture(Tx[4], 29, 482, WHITE);
            DrawTexture(Tx[4], 629, 482, WHITE);
            DrawTexture(Tx[4], 335, 482, WHITE);
            DrawTextEx(font[5], "Add more APPs:", {300, 350}, 17, 3 , WHITE);
            DrawTextEx(font[5], "Type the windowtitle of your app below", {150, 370}, 17, 3 , WHITE);
            if (GuiTextBox(CATextBox, TextMem, 64, TextBoxEditMode))
            {
                TextBoxEditMode = !TextBoxEditMode;
            }
            DrawTexture(Tx[5], 444, 395, WHITE);
            DrawTextEx(font[2], "BACK", {53, 515}, 30, 3, BLACK);
            DrawTextEx(font[2], "LOCK", {654, 515}, 30, 3, BLACK);
            DrawTextEx(font[2], "CLEAR", {350, 515}, 30, 3, BLACK);
            DrawTextEx(font[4], "ADD", {455, 405}, 20, 2, BLACK);
            DrawTexture(Tx[9], 45, 57, WHITE);
            DrawTexture(Tx[10], 78, 140, WHITE);
            DrawTexture(Tx[11], 78, 190, WHITE);
            DrawTexture(Tx[12], 78, 240, WHITE);
            DrawTexture(Tx[13], 78, 290, WHITE);
            DrawTexture(Tx[14], 678, 90, WHITE);
            DrawTexture(Tx[15], 678, 140, WHITE);
            DrawTexture(Tx[16], 678, 190, WHITE);
            DrawTexture(Tx[17], 678, 240, WHITE);
            DrawTexture(Tx[18], 678, 290, WHITE);
        }
        if (usc == LOCK_INITIALIZED)
        {
            DrawTexture(Tx[6], 8, 6, WHITE);
            DrawTexture(Tx[8], 30, 30, WHITE);
            if (lsc == LOCKED)
            {
                DrawTexture(Tx[7], 309, 345, WHITE);
                DrawTextEx(font[2], "UNLOCK", {336, 380}, 30, 3, BLACK);
            }
            if(lsc == UNLOCKED)
            {
                DrawTextEx(font[2], "BLOCKER UNLOCKED", {210, 380}, 30, 3, WHITE);
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