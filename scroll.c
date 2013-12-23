#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 80
#define HEIGHT 8

#define IMAGE_HEIGHT    4*100

// saves one image in the animation
static void save_image(int index, uint8_t frame[][WIDTH][3])
{
    FILE *file;
    char filename[128];
    int row, col;
    uint8_t r, g, b;

    sprintf(filename, "scroll%03d.ppm", index);
    file = fopen(filename, "wb");
    if (file != NULL) {
        fprintf(file, "P6\n%d %d\n%d\n", WIDTH, HEIGHT, 255);
        for (row = 0; row < HEIGHT; row++) {
            for (col = 0; col < WIDTH; col++) {
                r = frame[row][col][0];
                fwrite(&r, 1, 1, file);
                g = frame[row][col][1];
                fwrite(&g, 1, 1, file);
                b = frame[row][col][2];
                fwrite(&b, 1, 1, file);
            }
        }
        fclose(file);
    }
}


static void create_frame(int index, uint8_t frame[][WIDTH][3], uint8_t image[][WIDTH][3])
{
    int sy;
    int dy;

    for (dy = 0; dy < HEIGHT; dy++) {
        sy = (dy * 4) + index;
        if ((sy < 0) || (sy >= IMAGE_HEIGHT)) {
            memset(frame[dy], 0, WIDTH * 3);
        } else {
            memcpy(frame[dy], image[sy], WIDTH * 3);
        }
    }
}


static bool load_image(const char *name, uint8_t *image_data)
{
    FILE *f;
    int read;

    f = fopen(name, "rb");
    if (f == NULL) {
        return false;
    }

    read = fread(image_data, 3, IMAGE_HEIGHT*WIDTH, f);
    fclose(f);

    return (read == IMAGE_HEIGHT * WIDTH);
}


int main(int argc, char *argv[])
{
    int i;
    uint8_t image[IMAGE_HEIGHT][WIDTH][3];
    uint8_t frame[HEIGHT][WIDTH][3];

    (void)argc;
    (void)argv;

    // load image
    load_image("scroll.rgb", (uint8_t *)image);

    // scroll image
    int range = (HEIGHT * 4) + IMAGE_HEIGHT;
    for (i = 0; i < range; i++) {
        create_frame(i - (HEIGHT * 4), frame, image);
        save_image(i, frame);
    }

    return 0;
}
