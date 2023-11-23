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
static uint32_t pixels[HEIGHT * WIDTH];

#define IMGS_DIR_PATH "./imgs"

uint32_t line_shader1(PastelShaderContext* shader_param) {
  if (shader_param->count == 10) {
    shader_param->color_index++;
    if (shader_param->color_index > 2) shader_param->color_index = 0;
    shader_param->count = 0;
  } else {
    shader_param->count++;
  }
  return shader_param->colors[shader_param->color_index];
}

uint32_t line_shader2(PastelShaderContext*) {
  return PASTEL_RED;
}

int main() {
  PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
  pastel_fill(canvas, PASTEL_BLACK);
  uint32_t colors[3] = { PASTEL_RED, PASTEL_GREEN, PASTEL_BLUE };
  PastelShaderContext shader_context = pastel_shader_context_create(0, 0, 0, 0, colors);
  for (size_t i = 0; i < 1000000; ++i) {

    #if 0
    pastel_fill_triangle_oriented(canvas,
                                  0, HEIGHT / 2,
                                  (WIDTH - 1) / 2, HEIGHT - 1,
                                  (2*WIDTH)/3, 0,
                                  PASTEL_RED);
    #endif

    #if 0
    pastel_fill_triangle2(canvas,
                          0, HEIGHT / 2,
                          (WIDTH - 1) / 2, HEIGHT - 1,
                          (2*WIDTH)/3, 0,
                          PASTEL_RED);
    #endif

    #if 1
      pastel_draw_line_with_shader(canvas, 0, 0, WIDTH-1, HEIGHT-1, line_shader1, &shader_context);
    #endif

    #if 0
      pastel_draw_line_with_shader(canvas, 0, 0, WIDTH-1, HEIGHT-1, line_shader2, &shader_context);
    #endif

    #if 0
      pastel_draw_line(canvas, 0, 0, WIDTH-1, HEIGHT-1, PASTEL_RED);
    #endif

    #if 0
    // Side lines
    pastel_draw_line(canvas, 0, 0, 0, HEIGHT-1, PASTEL_RED);
    pastel_draw_line(canvas, WIDTH-1, 0, WIDTH-1, HEIGHT-1, PASTEL_RED);
    // Middle lines
    pastel_draw_line(canvas, WIDTH / 2, HEIGHT-1, WIDTH / 2, 0, PASTEL_GREEN);
    pastel_draw_line(canvas, 0, HEIGHT / 2, WIDTH-1, HEIGHT / 2, PASTEL_GREEN);
    // Diagonal lines
    pastel_draw_line(canvas, 0, 0, WIDTH-1, HEIGHT-1, PASTEL_BLUE);
    pastel_draw_line(canvas, 0, HEIGHT-1, WIDTH-1, 0, PASTEL_BLUE);
    #endif
  }

  const char* file_path = IMGS_DIR_PATH"/misc1.png";
  printf("Generated image %s\n", file_path);
  if (!stbi_write_png(file_path, WIDTH, HEIGHT, 4, canvas.pixels, WIDTH*sizeof(uint32_t))) {
      fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
      return 1;
  }
  return 0;
}
