#pragma once

struct GNSS_Data {
    double latitude;  // xx.xxxxxx
    double longitude; // xx.xxxxxx
    int bearing;      // 0 - 360 deg
    int speed;        // km/h
};

int get_gnss_data(struct GNSS_Data *out);