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

    // misc colors
    {'r', 0xff0000},   // red
    {'o', 0xff8000},   // orange
    {'y', 0xffff00},   // yellow
    {'g', 0x00ff00},   // green
    {'b', 0x0080ff},   // blue

    // end marker
    {'\0', 0}
};

typedef struct {
    int width;
    int height;
    char *data;
} blob_t;

// left-to-right ghost
static const blob_t ghost1 = {
    8, 8,
    "  gggg  "
    " gggggg "
    "gWWggWWg"
    "gWBggWBg"
    "gggggggg"
    "gggggggg"
    "gggggggg"
    " gg gg g"
};
static const blob_t ghost2 = {
    8, 8,
    "  gggg  "
    " gggggg "
    "gWWggWWg"
    "gWBggWBg"
    "gggggggg"
    "gggggggg"
    "gggggggg"
    "gg gg gg"
};
static const blob_t ghost3 = {
    8, 8,
    "  gggg  "
    " gggggg "
    "gWWggWWg"
    "gWWggWWg"
    "gggggggg"
    "gggggggg"
    "gggggggg"
    "g gg gg "
};

// right-to-left ghost
static const blob_t ghost4 = {
    8, 8,
    "  bbbb  "
    " bbbbbb "
    "bbybbybb"
    "bbbbbbbb"
    "bbybybyb"
    "bybybybb"
    "bbbbbbbb"
    "bb bb bb"
};
static const blob_t ghost5 = {
    8, 8,
    "  bbbb  "
    " bbbbbb "
    "bbybbybb"
    "bbbbbbbb"
    "bbybybyb"
    "bybybybb"
    "bbbbbbbb"
    " bb bb b"
};
static const blob_t ghost6 = {
    8, 8,
    "  bbbb  "
    " bbbbbb "
    "bbbbbbbb"
    "bbbbbbbb"
    "bbybybyb"
    "bybybybb"
    "bbbbbbbb"
    "b bb bb "
};

// left-to-right pacman
static const blob_t pac1 = {
    8, 8,
    "  yyyy  "
    " yyyyyy "
    "yyyyy   "
    "yyy     "
    "yyy     "
    "yyyyy   "
    " yyyyyy "
    "  yyyy  "
};
static const blob_t pac2 = {
    8, 8,
    "  yyyy  "
    " yyyy   "
    "yyyy    "
    "yyy     "
    "yyy     "
    "yyyy    "
    " yyyy   "
    "  yyyy  "
};

// right-to-left pacman
static const blob_t pac3 = {
    8, 8,
    "  yyyy  "
    " yyyyyy "
    "   yyyyy"
    "     yyy"
    "     yyy"
    "   yyyyy"
    " yyyyyy "
    "  yyyy  "
};

static const blob_t pac4 = {
    8, 8,
    "  yyyy  "
    "   yyyy "
    "    yyyy"
    "     yyy"
    "     yyy"
    "    yyyy"
    "   yyyy "
    "  yyyy  "
};

// dots and power-pill
static const blob_t dot = {
    4, 4,
    "    "
    " WW "
    " WW "
    "    "
};
static const blob_t bigdot = {
    4, 4,
    " WW "
    "WWWW"
    "WWWW"
    " WW "
};

// draws a blob into the image at (x,y), taking into account transparency
static void draw_blob(char *image, const blob_t *blob, int x, int y)
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
    memset(image, 'B', WIDTH*HEIGHT);
}

// draws the dots and the 'pill'
static void draw_dots(int frame, int position, char *image)
{
    int x;
    for (x = 8 * ((position + 7) / 8); x < 72; x += 8) {
        draw_blob(image, &dot, x, 2);
    }
    if ((frame / 2) % 2) {
        draw_blob(image, &bigdot, x, 2);
    }
}

// draws pacman in the left-to-right chase
static void draw_pac_lr(int frame, int position, char *image)
{
    static const blob_t *pacs[] = {&pac1, &pac2};

    int phase = (frame / 4) % 2;
    draw_blob(image, pacs[phase], position, 0);
}

// draws pacman in the right-to-left chase
static void draw_pac_rl(int frame, int position, char *image)
{
   static const blob_t *pacs[] = {&pac3, &pac4};

    int phase = (frame / 4) % 2;
    draw_blob(image, pacs[phase], position, 0);
}

// draws the ghost in the left-to-right chase
static void draw_ghost_lr(int frame, int position, char *image)
{
    static const blob_t *ghosts[] = {&ghost1, &ghost2, &ghost3};

    int phase = (frame % 3);
    draw_blob(image, ghosts[phase], position, 0);
}

// draws the ghost in the left-to-right chase
static void draw_ghost_rl(int frame, int position, char *image)
{
    static const blob_t *ghosts[] = {&ghost4, &ghost5, &ghost6};

    int phase = (frame % 3);
    draw_blob(image, ghosts[phase], position, 0);
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

    sprintf(filename, "pacman%03d.ppm", index);
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
    int frame;
    char image[HEIGHT][WIDTH];
    int pos_pac, pos_ghost;
    int distance;

    (void)argc;
    (void)argv;

    for (frame = 0; frame < 144; frame++) {
        draw_back(frame, (char *)image);
        
        distance = (144 - frame) / 4;
        if (frame < 72) {
            // left to right chase
            pos_pac = frame;
            pos_ghost = pos_pac - distance - 8;
            
            draw_dots(frame, pos_pac, (char *)image);
            draw_pac_lr(frame, pos_pac, (char *)image);
            draw_ghost_lr(frame, pos_ghost, (char *)image);
        } else {
            // right to left chase
            pos_pac = 144 - frame;
            pos_ghost = pos_pac - distance - 8;
            
            draw_pac_rl(frame, pos_pac, (char *)image);
            draw_ghost_rl(frame, pos_ghost, (char *)image);
        }

        save_image(frame, (char *)image);
    }

    return 0;
}
