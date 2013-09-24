#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 8

#define FIRE_HEIGHT (HEIGHT+2)

// saves one image in the animation
static void save_image(int index, uint8_t field[][WIDTH], uint32_t palet[])
{
    FILE *file;
    char filename[128];
    int row, col;
    uint32_t rgb;
    uint8_t b;

    sprintf(filename, "fire%03d.ppm", index);
    file = fopen(filename, "wb");
    if (file != NULL) {
        fprintf(file, "P6\n%d %d\n%d\n", WIDTH, HEIGHT, 255);
        for (row = 0; row < HEIGHT; row++) {
            for (col = 0; col < WIDTH; col++) {
                rgb = palet[field[row][col]];
                b = (rgb >> 16) & 0xFF;
                fwrite(&b, 1, 1, file);
                b = (rgb >> 8) & 0xFF;
                fwrite(&b, 1, 1, file);
                b = (rgb >> 0) & 0xFF;
                fwrite(&b, 1, 1, file);
            }
        }
        fclose(file);
    }
}

static int create_palette(uint32_t palet[])
{
    int i;
    int k = 0;
    int r = 0, g = 0, b = 0;

    // black to blue
    for (i = 0; i < 15; i++) {
        palet[k++] = (r << 20) + (g << 12) + (b << 4);
        b++;
    }
    // blue to red
    for (i = 0; i < 15; i++) {
        palet[k++] = (r << 20) + (g << 12) + (b << 4);
        b--;
        r++;
    }
    // red to yellow
    for (i = 0; i < 15; i++) {
        palet[k++] = (r << 20) + (g << 12) + (b << 4);
        g++;
    }
    // yellow to white
    for (i = 0; i < 15; i++) {
        palet[k++] = (r << 20) + (g << 12) + (b << 4);
        b++;
    }
    // just white
    for (i = 0; i < 30; i++) {
        palet[k++] = (r << 20) + (g << 12) + (b << 4);
    }
    return k;
}

static void move_fire(uint8_t field[][WIDTH], int height)
{
    int x,y, flame;

    // move flames up
    for (y = 0; y < height - 1; y++) {
        for (x = 0; x < WIDTH; x++) {
            // average
            if (x == 0) {
                flame = (field[y][x] + 2*field[y+1][x] + field[y+1][x+1]) / 4;
            } else if (x == (WIDTH - 1)) {
                flame = (field[y][x] + 2*field[y+1][x] + field[y+1][x-1]) / 4;
            } else {
                flame = (field[y][x] + field[y+1][x-1] + field[y+1][x] + field[y+1][x+1]) / 4;
            }
            // decay
            if (flame > 4) {
                flame -= 4;
            } else {
                flame /=2 ;
            }
            field[y][x] = flame;
        }
    }
}

// create new fire at the bottom
static void new_fire(uint8_t field[][WIDTH], int height, int numcol)
{
    int x;
    
    for (x = 0; x < WIDTH; x++) {
        field[height-1][x] = numcol * ((rand() % 10) > 3);
    }
}

int main(int argc, char *argv[])
{
    int i;
    uint8_t field[FIRE_HEIGHT][WIDTH];
    uint32_t palet[256];

    (void)argc;
    (void)argv;

    // create palette
    int numcol = create_palette(palet);

    // draw fire
    memset(field, 0, sizeof(field));
    for (i = 0; i < 200; i++) {
        new_fire(field, FIRE_HEIGHT, (i < 190) ? numcol : 0);
        move_fire(field, FIRE_HEIGHT);
        save_image(i, field, palet);
    }

    return 0;
}
