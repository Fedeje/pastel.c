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

typedef unsigned char uint8_t;

#define WIDTH 800
#define HEIGHT 600
// A pixel is 0xAABBGGRR in hexadecimal i.e 4 bytes i.e 32 bits
static uint32_t pixels[HEIGHT * WIDTH];
// Note:
// If `T array_of_T[50]`; is defined at file scope (outside of all functions)
// or is static, it's not going to be on the stack, it's going to be a global
// preallocated at program's start variable.
// If it's not static and is defined at function scope, it's going to be
// on the stack.

// Typedef for error number.
// Also, errno is a variable which contains the last
// error returned by the system.
// Returning errno is standard a way to catch errors.
typedef int Errno;
#define return_defer(value) do { result = (value); goto defer; } while (0)

Errno pastel_write_to_ppm_file(uint32_t* pixels, // const is useless
                               size_t width, size_t height,
                               const char* file_path) {
  // Header where stuff is allocated.
  // When function needs to return, call return_defer.
  // The `defer` label below takes care of de-allocating stuff.
  int result = 0;
  FILE* f = NULL;

  // Body of the function
  {
    f = fopen(file_path, "wb"); /* write to binary */
    if (f == NULL) return_defer(errno);

    fprintf(f, "P6\n%zu %zu 255\n", width, height);
    if (ferror(f)) return_defer(errno);

    size_t i;
    for (i = 0; i < width * height; ++i) {
      uint32_t pixel = pixels[i];
      // Pixel is: 0xAABBGGRR
      // We use a combination of bit-shifting and bit-masking
      // to extract the bytes corresponding to Red, Green and Blue
      uint8_t bytes[3] = {
        (pixel>>(8*0))&0xFF, // R
        (pixel>>(8*1))&0xFF, // G
        (pixel>>(8*2))&0xFF, // B
      };
      fwrite(bytes, sizeof(bytes), 1, f);
      if (ferror(f)) return_defer(errno);
    } // for loop
  } // body of function


defer: // this is a label you can "goto"
  if (f) fclose(f);
  return result;
} // function `Errno pastel_write_to_ppm_file`


void pastel_fill(uint32_t* pixels,
                 size_t width, size_t height,
                 uint32_t color) {
  for (size_t i = 0; i < width * height; ++i) {
    pixels[i] = color;
  }
} // function `void pastel_fill`

int main (void) {
  // Step 1: render an uniform image.
  pastel_fill(pixels, WIDTH, HEIGHT, 0x000000FF);
  pastel_write_to_ppm_file(pixels, WIDTH, HEIGHT, "output.ppm");
  return 0;
}
