#include <lora/dev.h>
#include <lora/fifo.h>
#include <lora/thread.h>
#include <stdio.h>
#include <zephyr/kernel.h>

/* size of stack area used by the thread */
#define STACKSIZE 1024

/* scheduling priority used by the thread */
#define PRIORITY 7

#define PACKET_SLEEP_MS 20

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(lora_thread);

static struct k_thread lora_thread_data;

void lora_thread(void *dummy1, void *dummy2, void *dummy3) {
    ARG_UNUSED(dummy1);
    ARG_UNUSED(dummy2);
    ARG_UNUSED(dummy3);

    int ret = 0;
    for (;;) {
        // This blocks on the fifo
        char *packet = _lora_packet_next();

        if (packet == NULL) {
            k_msleep(PACKET_SLEEP_MS);
            continue;
        }

        // TODO: Eventually there will be more than APRS packets here
        // Ideally there should be a struct sent over the fifo with void* to data
        // and its size
        printf("Sending new APRS packet: %s\n", packet + 3);
        ret = lora_dev_send((uint8_t *)packet, strlen(packet));

        k_msleep(1000);
        k_free(packet);
        packet = NULL;
    }
}
K_THREAD_STACK_DEFINE(lora_thread_stack_area, STACKSIZE);

void spawn_lora_thread() {
    k_thread_create(&lora_thread_data, lora_thread_stack_area, K_THREAD_STACK_SIZEOF(lora_thread_stack_area),
                    lora_thread, NULL, NULL, NULL, PRIORITY, 0, K_FOREVER);
    k_thread_name_set(&lora_thread_data, "lora_dev_thread");

    // yay!
    k_thread_start(&lora_thread_data);
}