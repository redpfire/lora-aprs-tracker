#include <aprs/format.h>
#include <math.h>
#include <stdio.h>

size_t aprs_format_latitude(double latitude, char *out) {
    char ns = (latitude < 0) ? 'S' : 'N';
    latitude = fabs(latitude);
    int degrees = (int)latitude;
    double minutes = (latitude - (double)degrees) * 60.0;
    return sprintf(out, "%02d%05.2f%c", degrees, minutes, ns);
}

size_t aprs_format_longitude(double longitude, char *out) {
    char ew = (longitude < 0) ? 'W' : 'E';
    longitude = fabs(longitude);
    int degrees = (int)longitude;
    double minutes = (longitude - (double)degrees) * 60.0;
    return sprintf(out, "%03d%05.2f%c", degrees, minutes, ew);
}

size_t aprs_format_speed(int speed, char *out) {
    return sprintf(out, "%03d", speed);
}

size_t aprs_format_bearing(int bearing, char *out) {
    return sprintf(out, "%03d", bearing);
}