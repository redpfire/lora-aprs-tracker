#pragma once
#include <stdlib.h>

struct APRS_Header {
    char *source_call;
    char *path;
    char data_type; // '!'
    char symbol_table; // '/'
    char symbol;
};

enum APRS_Beacon_Mode {
    BEACON_SMART = 0
};

void aprs_set_header(struct APRS_Header *data);
void aprs_get_header(struct APRS_Header *data);

size_t aprs_write_header(struct APRS_Header* header, char *out);
size_t aprs_write_raw_packet(struct APRS_Header* header, char *payload, char *out);