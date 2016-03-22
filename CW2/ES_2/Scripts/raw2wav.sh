#!/bin/sh
if [ $# -eq "0" ]; then
	echo "Usage: ./raw2wav [raw file]"
	exit;
fi;

OUTFILE=`basename $1 .raw`.wav
sox -b 8 --encoding signed-integer -r 8000 $1 $OUTFILE


