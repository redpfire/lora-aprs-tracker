# LoRa APRS Tracker POC using Zephyr

This is very limited. Be cautioned. And this project only compiles for the STM32F401 (Blackpill dev board).

Licensed under the Apache 2.0 license so use freely.

What it does:

[x] Fetch GPS position from GNSS receiver (uBlox M6)
[x] Send LoRa packets (with SX1276)
[x] SmartBeacon(R) !!
[x] Multithreading - LoRa thread that sends data and APRS thread (could do more than LoRa APRS)

What it does not do:
[ ] Configuration (everything is in code and with magic numbers; womp womp)
[ ] OLED Screen
[ ] You tell me

## Prerequisites

```sh
git submodule update --init --recursive 
west init
```

## Build

```sh
make # will build everything for STM32F401
```

# Flash

```sh
make flash
```