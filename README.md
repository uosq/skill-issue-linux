# Skill Issue Linux

A project for me to learn more about TF2 and how it works

This is made for Linux, so trying to build it on Windows or non Unix-like systems might not work!

[Download latest build](https://nightly.link/uosq/skill-issue-linux/workflows/main/main/artifact.zip)

[Click me to join the discord server](https://discord.com/invite/dA4nYDBZzu)


## Credits

8dcc - [libdetour](https://github.com/8dcc/libdetour)

8dcc - [libsigscan](https://github.com/8dcc/libsigscan)

ocornut - [imgui](https://github.com/ocornut/imgui)

wolfpld - [tracy](github.com/wolfpld/tracy)

## How to attach

Run the `attach.sh` file as **sudo**/**root**!

TF2 **should be** open and in the main menu. Currently this does not wait for the libraries to be opened so it will **silently fail** if you dont wait long enough

```bash
sudo ./attach.sh
```

## How to build

* Requirements
  * Linux
  * GCC/G++ (compiled with atleast standard C++17)
  * Pluto 0.12.2

Run `build.sh` to build the .so file

```bash
./build.sh
```