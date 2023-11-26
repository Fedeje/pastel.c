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
#define PASTEL_IMPLEMENTATION
#include "pastel.h"
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

bool corners_example(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);

  // Draw background
  Color color = BG_COLOR;
  PastelShaderContext context = pastel_shader_context_create(0, 0, 0, 0, &color);
  pastel_fill(canvas, pastel_monochrome_shader, &context);

  // Draw corners
  color = FG_COLOR;
  Vec2i pos = {0, 0};
  Vec2ui dim = {10, 10};
  pastel_fill_rect(canvas, &pos, &dim, pastel_monochrome_shader, &context);

  pos.x = 0; pos.y = HEIGHT - 10;
  pastel_fill_rect(canvas, &pos, &dim, pastel_monochrome_shader, &context);

  pos.x = WIDTH - 10; pos.y = 0;
  pastel_fill_rect(canvas, &pos, &dim, pastel_monochrome_shader, &context);

  pos.x = (WIDTH-10)/2; pos.y = (HEIGHT-10)/2;
  pastel_fill_rect(canvas, &pos, &dim, pastel_monochrome_shader, &context);

  pos.x = WIDTH-10; pos.y = HEIGHT-10;
  pastel_fill_rect(canvas, &pos, &dim, pastel_monochrome_shader, &context);

  // Save to png
  const char* file_path = IMGS_DIR_PATH"/corners_examples.png";
  printf("Generated image %s\n", file_path);
  if (!stbi_write_png(file_path, WIDTH, HEIGHT, 4, canvas.pixels, WIDTH*sizeof(Color))) {
      fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
      return false;
  }
  return true;
}

bool checker_example(void) {
  // initialize buffer with bg color
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);

  Color bg_color = BG_COLOR;
  PastelShaderContext context = pastel_shader_context_create(0, 0, 0, 0, &bg_color);
  pastel_fill(canvas, pastel_monochrome_shader, &context);

  size_t cols = 10;
  size_t rect_width = WIDTH / cols;
  size_t rows = 8;
  size_t rect_height = HEIGHT / rows;
  // Pixel at col m, row n is at pos (m * WIDTH/cols, n * HEIGHT/rows)

  Color fg_color = FG_COLOR;
  context.colors = &fg_color;
  Vec2i pos;
  Vec2ui dim = {rect_width, rect_height};
  for (size_t i = 0; i < rows; ++i) {
    size_t j = 0;
    if (i % 2 == 1) { j = 1; }

    pos.y = i * (HEIGHT / rows);
    for (; j < cols; j += 2) {
      pos.x = j * (WIDTH / cols);
      pastel_fill_rect(canvas, &pos, &dim, pastel_monochrome_shader, &context);
    }
  }

  const char* file_path = IMGS_DIR_PATH"/checker_example.png";
  printf("Generated image %s\n", file_path);
  if (!stbi_write_png(file_path, WIDTH, HEIGHT, 4, canvas.pixels, WIDTH*sizeof(Color))) {
      fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
      return false;
  }
  return true;
}

bool circle_example(void) {
  // initialize buffer with bg color
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);

  Color color = BG_COLOR;
  PastelShaderContext context = pastel_shader_context_create(0, 0, 0, 0, &color);
  pastel_fill(canvas, pastel_monochrome_shader, &context);

  color = FG_COLOR;
  Vec2i pos = {WIDTH/2, HEIGHT/2};
  pastel_fill_circle(canvas, &pos, 25, pastel_monochrome_shader, &context);
  const char* file_path = IMGS_DIR_PATH"/circle_example.png";
  printf("Generated image %s\n", file_path);
  if (!stbi_write_png(file_path, WIDTH, HEIGHT, 4, canvas.pixels, WIDTH*sizeof(Color))) {
      fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
      return false;
  }
  return true;
}

bool line_example(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);

  Color color = BG_COLOR;
  PastelShaderContext context = pastel_shader_context_create(0, 0, 0, 0, &color);
  pastel_fill(canvas, pastel_monochrome_shader, &context);

  Vec2i p1, p2;

  //
  // Side lines
  color = PASTEL_RED;
  p1.x = 0; p1.y = 0;
  p2.x = 0; p2.y = HEIGHT-1;
  pastel_draw_line(canvas, &p1, &p2, pastel_monochrome_shader, &context);

  p1.x = WIDTH-1; p1.y = 0;
  p2.x = WIDTH-1; p2.y = HEIGHT-1;
  pastel_draw_line(canvas, &p1, &p2, pastel_monochrome_shader, &context);

  //
  // Middle lines
  color = PASTEL_GREEN;
  p1.x = WIDTH/2; p1.y = HEIGHT-1;
  p2.x = WIDTH/2; p2.y = 0;
  pastel_draw_line(canvas, &p1, &p2, pastel_monochrome_shader, &context);

  p1.x = 0; p1.y = HEIGHT/2;
  p2.x = WIDTH-1; p2.y = HEIGHT/2;
  pastel_draw_line(canvas, &p1, &p2, pastel_monochrome_shader, &context);

  //
  // Diagonal lines
  color = PASTEL_BLUE;
  p1.x = 0; p1.y = 0;
  p2.x = WIDTH-1; p2.y = HEIGHT-1;
  pastel_draw_line(canvas, &p1, &p2, pastel_monochrome_shader, &context);

  p1.x = 0; p1.y = HEIGHT-1;
  p2.x = WIDTH-1; p2.y = 0;
  pastel_draw_line(canvas, &p1, &p2, pastel_monochrome_shader, &context);

  const char* file_path = IMGS_DIR_PATH"/line_example.png";
  printf("Generated image %s\n", file_path);
  if (!stbi_write_png(file_path, WIDTH, HEIGHT, 4, canvas.pixels, WIDTH*sizeof(Color))) {
      fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
      return false;
  }
  return true;
}

