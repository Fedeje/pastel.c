#ifndef PASTEL_H_
#define PASTEL_H_
//
// How does a header only C library typically look like?
// You have two components:
//   - The declaration section, surrounded by the "BLA_H" header guard.
//   - The definition section, which contains the actual implementation
//     of the functions, surround by the "BLA_IMPLEMENTATION" guard.
// Suppose you are building a lib with executables, which use the "Bla" project.
// Two scenarios:
//   - You are in a header file, i.e "whatever.h", you simply put:
//        #include "bla.h"
//   - You are in a compiled file (a .c typically), after ALL the includes, you
//     put:
//        #include "whatever.h"
//        #include "something.h"
//        ...
//        #define BLA_IMPLEMENTATION
//        #include "bla.h"
// In that way, the implementation will only be available once, in the compilation
// unit. Since you don't need the implementations in the header files like
// "whatever.h", if you were to add the #define BLA_IMPLEMENTATION flag in one
// of them, the compiler would throw an error message, which is good.
//
// In the rest of this file, we make functions static and inline
//   - static: if another compilation unit requires a function from bla.h, it
//     cannot go look for it elsewhere than its own compilation unit.
//     The static keyword means the function is only visible by the current
//     compilation unit.
//   - inline: to have more compiler optimization.
//     This makes the code heavier however.
//

// 
// What does it take to render something on the screen?
// 1 - Filling the image buffer
// 2 - Sending the buffer to the screen
// Here, we'll mainly focus on step 1, but if we have time, we'll
// also take a look at step 2.
// 
// Convention on image axis: 
//      +----------> x-axis
//      |
//      |
//      |
//      |
//       
//      y-axis

// Notes on shaders:
// A shader is simply a function which takes a context (information about a scene and the pixel at which the shader is computed)
// and performs computation on the current pixel.
// A shader is meant to be applied to every pixel of the image / every pixel of the object being rendered.
// 
// TODO: 2D color gradient
// TODO: Rainbow triangle
// TODO: SDL to have a window instead of png / web thingy
// TODO: Immediate mode GUI? Like DearImgui (cimgui) or Nuklear
// TODO: 3D - Projective camera
// TODO: Load mesh model
// TODO: z-buffer
// TODO: super-sampling for lines
// TODO: super-sampling for triangles
// TODO: alpha blending
// TODO: font
// TODO: terminal rendering
// TODO: bezier curves
// TODO: textures
// TODO: signed distance functions / shape blending / ray marching
// 

#define PASTEL_RED    0xFF5472E8
#define PASTEL_GREEN  0xFF79E854
#define PASTEL_BLUE   0xFFE86056
#define PASTEL_YELLOW 0xFF3EF0AF
#define PASTEL_BLACK  0xFF3A3C45
#define PASTEL_WHITE  0xFFFFFFFF

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// ------------------------------------
// -------------- MACROS --------------
// ------------------------------------
#define PASTEL_SWAP(T, val1, val2) do { T tmp = val1; val1 = val2; val2 = tmp; } while (0)
#define PASTEL_SIGN(T, x) ((T)((x) > 0) - (T)((x) < 0))
#define PASTEL_ABS(T, x) (PASTEL_SIGN(T, x) * (x))
#define PASTEL_MIN2(min, x, y) do { min = x; if (min > y) min = y; } while (0)
#define PASTEL_MIN3(min, x, y, z) do { min = x; if (min > y) min = y; if (min > z) min = z; } while (0)
#define PASTEL_MAX2(max, x, y) do { max = x; if (max < y) max = y; } while (0)
#define PASTEL_MAX3(max, x, y, z) do { max = x; if (max < y) max = y; if (max < z) max = z; } while (0)
#define PASTEL_PIXEL(canvas, x, y) (canvas)->pixels[(y) * (canvas)->stride + (x)]

typedef uint32_t Color;
#define PASTEL_RED_CHANNEL(color)   (((color)&0x000000FF)>>(8*0))
#define PASTEL_GREEN_CHANNEL(color) (((color)&0x0000FF00)>>(8*1))
#define PASTEL_BLUE_CHANNEL(color)  (((color)&0x00FF0000)>>(8*2))
#define PASTEL_ALPHA_CHANNEL(color) (((color)&0xFF000000)>>(8*3))
#define PASTEL_RGBA(r, g, b, a)  (((a)&0xFF)<<(8*3))|(((b)&0xFF)<<(8*2))|(((g)&0xFF)<<(8*1))|(((r)&0xFF)<<(8*0));

