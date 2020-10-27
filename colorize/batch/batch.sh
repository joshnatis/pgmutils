#!/bin/bash

set -e

if [ $# -lt 1 ] || [ $# -gt 2 ]; then
	echo "Usage: $0 <file.pgm> [PIXLVL]"
	exit
elif [ ! -f "$1" ]; then
	echo "File not found: $1"
	exit
elif [ $# -eq 2 ]; then
	PIXLVL=$2
else
	PIXLVL="4"
fi

FILE="$1"
NAME="$(echo $FILE | rev | cut -d. -f2 | rev)"
PROGRAM="colorize.py"
SCALE="1.8"

for PALETTE in ./palettes/*; do
	OUTPUT="$NAME"_"$(echo $PALETTE | rev | cut -d/ -f1 | rev)"

	DIM="$(cat $FILE | head -2 | tail -1)"
	WIDTH="$(echo $DIM | cut -d ' ' -f1)"
	WIDTH="$(echo $WIDTH \* $SCALE | bc -l)"
	HEIGHT="$(echo $DIM | cut -d ' ' -f2)"
	HEIGHT="$(echo $HEIGHT \* $SCALE | bc -l)"

	python3 "$PROGRAM" "$FILE" -p "$PIXLVL" --file "$PALETTE" -o "$OUTPUT.ps" --batch
	cupsfilter "$OUTPUT.ps" > "$OUTPUT.pdf"
	sips -s format png "$OUTPUT.pdf" -c $HEIGHT $WIDTH --out "$OUTPUT.png"

	rm "$OUTPUT.ps"
	rm "$OUTPUT.pdf"
done

# mv -i *.png batch_outputs/
