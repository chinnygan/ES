#!/bin/sh

if [ "$1 " == " " ]; then
	echo "Usage: ./flash.sh [ELF binary to flash]"
	exit
fi;

OLDFIRMWARE=`lsusb | grep "15a2:0058"`

if [ "$OLDFIRMWARE " != " " ]; then
	echo "Alert! The connected board is running an old firmware version. Please hand it to the TA to be upgraded."
	exit
fi;

DEVICE=`lsusb | grep "15a2:005e"`

if [ "$DEVICE " == " " ]; then
	echo "Could not find device. Please try unplugging and plugging it in again"
	exit
fi;

OPENOCD_ROOT=/group/teaching/espractical/OpenOCD
OPENOCD=$OPENOCD_ROOT/bin/openocd

$OPENOCD -s $OPENOCD_ROOT/share/openocd/scripts/ -f $OPENOCD_ROOT/flash.cfg -c "flash_program $1"
