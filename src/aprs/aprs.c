#include <aprs/aprs.h>
#include <zephyr/kernel.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct APRS_Header aprs_header;
K_SEM_DEFINE(aprs_sem, 1, 1);

void aprs_set_header(struct APRS_Header *data) {
    k_sem_take(&aprs_sem, K_FOREVER);

    memcpy(&aprs_header, data, sizeof(struct APRS_Header));

    k_sem_give(&aprs_sem);
}

void aprs_get_header(struct APRS_Header *data) {
    k_sem_take(&aprs_sem, K_FOREVER);

    memcpy(data, &aprs_header, sizeof(struct APRS_Header));

    k_sem_give(&aprs_sem);
}

size_t aprs_write_header(struct APRS_Header *header, char *out) {
    return sprintf(out, "%s>%s:%c", header->source_call, header->path, header->data_type);
}

size_t aprs_write_raw_packet(struct APRS_Header *header, char *payload, char *out) {
    // LORA APRS packet identifier
    out[0] = '<';
    out[1] = 0xff;
    out[2] = 0x01;
    out += 3;

    size_t header_len = aprs_write_header(header, out);
    out += header_len;

    return 3 + header_len + sprintf(out, "%s", payload);
}