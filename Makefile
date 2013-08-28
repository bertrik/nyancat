CFLAGS = -O3 -W -Wall

all: nyancat.gif pacman.gif invaders.gif

nyancat.gif: nyancat
	./nyancat
	convert -loop 0 -delay 5 +map nyancat*.ppm nyancat.gif

pacman.gif: pacman
	./pacman
	convert -loop 0 -delay 5 +map pacman*.ppm pacman.gif

invaders.gif: invaders
	./invaders
	convert -loop 0 -delay 15 +map invaders*.ppm invaders.gif

clean:
	rm -f nyancat
	rm -f nyancat*.ppm nyancat.gif
	rm -f pacman
	rm -f pacman*.ppm pacman.gif
	rm -f invaders
	rm -f invaders*.ppm invaders.gif
