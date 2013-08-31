CFLAGS = -O3 -W -Wall

all: nyancat.gif pacman.gif invaders.gif pvz.gif

nyancat.gif: nyancat
	./nyancat && convert -loop 0 -delay 5 +map nyancat*.ppm nyancat.gif

pacman.gif: pacman
	./pacman && convert -loop 0 -delay 5 +map pacman*.ppm pacman.gif

invaders.gif: invaders
	./invaders && convert -loop 0 -delay 15 +map invaders*.ppm invaders.gif

pvz.gif: pvz
	./pvz && convert -loop 0 -delay 15 +map pvz*.ppm pvz.gif

clean:
	rm -f nyancat nyancat*.ppm nyancat.gif
	rm -f pacman pacman*.ppm pacman.gif
	rm -f invaders invaders*.ppm invaders.gif
	rm -f pvz pvz*.ppm pvz.gif
