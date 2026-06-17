# 🔥 Fire Simulation in C++ with SFML

A real-time particle-based fire simulation built from scratch using **C++17** and **SFML 3.0**, featuring dynamic wind, ember physics, additive blending, and interactive fire movement.

![Fire Simulation Preview](https://raw.githubusercontent.com/snehal-thombare08/Fire-Simulation-/main/Screenshot%202026-06-17%20113609.png)

---

## ✨ Features

- **Particle System** — 1000+ particles simulated in real-time at 60 FPS
- **Realistic Color Gradient** — Black → Deep Red → Orange → Yellow → White hot core
- **Ember System** — Glowing embers fly upward and flutter in the wind
- **Additive Blending** — GPU-style glow effect for realistic fire brightness
- **Dynamic Wind** — Smooth wind interpolation with natural turbulence sway
- **Ground Glow** — Pulsing orange light illuminates the ground beneath the fire
- **Interactive** — Move fire anywhere with mouse, control wind and intensity

---

## 🎮 Controls

| Key / Button | Action |
|---|---|
| `LMB` (Left Click + Drag) | Move fire to mouse position |
| `W` | Wind blows LEFT |
| `S` | Wind blows RIGHT |
| `+` | Increase fire intensity |
| `-` | Decrease fire intensity |
| `R` | Reset fire and wind |
| `Esc` | Quit |

---

## 🛠️ Tech Stack

| Tool | Version |
|---|---|
| Language | C++17 |
| Graphics | SFML 3.0.2 |
| Build System | CMake 3.16+ |
| Compiler | MinGW GCC 16.1 |
| Package Manager | vcpkg |

---

## ⚙️ Physics Details

- **Particle Lifetime:** 0.6s – 1.8s (fire), 1.0s – 2.5s (embers)
- **Velocity:** Random upward with horizontal turbulence per frame
- **Wind:** Smooth lerp toward target with sinusoidal natural sway
- **Ember Flutter:** Sin-wave horizontal oscillation for realistic floating
- **Color Mapping:** 6-stage gradient based on normalized lifetime ratio

---

## 🚀 Build & Run (Windows)

### Prerequisites
- [MinGW-w64](https://www.mingw-w64.org/)
- [CMake](https://cmake.org/)
- [vcpkg](https://github.com/microsoft/vcpkg) with SFML 3

### Steps

```bash
git clone https://github.com/snehal-thombare08/Fire-Simulation-.git
cd Fire-Simulation-
mkdir build && cd build

cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake \
         -DSFML_DIR=C:/vcpkg/installed/x86-mingw-dynamic/share/sfml \
         -DCMAKE_CXX_COMPILER="C:/mingw64/mingw32/bin/g++.exe" \
         -G "MinGW Makefiles"

mingw32-make
```

Copy DLLs and run:
```bash
copy "C:\vcpkg\installed\x86-mingw-dynamic\debug\bin\*.dll" .
.\FireSim.exe
```

---

## 📦 Download

👉 **[Download FireSim-v1.0-Windows.zip](https://github.com/snehal-thombare08/Fire-Simulation-/releases/tag/v1.0)**

Extract all files and run `FireSim.exe` — no install needed!

---

## 📁 Project Structure

```
Fire-Simulation-/
├── src/
│   └── main.cpp        # Particle system + physics + rendering
├── CMakeLists.txt      # Build configuration
├── .gitignore
└── README.md
```

---

## 🎯 What I Learned

- Building a **particle system** from scratch with spawn, update, and kill lifecycle
- **Additive blending** (`sf::BlendAdd`) for realistic glow effects
- **Color gradient mapping** based on particle lifetime for fire realism
- Smooth **wind physics** with lerp and sinusoidal turbulence
- Optimizing **1000+ particles** at 60 FPS with efficient rendering

---

*Built with ❤️ using C++ and SFML 3.0*
