#pragma once
#include "aprs.h"
#include <gnss/gnss.h>

char *aprs_generate_pos_packet(struct APRS_Header *header, struct GNSS_Data *gnss_data, const char* comment);