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

int main() {
  for (size_t i = 0; i < 100000; ++i) {
    PastelCanvas canvas = pastel_canvas_create(pixels, WIDTH, HEIGHT);
    pastel_fill(canvas, PASTEL_BLACK);
    // Side lines
    pastel_draw_line2(canvas, 0, 0, 0, HEIGHT-1, PASTEL_RED);
    pastel_draw_line2(canvas, WIDTH-1, 0, WIDTH-1, HEIGHT-1, PASTEL_RED);
    // Middle lines
    pastel_draw_line2(canvas, WIDTH / 2, HEIGHT-1, WIDTH / 2, 0, PASTEL_GREEN);
    pastel_draw_line2(canvas, 0, HEIGHT / 2, WIDTH-1, HEIGHT / 2, PASTEL_GREEN);
    // Diagonal lines
    pastel_draw_line2(canvas, 0, 0, WIDTH-1, HEIGHT-1, PASTEL_BLUE);
    pastel_draw_line2(canvas, 0, HEIGHT-1, WIDTH-1, 0, PASTEL_BLUE);
  }

  // const char* file_path = IMGS_DIR_PATH"/misc2.png";
  // printf("Generated image %s\n", file_path);
  // if (!stbi_write_png(file_path, WIDTH, HEIGHT, 4, canvas.pixels, WIDTH*sizeof(uint32_t))) {
  //     fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
  //     return false;
  // }
  // return true;
  return 0;
}

