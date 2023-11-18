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
// A rectangle starts at pixel (i0, j0) and has a width and a height.
void pastel_fill_rect(uint32_t* pixels,
                      size_t pixels_width, size_t pixels_height,
                      int j0, int i0,
                      size_t rect_width, size_t rect_height, uint32_t color) {
  // A pixel image is row-major
  for (int i = i0; i < i0 + (int)rect_height; ++i) {
    if (0 <= i && i < (int)pixels_height) {
      for (int j = j0; j < j0 + (int)rect_width; ++j) {
        if (0 <= j && j < (int)pixels_width) {
        pixels[i * pixels_width + j] = color;
        }
      }
    }
  }
}

//
// Fill a circle with a given color.
// A circle has center (i0, j0) and radius r.
void pastel_fill_circle(uint32_t* pixels,
                        size_t pixels_width, size_t pixels_height,
                        int j0, int i0,
                        size_t r, uint32_t color) {
  int i0_aabb = i0 - r;
  int j0_aabb = j0 - r;
  int r2 = (int)(r * r);
  for (int i = i0_aabb; i < i0_aabb + 2 * (int)r; ++i) {
    if (0 <= i && i < (int)pixels_height) {
      int dist_to_center_x2 = (i - i0) * (i - i0);
      for (int j = j0_aabb; j < j0_aabb + 2 * (int)r; ++j) {
        if (0 <= j && j < (int)pixels_width) {
          int dist_to_center_y2 = (j - j0) * (j - j0);
          if ((dist_to_center_x2 + dist_to_center_y2) <= r2) {
            pixels[i * pixels_width + j] = color;
          }
        }
      }
    }
  }
}

#endif // PASTEL_C_
