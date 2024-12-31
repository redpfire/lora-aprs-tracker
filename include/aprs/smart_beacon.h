#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <gnss/gnss.h>

struct APRS_Smart_Beacon_Config {
    uint8_t slow_speed;
    uint8_t fast_speed;

    uint32_t slow_rate_ms;
    uint32_t fast_rate_ms;

    uint32_t min_turn_time_ms;
    uint16_t min_turn_angle;
    uint16_t turn_slope;
};

void aprs_sb_get_config(struct APRS_Smart_Beacon_Config *data);
void aprs_sb_set_config(struct APRS_Smart_Beacon_Config *data);
void aprs_sb_apply_defaults(struct APRS_Smart_Beacon_Config *config);

size_t aprs_sb_check_corner_peg(struct APRS_Smart_Beacon_Config *config, struct GNSS_Data *gnss_data, int heading_delta_since_beacon, int ms_since_beacon);
int aprs_sb_get_rate_ms(struct APRS_Smart_Beacon_Config *config, struct GNSS_Data *gnss_data);