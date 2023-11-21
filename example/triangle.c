#define PASTEL_IMPLEMENTATION
#include "pastel.h"

#define WIDTH 800
#define HEIGHT 600
#define PI 3.1416
static uint32_t pixels[WIDTH * HEIGHT];

static float angle = 0.0;
static float freq = 0.1;

float sqrtf(float x);
float atan2f(float y, float x);
float sinf(float x);
float cosf(float x);

void rotate_point(int* x_, int* y_) {
  int x = *x_ - WIDTH/2;
  int y = *y_ - HEIGHT/2;
  float norm  = sqrtf(x*x + y*y);
  float theta = atan2f(y, x) + angle;
  *x_ = (cosf(theta)*norm + (float)WIDTH/2); 
  *y_ = (sinf(theta)*norm + (float)HEIGHT/2);
}

uint32_t* render(float dt) {
  angle += dt * (2*PI)*freq;
  if (angle > 2*PI) angle -= 2*PI;

  PastelCanvas canvas = pastel_create_canvas(pixels, WIDTH, HEIGHT);
  {
    pastel_fill(canvas, PASTEL_BLACK);
    int x0 = 0; int y0 = HEIGHT/2;
    int x1 = WIDTH*2/3; int y1 = HEIGHT*5/6;
    int x2 = WIDTH*3/4; int y2 = 0;
    rotate_point(&x0, &y0);
    rotate_point(&x1, &y1);
    rotate_point(&x2, &y2);
    pastel_fill_triangle(canvas, x0, y0, x1, y1, x2, y2, PASTEL_RED);

  }
  return canvas.pixels;
}
