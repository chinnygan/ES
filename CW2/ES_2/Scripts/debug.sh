#!/bin/sh

if [ "$1 " == " " ]; then
	echo "Usage: ./debug.sh [ELF binary to debug (should already have been flashed to the device)]"
	exit
fi;

OPENOCD_ROOT=/group/teaching/espractical/OpenOCD/
OPENOCD=$OPENOCD_ROOT/bin/openocd
GDB=/group/teaching/espractical/gdb/bin/arm-unknown-eabi-gdb

# Open OpenOCD in such a way that it is backgrounded, but that we wait for it to start up
$OPENOCD -s $OPENOCD_ROOT/share/openocd/scripts/ -f $OPENOCD_ROOT/debug.cfg &

# Need to wait for openocd to get started before we try and connect
echo "Waiting for OpenOCD to start..."
sleep 1

# Start up GDB, debugging the specified program, pointing at openocd
$GDB -ex "target remote localhost:3333" $1

# We're done, so kill openocd
pkill openocd
