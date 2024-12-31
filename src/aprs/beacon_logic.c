#include <aprs/beacon_logic.h>
#include <aprs/packet.h>
#include <stdio.h>
#include <string.h>

char *aprs_beacon_tick(struct GNSS_Data *gnss_data, 
                      struct APRS_Beacon_State *state,
                      struct APRS_Smart_Beacon_Config *aprs_sb_config,
                      struct APRS_Header *aprs_header) {
    // don't send duplicate packets
    if (gnss_data->latitude == state->last_gnss_data.latitude 
        && gnss_data->longitude == state->last_gnss_data.longitude
        && state->was_first_fix == 0) {
        return NULL;
    }

    const int bearing_delta = abs(state->last_gnss_data.bearing - gnss_data->bearing);
    // TOFIX: bool maybe? Not in C, i guess.
    size_t is_corner_peg = aprs_sb_check_corner_peg(aprs_sb_config, gnss_data, bearing_delta, state->ms_since_last_beacon);

    int beacon_rate_ms = aprs_sb_get_rate_ms(aprs_sb_config, gnss_data);

    if (beacon_rate_ms < 15000) { // don't update more often than every 15s
        beacon_rate_ms = 15000;
    }

    if (state->ms_since_last_beacon < beacon_rate_ms && is_corner_peg == 0 && state->was_first_fix == 0) {
        return NULL;
    }

    printf("Generating POS packet\n");
    char *packet = aprs_generate_pos_packet(aprs_header, gnss_data, "kf20.org | is this beacon smart?");
    
    memcpy(&state->last_gnss_data, gnss_data, sizeof(struct GNSS_Data));
    state->ms_since_last_beacon = 0;

    return packet;
}