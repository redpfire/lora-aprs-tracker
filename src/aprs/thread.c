#include <aprs/aprs.h>
#include <aprs/beacon_logic.h>
#include <aprs/smart_beacon.h>
#include <aprs/thread.h>
#include <gnss.h>
#include <lora/fifo.h>
#include <string.h>
#include <zephyr/kernel.h>

/* size of stack area used by the thread */
#define STACKSIZE 1024

/* scheduling priority used by the thread */
#define PRIORITY 7

static struct k_thread aprs_thread_data;

void aprs_thread(void *dummy1, void *dummy2, void *dummy3) {
    ARG_UNUSED(dummy1);
    ARG_UNUSED(dummy2);
    ARG_UNUSED(dummy3);

    struct APRS_Beacon_State state;
    // TODO: Get rid of magic numbers etc.
    state.beacon_mode = BEACON_SMART;
    state.beacon_sleep_ms = 5000;

    state.ms_since_last_beacon = 0;
    state.last_fix_time = 0;

    struct GNSS_Data gnss_data;
    struct APRS_Header aprs_header;
    struct APRS_Smart_Beacon_Config aprs_sb_config;

    for (;;) {
        k_msleep(state.beacon_sleep_ms);
        if (get_gnss_data(&gnss_data) < 0) {
            // We don't have a fix yet
            continue;
        }

        state.was_first_fix = 0;

        if (state.last_fix_time == 0) { // this is our first fix
            memcpy(&state.last_gnss_data, &gnss_data, sizeof(struct GNSS_Data));
            state.was_first_fix = 1;
        }

        state.last_fix_time = k_uptime_get();
        state.ms_since_last_beacon += state.beacon_sleep_ms;

        // update our copy in case another thread changed it
        aprs_sb_get_config(&aprs_sb_config);
        aprs_get_header(&aprs_header);

        char *packet = aprs_beacon_tick(&gnss_data, &state, &aprs_sb_config, &aprs_header);

        if (packet != NULL) {
            lora_packet_push(packet);
        }
    }
}
K_THREAD_STACK_DEFINE(aprs_thread_stack_area, STACKSIZE);

void spawn_aprs_thread() {
    // TODO: well, yeah... mess...
    struct APRS_Header header;
    header.source_call = "SP9OTA-7";
    // TODO: separate dst call and path for sure
    header.path = "APOTA0,WIDE1-1,WIDE2-1";
    header.data_type = '!';
    header.symbol_table = '\\';
    header.symbol = 'U';

    aprs_set_header(&header);

    struct APRS_Smart_Beacon_Config sb_config;
    aprs_sb_apply_defaults(&sb_config);

    aprs_sb_set_config(&sb_config);

    k_thread_create(&aprs_thread_data, aprs_thread_stack_area, K_THREAD_STACK_SIZEOF(aprs_thread_stack_area),
                    aprs_thread, NULL, NULL, NULL, PRIORITY, 0, K_FOREVER);
    k_thread_name_set(&aprs_thread_data, "aprs_thread");

    // yay!
    k_thread_start(&aprs_thread_data);
}