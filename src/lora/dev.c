#include <lora/dev.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/lora.h>
#include <stdio.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(lora_dev);

K_SEM_DEFINE(lora_dev_sem, 1, 1);

int lora_dev_init() {
    k_sem_take(&lora_dev_sem, K_FOREVER);

	struct lora_modem_config config;
	const struct device *const lora_dev = DEVICE_DT_GET(LORA_NODE);
    int ret = 0;

	if (!device_is_ready(lora_dev)) {
		printf("Failed to configure lora: device not ready\n");
		ret = -1;
        // I hate goto but I hate nesting hell more
        goto end;
	}

    // TODO: Dynamic configuration
	config.frequency = 434855000;
	config.bandwidth = BW_125_KHZ;
	config.datarate = SF_9;
	config.preamble_len = 8;
	config.coding_rate = CR_4_7;
	config.iq_inverted = false;
	config.public_network = false;
	config.tx_power = 20;
	config.tx = true;

	printf("Configuring lora modem\n");
	ret = lora_config(lora_dev, &config);
	if (ret < 0) {
		printf("Failed to configure lora\n");
        ret = -1;
	}

end:
    k_sem_give(&lora_dev_sem);
    return ret;
}

int lora_dev_send(uint8_t *data, uint32_t len) {
    k_sem_take(&lora_dev_sem, K_FOREVER);

	const struct device *const lora_dev = DEVICE_DT_GET(LORA_NODE);
    int ret = lora_send(lora_dev, data, len);

    if (ret < 0) {
        printf("Failed sending lora packet\n");
    }

    k_sem_give(&lora_dev_sem);

    return ret;
}