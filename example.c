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
#include "pastel.c"

#define WIDTH 800
#define HEIGHT 600
// A pixel is 0xAABBGGRR in hexadecimal i.e 4 bytes i.e 32 bits
// A pixel image is row major (meaning we store row by row).
// This means we index pixels the following:
//   if `i` is the row and `j` is the column then 
//   pixels(i, j) <=> pixels[i * height + j]
static uint32_t pixels[HEIGHT * WIDTH];
// Note:
// If `T array_of_T[50]`; is defined at file scope (outside of all functions)
// or is static, it's not going to be on the stack, it's going to be a global
// preallocated at program's start variable.
// If it's not static and is defined at function scope, it's going to be
// on the stack.

#define BG_COLOR 0x00000000
#define FG_COLOR 0x000000FF

bool corners_example(void) {
  pastel_fill(pixels, WIDTH, HEIGHT, BG_COLOR);
  pastel_fill_rect(pixels, WIDTH, HEIGHT, 0, 0, 10, 10, FG_COLOR);
  pastel_fill_rect(pixels, WIDTH, HEIGHT, 0, WIDTH - 10, 10, 10, FG_COLOR);
  pastel_fill_rect(pixels, WIDTH, HEIGHT, HEIGHT - 10, 0, 10, 10, FG_COLOR);
  pastel_fill_rect(pixels, WIDTH, HEIGHT, (HEIGHT - 10) / 2, (WIDTH - 10) / 2, 10, 10, FG_COLOR);
  pastel_fill_rect(pixels, WIDTH, HEIGHT, HEIGHT - 10, WIDTH - 10, 10, 10, FG_COLOR);

  const char* file_path = "./outputs/corners.ppm";
  Errno err = pastel_write_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
  if (err) {
      fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
      return false;
  }
  return true;
}

int main (void) {
  corners_example();
  return 0;
}
