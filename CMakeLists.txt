# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb")
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(lora_tracker)

target_sources(app PRIVATE 
               src/main.c
               src/aprs/aprs.c
               src/aprs/format.c
               src/aprs/packet.c 
               src/aprs/thread.c 
               src/aprs/smart_beacon.c
               src/aprs/beacon_logic.c
               src/gnss.c
               src/lora/dev.c
               src/lora/fifo.c
               src/lora/thread.c)
target_include_directories(app PRIVATE include include/aprs include/gnss include/lora)
