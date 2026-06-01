#include <raylib.h>
#include <ctime>

// Screen states
enum Screen { MAIN, SET_TIME };

Texture2D Set_Texture(Image &img, Texture2D &txtu, int width, int height, const char* filename)
{
    img = LoadImage(filename);
    ImageResize(&img, width, height);
    txtu = LoadTextureFromImage(img);
    UnloadImage(img);
    return txtu;
}

int main()
{
    InitWindow(800, 600, "alarm clock");
    SetTargetFPS(120);
    InitAudioDevice();

    //textures
    Image img_bg;
    Texture2D txtu_bg;
    Set_Texture(img_bg, txtu_bg, 800, 600, "bg.png");

    Image img_btn;
    Texture2D txtu_btn_settings;
    Set_Texture(img_btn, txtu_btn_settings, 70, 70, "add_button.png");

    //font
    Font customFont = LoadFontEx("Pricedown.otf", 100, 0, 0);
    SetTextureFilter(customFont.texture, TEXTURE_FILTER_BILINEAR);

    //alarm sound
    Sound alarmSound = LoadSound("alarm.wav");

    // Main button
    Rectangle btnSettings = { (800.0f - 70) / 2.0f, 600.0f - 120.0f, 70, 70 };

    // Time selection interface elements
    const float trackX_hours = 250.0f;
    const float trackX_minutes = 450.0f;
    const float trackY = 150.0f;
    const float trackHeight = 350.0f;
    const float trackWidth = 20.0f;

    Rectangle hourTrack = { trackX_hours, trackY, trackWidth, trackHeight };
    Rectangle minuteTrack = { trackX_minutes, trackY, trackWidth, trackHeight };

    float hourThumbRatio = 0.0f;
    float minuteThumbRatio = 0.0f;

    int hours = 12;
    int minutes = 0;
    bool isAM = true;

    bool draggingHour = false;
    bool draggingMinute = false;

    // AM / PM checkboxes
    Rectangle checkAM = { 300.0f, 530.0f, 60, 30 };
    Rectangle checkPM = { 400.0f, 530.0f, 60, 30 };

    // Buttons on time screen
    Rectangle btnBack = { 20, 20, 80, 30 };
    Rectangle btnSet = { 680, 530, 80, 30 };

    // Stop alarm button 
    Rectangle btnStopAlarm = { 350, 350, 100, 40 };

    Screen currentScreen = MAIN;

    // Alarm state
    bool alarm_active = false;
    bool alarm_sounding = false;
    int alarmHour24 = 0;
    int alarmMinute = 0;

    auto DrawTextF = [&](const char* text, float x, float y, float fontSize, Color color)
    {
        DrawTextEx(customFont, text, {x, y}, fontSize, 1.0f, color);
    };

    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();

        //Alarm check
        if (alarm_active && !alarm_sounding)
        {
            time_t now = time(0);
            tm* ltm = localtime(&now);
            int currentHour = ltm->tm_hour;
            int currentMinute = ltm->tm_min;

            if (currentHour == alarmHour24 && currentMinute == alarmMinute)
            {
                PlaySound(alarmSound);
                alarm_sounding = true;
            }
        }

        if (alarm_sounding)
        {
            if (!IsSoundPlaying(alarmSound))
                PlaySound(alarmSound);
        }

        //Screen interactions
        if (currentScreen == MAIN)
        {
            if (CheckCollisionPointRec(mouse, btnSettings) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                currentScreen = SET_TIME;

            if (alarm_sounding)
            {
                if (CheckCollisionPointRec(mouse, btnStopAlarm) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    StopSound(alarmSound);
                    alarm_sounding = false;
                    alarm_active = false;
                }
            }
        }
        else if (currentScreen == SET_TIME)
        {
            if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                currentScreen = MAIN;

            if (CheckCollisionPointRec(mouse, btnSet) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // Stop any currently sounding alarm
                if (alarm_sounding)
                {
                    StopSound(alarmSound);
                    alarm_sounding = false;
                }

                alarmHour24 = hours;
                if (isAM)
                {
                    if (alarmHour24 == 12) alarmHour24 = 0;
                }
                else
                {
                    if (alarmHour24 != 12) alarmHour24 += 12;
                }
                alarmMinute = minutes;
                alarm_active = true;
                alarm_sounding = false;
                currentScreen = MAIN;
            }

            if (CheckCollisionPointRec(mouse, hourTrack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                draggingHour = true;
            if (draggingHour)
            {
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                {
                    hourThumbRatio = (mouse.y - hourTrack.y) / hourTrack.height;
                    if (hourThumbRatio < 0.0f) hourThumbRatio = 0.0f;
                    if (hourThumbRatio > 1.0f) hourThumbRatio = 1.0f;
                    hours = (int)(1 + hourThumbRatio * 11.0f + 0.5f);
                }
                else draggingHour = false;
            }

            if (CheckCollisionPointRec(mouse, minuteTrack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                draggingMinute = true;
            if (draggingMinute)
            {
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                {
                    minuteThumbRatio = (mouse.y - minuteTrack.y) / minuteTrack.height;
                    if (minuteThumbRatio < 0.0f) minuteThumbRatio = 0.0f;
                    if (minuteThumbRatio > 1.0f) minuteThumbRatio = 1.0f;
                    minutes = (int)(minuteThumbRatio * 59.0f + 0.5f);
                }
                else draggingMinute = false;
            }

            if (CheckCollisionPointRec(mouse, checkAM) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                isAM = true;
            if (CheckCollisionPointRec(mouse, checkPM) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                isAM = false;
        }

        //drawing
        BeginDrawing();
        ClearBackground(BEIGE);

        if (currentScreen == MAIN)
        {
            DrawTexture(txtu_bg, 0, 0, WHITE);
            DrawTexture(txtu_btn_settings, (int)btnSettings.x, (int)btnSettings.y, WHITE);

            const char* remindText = "Stay reminded.";
            float fontSize = 90.0f;
            float spacing = 2.0f;
            Vector2 textSize = MeasureTextEx(customFont, remindText, fontSize, spacing);
            Vector2 textPos = { (800.0f - textSize.x) / 2.0f, 30.0f };
            DrawTextEx(customFont, remindText, textPos, fontSize, spacing, Fade(DARKGRAY, 0.8f));

            if (alarm_sounding)
            {
                DrawTextF("ALARM!", 300, 200, 50, RED);
                DrawRectangleRec(btnStopAlarm, RED);
                DrawRectangleLinesEx(btnStopAlarm, 2, DARKGRAY);
                DrawTextF("Stop", btnStopAlarm.x + 20, btnStopAlarm.y + 8, 20, WHITE);
            }
            else if (alarm_active)
            {
                int displayHour = alarmHour24;
                const char* ampm = "AM";
                if (displayHour == 0) { displayHour = 12; }
                else if (displayHour == 12) { ampm = "PM"; }
                else if (displayHour > 12) { displayHour -= 12; ampm = "PM"; }

                const char* status = TextFormat("Alarm set for %d:%02d %s", displayHour, alarmMinute, ampm);
                DrawTextF(status, 250, 300, 40, BLACK);
            }
        }
        else if (currentScreen == SET_TIME)
        {
            DrawTexture(txtu_bg, 0, 0, WHITE);

            DrawTextF("Set Alarm Time", 250, 30, 30, DARKGRAY);

            DrawRectangleRec(hourTrack, LIGHTGRAY);
            DrawRectangleLinesEx(hourTrack, 2, DARKGRAY);
            float hourThumbY = hourTrack.y + hourThumbRatio * hourTrack.height - 4;
            DrawRectangleRec({ hourTrack.x - 10, hourThumbY, 40, 8 }, BLUE);
            for (int h = 1; h <= 12; h++)
            {
                float y = hourTrack.y + (h - 1) / 11.0f * hourTrack.height;
                DrawTextF(TextFormat("%d", h), hourTrack.x - 45, y - 6, 16, DARKGRAY);
            }
            DrawTextF("Hour", hourTrack.x - 45, hourTrack.y - 30, 30, BLACK);

            DrawRectangleRec(minuteTrack, LIGHTGRAY);
            DrawRectangleLinesEx(minuteTrack, 2, DARKGRAY);
            float minuteThumbY = minuteTrack.y + minuteThumbRatio * minuteTrack.height - 4;
            DrawRectangleRec({ minuteTrack.x - 10, minuteThumbY, 40, 8 }, BLUE);
            for (int m = 0; m <= 50; m += 10)
            {
                float y = minuteTrack.y + (m / 59.0f) * minuteTrack.height;
                DrawTextF(TextFormat("%02d", m), minuteTrack.x + 30, y - 6, 16, DARKGRAY);
            }
            DrawTextF("Minute", minuteTrack.x - 45, minuteTrack.y - 30, 30, BLACK);

            Color amColor = isAM ? SKYBLUE : LIGHTGRAY;
            Color pmColor = !isAM ? SKYBLUE : LIGHTGRAY;
            Color amTextColor = isAM ? BLACK : GRAY;
            Color pmTextColor = !isAM ? BLACK : GRAY;

            DrawRectangleRec(checkAM, amColor);
            DrawRectangleLinesEx(checkAM, 2, DARKGRAY);
            DrawTextF("AM", checkAM.x + 20, checkAM.y + 8, 16, amTextColor);

            DrawRectangleRec(checkPM, pmColor);
            DrawRectangleLinesEx(checkPM, 2, DARKGRAY);
            DrawTextF("PM", checkPM.x + 20, checkPM.y + 8, 16, pmTextColor);

            DrawTextF(TextFormat("Selected: %d:%02d %s", hours, minutes, isAM ? "AM" : "PM"),
                      250, 500, 30, BLACK);

            DrawRectangleRec(btnBack, GRAY);
            DrawRectangleLinesEx(btnBack, 2, DARKGRAY);
            DrawTextF("Back", btnBack.x + 15, btnBack.y + 7, 20, WHITE);

            DrawRectangleRec(btnSet, DARKGREEN);
            DrawRectangleLinesEx(btnSet, 2, BLACK);
            DrawTextF("Set", btnSet.x + 25, btnSet.y + 7, 20, WHITE);
        }

        EndDrawing();
    }

    UnloadTexture(txtu_bg);
    UnloadTexture(txtu_btn_settings);
    UnloadFont(customFont);
    UnloadSound(alarmSound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}