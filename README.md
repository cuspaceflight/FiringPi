# Cold-Flow-Test-Electronics
This project is designed to be run on a Raspberry Pi connected to various peripherals, to act as both fire controller and datalogger for CUSF's White Dwarf hot and cold test fires. The utility files only run on linux, but you should be able to work out equivalent commands for windows if you need to (if you do this please do put them in a .bat file and pull request.).

## Requirements
To build on your local machine, you will need a native cross-compiler such as [gcc/g++](https://www.gnu.org/software/gcc/) (You can change the compiler by editing the variable $(GXX) in a local copy of the makfile and specifying `-f <local.makefile>` in the make command. The project uses ncurses for a TUI, which works over standard ssh tty. You will therefore need to make sure you have `libncurses5-dev` and `libncursesw5-dev` installed. When running on the Raspberry Pi, `wiringPi` is used for gpio access, which will need to be installed on your local machine to compile, if you wish to do so. All of these are already installed on the flight computer.

## Building and running
For a local build, use the makefile.
```console
$ make && ./main
```
To build on the Pi, you need to be on the same network as it is, and make sure you have updated its IP address in [/pi](pi). This file contains a script to upload source code directly to the Pi (with rsync, no need for a git sync) and execute it over ssh, forcing interactive terminal allocation so you can use the TUI. Using the script:
```console
$ ./pi
```
Or just to run without building (will add options to [/pi](pi) for this in the future)
```console
$ ssh -tt cusf@10.9.36.130 "cd cft-controller && make && exec ./main"
```

## Project Structure
```markdown
project
├── inc           -> c++ headers
│   └── mux.hpp
├── [main]        -> compiled binary
├── makefile
├── obj           -> build directory
│   ├── main.o
│   └── mux.o
├── pi         
├── README.md
└── src           -> c++ sources
    ├── main.cpp
    └── mux.cpp
```

## Useful Stuff
[ncurses HOWTO](https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/)
