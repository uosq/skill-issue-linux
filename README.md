# Skill Issue Linux

A project for me to learn more about TF2 and how it works

This is designed exclusively for Linux. Attempting to build or run it on Windows or other non-Unix-like systems might/will not work

[Click me to join the discord server](https://discord.com/invite/dA4nYDBZzu)

## Credits

8dcc - [libdetour](https://github.com/8dcc/libdetour)

8dcc - [libsigscan](https://github.com/8dcc/libsigscan)

ocornut - [imgui](https://github.com/ocornut/imgui)

brofield - [simpleini](https://github.com/brofield/simpleini)

## Prerequisites

Before building, make sure your system meets the following requirements:

* OS: Linux (Ubuntu, Arch, Gentoo, Nix, etc)
* Compiler: GCC (supporting at least the C++17 standard)
* Dependencies: libsdl2-dev, libcurl4-openssl-dev, libsdl2-dev, libvulkan-dev

## Build Architecture

| Flag | Target | Description
| - | - | - |
| v3 | x86-64-v3 | **Recommended**, has better performance |
| compat | x86-64 (Generic) | Use this if you see "Illegal Instruction" errors when attaching |

## How To Build

You can compile the project using the provided `build.sh` script

### First Option: Auto

If you don't specify a mode, the build script will try to identify what instructions your CPU can use and will choose between `v3` and `compat`

```bash
./build.sh
```

### Second Option: Manual

If you specify a mode (either `v3` or `compat`), the build script will compile to that architecture

[Go to Build Architecture to understand the differences](#build-architecture)

The files `attach.sh`, `build.sh` and `debug.sh` can be specified which version to use

## How To Attach

Once you have successfully built the `.so` file, you can follow these very simple steps to attach it to the game:

1. Launch TF2
2. Run the `attach.sh` script inside the `build` folder as **sudo**

You don't need to wait for TF2 to be fully loaded to attach, but it is safer to wait until you can see the main menu to avoid crashes while attaching

Example:

```bash
cd build
sudo ./attach.sh
```
## Loader

There is a separate loader you can use: `sk_loader.sh` that provides a very user-friendly and easy to use UI

To use it, you'll need the following dependencies: **wget**, **whiptail**, **unzip**, **make**, **grep**
