#pragma once
#include <stdint.h>
#include <stdlib.h>

#define LORA_NODE DT_ALIAS(lora0)

int lora_dev_init();
int lora_dev_send(uint8_t *data, uint32_t len);