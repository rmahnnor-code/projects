Here is a professional, detailed, and clean `README.md` file tailored specifically for your app layout, thread monitoring, and asset structure.

---

# README.md

```markdown
# LOCK IN - Focus Lock & Process Blocker

**LOCK IN** is a lightweight, multi-threaded C++ productivity tool built using the **raylib** and **raygui** frameworks. It helps developers, students, and professionals reclaim their focus by actively monitoring and terminating distracting background software (browsers, games, launchers) matching a customized target list.

Featuring a tailored medieval/high-contrast retro UI wrapper, the app utilizes low-level Windows APIs to enforce screen-time discipline securely in real time.

---

## 🚀 Features

* **Multi-Threaded Monitoring**: Dedicated background worker thread runs independently of the 60 FPS visual UI thread, ensuring zero frame drops or input lag.
* **Active Title Detection**: Directly scans the OS foreground window and targets process signatures (`HWND`) instead of relying on heavy performance-sapping loops.
* **Pre-configured Targets & Custom Ingestion**: One-click selection for popular applications (Brave, Steam, Discord, Chrome, YouTube, etc.) alongside a dynamic text field entry system to target any custom application window name.
* **Immediate Process Enforcement**: Instantly terminates blocked application domains the exact moment they grab system focus.

---

## 🛠️ Architecture & Core Mechanics

The program operates using an asynchronous thread division mechanism to optimize system performance:


```

┌──────────────────────────────────────┐     Signals     ┌──────────────────────────────────────┐
│            MAIN UI THREAD            │ ──────────────> │       BACKGROUND MONITOR THREAD      │
│  • Raylib UI Engine (60 FPS)         │  (Atomic Flags) │  • High-Precision Kernel Scan (~2Hz) │
│  • Render Input Text / Textures      │ <────────────── │  • Window Title Extraction & Lock    │
└──────────────────────────────────────┘                 └──────────────────────────────────────┘

```

1. **Foreground Interaction Engine**: Runs a static 60 FPS render cycle mapping raw application logic arrays onto custom texture slices and icon fonts.
2. **Asynchronous Process Interceptor**: Wakes every 500–1000ms, grabs the active foreground handle (`GetForegroundWindow`), reads its wide-string text layout, and references it against a thread-safe mutex-locked dynamic list (`std::lock_guard`). 

---

## 📂 Project Structure

Ensure your executable directory maintains the following asset infrastructure for textures and typefaces to map properly:

```text
├── assets/
│   ├── Font/
│   │   ├── Restful_Silent.ttf    # Elegant title branding typography
│   │   └── AUGUSTUS.ttf          # Classic serif style for UI buttons & labels
│   ├── Textures/
│   │   ├── BG3.png               # System background panorama texture
│   │   ├── button3.png           # Specialized structural button frames
│   │   ├── CheckBox2.png         # Custom unselected checkbox layout
│   │   ├── CheckBoxChecked2.png  # Custom selected checkbox layout
│   │   └── HomeIcon.png          # Main menu utility graphic
│   └── ICONS/
│       └── [BI.png ... RI.png]   # Application-specific visual logo indicators
└── main.cpp                      # Core source code pipeline

```

---

## ⚡ Setup and Compilation

### Prerequisites

* Windows OS (Utilizes Win32 APIs like `<windows.h>`)
* C++17 or higher compatible compiler (GCC/MinGW, MSVC)
* **raylib** v4.5+ and accompanying **raygui** headers

### Compilation Command (MinGW Example)

Compile from your root directory including standard Windows library linkage flags:

```bash
g++ main.cpp -o LockIn.exe -lraylib -lopengl32 -lgdi32 -lwinmm

```

---

## 📖 How to Use

1. **Launch**: Open the application to find the **LOCK TF IN** initialization screen.
2. **Select Targets**: Hit *Select APPs* to view your dashboard. Check target boxes or use the entry input block at the bottom to register specific target titles.
3. **Engage Blocker**: Click **LOCK** to initialize background monitoring. The UI moves to its restriction layout, and the background thread begins process tracking.
4. **Disengage Safely**: Click **UNLOCK** to pause process monitoring cycles safely whenever you need to return to normal operation.

---

## 🔒 Security & Optimization Note

This software communicates safely with Windows process subsystems using thread-isolated atomic operations (`std::atomic<bool>`). Process destruction sequences utilize explicit, limited-access kernel queries (`PROCESS_TERMINATE`) avoiding intrusive system permission exploits.

```

```