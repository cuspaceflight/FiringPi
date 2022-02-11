# Cold-Flow-Test-Electronics
This project is designed to be run on a Raspberry Pi connected to various peripherals, to act as both fire controller and datalogger for CUSF's White Dwarf hot and cold test fires.
The utility files only run on linux, but you should be able to work out equivalent commands for windows if you need to (if you do this please do put them in a .bat file and pull request.).

## Requirements
To build on your local machine, you will need a native cross-compiler such as [gcc/g++](https://www.gnu.org/software/gcc/) (You can change the compiler by editing the variable `$(GXX)` in a local copy of the makfile and specifying `-f <local.makefile>` in the make command.
The project uses [ncurses](https://invisible-island.net/ncurses/announce.html) for its TUI, which works over standard ssh tty.
You will therefore need to make sure you have `libncurses5-dev` and `libncursesw5-dev` installed.
When running on the Raspberry Pi, [WiringPi](http://wiringpi.com/) is used for gpio access, which will need to be installed on your local machine to compile if you wish to do so.
All of these are already installed on the flight computer.

## Building and running
Clone the repo, then manually create the build directory with `mdkir obj`.
For a local build, use the makefile:
```console
make && ./main
```
To build on the Pi, you need to have ssh access, and make sure you have updated its IP address in [/pi](pi). This file contains a script to upload source code directly to the Pi (with rsync, no need for a git sync) and execute it over ssh, forcing interactive terminal allocation so you can use the TUI. Using the script:
```console
./pi
```
Or just to run without building (will add options to [/pi](pi) for this in the future)
```console
ssh -tt cusf@<pi-addr> "cd cft-controller && make && exec ./main"
```

## Using the Program {curly brackets mean TODO} 
Once you have launched the TUI successfully, you must launch the system into SAFE mode by pressing <s[afe]>.
The system may be armed by pressing <CTRL-A>, and once you are satisfied that the firing is safe, STARTUP mode may be entered by pressing <SPACE>.
The computer will then enter STARTUP, followed by FIRING and SHUTDOWN {as it runs the preconfigured fire sequence}.
An ABORT may be triggered by pressing <BACKSPACE> {or automatically by the system detecting unsafe parameters}. 
ABORT mode can only be exitied by the computer, for safety reasons.
SHUTDOWN exits to SAFE, and ABORT exits to ERROR, from which SAFE mode can be entered by pressing <s>.
To quit the program, the system must first be in the OFF state, entered by pressing <o[ff]> from SAFE mode, only then it will exit when <q[uit]> is pressed.

## Project Structure
```markdown
cft-controller
├── inc           -> c++ headers
│   └── mux.hpp
├── [main]        -> compiled binary (generate during build)
├── makefile
├── obj           -> build directory (make before first build)
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
