#include <aprs/aprs.h>
#include <aprs/format.h>
#include <aprs/packet.h>
#include <zephyr/kernel.h>

#include <math.h>
#include <stdio.h>
#include <string.h>

char *aprs_generate_pos_packet(struct APRS_Header *header, struct GNSS_Data *gnss_data, const char *comment) {
    // TOFIX: assert this is not null
    // this packet will be sent over fifo to other threads
    // to assure that we don't have to copy it i used k_malloc here
    // remember to k_free() it and prevent use after free!
    char *packet = (char *)k_malloc(400);
    if (!packet) {
        // TODO: Debug log
        printf("Could not generate POS packet. Out of memory.");
        return NULL;
    }

    memset(packet, 0, 400);

    char raw_packet[100] = {0};
    char latString[20] = {0}, lonString[20] = {0}, speedString[20] = {0}, bearingString[20] = {0};

    int speed = (int)round((double)gnss_data->speed * 0.539957); // km/h to knots

    aprs_format_latitude(gnss_data->latitude, latString);
    aprs_format_longitude(gnss_data->longitude, lonString);

    if (speed > 0) {
        aprs_format_speed(speed, speedString);
        aprs_format_bearing(gnss_data->bearing, bearingString);

        sprintf(raw_packet, "%s%c%s%c%s/%s%s", latString, header->symbol_table, lonString, header->symbol,
                bearingString, speedString, comment);
    } else {
        sprintf(raw_packet, "%s%c%s%c%s", latString, header->symbol_table, lonString, header->symbol, comment);
    }

    aprs_write_raw_packet(header, raw_packet, packet);
    // TODO: assert that there is no buffer overflow

    return packet;
}