[![Download](https://img.shields.io/github/downloads/gpt32/application/latest/gpt32.exe?sort=semver&style=for-the-badge&logo=mingww64&label=Windows)](https://github.com/gpt32/application/releases/latest/download/gpt32.exe)

Minimal ChatGPT client for Windows built entirely with the Win32 API.

![screenshot](https://raw.githubusercontent.com/GPT32/.github/refs/heads/main/assets/demo.gif)

## Building

Download and run [Microsoft C++ Build Tools 2022](https://visualstudio.microsoft.com/visual-cpp-build-tools/).

Ensure the following components are installed from the Main Menu.

- Visual C++ build tools
  - MSVC compiler
  - C++ CMake tools for Windows
  - C++ Clang tools for Windows
  - Windows 10 or 11 SDK

```bash
cmake --preset msvc-configure
cmake --build --preset msvc-build
```
