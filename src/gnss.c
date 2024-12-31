#include <gnss/gnss.h>
#include <lora/fifo.h>
#include <zephyr/drivers/gnss.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

static struct GNSS_Data gnss_data;
ATOMIC_DEFINE(fix_flag_bits, 1);

K_SEM_DEFINE(gnss_sem, 1, 1);

#define GNSS_MODEM DEVICE_DT_GET(DT_ALIAS(gnss))

int get_gnss_data(struct GNSS_Data *out) {
	int ret = -1;
	k_sem_take(&gnss_sem, K_FOREVER);

	if(atomic_test_bit(fix_flag_bits, 0)) {
		ret = 0;

		memcpy(out, &gnss_data, sizeof(struct GNSS_Data));
	}

	k_sem_give(&gnss_sem);

	return ret;
}

static void gnss_data_cb(const struct device *dev, const struct gnss_data *data)
{
	if (data->info.fix_status != GNSS_FIX_STATUS_NO_FIX) {
		// Those updates are frequent enough that we can drop some
		if (k_sem_take(&gnss_sem, K_NO_WAIT) < 0) {
			return;
		}

		gnss_data.latitude = (double)(data->nav_data.latitude) * 1e-9; // from nanodegrees to degrees
		gnss_data.longitude = (double)(data->nav_data.longitude) * 1e-9; // from nanodegrees to degrees
		gnss_data.bearing = (int)round((double)(data->nav_data.bearing) * 1e-3);
		gnss_data.speed = (int)round((double)(data->nav_data.speed) * 36e-4); // from mm/s to km/h

        // TOFIX: Make this a debug log instead
		printf("Newest fix: LAT %f   LONG %f   BEAR %d   SPD %d\n", gnss_data.latitude, gnss_data.longitude, gnss_data.bearing, gnss_data.speed);

		atomic_set_bit(fix_flag_bits, 0);
		k_sem_give(&gnss_sem);
	}
}
GNSS_DATA_CALLBACK_DEFINE(GNSS_MODEM, gnss_data_cb);

#if CONFIG_GNSS_SATELLITES
static void gnss_satellites_cb(const struct device *dev, const struct gnss_satellite *satellites,
			       uint16_t size)
{
	unsigned int tracked_count = 0;

	for (unsigned int i = 0; i != size; ++i) {
		tracked_count += satellites[i].is_tracked;
	}
	/* printf("%u satellite%s reported (of which %u tracked)!\n", */
	/* 	size, size > 1 ? "s" : "", tracked_count); */
}
#endif
GNSS_SATELLITES_CALLBACK_DEFINE(GNSS_MODEM, gnss_satellites_cb);