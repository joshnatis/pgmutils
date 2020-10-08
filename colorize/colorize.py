import turtle
import random
import sys
import argparse
import matplotlib.colors

PIXEL_WIDTH = 4
SEED_COLORS = [(191, 152, 78), (242, 215, 133), (166, 102, 46), (217, 206, 173)]

def palettefromfile(fn):
	try:
		with open(fn, 'r') as f:
			colors = [c.strip("\n") for c in f.readlines()]
			colors = [tuple([c * 255 for c in matplotlib.colors.to_rgb(color)]) \
				if color[0] != '(' \
				else tuple([c for c in eval(color)]) for color in colors]
			return colors
	except:
		print("An error occured while reading", fn)
		exit(1)

def normalize(palette):
	try:
		colors = [tuple([c * 255 for c in matplotlib.colors.to_rgb(color)]) \
			if color[0] != '(' \
			else tuple([c for c in eval(color)]) for color in palette]
		return colors
	except ValueError:
		print("Invalid color argument found")
		exit(1)

def brightness(color):
	s = 0
	for val in color:
		s += val
	return s/255

def tocolor(gray):
	quartile = int(gray * len(SEED_COLORS) - 0.0001)
	color = SEED_COLORS[quartile]
	r = (color[0]*gray)/255
	g = (color[1]*gray)/255
	b = (color[2]*gray)/255
	return (r, g, b)

def setup(turt, screen, dimx, dimy, watch):
	screen.setup(dimx, dimy)
	screen.setworldcoordinates(0, dimy+10, dimx+10, 0)

	if watch == False:
		screen.delay(0)
		screen.tracer(0, 0)
	turt.speed(0)
	turt.hideturtle()
	turt.penup()
	loading(turt, screen, dimx, dimy)
	turt.goto(0, 0)
	turt.pendown()

def loading(turt, screen, dimx, dimy):
	screen.bgcolor("black");
	turt.color("#39FF14")
	turt.goto(dimx/16, dimy/4)
	turt.write("YOUR PATIENCE", font=("Arial", int((dimx + dimy)/25), "bold"))
	turt.goto(dimx/16, dimy/3)
	turt.write("IS APPRECIATED...", font=("Arial", int((dimx + dimy)/40), "bold", "italic"))

def colorize(fn, out, watch, invert):
	doge = turtle.Turtle()
	screen = turtle.Screen()

	try:
		with open(fn, 'r') as img:
			magic_number = img.readline()
			if magic_number[0] != 'P':
				print('Not a valid pgm file')
				exit(1)
			dimx, dimy = img.readline()[:-1].split(' ')
			dimx = int(dimx)
			dimy = int(dimy)
			max_gray = int(img.readline()[:-1])

			setup(doge, screen, dimx, dimy, watch)

			for i in range(dimy):
				pixels = img.readline().strip(' \n').split(' ')[::PIXEL_WIDTH]
				for pixel in pixels:
					gray = int(pixel.strip())/max_gray
					r, g, b = tocolor(gray) if invert == False else tocolor(1 - gray)
					doge.color(r, g, b)
					#print("Writing...", PIXEL_WIDTH, "steps in color ", r, g, b, "from ", gray)
					doge.forward(PIXEL_WIDTH)

				doge.penup()
				doge.goto(0, i+1)
				doge.pendown()

		if watch == False:
			screen.update()

		if out != None:
			turtle.getscreen().getcanvas().postscript(file=out)

		print("Complete.")
		screen.exitonclick()

	except KeyboardInterrupt:
		exit(1)
	except FileNotFoundError:
		print("Not a valid pgm file")
		exit(1)
	except:
		exit(1)

def main():
	parser = argparse.ArgumentParser()
	parser.add_argument("image", type=str, help="Input PGM image")
	parser.add_argument("-o", "--out", type=str, metavar="", help="Output postscript file")
	group = parser.add_mutually_exclusive_group()
	group.add_argument("-c", "--colors", nargs='+', type=str, metavar="", \
		help="A color palette for the resultant image, e.g. 'blue #fab123 magenta (255, 4, 12)'")
	group.add_argument("-f", "--file", type=str, metavar="", help="File containing new-line separated colors for palette")
	parser.add_argument("-w", "--watch", action="store_true", help="Watch the turtle do its magic (slow)")
	parser.add_argument("-i", "--invert", action="store_true", help="Invert the image")
	parser.add_argument("-p", "--pixelate", type=int, help="Level of pixelation for the image (1-∞)")
	args = parser.parse_args()

	global SEED_COLORS
	global PIXEL_WIDTH

	if args.pixelate != None:
		PIXEL_WIDTH = args.pixelate

	if args.file != None:
		SEED_COLORS = palettefromfile(args.file)
	elif args.colors != None:
		SEED_COLORS = normalize(list(args.colors))

	SEED_COLORS = sorted(SEED_COLORS, key=brightness)
	colorize(args.image, args.out, args.watch, args.invert)

main()