PASTELDEF Color line_shader1(PastelShaderContext* shader_context) {
  if (shader_context->count == 10) {
    shader_context->color_index++;
    if (shader_context->color_index > 2) shader_context->color_index = 0;
    shader_context->count = 0;
  } else {
    shader_context->count++;
  }
  return shader_context->colors[shader_context->color_index];
}

PASTELDEF Color line_shader2(PastelShaderContext* shader_context) {
  if (shader_context->x < WIDTH/2) {
    return PASTEL_RED;
  }
  return PASTEL_GREEN;
}

PASTELDEF Color line_shader3(PastelShaderContext* shader_context) {
  if (shader_context->y < HEIGHT/2) {
    return PASTEL_BLUE;
  }
  return PASTEL_YELLOW;
}

bool line_shader_example(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);

  Color bg_color = BG_COLOR;
  PastelShaderContext context = pastel_shader_context_create(0, 0, 0, 0, &bg_color);
  pastel_fill(canvas, pastel_monochrome_shader, &context);

  Color colors[3] = { PASTEL_RED, PASTEL_GREEN, PASTEL_BLUE };
  PastelShaderContext shader_context = pastel_shader_context_create(0, 0, 0, 0, colors);

  Vec2i p1, p2;

  //
  // Middle lines
  p1.x = WIDTH/2; p1.y = HEIGHT-1;
  p2.x = WIDTH/2; p2.y = 0;
  pastel_draw_line(canvas, &p1, &p2, line_shader1, &shader_context);

  p1.x = 0; p1.y = HEIGHT/2;
  p2.x = WIDTH-1; p2.y = HEIGHT/2;
  pastel_draw_line(canvas, &p1, &p2, line_shader1, &shader_context);

  //
  // Diagonal lines
  p1.x = 0; p1.y = HEIGHT-1;
  p2.x = WIDTH-1; p2.y = 0;
  pastel_draw_line(canvas, &p1, &p2, line_shader2, &shader_context);

  p1.x = 0; p1.y = 0;
  p2.x = WIDTH-1; p2.y = HEIGHT-1;
  pastel_draw_line(canvas, &p1, &p2, line_shader3, &shader_context);

  const char* file_path = IMGS_DIR_PATH"/line_shader_example.png";
  printf("Generated image %s\n", file_path);
  if (!stbi_write_png(file_path, WIDTH, HEIGHT, 4, canvas.pixels, WIDTH*sizeof(Color))) {
      fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
      return false;
  }
  return true;
}

bool triangle_example(void) {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);

  Color color = BG_COLOR;
  PastelShaderContext context = pastel_shader_context_create(0, 0, 0, 0, &color);
  pastel_fill(canvas, pastel_monochrome_shader, &context);

  Vec2i p1, p2, p3;

  color = PASTEL_RED;
  p1.x = 0; p1.y = HEIGHT/2;
  p2.x = (WIDTH-1)/2; p2.y = HEIGHT-1;
  p3.x = (2*WIDTH)/3; p3.y = 0;
  pastel_fill_triangle(canvas, &p1, &p2, &p3, pastel_monochrome_shader, &context);

  color = PASTEL_GREEN;
  p1.x = 0; p1.y = HEIGHT/4;
  p2.x = (2*WIDTH)/3; p2.y = (5*HEIGHT)/6;
  p3.x = (3*WIDTH)/4; p3.y = (2*HEIGHT)/3;
  pastel_fill_triangle(canvas, &p1, &p2, &p3, pastel_monochrome_shader, &context);

  color = PASTEL_BLUE;
  p1.x = (2*WIDTH)/3; p1.y = HEIGHT/4;
  p2.x = WIDTH-1; p2.y = HEIGHT/2;
  p3.x = (4*WIDTH)/5; p3.y = (3*HEIGHT)/4;
  pastel_fill_triangle(canvas, &p1, &p2, &p3, pastel_monochrome_shader, &context);

  const char* file_path = IMGS_DIR_PATH"/triangle_example.png";
  printf("Generated image %s\n", file_path);
  if (!stbi_write_png(file_path, WIDTH, HEIGHT, 4, canvas.pixels, WIDTH*sizeof(Color))) {
      fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
      return false;
  }
  return true;
}

int main (void) {
  if (!corners_example()) return -1;
  if (!checker_example()) return -1;
  if (!circle_example()) return -1;
  if (!line_example()) return -1;
  if (!line_shader_example()) return -1;
  if (!triangle_example()) return -1;
  return 0;
}
