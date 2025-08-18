<div align="center">
  <p>
    <a href="https://github.com/gpt32/application/releases/latest/download/gpt32.exe"><img src="https://img.shields.io/github/v/release/gpt32/application?sort=semver&style=for-the-badge&logo=github&label=Download%20Exe" /></a>
    <a href="https://github.com/GPT32/application/milestones"><img src="https://img.shields.io/badge/view_the-roadmap-blue?style=for-the-badge&logo=rocket&logoColor=white" /></a>
  </p>
  <p>Minimal ChatGPT client for Windows built entirely with the Win32 API.</p>
  <img src="https://raw.githubusercontent.com/GPT32/.github/refs/heads/main/assets/demo.gif" />
</div>

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
