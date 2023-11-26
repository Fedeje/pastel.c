#ifndef PASTEL_SHADER_UTILS_H_
#define PASTEL_SHADER_UTILS_H_
#endif // PASTEL_SHADER_UTILS_H_

// -------------------- PASTEL SHADER UTILS --------------------
//    Collection of reccuring usefull shaders and their context
// -------------------------------------------------------------

// Usage:
// At the beginning of your file, define and include IN THAT ORDER:
//     #define PASTEL_SHADER_UTILS_IMPLEMENTATION // if implem is needed
//     #include "pastel_shader_utils.h"
//     #define PASTEL_IMPLEMENTATION // if implem is needed
//     #include "pastel.h"
// 
// Again, use `#define ..._IMPLEMENTATION` if and only if the
// implementations are needed in the compilation unit you are working on.
// 

#include "pastel.h"

//
// Monochrome shader
typedef struct {
  Color color;
} PastelShaderContextMonochrome;

PASTELDEF Color pastel_shader_func_monochrome(int x, int y, void* context);

//
// Gradient shader 1D.
typedef struct {
  Color c1;
  Color c2;
  int min;
  int max;
} PastelShaderContextGradient1D;

PASTELDEF Color pastel_shader_func_gradient1dx(int x, int y, void* context);
PASTELDEF Color pastel_shader_func_gradient1dy(int x, int y, void* context);

//
// Gradient shader 2D.
typedef struct {
  Color c1;
  Color c2;
  Vec2i min;
  Vec2i max;
} PastelShaderContextGradient2D;


// ------------------------------------------------------
// -------------- SHADERS IMPLEMENTATIONS ---------------
// ------------------------------------------------------
#ifdef PASTEL_SHADER_UTILS_IMPLEMENTATION

PASTELDEF Color pastel_shader_func_monochrome(int x, int y, void* context) {
  PASTEL_UNUSED(x); PASTEL_UNUSED(y);
  PastelShaderContextMonochrome* _context = (PastelShaderContextMonochrome*)context;
  return _context->color;
}

#include <stdio.h>

PASTELDEF Color __pastel_compute_color_grad1d(int v, int vmin, int vmax, Color c1, Color c2) {
  if (v < vmin) v = vmin; if(v > vmax) v = vmax;

  Color c1_red = PASTEL_RED_CHANNEL(c1);
  Color c2_red = PASTEL_RED_CHANNEL(c2);
  Color c_red  = ((vmax-v)*c1_red + (v-vmin)*c2_red)/(vmax-vmin); if (c_red > 255) c_red = 255;

  Color c1_green = PASTEL_GREEN_CHANNEL(c1);
  Color c2_green = PASTEL_GREEN_CHANNEL(c2);
  Color c_green  = ((vmax-v)*c1_green + (v-vmin)*c2_green)/(vmax-vmin); if (c_green > 255) c_green = 255;

  Color c1_blue = PASTEL_BLUE_CHANNEL(c1);
  Color c2_blue = PASTEL_BLUE_CHANNEL(c2);
  Color c_blue  = ((vmax-v)*c1_blue + (v-vmin)*c2_blue)/(vmax-vmin); if (c_blue > 255) c_blue = 255;

  Color c1_alpha = PASTEL_ALPHA_CHANNEL(c1);
  Color c2_alpha = PASTEL_ALPHA_CHANNEL(c2);
  Color c_alpha  = ((vmax-v)*c1_alpha + (v-vmin)*c2_alpha)/(vmax-vmin); if (c_alpha > 255) c_alpha = 255;

  Color color = PASTEL_RGBA(c_red, c_green, c_blue, c_alpha);
  return color;
}

PASTELDEF Color pastel_shader_func_gradient1dx(int x, int y, void* context) {
  PASTEL_UNUSED(y);
  PastelShaderContextGradient1D* _context = (PastelShaderContextGradient1D*)context;
  Color color = __pastel_compute_color_grad1d(x, _context->min, _context->max, _context->c1, _context->c2);
  return color;
}

PASTELDEF Color pastel_shader_func_gradient1dy(int x, int y, void* context) {
  PASTEL_UNUSED(x);
  PastelShaderContextGradient1D* _context = (PastelShaderContextGradient1D*)context;
  Color color = __pastel_compute_color_grad1d(y, _context->min, _context->max, _context->c1, _context->c2);
  return color;
}

#endif // PASTEL_SHADER_UTILS_IMPLEMENTATION
