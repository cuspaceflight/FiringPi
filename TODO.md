# DESIGN TODO

This document is a place to lay out design decisions and problems that still need to be sorted or redesigned in the project.

## Essential:

- Daemon should run on the Pi, which the remote station can connect to, as is the case now.
- Data should be able to be transferred from the pi to the base station over a TCP socket, which should handle all types of disconnect gracefully and accept quick reconnects.
- Data stored locally at maximum rate, sent over socket at reduced rate.
- Utilise threading to avoid blocking in read and write operations.
- Can thread the I2C read and writes as each sensor will be on a separate bus.
- Control signals need to be accepted asynchronously so an abort is not held up by unresponsive I/O.

## Non-Essential
- GUI
- Control from multiple remote terminals for redundancy/ease of data aquisition.
- Send full-resolution data over remote link after firing.

## TODO
- ~~Run daemon on Pi~~
- Rewrite remote link system (as above) [feat/socket](cuspaceflight/Cold-Flow-Test-Electronics/tree/feat/sockets/)
- Write handler for I2C I/O
- Write handler for HX711 read
- Write handler for GPIO control of relay bank
- Write data storage handler
- Write data transmission handler
 - This transmission should really be UDP as speed is prioritised over individual packet integrity.

- Multiple remote connection support
- GUI

