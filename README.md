# Skill Issue Linux

A project for me to learn more about TF2 and how it works

This is made for Linux, so trying to build it on Windows or non Unix-like systems might not work!

[Download latest compatiblity build](https://nightly.link/uosq/skill-issue-linux/workflows/main/main/artifact.zip)
[Download latest v3 build](https://nightly.link/uosq/skill-issue-linux/workflows/main/main/artifact.zip)

[Click me to join the discord server](https://discord.com/invite/dA4nYDBZzu)

## Credits

8dcc - [libdetour](https://github.com/8dcc/libdetour)

8dcc - [libsigscan](https://github.com/8dcc/libsigscan)

ocornut - [imgui](https://github.com/ocornut/imgui)

brofield - [simpleini](https://github.com/brofield/simpleini)

## Info

The files `attach.sh`, `build.sh` and `debug.sh` can be specified which version to use

Example:

Building with x86-64-v3 march
```bash
./build.sh v3
```

Building with x86-64 march
```bash
./build.sh compat
```

The scripts will try to identify which architecture it should use if you don't specify it (v3 or compat)
```bash
./build.sh
```

## How to attach

Run the `attach.sh` file as **sudo**/**root**!

TF2 **should be** open and in the main menu. Currently this does not wait for the libraries to be opened so it will **silently fail** if you dont wait long enough

```bash
cd build
sudo ./attach.sh
```

## How to build

* Requirements
  * Linux
  * GCC/G++ (compiled with atleast standard C++17)

Run `build.sh` to build the .so file

```bash
./build.sh
```