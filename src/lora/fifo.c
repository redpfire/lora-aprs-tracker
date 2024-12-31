#include <lora/fifo.h>
#include <zephyr/kernel.h>
#include <zephyr/types.h>

K_FIFO_DEFINE(lora_packet_fifo);
K_SEM_DEFINE(lora_packet_sem, 1, 1);

void lora_packet_push_irq(char *packet) {
    k_sem_take(&lora_packet_sem, K_NO_WAIT);
    k_fifo_alloc_put(&lora_packet_fifo, packet);
    k_sem_give(&lora_packet_sem);
}

void lora_packet_push(char *packet) {
    k_sem_take(&lora_packet_sem, K_FOREVER);
    k_fifo_alloc_put(&lora_packet_fifo, packet);
    k_sem_give(&lora_packet_sem);
}

char *_lora_packet_next() {
    k_sem_take(&lora_packet_sem, K_FOREVER);
    char *pkt = k_fifo_get(&lora_packet_fifo, K_NO_WAIT);
    k_sem_give(&lora_packet_sem);

    return pkt;
}