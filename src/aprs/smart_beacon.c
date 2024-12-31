#include <aprs/packet.h>
#include <aprs/smart_beacon.h>
#include <string.h>
#include <zephyr/kernel.h>

static struct APRS_Smart_Beacon_Config sb_config;
K_SEM_DEFINE(aprs_sb_sem, 1, 1);

void aprs_sb_get_config(struct APRS_Smart_Beacon_Config *data) {
    k_sem_take(&aprs_sb_sem, K_FOREVER);

    memcpy(data, &sb_config, sizeof(struct APRS_Smart_Beacon_Config));

    k_sem_give(&aprs_sb_sem);
}

void aprs_sb_set_config(struct APRS_Smart_Beacon_Config *data) {
    k_sem_take(&aprs_sb_sem, K_FOREVER);

    memcpy(&sb_config, data, sizeof(struct APRS_Smart_Beacon_Config));

    k_sem_give(&aprs_sb_sem);
}

void aprs_sb_apply_defaults(struct APRS_Smart_Beacon_Config *config) {
    // TODO: A real config
    // Those are for a walking human
    config->fast_speed = 15; // km/h
    config->slow_speed = 3;  // km/h

    config->fast_rate_ms = 60 * 1000;
    config->slow_rate_ms = 120 * 1000;

    // corner peg detection params
    config->min_turn_time_ms = 15 * 1000;
    config->min_turn_angle = 3; // deg
    config->turn_slope = 40;    // quote: This number, when divided by your current
                                // speed will be added to the Min Turn Angle in order
                                // to increase the turn threshold at lower speeds.
}

size_t aprs_sb_check_corner_peg(struct APRS_Smart_Beacon_Config *config, struct GNSS_Data *gnss_data,
                                int heading_delta_since_beacon, int ms_since_beacon) {
    if (gnss_data->speed == 0) {
        return 0;
    }

    int turn_threshold = config->min_turn_angle + config->turn_slope / gnss_data->speed;

    if (heading_delta_since_beacon > turn_threshold && ms_since_beacon > config->min_turn_time_ms) {
        return 1;
    }

    return 0;
}

int aprs_sb_get_rate_ms(struct APRS_Smart_Beacon_Config *config, struct GNSS_Data *gnss_data) {
    if (gnss_data->speed < config->slow_speed) { // stopped condition
        return config->slow_rate_ms;
    }

    if (gnss_data->speed > config->fast_speed) { // wzium
        return config->fast_rate_ms;
    }

    return config->fast_rate_ms * config->fast_speed / gnss_data->speed;
}