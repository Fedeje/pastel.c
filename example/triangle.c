#define PASTEL_SHADER_UTILS_IMPLEMENTATION
#include "pastel_shader_utils.h"
#define PASTEL_IMPLEMENTATION
#include "pastel.h"

#define WIDTH 800
#define HEIGHT 600
#define PI 3.1416
static Color pixels[WIDTH * HEIGHT];
static PastelShaderContextGradient1D context_grad;
static PastelShader shader_grady = { pastel_shader_func_gradient1dy, &context_grad };
static PastelShader shader_gradx = { pastel_shader_func_gradient1dx, &context_grad };

static float angle = 0.0;
static float freq = 0.1;

float sqrtf(float x);
float atan2f(float y, float x);
float sinf(float x);
float cosf(float x);

void rotate_point(Vec2i* p) {
  int x = p->x - WIDTH/2;
  int y = p->y - HEIGHT/2;
  float norm  = sqrtf(x*x + y*y);
  float theta = atan2f(y, x) + angle;
  p->x = (cosf(theta)*norm + (float)WIDTH/2); 
  p->y = (sinf(theta)*norm + (float)HEIGHT/2);
}

Color* render(float dt) {
  angle += dt * (2*PI)*freq;
  if (angle > 2*PI) angle -= 2*PI;

  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  {
    context_grad.c1 = PASTEL_BLUE;
    context_grad.c2 = PASTEL_YELLOW;
    context_grad.min = 0;
    context_grad.max = HEIGHT;
    pastel_fill(&canvas, shader_grady);

    // Rotate points
    Vec2i p1, p2, p3;
    p1.x = 0; p1.y = HEIGHT/2;
    p2.x = WIDTH*2/3; p2.y = HEIGHT*5/6;
    p3.x = WIDTH*3/4; p3.y = 0;
    rotate_point(&p1);
    rotate_point(&p2);
    rotate_point(&p3);

    context_grad.c1 = PASTEL_RED;
    context_grad.c2 = PASTEL_GREEN;
    context_grad.min = 0;
    context_grad.max = WIDTH;
    pastel_fill_triangle(&canvas, &p1, &p2, &p3, shader_gradx);

  }
  return canvas.pixels;
}

#ifdef PLATFORM_SDL
#include <stdio.h>
#include <SDL2/SDL.h>
#define DELAY 3000

#define return_defer(value) do { result = (value); goto defer; } while (0)

int main(void)
{
  int result = 0;

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Texture *texture = NULL;

  {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return_defer(1);

    window = SDL_CreateWindow("Pastel", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) return_defer(1);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) return_defer(1);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    if (texture == NULL) return_defer(1);

    Uint32 prev = SDL_GetTicks();
    for (;;) {
      // Compute Delta Time
      Uint32 curr = SDL_GetTicks();
      float dt = (curr - prev)/1000.f;
      prev = curr;

      // Flush the events
      SDL_Event event;
      while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT) return_defer(0);

      // Render the texture
      SDL_Rect window_rect = {0, 0, WIDTH, HEIGHT};
      uint32_t *pixels_src = render(dt);
      void *pixels_dst;
      int pitch;
      if (SDL_LockTexture(texture, &window_rect, &pixels_dst, &pitch) < 0) return_defer(1);
      for (size_t y = 0; y < HEIGHT; ++y) {
        memcpy(pixels_dst + y*pitch, pixels_src + y*WIDTH, WIDTH*sizeof(uint32_t));
      }
      SDL_UnlockTexture(texture);

      // Display the texture
      if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0) < 0) return_defer(1);
      if (SDL_RenderClear(renderer) < 0) return_defer(1);
      if (SDL_RenderCopy(renderer, texture, &window_rect, &window_rect) < 0) return_defer(1);
      SDL_RenderPresent(renderer);
    }
  }

defer:
  switch (result) {
    case 0:
      printf("OK\n");
      break;
    default:
      fprintf(stderr, "\nSDL ERROR: %s\n", SDL_GetError());
  }
  if (texture) SDL_DestroyTexture(texture);
  if (renderer) SDL_DestroyRenderer(renderer);
  if (window) SDL_DestroyWindow(window);
  SDL_Quit();
  return result;
}

#endif // PLATFORM_SDL
