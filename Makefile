CFLAGS = -O3 -W -Wall

all: nyancat create gif

create:
	./nyancat.exe

gif:
	convert -loop 0 -delay 5 image*.ppm nyancat.gif

clean:
	rm -f nyancat.exe
	rm -f image*.ppm nyancat.gif
