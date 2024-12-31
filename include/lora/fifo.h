#pragma once

void lora_packet_push_irq(char *packet); // non-blocking version
void lora_packet_push(char *packet);

// to be used by the thread
char *_lora_packet_next();