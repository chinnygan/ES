#!/bin/sh
if [ $# -eq "0" ]; then
	echo "Usage: ./wav2raw [wav file]"
	exit;
fi;

OUTFILE=`basename $1 .wav`.raw
sox $1 -b 8 --encoding signed-integer -r 8000 $OUTFILE


