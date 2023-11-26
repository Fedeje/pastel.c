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
  if (v < vmin) v = vmin;
  float t = (float)(v - vmin) / (float)(vmax - vmin);

  typedef uint8_t byte;


  byte c1_red = (c1>>(8*0))&0xFF;
  byte c2_red = (c2>>(8*0))&0xFF;
  byte c_red  = (byte)((1-t)*c1_red + t*c2_red);

  byte c1_green = (c1>>(8*1))&0xFF;
  byte c2_green = (c2>>(8*1))&0xFF;
  byte c_green  = (byte)((1-t)*c1_green + t*c2_green);

  byte c1_blue = (c1>>(8*2))&0xFF;
  byte c2_blue = (c2>>(8*2))&0xFF;
  byte c_blue  = (byte)((1-t)*c1_blue + t*c2_blue);

  byte c1_alpha = (c1>>(8*3))&0xFF;
  byte c2_alpha = (c2>>(8*3))&0xFF;
  byte c_alpha  = (byte)((1-t)*c1_alpha + t*c2_alpha);

  // We will gradually shift the alpha to the 4th byte.
  Color color = (c_alpha << (8*3)) | (c_blue << (8*2)) | (c_green << (8*1)) | (c_red << (8*0));
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
