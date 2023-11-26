// 
// What does it take to render something on the screen?
// 1 - Filling the image buffer
// 2 - Sending the buffer to the screen
// Here, we'll mainly focus on step 1, but if we have time, we'll
// also take a look at step 2.
// 
// Goal: render a triangle and send it to the std output, following a
// .ppm format style.
// 

#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#define PASTEL_TEST_IMPLEMENTATION
#include "test.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "third-party/stb_image_write.h"

#define WIDTH 800
#define HEIGHT 600
// A pixel is 0xAABBGGRR in hexadecimal i.e 4 bytes i.e 32 bits
// A pixel image is row major (meaning we store row by row).
// This means we index pixels the following:
//   if `i` is the row and `j` is the column then 
//   pixels(i, j) <=> pixels[i * height + j]
static Color pixels[HEIGHT * WIDTH];
// Note:
// If `T array_of_T[50]`; is defined at file scope (outside of all functions)
// or is static, it's not going to be on the stack, it's going to be a global
// preallocated at program's start variable.
// If it's not static and is defined at function scope, it's going to be
// on the stack.

#define IMGS_DIR_PATH "./imgs"
#define ASSETS_DIR_PATH "./assets"
// Hex color: 0xAABBGGRR
#define BG_COLOR PASTEL_BLACK
#define FG_COLOR PASTEL_RED

bool save_canvas_to_png(const PastelCanvas* canvas, const char* file_path) {
  printf("Generated image %s\n", file_path);
  if (!stbi_write_png(file_path, canvas->width, canvas->height, 4, canvas->pixels, canvas->stride*sizeof(Color))) {
      fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
      return false;
  }
  return true;
}

bool example_corners(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  __fill_bg(&canvas, BG_COLOR);

  PastelShaderContextMonochrome context;
  PastelShader shader = {pastel_shader_func_monochrome, &context};

  // Draw corners
  context.color = FG_COLOR;
  Vec2i pos = {0, 0};
  Vec2ui dim = {10, 10};
  pastel_fill_rect(&canvas, &pos, &dim, shader);

  pos.x = 0; pos.y = HEIGHT - 10;
  pastel_fill_rect(&canvas, &pos, &dim, shader);

  pos.x = WIDTH - 10; pos.y = 0;
  pastel_fill_rect(&canvas, &pos, &dim, shader);

  pos.x = (WIDTH-10)/2; pos.y = (HEIGHT-10)/2;
  pastel_fill_rect(&canvas, &pos, &dim, shader);

  pos.x = WIDTH-10; pos.y = HEIGHT-10;
  pastel_fill_rect(&canvas, &pos, &dim, shader);

  // Save to png
  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/000_example_corners.png");
  return ok;
}

bool example_checker(void) {
  // initialize buffer with bg color
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  __fill_bg(&canvas, BG_COLOR);

  PastelShaderContextMonochrome context;
  PastelShader shader = {pastel_shader_func_monochrome, &context};

  size_t cols = 10;
  size_t rect_width = WIDTH / cols;
  size_t rows = 8;
  size_t rect_height = HEIGHT / rows;
  // Pixel at col m, row n is at pos (m * WIDTH/cols, n * HEIGHT/rows)

  context.color = FG_COLOR;
  Vec2i pos;
  Vec2ui dim = {rect_width, rect_height};
  for (size_t i = 0; i < rows; ++i) {
    size_t j = 0;
    if (i % 2 == 1) { j = 1; }

    pos.y = i * (HEIGHT / rows);
    for (; j < cols; j += 2) {
      pos.x = j * (WIDTH / cols);
      pastel_fill_rect(&canvas, &pos, &dim, shader);
    }
  }

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/001_example_checker.png");
  return ok;
}

bool example_rect(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  pastel_test_fill_rects(&canvas);

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/002_example_rect.png");
  return ok;
}


bool example_circle(void) {
  // initialize buffer with bg color
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  __fill_bg(&canvas, BG_COLOR);

  PastelShaderContextMonochrome context;
  PastelShader shader = {pastel_shader_func_monochrome, &context};

  context.color = FG_COLOR;
  Vec2i pos = {WIDTH/2, HEIGHT/2};
  pastel_fill_circle(&canvas, &pos, 25, shader);

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/003_example_circle.png");
  return ok;
}

bool example_multi_circles(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  pastel_test_fill_circles(&canvas);

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/004_example_multi_circles.png");
  return ok;
}

bool example_line(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  pastel_test_draw_lines(&canvas);

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/005_example_line.png");
  return ok;
}

bool example_line_shader(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  pastel_test_draw_lines_with_shaders(&canvas);

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/006_example_line_shader.png");
  return ok;
}

bool example_triangle(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  pastel_test_fill_triangles(&canvas);

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/007_example_triangle.png");
  return ok;
}

bool example_gradientx(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  pastel_test_gradientx(&canvas);

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/008_example_gradientx.png");
  return ok;
}

bool example_gradienty(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  pastel_test_gradienty(&canvas);

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/009_example_gradienty.png");
  return ok;
}

bool example_circle_gradientx(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  pastel_test_gradienty(&canvas);

  Vec2i p = { WIDTH/2, HEIGHT/2 };
  size_t r = { WIDTH/4 } ; 
  PastelShaderContextGradient1D context = { PASTEL_RED, PASTEL_YELLOW, p.x-(int)r, p.x+(int)r};
  PastelShader shader = { pastel_shader_func_gradient1dx, &context };

  pastel_fill_circle(&canvas, &p, r, shader);

  bool ok1 = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/010_example_circle_gradientx.png");
  return ok1;
}

bool example_alpha_blending(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  pastel_test_alpha_blending(&canvas);

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/011_example_alpha_blending.png");
  return ok;
}

static Color asset_pixels[1920 * 1080];
bool asset_circle_gradientx(void) {
  PastelCanvas canvas = pastel_canvas_create(asset_pixels, 1920, 1080);
  pastel_test_gradienty(&canvas);

  Vec2i p = { canvas.width/2, canvas.height/2 };
  size_t r = { canvas.width/4 } ; 
  PastelShaderContextGradient1D context = { PASTEL_RED, PASTEL_YELLOW, p.x-(int)r, p.x+(int)r};
  PastelShader shader = { pastel_shader_func_gradient1dx, &context };

  pastel_fill_circle(&canvas, &p, r, shader);

  bool ok1 = save_canvas_to_png(&canvas, ASSETS_DIR_PATH"/pastel.png");
  return ok1;
}

int main (void) {
  if (!example_corners()) return -1;
  if (!example_checker()) return -1;
  if (!example_rect()) return -1;
  if (!example_circle()) return -1;
  if (!example_multi_circles()) return -1;
  if (!example_line()) return -1;
  if (!example_line_shader()) return -1;
  if (!example_triangle()) return -1;
  if (!example_gradientx()) return -1;
  if (!example_gradienty()) return -1;
  if (!example_circle_gradientx()) return -1;
  if (!example_alpha_blending()) return -1;
  if (!asset_circle_gradientx()) return -1;
  return 0;
}
