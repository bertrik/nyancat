CFLAGS = -O3 -W -Wall
LDLIBS = -lm

all: nyancat.gif pacman.gif invaders.gif pvz.gif metaballs.gif fire.gif scroll.gif

nyancat.gif: nyancat
	./nyancat && convert -loop 0 -delay 5 +map nyancat*.ppm nyancat.gif

pacman.gif: pacman
	./pacman && convert -loop 0 -delay 5 +map pacman*.ppm pacman.gif

invaders.gif: invaders
	./invaders && convert -loop 0 -delay 15 +map invaders*.ppm invaders.gif

pvz.gif: pvz
	./pvz && convert -loop 0 -delay 15 +map pvz*.ppm pvz.gif

metaballs.gif: metaballs
	./metaballs && convert -loop 0 -delay 1 +map metaballs*.ppm metaballs.gif

fire.gif: fire
	./fire && convert -loop 0 -delay 5 +map fire*.ppm fire.gif

scroll.gif: scroll
	./scroll && convert -loop 0 -delay 1 +map scroll*.ppm scroll.gif

clean:
	rm -f nyancat nyancat*.ppm
	rm -f pacman pacman*.ppm
	rm -f invaders invaders*.ppm
	rm -f pvz pvz*.ppm
	rm -f metaballs metaballs*.ppm
	rm -f fire fire*.ppm
	rm -f scroll scroll*.ppm

