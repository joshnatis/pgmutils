# batch.sh

Automates the creation of PNG files from an input PGM file (one for each palette in `palettes/`).

## Usage
`Usage: ./batch.sh <file.pgm> [PIXLVL]`

PIXLVL is 4 by default. Btw, make sure you put this file in the same directory as `colorize.py` and the `palettes/` directory
 when actually using it, it's in its own directory right now solely for organizational purposes.

## Dependencies
The script uses some utilities that are available by default on MacOS, but not on other systems.
Specifically, it uses `cupsfilter` to convert PostScript to PDF, and `sips` to convert PDF to PNG.
There's nothing too special going on with those two, you could easily replace them with Ghostscript
and/or ImageMagick.