#define PASTEL_SHADER(shader) Color (*(shader))(PastelShaderContext*)
#define PASTEL_SHADER_FUNC(shader) Color (*(shader))(void*)
#define PASTEL_UNUSED(x) (void)(x)

#ifndef PASTELDEF
#define PASTELDEF static inline
#endif

// --------------------------------------
// -------------- STRUCTS ---------------
// --------------------------------------
#define PASTEL_TYPEDEF_VEC2(T, name) typedef struct { T x; T y; } name
PASTEL_TYPEDEF_VEC2(size_t, Vec2ui);
PASTEL_TYPEDEF_VEC2(int, Vec2i);

typedef struct {
  Color* pixels;
  size_t width;
  size_t height;
  size_t stride;
} PastelCanvas;

// A shader is a struct which has 2 things:
//   - A shader function which acts on a pixel at position (x, y) on the image.
//   - A shader context which can be required by the shader function.
//     This context provides parameters to the shader function so that it
//     can do its computations.
typedef struct {
  Color (*run)(int x, int y, void*);
  void* context;
} PastelShader;

// ----------------------------------------
// -------------- FUNCTIONS ---------------
// ----------------------------------------

// @brief Create a canvas: image with its width, height and stride (width if row-major, height if column-major).
PASTELDEF PastelCanvas pastel_canvas_create(Color* pixels, size_t pixels_width, size_t pixels_height);

// @brief Alpha-blends two colors.
// See https://fr.wikipedia.org/wiki/Alpha_blending
// @param c1 the color of object on the lower layer
// @param c2 the color of object on the upper layer
PASTELDEF void pastel_blend_colors(Color* c1, Color c2);

// @brief Fill the entire image buffer with a given shader.
// This function does *not* blend with the existing canvas.
// It replaces each pixel of the canvas according to the @param shader.
PASTELDEF void pastel_fill(PastelCanvas* canvas, PastelShader shader);

// @brief Same as `pastel_fill` but blends with the existing canvas
PASTELDEF void pastel_fill_blend(PastelCanvas* canvas, PastelShader shader);

// @brief Fill a rectangle with a given shader.
// A rectangle starts at pixel (x0, y0) and has a width and a height.
// @param pos the upper left corner of the rectangle
// @param dim_rect the width and length of the rectangle
PASTELDEF void pastel_fill_rect(PastelCanvas* canvas, const Vec2i* p, const Vec2ui* dim_rect, PastelShader shader);

// @brief Fill a circle with a given shader.
// A circle has center (x0, y0) and radius r.
// @param p the center position.
// @param r the radius.
PASTELDEF void pastel_fill_circle(PastelCanvas* canvas, const Vec2i* p, size_t r, PastelShader shader);

// @brief Draw a line with a given shader.
// A line starts at (x0, y0) and ends at (x1, y1).
// @param p1 and p2 the two points of the line segment.
PASTELDEF void pastel_draw_line(PastelCanvas* canvas, const Vec2i* p1, const Vec2i* p2, PastelShader shader);

// @brief Fill a triangle with a given shader.
// Instead of using aabb and checking if pixels are inside triangle, fill the triangle by drawing it line by line.
// On the CPU, this version is MUCH faster than the AABB ones above (3x faster).
// However, in practice, on the GPU, it's the AABB one which is implemented.
// This is simply because you can assign each pixel of the AABB to a GPU core. Parallelism changes a lot (benchmarking is very important).
// @param p1, p2 and p3 the triangle vertices.
PASTELDEF void pastel_fill_triangle(PastelCanvas* canvas, const Vec2i* p1, const Vec2i* p2, const Vec2i* p3, PastelShader shader);

// @brief Fill a triangle with a given shader.
// A triangle is 3 points (x0, y0), (x1, y1) and (x2, y2)
// Convention: the triangles are stored counter-clockwise.
// This function uses aabb to isolate a rectangle of pixels where the triangle lives.
// For each pixel in this aabb, it then checks if the pixel is in the triangle.
// This is what is used in GPUs as it can parallelize better than drawing triangles line by line.
PASTELDEF void pastel_fill_triangle2_oriented(PastelCanvas* canvas, const Vec2i* p1, const Vec2i* p2, const Vec2i* p3, PastelShader shader);

