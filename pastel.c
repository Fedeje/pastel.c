#ifndef PASTEL_C_
#define PASTEL_C_
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

#define PASTEL_RED   0xFF5472E8
#define PASTEL_GREEN 0xFF79E854
#define PASTEL_BLUE  0xFFE86056
#define PASTEL_BLACK 0xFF3A3C45

#include <stdint.h>
#include <stdio.h>
#include <errno.h>

// Typedef for error number.
// Also, errno is a variable which contains the last
// error returned by the system.
// Returning errno is standard a way to catch errors.
typedef int Errno;
#define return_defer(value) do { result = (value); goto defer; } while (0)


//
// Save an image to ppm format
Errno pastel_write_to_ppm_file(uint32_t* pixels, // const is useless
                               size_t pixels_width, size_t pixels_height,
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

    fprintf(f, "P6\n%zu %zu 255\n", pixels_width, pixels_height);
    if (ferror(f)) return_defer(errno);

    size_t i;
    for (i = 0; i < pixels_width * pixels_height; ++i) {
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


//
// Fill the entire image buffer with a given color.
void pastel_fill(uint32_t* pixels,
                 size_t pixels_width, size_t pixels_height,
                 uint32_t color) {
  for (size_t i = 0; i < pixels_width * pixels_height; ++i) {
    pixels[i] = color;
  }
} // function `void pastel_fill`


//
// Fill a rectangle with a given color.
// A rectangle starts at pixel (x0, y0) and has a width and a height.
void pastel_fill_rect(uint32_t* pixels,
                      size_t pixels_width, size_t pixels_height,
                      int x0, int y0,
                      size_t rect_width, size_t rect_height, uint32_t color) {
  // A pixel image is row-major
  for (int y = y0; y < y0 + (int)rect_height; ++y) {
    if (0 <= y && y < (int)pixels_height) {
      for (int x = x0; x < x0 + (int)rect_width; ++x) {
        if (0 <= x && x < (int)pixels_width) {
        pixels[y * pixels_width + x] = color;
        }
      }
    }
  }
}

//
// Fill a circle with a given color.
// A circle has center (x0, y0) and radius r.
void pastel_fill_circle(uint32_t* pixels,
                        size_t pixels_width, size_t pixels_height,
                        int x0, int y0,
                        size_t r, uint32_t color) {
  int x0_aabb = x0 - r;
  int y0_aabb = y0 - r;
  int r2 = (int)(r * r);
  for (int y = y0_aabb; y < y0_aabb + 2 * (int)r; ++y) {
    if (0 <= y && y < (int)pixels_height) {
      int dist_to_center_y2 = (y - y0) * (y - y0);
      for (int x = x0_aabb; x < x0_aabb + 2 * (int)r; ++x) {
        if (0 <= x && x < (int)pixels_width) {
          int dist_to_center_x2 = (x - x0) * (x - x0);
          if ((dist_to_center_x2 + dist_to_center_y2) <= r2) {
            pixels[y * pixels_width + x] = color;
          }
        }
      }
    }
  }
}

#endif // PASTEL_C_
