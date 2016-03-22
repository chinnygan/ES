#!/bin/sh

if [ $# -ne 2 ]; then
	echo "Usage: ./pipe.sh [infile] [outfile]"
	exit
fi;

echo "Setting up UART..."
stty -F /dev/fsl-tty raw 115200 

echo "Preparing output pipe..."
cat /dev/fsl-tty > $2 &
CATIN=$!

echo "Beginning input pipe..."
cat $1 > /dev/fsl-tty
echo "Waiting for output to terminate..."
sleep 2
kill $CATIN
echo "All done."