// @brief Same as `pastel_fill_triangle_oriented` but the triangle does not need to have an orientation.
PASTELDEF void pastel_fill_triangle2(PastelCanvas* canvas, const Vec2i* p1, const Vec2i* p2, const Vec2i* p3, PastelShader shader);

#endif // PASTEL_H_


// -----------------------------------------------------
// -------------- PASTEL IMPLEMENTATIONS ---------------
// -----------------------------------------------------
#ifdef PASTEL_IMPLEMENTATION

PASTELDEF PastelCanvas pastel_canvas_create(Color* pixels, size_t pixels_width, size_t pixels_height) {
  PastelCanvas canvas = {
    .pixels = pixels,
    .width = pixels_width,
    .height = pixels_height,
    .stride = pixels_width
  };
  return canvas;
}

PASTELDEF void pastel_blend_colors(Color* c1, Color c2) {
  Color c1r = PASTEL_RED_CHANNEL(*c1);
  Color c2r = PASTEL_RED_CHANNEL(c2);

  Color c1g = PASTEL_GREEN_CHANNEL(*c1);
  Color c2g = PASTEL_GREEN_CHANNEL(c2);

  Color c1b = PASTEL_BLUE_CHANNEL(*c1);
  Color c2b = PASTEL_BLUE_CHANNEL(c2);

  Color c1a = PASTEL_ALPHA_CHANNEL(*c1);
  Color c2a = PASTEL_ALPHA_CHANNEL(c2);

  c1r  = (c2r*c2a + c1r*(255-c2a))/255; if (c1r > 255) c1r = 255;
  c1g  = (c2g*c2a + c1g*(255-c2a))/255; if (c1g > 255) c1g = 255;
  c1b  = (c2b*c2a + c1b*(255-c2a))/255; if (c1b > 255) c1b = 255;
  *c1 = PASTEL_RGBA(c1r, c1g, c1b, c1a);
}

PASTELDEF void pastel_fill(PastelCanvas* canvas, PastelShader shader) {
  for (int y = 0; y < (int)canvas->height; ++y) {
    for (int x = 0; x < (int)canvas->width; ++x) {
      Color color = shader.run(x, y, shader.context);
      PASTEL_PIXEL(canvas, x, y) = color;
    }
  }
} // function `void pastel_fill`

PASTELDEF void pastel_fill_blend(PastelCanvas* canvas, PastelShader shader) {
  for (int y = 0; y < (int)canvas->height; ++y) {
    for (int x = 0; x < (int)canvas->width; ++x) {
      Color color = shader.run(x, y, shader.context);
      pastel_blend_colors(&PASTEL_PIXEL(canvas, x, y), color);
    }
  }
} // function `void pastel_fill`

PASTELDEF void pastel_fill_rect(PastelCanvas* canvas, const Vec2i* p, const Vec2ui* dim_rect, PastelShader shader) {
  // A pixel image is row-major
  for (int y = p->y; y <= p->y + (int)dim_rect->y; ++y) {
    if (0 <= y && y < (int)canvas->height) {
      for (int x = p->x; x <= p->x + (int)dim_rect->x; ++x) {
        if (0 <= x && x < (int)canvas->width) {
          Color color = shader.run(x, y, shader.context);
          pastel_blend_colors(&PASTEL_PIXEL(canvas, x, y), color);
        }
      }
    }
  }
}

PASTELDEF void pastel_fill_circle(PastelCanvas* canvas, const Vec2i* p, size_t r, PastelShader shader) {
  int x0_aabb = p->x - r;
  int y0_aabb = p->y - r;
  int r2 = (int)(r * r);
  for (int y = y0_aabb; y <= y0_aabb + 2 * (int)r; ++y) {
    if (0 <= y && y < (int)canvas->height) {
      int dist_to_center_y2 = (y - p->y) * (y - p->y);
      for (int x = x0_aabb; x <= x0_aabb + 2 * (int)r; ++x) {
        if (0 <= x && x < (int)canvas->width) {
          int dist_to_center_x2 = (x - p->x) * (x - p->x);
          if ((dist_to_center_x2 + dist_to_center_y2) <= r2) {
            Color color = shader.run(x, y, shader.context);
            pastel_blend_colors(&PASTEL_PIXEL(canvas, x, y), color);
          }
        }
      }
    }
  }
}

