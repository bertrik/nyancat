#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 8

// saves one image in the animation
static void save_image(int index, double field[][WIDTH], uint32_t palet[], int max_col, double scale)
{
    FILE *file;
    char filename[128];
    int row, col;
    uint32_t rgb;
    uint8_t b;
    
    sprintf(filename, "metaballs%03d.ppm", index);
    file = fopen(filename, "wb");
    if (file != NULL) {
        fprintf(file, "P6\n%d %d\n%d\n", WIDTH, HEIGHT, 255);
        for (row = 0; row < HEIGHT; row++) {
            for (col = 0; col < WIDTH; col++) {
                double v = field[row][col];
                
                int j = max_col * v / scale;
                if (j > max_col) {
                    j = max_col;
                }
                
                rgb = palet[j];
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

typedef struct {
    double cx, cy;
    double vx, vy;
    double d;
} move_t;

double dropoff(double x, double y, double d)
{
    double d2 = (x * x + y * y) / (d * d);
    if (d2 < 1.0) {
        double v = (1 - d2);
        return v * v;
    } else {
        return 0.0;
    }
}

static void move_ball(move_t *move)
{
    move->cx += move->vx;
    if ((move->cx > WIDTH) || (move->cx < -WIDTH)) {
        move->vx = -move->vx;
    }
    
    move->cy += move->vy;
    if ((move->cy > 2*HEIGHT) || (move->cy < -2*HEIGHT)) {
        move->vy = -move->vy;
    }
}

static void draw_ball(double field[][WIDTH], move_t *move)
{
    int row, col;

    double x = move->cx + (WIDTH / 2);
    double y = move->cy + (HEIGHT / 2);
    for (row = 0; row < HEIGHT; row++) {
        for (col = 0; col < WIDTH; col++) {
            field[row][col] += dropoff(col - x, row - y, move->d);
        }
    }
}

#define RAND_RANGE 1000000
double myrand(double range)
{
    double d = rand() % RAND_RANGE;
    return range * d / RAND_RANGE;
}

static int create_palette(uint32_t palet[])
{
    int i;
    int k = 0;
    int r = 0, g = 0, b = 0;

    // black to red
    for (i = 0; i < 15; i++) {
        palet[k++] = (r << 20) + (g << 12) + (b << 4);
        r++;
    }
    // red to yellow
    for (i = 0; i < 15; i++) {
        palet[k++] = (r << 20) + (g << 12) + (b << 4);
        g++;
    }
    // yellow to green
    for (i = 0; i < 15; i++) {
        palet[k++] = (r << 20) + (g << 12) + (b << 4);
        r--;
    }
    // green to cyan
    for (i = 0; i < 15; i++) {
        palet[k++] = (r << 20) + (g << 12) + (b << 4);
        b++;
    }
    // cyan to blue
    for (i = 0; i < 15; i++) {
        palet[k++] = (r << 20) + (g << 12) + (b << 4);
        g--;
    }
    // blue to magenta
    for (i = 0; i < 15; i++) {
        palet[k++] = (r << 20) + (g << 12) + (b << 4);
        r++;
    }
    // magenta to white
    for (i = 0; i < 15; i++) {
        palet[k++] = (r << 20) + (g << 12) + (b << 4);
        g++;
    }
    
    return k;
}

#define NUM_BALLS 100

int main(int argc, char *argv[])
{
    int i;
    double field[HEIGHT][WIDTH];
    uint32_t palet[256];

    (void)argc;
    (void)argv;
    
    // create palette
    int numcol = create_palette(palet);

    // define movement
    move_t *moves[NUM_BALLS];
    for (i = 0; i < NUM_BALLS; i++) {
        move_t *move = malloc(sizeof(move_t));
        move->cx = myrand(2*WIDTH) - WIDTH;
        move->cy = myrand(4*HEIGHT) - 2*HEIGHT;
        move->vx = (2.0 - myrand(4.0)) / 10.0;
        move->vy = (2.0 - myrand(4.0)) / 10.0;
        move->d  = 2.0 + myrand(12.0);
        moves[i] = move;
    }

    // draw metaballs
    int j;
    for (i = 0; i < 1000; i++) {
        memset(field, 0, sizeof(field));
        
        for (j = 0; j < NUM_BALLS; j++) {
            move_ball(moves[j]);
            draw_ball(field, moves[j]);
        }
        
        save_image(i, field, palet, numcol - 1, 3.0);
    }
    
    // free
    for (i = 0; i < NUM_BALLS; i++) {
        free(moves[i]);
    }

    return 0;
}
