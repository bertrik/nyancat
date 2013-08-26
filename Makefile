CFLAGS = -O3 -W -Wall

all: nyancat pacman invaders

ppm_nyancat: nyancat
	./nyancat

ppm_pacman: pacman
	./pacman

ppm_invaders: invaders
	./invaders

nyancat.gif: ppm_nyancat
	convert -loop 0 -delay 5 nyancat*.ppm nyancat.gif

pacman.gif: ppm_pacman
	convert -loop 0 -delay 5 pacman*.ppm pacman.gif

invaders.gif: ppm_invaders
	convert -loop 0 -delay 15 invaders*.ppm invaders.gif

clean:
	rm -f nyancat
	rm -f nyancat*.ppm nyancat.gif
	rm -f pacman
	rm -f pacman*.ppm pacman.gif
	rm -f invaders
	rm -f invaders*.ppm invaders.gif