PASTELDEF void pastel_draw_line(PastelCanvas* canvas, const Vec2i* p1, const Vec2i* p2, PastelShader shader) {
  int x0 = p1->x; int y0 = p1->y;
  int x1 = p2->x; int y1 = p2->y;
  if (x0 == x1) {
    // Vertical line
    if (0 <= x0 && x0 < (int)canvas->width) {
      if (y0 > y1) PASTEL_SWAP(int, y0, y1);
      for (int y = y0; y <= y1; ++y) {
        if (0 <= y && y < (int)canvas->height) {
          Color color = shader.run(x0, y, shader.context);
          pastel_blend_colors(&PASTEL_PIXEL(canvas, x0, y), color);
        }
      }
    }
  } else if (y0 == y1) {
    // Horizontal line
    if (0 <= y0 && y0 < (int)canvas->height) {
      if (x0 > x1) PASTEL_SWAP(int, x0, x1);
      for (int x = x0; x <= x1; ++x) {
        if (0 <= x && x < (int)canvas->width) {
          Color color = shader.run(x, y0, shader.context);
          pastel_blend_colors(&PASTEL_PIXEL(canvas, x, y0), color);
        }
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
    int dx = x1 - x0; // dx != 0 here
    int dy = y1 - y0;
    // float slope = ((float)dy) / ((float)dx);
    for (int x = x0; x <= x1; ++x) {
      if (0 <= x && x < (int)canvas->width) {
        int ystart = y0 + ((x-x0)*dy)/dx;
        int yend   = y0 + ((x+1-x0)*dy)/dx;
        if (ystart > yend) PASTEL_SWAP(int, ystart, yend);
        for(int y = ystart; y <= yend; ++y) {
          if (0 <= y && y < (int)canvas->height) {
            Color color = shader.run(x, y, shader.context);
            pastel_blend_colors(&PASTEL_PIXEL(canvas, x, y), color);
          }
        }
      }
    }
  }
}

// Convention: the triangles are stored counter-clockwise.
//
//                  ^
//                  | normal -n (because we are on an image
//                  |            so y-axis points DOWN)
//         P0 +-----------+ P2
//           |         |
//          |       |
//         |     |
//        |   |
//       + |
//       P1
// The normals n point always outside the triangle, so the triangle goes as:
// P2 - P0, P1 - P2 and P0 - P1
PASTELDEF void pastel_fill_triangle2_oriented(PastelCanvas* canvas, const Vec2i* p1, const Vec2i* p2, const Vec2i* p3, PastelShader shader) {
  int x0 = p1->x; int y0 = p1->y;
  int x1 = p2->x; int y1 = p2->y;
  int x2 = p3->x; int y2 = p3->y;

  int aabb_x0, aabb_y0, aabb_x1, aabb_y1;
  PASTEL_MIN3(aabb_x0, x0, x1, x2);
  PASTEL_MIN3(aabb_y0, y0, y1, y2);
  PASTEL_MAX3(aabb_x1, x0, x1, x2);
  PASTEL_MAX3(aabb_y1, y0, y1, y2);

  for (int y = aabb_y0; y <= aabb_y1; ++y) {
    if (0 <= y && y < (int)canvas->height) {
      for (int x = aabb_x0; x <= aabb_x1; ++x) {
        if (0 <= x && x < (int)canvas->width) {
          //
          // Test if pixel is in triangle
          //
          int d1, d2, d3; // Dist to hyperplanes
          // WARNING: because we are on an image, x-axis points to right
          // but y-axis points to DOWN!
          // If vector (x, y) then right-hand normal is (-y, x)
          //
          // Compute d1:
          d1 = (x - x0) * (y0 - y2) + (y - y0) * (x2 - x0);
          if (d1 < 0) continue;
          // Compute d2:
          d2 = (x - x2) * (y2 - y1) + (y - y2) * (x1 - x2);
          if (d2 < 0) continue;
          // Compute d3:
          d3 = (x - x1) * (y1 - y0) + (y - y1) * (x0 - x1);
          if (d3 < 0) continue;
          Color color = shader.run(x, y, shader.context);
          pastel_blend_colors(&PASTEL_PIXEL(canvas, x, y), color);

        }
      }
    }
  }
}

// Same as `pastel_fill_triangle_oriented` but the triangle does not
// need to have an orientation.
PASTELDEF void pastel_fill_triangle2(PastelCanvas* canvas, const Vec2i* p1, const Vec2i* p2, const Vec2i* p3, PastelShader shader) {
  int x0 = p1->x; int y0 = p1->y;
  int x1 = p2->x; int y1 = p2->y;
  int x2 = p3->x; int y2 = p3->y;

  int aabb_x0, aabb_y0, aabb_x1, aabb_y1;
  PASTEL_MIN3(aabb_x0, x0, x1, x2);
  PASTEL_MIN3(aabb_y0, y0, y1, y2);
  PASTEL_MAX3(aabb_x1, x0, x1, x2);
  PASTEL_MAX3(aabb_y1, y0, y1, y2);

  for (int y = aabb_y0; y <= aabb_y1; ++y) {
    if (0 <= y && y < (int)canvas->height) {
      for (int x = aabb_x0; x <= aabb_x1; ++x) {
        if (0 <= x && x < (int)canvas->width) {
          //
          // Test if pixel is in triangle
          //
          int d1, d2, d3; // Dist to hyperplanes
          d1 = (x - x0) * (y0 - y2) + (y - y0) * (x2 - x0);
          d2 = (x - x2) * (y2 - y1) + (y - y2) * (x1 - x2);
          d3 = (x - x1) * (y1 - y0) + (y - y1) * (x0 - x1);

          bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
          bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
          if (!(has_neg && has_pos)) {
            Color color = shader.run(x, y, shader.context);
            pastel_blend_colors(&PASTEL_PIXEL(canvas, x, y), color);
          }
        }
      }
    }
  }
}

PASTELDEF void pastel_fill_triangle(PastelCanvas* canvas, const Vec2i* p1, const Vec2i* p2, const Vec2i* p3, PastelShader shader) {
  int x0 = p1->x; int y0 = p1->y;
  int x1 = p2->x; int y1 = p2->y;
  int x2 = p3->x; int y2 = p3->y;
  if ((y0 == y1 && y0 == y2) || (x0 == x1 && x0 == x2)) return; // degenerate triangle

  // Sort the vertices according to the y-axis
  if (y0 > y1) { PASTEL_SWAP(int, x0, x1); PASTEL_SWAP(int, y0, y1); }
  if (y1 > y2) { PASTEL_SWAP(int, x1, x2); PASTEL_SWAP(int, y1, y2); }
  if (y0 > y1) { PASTEL_SWAP(int, x0, x1); PASTEL_SWAP(int, y0, y1); }

  // Draw first half of the triangle
  int dx1 = x1 - x0;
  int dy1 = y1 - y0;
  int dx2 = x2 - x0;
  int dy2 = y2 - y0;
  for (int y = y0; y <= y1; ++y) {
    if (0 <= y && y < (int)canvas->height) {
      int xl1 = dy1 != 0 ? x0 + ((y-y0)*dx1)/dy1 : x0;
      int xl2 = dy2 != 0 ? x0 + ((y-y0)*dx2)/dy2 : x0;
      if (xl1 > xl2) PASTEL_SWAP(int, xl1, xl2);
      for (int x = xl1; x <= xl2; ++x) {
        if (0 <= x && x < (int)canvas->width) {
          Color color = shader.run(x, y, shader.context);
          pastel_blend_colors(&PASTEL_PIXEL(canvas, x, y), color);
        }
      }
    }
  }

  // Draw second half of the triangle
  dx1 = x2 - x0;
  dy1 = y2 - y0;
  dx2 = x2 - x1;
  dy2 = y2 - y1;
  for (int y = y1; y <= y2; ++y) {
    if (0 <= y && y < (int)canvas->height) {
      int xl1 = dy1 != 0 ? x2 + ((y-y2)*dx1)/dy1 : x2;
      int xl2 = dy2 != 0 ? x2 + ((y-y2)*dx2)/dy2 : x2;
      if (xl1 > xl2) PASTEL_SWAP(int, xl1, xl2);
      for (int x = xl1; x <= xl2; ++x) {
        if (0 <= x && x < (int)canvas->width) {
          Color color = shader.run(x, y, shader.context);
          pastel_blend_colors(&PASTEL_PIXEL(canvas, x, y), color);
        }
      }
    }
  }
}

#endif // PASTEL_IMPLEMENTATION
