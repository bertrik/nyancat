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
    {'z', 0x808080},   // dark grey (Z for zombie)

    // shades of gray
    {'1', 0x041004},
    {'2', 0x082008},
    {'3', 0x202020},
    {'4', 0x202020},
    {'5', 0x202020},

    // green tints
    {'d', 0x008000},    // dark green
    {'g', 0x00C000},    // normal green
    {'G', 0x40FF40},    // light green

    // misc colors
    {'r', 0xff0000},   // red
    {'o', 0xff8000},   // orange
    {'y', 0xc0c000},   // normal yellow
    {'Y', 0xffff00},   // bright yellow
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

// pea shooter
static const blob_t shooter = {
    11, 6,
    "   gggg    "
    "  gGggWB gg"
    "gdgggWBBgGd"
    "d gggBBggGd"
    "  gggggg gg"
    "   gddd    "
};

// sun flower
static const blob_t sunflower = {
    6, 6,
    " YYYY "
    "YyByBY"
    "YyyyyY"
    "YByyyB"
    "YyBBBY"
    " YYYY "
};

// stalk
static const blob_t stalk = {
    8, 2,
    "GG gd GG"
    " gggggg "
};

// pea
static const blob_t pea = {
    4, 4,
    " gg "
    "gGgd"
    "gggd"
    " dd "
};

// zombie (head)
static const blob_t zombie = {
    8, 7,
    " zzzzzz "
    "zzzzyyyz"
    "ByzzyByz"
    "yyzzyyyz"
    " Wrrzzzz"
    " rrWzzz "
    " zzzzz  "
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
    int x, y;
    char c;
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            if (((x / 12) % 2) == 0) {
                c = '1';
            } else {
                c = '2';
            }
            image[y * WIDTH + x] = c;
        }
    }
}

static int pattern[][2] = {
    { 0, 0 },
    {-1, 1 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 1, 1 },
    { 0, 0 },
    { 0, 0 },
};

// draws pea shooter
static void draw_shooter(int frame, int x, int y, char *image)
{
    int phase;
    
    phase = frame % 8;
    
    int xx = x + pattern[phase][0] + 1;
    int yy = y + pattern[phase][1];

    draw_blob(image, &stalk, x + 2, y + 6);
    draw_blob(image, &shooter, xx, yy);
}

// draws sun flower
static void draw_sunflower(int frame, int x, int y, char *image)
{
    int phase;
    
    phase = (frame + 2) % 8;
    
    int xx = x + pattern[phase][0] + 3;
    int yy = y + pattern[phase][1];

    draw_blob(image, &stalk, x + 2, y + 6);
    draw_blob(image, &sunflower, xx, yy);
}

static void draw_pea(int frame, int x, int y, char *image)
{
    (void)frame;
    draw_blob(image, &pea, x, y);
}

static int wobble[][2] = {
    { -1, 1},
    { -2, 0},
    { -1, 0},
    { 0, 0},
    { 0, 0},
    {-1, 0},
    {-1, 1},
    { 0, 1}
};

static void draw_zombie(int frame, int x, int y, char *image)
{
    int phase = (frame) % 8;

    draw_blob(image, &zombie, x + wobble[phase][0], y + wobble[phase][1]);
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

    sprintf(filename, "pvz%03d.ppm", index);
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
    int zombie_x, pea_x;

    (void)argc;
    (void)argv;
    
    for (frame = 0; frame < 100; frame++) {
        draw_back(frame, (char *)image);

        draw_sunflower(frame, 0, 0, (char *)image);
        draw_shooter(frame, 12, 0, (char *)image);

        zombie_x = 80 - (frame / 2);
        draw_zombie(frame, zombie_x, 0, (char *)image);
        
        pea_x = 20 + (frame % 32) * 4;
        if (pea_x <= zombie_x) {
            draw_pea(frame, pea_x, 2, (char *)image);
        }

        save_image(frame, (char *)image);
    }

    return 0;
}
