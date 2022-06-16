# FiringPi

This project is designed to be run on a Raspberry Pi connected to various peripherals, to act as both fire controller
and datalogger for CUSF's White Dwarf hot and cold test fires. The utility files only run on linux, but you should be
able to work out equivalent commands for windows if you need to (if you do this please do put them in a .bat file and
pull request.).

## Requirements

To build on your local machine, you will need a native cross-compiler such
as [gcc/g++](https://www.gnu.org/software/gcc/) (You can change the compiler by editing the variable `$(GXX)` in a local
copy of the makfile and specifying `-f <local.makefile>` in the make command. The project
uses [ncurses](https://invisible-island.net/ncurses/announce.html) for its TUI, which works over standard ssh tty. You
will therefore need to make sure you have at least `libncurses5-dev` and `libncursesw5-dev` installed. When running on
the Raspberry Pi, [WiringPi](http://wiringpi.com/) is used for gpio access, which will need to be installed on your
local machine to compile if you wish to do so. The raspberry Pi also uses [tmux](https://linuxhandbook.com/tmux/) to
manage disconnects safely (the system leaving all of the valves open after an unhandled disconnect would not be good),
so if you use the script [/pi](/pi), then you will also require this. All of these are already installed on the flight
computer.

## Building and running

Clone the repo, then assert the directory is set up correctly with `make clean`. For a local build, use the makefile:

```console
make && ./main
```

To build on the Pi, you need to have ssh access, and make sure you have updated its IP address in either [/pi](/pi).
This file contains a script to upload source code directly to the Pi (with scp, no need for a git sync) and execute it
over ssh, forcing interactive terminal allocation, then building the project on the pi and executing it in a detatched
tmux session. You can also just run the executable on the pi in tmux without building with `./pi -r` or conversely to
build without running `./pi -b`

## Using the Program {curly brackets mean TODO}

Once you have launched the TUI successfully, you must launch the system into SAFE mode by pressing s[afe]. The system
may be armed by pressing <CTRL-A>, then {once you are satisfied that the system has booted correctly,} IGNITION mode may
be entered by pressing <SPACE>. The computer will then enter IGNITION, followed by FIRING and SHUTDOWN {as it runs the
preconfigured fire sequence}. An ABORT may be triggered by pressing <BACKSPACE> {or automatically by the system
detecting unsafe parameters}. ABORT mode can only be exitied by the computer, for safety reasons {currently press
e[rror] to debug exit}. SHUTDOWN exits to SAFE, and ABORT exits to ERROR, from which SAFE mode can be entered by
pressing s[afe]. To quit the program, the system must first be in the OFF state, entered by pressing o[ff] from SAFE
mode, only then it will exit when q[uit] is pressed.

## Project Structure

```markdown
cft-controller
├── build
│   ├── Display.o
│   ├── main.o
│   └── State.o
├── include
│   ├── Display.hpp
│   └── State.hpp
├── main
├── Makefile
├── pi
├── README.md
├── src
│   ├── Display.cpp
│   ├── main.cpp
│   └── State.cpp
└── TODO.md
```

## Useful Stuff

[ncurses HOWTO](https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/)
