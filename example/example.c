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
// Hex color: 0xAABBGGRR
#define BG_COLOR PASTEL_BLACK
#define FG_COLOR PASTEL_RED

bool save_canvas_to_png(const PastelCanvas* canvas, const char* file_path) {
  printf("Generated image %s\n", file_path);
  if (!stbi_write_png(file_path, WIDTH, HEIGHT, 4, canvas->pixels, WIDTH*sizeof(Color))) {
      fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
      return false;
  }
  return true;
}

bool corners_example(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);

  PastelShaderContextMonochrome context;
  PastelShader shader = {pastel_shader_func_monochrome, &context};

  // Draw background
  context.color = BG_COLOR;
  pastel_fill(canvas, shader);

  // Draw corners
  context.color = FG_COLOR;
  Vec2i pos = {0, 0};
  Vec2ui dim = {10, 10};
  pastel_fill_rect(canvas, &pos, &dim, shader);

  pos.x = 0; pos.y = HEIGHT - 10;
  pastel_fill_rect(canvas, &pos, &dim, shader);

  pos.x = WIDTH - 10; pos.y = 0;
  pastel_fill_rect(canvas, &pos, &dim, shader);

  pos.x = (WIDTH-10)/2; pos.y = (HEIGHT-10)/2;
  pastel_fill_rect(canvas, &pos, &dim, shader);

  pos.x = WIDTH-10; pos.y = HEIGHT-10;
  pastel_fill_rect(canvas, &pos, &dim, shader);

  // Save to png
  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/corners_examples.png");
  return ok;
}

bool checker_example(void) {
  // initialize buffer with bg color
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);

  PastelShaderContextMonochrome context;
  PastelShader shader = {pastel_shader_func_monochrome, &context};

  context.color = BG_COLOR;
  pastel_fill(canvas, shader);

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
      pastel_fill_rect(canvas, &pos, &dim, shader);
    }
  }

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/checker_example.png");
  return ok;
}

bool rect_example(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  pastel_test_fill_rects(&canvas);

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/rect_example.png");
  return ok;
}


bool circle_example(void) {
  // initialize buffer with bg color
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);

  PastelShaderContextMonochrome context;
  PastelShader shader = {pastel_shader_func_monochrome, &context};

  context.color = BG_COLOR;
  pastel_fill(canvas, shader);

  context.color = FG_COLOR;
  Vec2i pos = {WIDTH/2, HEIGHT/2};
  pastel_fill_circle(canvas, &pos, 25, shader);

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/circle_example.png");
  return ok;
}

bool multi_circles_example(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  pastel_test_fill_circles(&canvas);

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/multi_circles_example.png");
  return ok;
}

bool line_example(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  pastel_test_draw_lines(&canvas);

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/line_example.png");
  return ok;
}

bool line_shader_example(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  pastel_test_draw_lines_with_shaders(&canvas);

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/line_shader_example.png");
  return ok;
}

bool triangle_example(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  pastel_test_fill_triangles(&canvas);

  bool ok = save_canvas_to_png(&canvas, IMGS_DIR_PATH"/triangle_example.png");
  return ok;
}

int main (void) {
  if (!corners_example()) return -1;
  if (!checker_example()) return -1;
  if (!rect_example()) return -1;
  if (!circle_example()) return -1;
  if (!multi_circles_example()) return -1;
  if (!line_example()) return -1;
  if (!line_shader_example()) return -1;
  if (!triangle_example()) return -1;
  return 0;
}
