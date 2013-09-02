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
    {'p', 0xff0080},   // purple

    // end marker
    {'\0', 0}
};

typedef struct {
    int width;
    int height;
    char *data;
} blob_t;

// invader 1
static const blob_t invader1a = {
    10, 8,
    "    gg    "
    "   gggg   "
    "  gggggg  "
    " gg gg gg "
    " gggggggg "
    "  g gg g  "
    " g      g "
    "  g    g  "
};

static const blob_t invader1b = {
    10, 8,
    "    gg    "
    "   gggg   "
    "  gggggg  "
    " gg gg gg "
    " gggggggg "
    "   g  g   "
    "  g gg g  "
    " g g  g g "
};

// invader 2
static const blob_t invader2a = {
    11, 8,
    "  b     b  "
    "   b   b   "
    "  bbbbbbb  "
    " bb bbb bb "
    "bbbbbbbbbbb"
    "b bbbbbbb b"
    "b b     b b"
    "   bb bb   "
};
static const blob_t invader2b = {
    11, 8,
    "  b     b  "
    "b  b   b  b"
    "b bbbbbbb b"
    " bb bbb bb "
    "bbbbbbbbbbb"
    " bbbbbbbbb "
    "  b     b  "
    " b       b "
};

// invader 3
static const blob_t invader3a = {
    12, 8,
    "    pppp    "
    " pppppppppp "
    "pppppppppppp"
    "ppp  pp  ppp"
    "pppppppppppp"
    "  ppp  ppp  "
    " pp  pp  pp "
    "  pp    pp  "
};
static const blob_t invader3b = {
    12, 8,
    "    pppp    "
    " pppppppppp "
    "pppppppppppp"
    "ppp  pp  ppp"
    "pppppppppppp"
    "   pp  pp   "
    "  pp pp pp  "
    "pp        pp"
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

// draws invaders
static void draw_invaders(int frame, int xoffset, int yoffset, const blob_t *invaders[], char *image)
{
    int x, y, phase, i;
    
    phase = frame % 2;
    for (i = 0; i < 5; i++) {
        x = xoffset + i * 15;
        y = ((frame / 2) + yoffset) % 30;
        draw_blob(image, invaders[phase], x, y - 30);
        draw_blob(image, invaders[phase], x, y);
    }
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

    sprintf(filename, "invaders%03d.ppm", index);
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
    int x;

    (void)argc;
    (void)argv;
    
    const blob_t *invaders1[] = {&invader1a, &invader1b};
    const blob_t *invaders2[] = {&invader2a, &invader2b};
    const blob_t *invaders3[] = {&invader3a, &invader3b};

    x = -10;
    for (frame = 0; frame < 60; frame++) {
        draw_back(frame, (char *)image);

        if ((frame / 10) % 2) {
            x -= 3;
        } else {
            x += 3;
        }

        draw_invaders(frame, x, 0, invaders1, (char *)image);
        draw_invaders(frame, x, 10, invaders2, (char *)image);
        draw_invaders(frame, x, 20, invaders3, (char *)image);

        save_image(frame, (char *)image);
    }

    return 0;
}
