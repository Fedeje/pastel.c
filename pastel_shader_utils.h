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

PASTELDEF Color __pastel_compute_color_grad1d(int v, int vmin, int vmax, Color c1, Color c2) {
  Color color = c1;

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
