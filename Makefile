# OG Source:
# https://github.com/CharlesDias/BLE-Watch/blob/main/Makefile
all: build

build:
	echo "--------------- Build the firmware ------------------"
	west build --build-dir build . --pristine always --board blackpill_f401cc -- -DDTC_OVERLAY_FILE:STRING="blackpill_f401cc.overlay.dts" -DCONF_FILE:STRING="prj.conf"

flash:
	echo "--------------- Flashing the firmware ---------------"
	west flash

clean:
	rm -rf build

.PHONY: build flash clean
