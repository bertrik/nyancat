#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define WIDTH 80
#define HEIGHT 8

typedef struct {
    char c;
    uint32_t rgb;
} color_t;

static const color_t colors[] = {
    // basic colors
    {' ', 0x000000},   // transparent
    {'B', 0x000000},   // black
    {'W', 0xffffff},   // white
    {'G', 0x808080},   // grey
    {'.', 0x004080},   // dark blue background

    // rainbow colors
    {'r', 0xff0000},   // red
    {'o', 0xff8000},   // orange
    {'y', 0xffff00},   // yellow
    {'g', 0x00ff00},   // green
    {'b', 0x0080ff},   // light blue
    {'p', 0x8000ff},   // deep purple

    {'s', 0xFF8080},   // oud-roze (pinkish dots on face)
    {'P', 0xFF80FF},   // light pink (body)
    {'f', 0xFF0080},   // fuchsia (dots on body)

    // end marker
    {'\0', 0}
};

typedef struct {
    int width;
    int height;
    char *data;
} blob_t;

static const blob_t head = {
#if 0   // wide head
    11, 7,
    " BB     BB "
    " BGBBBBBGB "
    "BGWBGGGWBGB"
    "BGBBGBGBBGB"
    "BsGGGGGGGsB"
    " BGGGGGGGB "
    "  BBBBBBB  "
#else   // narrow head
    9, 7,
    " BB   BB "
    " BGBBBGB "
    "BGWBGWBGB"
    "BGBBGBBGB"
    "BsGGGGGsB"
    " BGGGGGB "
    "  BBBBB  "
#endif
};

static const blob_t body = {
    10, 7,
    " BBBBBBBB "
    "BPPPPfPPPB"
    "BPfPPPPPPB"
    "BPPPfPPfPB"
    "BPPPPPPPPB"
    "BPfPPfPPPB"
    " BBBBBBBB "
};

static const blob_t rainbow = {
    8, 7,
    "rrrr    "
    "oooorrrr"
    "yyyyoooo"
    "ggggyyyy"
    "bbbbgggg"
    "ppppbbbb"
    "    pppp"
};

// copies a blob into the image at coordinates (x,y)
static void copy_blob(char *image, const blob_t *blob, int x, int y)
{
    char c;
    int row, col;
    int bx, by;

    for (row = 0; row < HEIGHT; row++) {
        for (col = 0; col < WIDTH; col++) {
            bx = col - x;
            by = row - y;
            if ((bx >= 0) && (bx < blob->width) &&
                (by >= 0) && (by < blob->height)) {
                c = blob->data[by * blob->width + bx];
                if (c != ' ') {
                    image[row * WIDTH + col] = c;
                }
            }
        }
    }
}

// draws background
static void draw_back(int frame, char *image)
{
    (void)frame;
    memset(image, '.', WIDTH*HEIGHT);
}

// draws the rainbow
static void draw_rainbow(int frame, char *image)
{
    int i;
    int phase;

    phase = frame % 8;
    for (i = -phase; i < 60; i += rainbow.width) {
        copy_blob(image, &rainbow, i, 1);
    }
}

// returns 0 or 1 to wobble a blob up/down/left/right
static int wobble(int frame, int offset, int len)
{
    int x, y;

    x = (frame + offset) % len;
    y = 2 * x / len;

    return y;
}

// draws the body
static void draw_body(int frame, char *image)
{
    int off_y;

    off_y = wobble(frame, 2, 8);

    copy_blob(image, &body, 60, off_y);
}

// draws the head
static void draw_head(int frame, char *image)
{
    int off_x, off_y;

    off_x = wobble(frame, 3, 8);
    off_y = wobble(frame, 1, 8);

    copy_blob(image, &head, 66 + off_x, off_y);
}

// converts a color character into an RGB value
static uint32_t get_rgb(char c)
{
    const color_t *color;

    for (color = colors; color->c != '\0'; color++) {
        if (c == color->c) {
            return color->rgb;
        }
    }

    // not found!
    fprintf(stderr, "color '%c' not found!\n", c);
    return 0;
}

// saves one image in the animation
static void save_image(int index, char *image)
{
    FILE *file;
    char filename[128];
    int row, col;
    char c;
    uint32_t rgb;
    uint8_t b;

    sprintf(filename, "image%03d.ppm", index);
    file = fopen(filename, "wb");
    if (file != NULL) {
        fprintf(file, "P6\n%d %d\n%d\n", WIDTH, HEIGHT, 255);
        for (row = 0; row < HEIGHT; row++) {
            for (col = 0; col < WIDTH; col++) {
                c = image[row * WIDTH + col];
                rgb = get_rgb(c);
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


int main(int argc, char *argv[])
{
    int i;
    int frame;
    char image[HEIGHT][WIDTH];

    (void)argc;
    (void)argv;

    for (i = 0; i < 96; i++) {
        frame = i % 8;

        draw_back(frame, (char *)image);
        draw_rainbow(frame, (char *)image);
        draw_body(frame, (char *)image);
        draw_head(frame, (char *)image);

        save_image(i, (char *)image);
    }

    return 0;
}