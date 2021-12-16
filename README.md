# About Hemiola

Hemiola aims to turn any n-key rollover keyboard into a chordable keyboard.
Instead of typing one letter at a time one would press multiple letters at
the same time and the letters would show as if one had typed the letters
in order. The only physical requirements are a usb keyboard and a
Rapberry Pi Zero. There are no dependency requirements on the OS side of
things.

# Building Hemiola
## Build Dependencies
  - C++ 17 compiler, GCC >= v8, Clang >= v7, MSVC >= 2017
  - CMake >= v3.12
  - ICU4C
## Building on GNU/Linux and Unixes

We first need to download the dependencies. Some may already be
preinstalled.

For Ubuntu and Debian:

```bash
sudo apt install gcc git cmake libicu-dev
```

To get the source code clone the repository

```bash
git clone --recurse-submodules https://github.com/erichlf/Hemiola.git
```

To build Hemiola

```bash
cd Hemiola
mkdir build
cmake -B ./build .
cmake --build ./build
cmake --install ./build
```
