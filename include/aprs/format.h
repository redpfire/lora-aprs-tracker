#pragma once
#include <stdlib.h>

size_t aprs_format_latitude(double latitude, char *out);
size_t aprs_format_longitude(double longitude, char *out);
size_t aprs_format_speed(int speed, char *out);
size_t aprs_format_bearing(int bearing, char *out);