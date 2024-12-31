#pragma once
#include <aprs/aprs.h>
#include <aprs/smart_beacon.h>
#include <gnss/gnss.h>
#include <stdint.h>
#include <stdlib.h>

struct APRS_Beacon_State {
    struct GNSS_Data last_gnss_data;
    int64_t last_fix_time;
    uint8_t was_first_fix;

    enum APRS_Beacon_Mode beacon_mode; // TODO:
    uint32_t ms_since_last_beacon;
    int beacon_sleep_ms;
};

// Beacon logic was separated so it could be easily tested
char *aprs_beacon_tick(struct GNSS_Data *gnss_data, 
                      struct APRS_Beacon_State *state,
                      struct APRS_Smart_Beacon_Config *aprs_sb_config,
                      struct APRS_Header *aprs_header);