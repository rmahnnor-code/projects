# Alarm Clock
A feature-rich alarm clock built in C++ using Raylib with a 
custom UI, real system time tracking, and looping alarm sound.

## Features
- Two-screen interface (Main & Set Time)
- Vertical sliders to set hours (1–12) and minutes (0–59)
- AM/PM selection with mutually exclusive checkboxes
- Real-time preview of selected alarm time
- Matches against actual system time to trigger alarm
- Looping alarm sound using a .wav file
- Stop button to dismiss the alarm
- Custom font, background image, and textured buttons
- Setting a new alarm auto-silences any active one

## Requirements
- [Raylib](https://www.raylib.com/)
- MinGW (C++ compiler)
- VS Code (recommended)

## How to Run
1. Clone the repo
2. Install Raylib
3. Place these files in the same folder as the executable:
   - `alarm.wav`
   - `bg.png`
   - `add_button.png`
   - `Pricedown.otf`
4. Compile with:
g++ alarm_clock.cpp -o alarm_clock -lraylib -lopengl32 -lgdi32 -lwinmm

5. Run the executable

## Built With
- C++
- Raylib