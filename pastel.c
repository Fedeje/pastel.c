#ifndef PASTEL_C_
#define PASTEL_C_
// 
// What does it take to render something on the screen?
// 1 - Filling the image buffer
// 2 - Sending the buffer to the screen
// Here, we'll mainly focus on step 1, but if we have time, we'll
// also take a look at step 2.
// 
// TODO: draw_line
// TODO: super-sampling for lines
// TODO: fill_triangle
// 

#define PASTEL_RED   0xFF5472E8
#define PASTEL_GREEN 0xFF79E854
#define PASTEL_BLUE  0xFFE86056
#define PASTEL_BLACK 0xFF3A3C45

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>

// Typedef for error number.
// Also, errno is a variable which contains the last
// error returned by the system.
// Returning errno is standard a way to catch errors.
typedef int Errno;
#define return_defer(value) do { result = (value); goto defer; } while (0)
#define PASTEL_SWAP(T, val1, val2) do { T tmp = val1; val1 = val2; val2 = tmp; } while (0)
#define PASTEL_SIGN(T, x) ((T)((x) > 0) - (T)((x) < 0))
#define PASTEL_ABS(T, x) (PASTEL_SIGN(T, x) * (x))

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
  for (int y = y0; y <= y0 + (int)rect_height; ++y) {
    if (0 <= y && y < (int)pixels_height) {
      for (int x = x0; x <= x0 + (int)rect_width; ++x) {
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
  for (int y = y0_aabb; y <= y0_aabb + 2 * (int)r; ++y) {
    if (0 <= y && y < (int)pixels_height) {
      int dist_to_center_y2 = (y - y0) * (y - y0);
      for (int x = x0_aabb; x <= x0_aabb + 2 * (int)r; ++x) {
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

//
// Draw a line with a given color.
// A line starts at (x0, y0) and ends at (x1, y1).
void pastel_draw_line(uint32_t* pixels,
                        size_t pixels_width, size_t pixels_height,
                        int x0, int y0, int x1, int y1, uint32_t color) {
  if (x0 == x1) {
    // Vertical line
    if (0 <= x0 && x0 < (int)pixels_width) {
      if (y0 > y1) PASTEL_SWAP(int, y0, y1);
      for (int y = y0; y <= y1; ++y) {
        pixels[y * pixels_width + x0] = color;
      }
    }
  } else if (y0 == y1) {
    // Horizontal line
    if (0 <= y0 && y0 < (int)pixels_height) {
      if (x0 > x1) PASTEL_SWAP(int, x0, x1);
      for (int x = x0; x <= x1; ++x) {
        pixels[y0 * pixels_width + x] = color;
      }
    }
  } else {
    if (x0 > x1) {
      PASTEL_SWAP(int, x0, x1);
      PASTEL_SWAP(int, y0, y1);
    }
    // So, there is an "issue" with using int.
    // If we simply increment y by the float slope, since it can be < 1,
    // the casting of the slope to int will resolve to 0 and we would
    // never increment y.
    // One solution is to truncate to nearest int, but this is costly for the CPU
    // as it means branching.
    // Solution which minimizes branching (best would be to benchmark tbh):
    int dx = x0 - x1;
    int dy = y0 - y1;
    int offset = y0 - (dy*x0)/dx;
    // float slope = ((float)dy) / ((float)dx);
    for (int x = x0; x <= x1; ++x) {
      if (0 <= x && x < (int)pixels_width) {
        int ystart = (dy*x)/dx + offset;
        int yend = (dy*(x+1))/dx + offset;
        if (ystart > yend) PASTEL_SWAP(int, ystart, yend);
        for(int y = ystart; y <= yend; ++y) {
          if (0 <= y && y < (int)pixels_height) {
            pixels[y * pixels_width + x] = color;
          }
        }
      }
    }
  }
}

//
// Draw a line with a given color.
// A line starts at (x0, y0) and ends at (x1, y1).
// NOTE: this code is not faster than the one above.
void pastel_draw_line2(uint32_t* pixels,
                        size_t pixels_width, size_t pixels_height,
                        int x0, int y0, int x1, int y1, uint32_t color) {
  bool steep = false;
  if (PASTEL_ABS(int, x0 - x1) < PASTEL_ABS(int, y0 - y1)) {
    // The line is too steep meaning an increment of x will lead
    // to multiple increments in y.
    // We want to fallback to the case where an increment of x can
    // only lead to at most one increment of y.
    // This is only the case when slope < 1.
    // Thus We transpose the line <=> slope = 1 / slope.
    PASTEL_SWAP(int, x0, y0);
    PASTEL_SWAP(int, x1, y1);
    steep = true;
  }

  if (x0 > x1) {
    PASTEL_SWAP(int, x0, x1);
    PASTEL_SWAP(int, y0, y1);
  }

  // When do we increment y?
  // When the error between currently drawn line and "real" line
  // is greater than one pixel, we increment y.
  int dx = x1 - x0;
  int dy = y1 - y0;
  int y = y0;
  int error = 0;
  // float error_increment = PASTEL_ABS(int, dy/dx);
  // we then compare the error to 0.5 (middle btw 0 and 1)
  // to see if we increment y.
  // To remove floats, we multiply everything by 2*dx
  int error_increment = PASTEL_ABS(int, dy) * 2;
  for (int x = x0; x <= x1; ++x) {
    // Draw pixel
    if (steep) {
      if (0 <= x0 && x < (int) pixels_height && 0 <= y0 && y < (int) pixels_width) {
        pixels[x * pixels_width + y] = color;
      }
    } else {
      if (0 <= x0 && x < (int) pixels_width && 0 <= y0 && y < (int) pixels_height) {
        pixels[y * pixels_width + x] = color;
      }
    }
    // Increment y if needed
    error += error_increment;
    if (error > dx) {
      y += (y1 > y0 ? 1 : -1);
      error -= dx * 2;
    }
  }
}

//
// Fill a triangle with a given color.
// A triangle is 3 points (x0, y0), (x1, y1) and (x2, y2)
#if 0
void pastel_fill_triangle(uint32_t* pixels,
                          size_t pixels_width, size_t pixels_height,
                          int x0, int y0, int x1, int y1, int x2, int y2,
                          uint32_t color) {
}
#endif

#endif // PASTEL_C_
