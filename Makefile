CFLAGS = -O3 -W -Wall

all: nyancat pacman

ppm_nyancat: nyancat
	./nyancat

ppm_pacman: pacman
	./pacman

nyancat.gif: ppm_nyancat
	convert -loop 0 -delay 5 nyancat*.ppm nyancat.gif

pacman.gif: ppm_pacman
	convert -loop 0 -delay 5 pacman*.ppm pacman.gif

clean:
	rm -f nyancat
	rm -f nyancat*.ppm nyancat.gif
	rm -f pacman
	rm -f pacman*.ppm pacman.gif
