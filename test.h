#ifndef PASTEL_TEST_H_
#define PASTEL_TEST_H_

// Warning: order of header import is important here!
// The header `pastel_shader_utils.h` uses `pastel.h`.
// However, `pastel.h` can be used on its own.
#define PASTEL_SHADER_UTILS_IMPLEMENTATION
#include "pastel_shader_utils.h"
#define PASTEL_IMPLEMENTATION
#include "pastel.h"

void pastel_test_fill_rects(PastelCanvas* canvas);
void pastel_test_fill_circles(PastelCanvas* canvas);
void pastel_test_draw_lines(PastelCanvas* canvas);
void pastel_test_draw_lines_with_shaders(PastelCanvas* canvas);
void pastel_test_fill_triangles(PastelCanvas* canvas);
void pastel_test_gradientx(PastelCanvas* canvas);
void pastel_test_gradienty(PastelCanvas* canvas);

#endif // PASTEL_TEST_H_

#ifdef PASTEL_TEST_IMPLEMENTATION

void __fill_bg(PastelCanvas* canvas, Color color) {
  PastelShaderContextMonochrome context = { color };
  PastelShader shader = {pastel_shader_func_monochrome, &context};
  pastel_fill(canvas, shader);
}

void pastel_test_fill_rects(PastelCanvas* canvas) {
  __fill_bg(canvas, PASTEL_BLACK);

  PastelShaderContextMonochrome context;
  PastelShader shader = {pastel_shader_func_monochrome, &context};

  Vec2i pos; Vec2ui dim;

  context.color = PASTEL_BLUE;
  pos.x = 0; pos.y = 0;
  dim.x = (canvas->width)/2; dim.y = (canvas->height)/3;
  pastel_fill_rect(canvas, &pos, &dim, shader);

  context.color = PASTEL_RED;
  pos.x = (canvas->width)/4; pos.y = (canvas->height)/2;
  dim.x = (2*canvas->width)/3; dim.y = (canvas->height)/2;
  pastel_fill_rect(canvas, &pos, &dim, shader);

  context.color = PASTEL_GREEN;
  pos.x = (canvas->width)/3; pos.y = (canvas->height)/4;
  dim.x = (canvas->width)/2; dim.y = (canvas->height)/2;
  pastel_fill_rect(canvas, &pos, &dim, shader);
}

void pastel_test_fill_circles(PastelCanvas* canvas) {
  __fill_bg(canvas, PASTEL_BLACK);

  PastelShaderContextMonochrome context;
  PastelShader shader = {pastel_shader_func_monochrome, &context};

  Vec2i pos;

  context.color = PASTEL_RED;
  pos.x = 0; pos.y = 0;
  pastel_fill_circle(canvas, &pos, canvas->width/2, shader);

  context.color = PASTEL_GREEN;
  pos.x = (canvas->width)/2; pos.y = (canvas->height)/2;
  pastel_fill_circle(canvas, &pos, canvas->width/3, shader);

  context.color = PASTEL_BLUE;
  pos.x = canvas->width; pos.y = canvas->height;
  pastel_fill_circle(canvas, &pos, canvas->height/3, shader);
}

void pastel_test_draw_lines(PastelCanvas* canvas) {
  __fill_bg(canvas, PASTEL_BLACK);

  PastelShaderContextMonochrome context;
  PastelShader shader = {pastel_shader_func_monochrome, &context};

  Vec2i p1, p2;

  //
  // Side lines
  context.color = PASTEL_RED;
  p1.x = 0; p1.y = 0;
  p2.x = 0; p2.y = canvas->height - 1;
  pastel_draw_line(canvas, &p1, &p2, shader);

  p1.x = canvas->width - 1; p1.y = 0;
  p2.x = canvas->width - 1; p2.y = canvas->height - 1;
  pastel_draw_line(canvas, &p1, &p2, shader);

  //
  // Middle lines
  context.color = PASTEL_GREEN;
  p1.x = (canvas->width)/2; p1.y = (canvas->height) - 1;
  p2.x = (canvas->width)/2; p2.y = 0;
  pastel_draw_line(canvas, &p1, &p2, shader);

  p1.x = 0; p1.y = (canvas->height)/2;
  p2.x = canvas->width - 1; p2.y = (canvas->height)/2;
  pastel_draw_line(canvas, &p1, &p2, shader);

  //
  // Diagonal lines
  context.color = PASTEL_BLUE;
  p1.x = 0; p1.y = 0;
  p2.x = (canvas->width)-1; p2.y = (canvas->height)-1;
  pastel_draw_line(canvas, &p1, &p2, shader);

  p1.x = 0; p1.y = (canvas->height)-1;
  p2.x = (canvas->width)-1; p2.y = 0;
  pastel_draw_line(canvas, &p1, &p2, shader);

}

typedef struct {
  Color c1;
  Color c2;
  size_t width;
  size_t height;
} ContextTwoColors;

// @param `context` is a ptr to `ContextTwoColors`
PASTELDEF Color line_shader_func2(int x, int y, void* context) {
  PASTEL_UNUSED(y);
  ContextTwoColors* _context = (ContextTwoColors*) context;
  if (x < (int)_context->width/2) {
    return _context->c1; // red
  }
  return _context->c2; // green
}

// @param `context` is a ptr to `ContextTwoColors`
PASTELDEF Color line_shader_func3(int x, int y, void* context) {
  PASTEL_UNUSED(x);
  ContextTwoColors* _context = (ContextTwoColors*) context;
  if (y < (int)_context->height/2) {
    return _context->c1; // blue
  }
  return _context->c2; // yellow
}

typedef struct {
  Color* colors;
  size_t color_index;
  size_t count;
} ContextLineThreeColors;

// @param `context` is a ptr to `ContextLineThreeColors`
PASTELDEF Color line_shader_func1(int x, int y, void* context) {
  PASTEL_UNUSED(x); PASTEL_UNUSED(y);
  ContextLineThreeColors* _context = (ContextLineThreeColors*)context;
  if (_context->count == 10) {
    _context->color_index++;
    if (_context->color_index > 2) _context->color_index = 0;
    _context->count = 0;
  } else {
    _context->count++;
  }
  return _context->colors[_context->color_index];
}

void pastel_test_draw_lines_with_shaders(PastelCanvas* canvas) {
  __fill_bg(canvas, PASTEL_BLACK);

  Vec2i p1, p2;

  //
  // Middle lines
  Color colors[3] = { PASTEL_RED, PASTEL_GREEN, PASTEL_BLUE };
  ContextLineThreeColors context_middle = {colors, 0, 0};
  PastelShader shader_middle = {line_shader_func1, &context_middle};

  p1.x = canvas->width/2; p1.y = canvas->height-1;
  p2.x = canvas->width/2; p2.y = 0;
  pastel_draw_line(canvas, &p1, &p2, shader_middle);

  p1.x = 0; p1.y = canvas->height/2;
  p2.x = canvas->width-1; p2.y = canvas->height/2;
  pastel_draw_line(canvas, &p1, &p2, shader_middle);

  //
  // Diagonal lines
  ContextTwoColors context_diagonal;
  context_diagonal.width = canvas->width;
  context_diagonal.height = canvas->height;
  PastelShader shader_diagonal = {line_shader_func2, &context_diagonal};

  context_diagonal.c1 = PASTEL_RED; context_diagonal.c2 = PASTEL_GREEN;
  p1.x = 0; p1.y = canvas->height-1;
  p2.x = canvas->width-1; p2.y = 0;
  pastel_draw_line(canvas, &p1, &p2, shader_diagonal);

  context_diagonal.c1 = PASTEL_BLUE; context_diagonal.c2 = PASTEL_YELLOW;
  shader_diagonal.run = line_shader_func3;
  p1.x = 0; p1.y = 0;
  p2.x = canvas->width-1; p2.y = canvas->height-1;
  pastel_draw_line(canvas, &p1, &p2, shader_diagonal);
}

void pastel_test_fill_triangles(PastelCanvas* canvas) {
  __fill_bg(canvas, PASTEL_BLACK);

  PastelShaderContextMonochrome context;
  PastelShader shader = {pastel_shader_func_monochrome, &context};

  Vec2i p1, p2, p3;

  context.color = PASTEL_RED;
  p1.x = 0; p1.y = canvas->height/2;
  p2.x = (canvas->width-1)/2; p2.y = canvas->height-1;
  p3.x = (2*canvas->width)/3; p3.y = 0;
  pastel_fill_triangle(canvas, &p1, &p2, &p3, shader);

  context.color = PASTEL_GREEN;
  p1.x = 0; p1.y = canvas->height/4;
  p2.x = (2*canvas->width)/3; p2.y = (5*canvas->height)/6;
  p3.x = (3*canvas->width)/4; p3.y = (2*canvas->height)/3;
  pastel_fill_triangle(canvas, &p1, &p2, &p3, shader);

  context.color = PASTEL_BLUE;
  p1.x = (2*canvas->width)/3; p1.y = canvas->height/4;
  p2.x = canvas->width-1; p2.y = canvas->height/2;
  p3.x = (4*canvas->width)/5; p3.y = (3*canvas->height)/4;
  pastel_fill_triangle(canvas, &p1, &p2, &p3, shader);
}

void pastel_test_gradientx(PastelCanvas* canvas) {
  PastelShaderContextGradient1D context = { PASTEL_RED, PASTEL_GREEN, 0, canvas->width };
  PastelShader shader = { pastel_shader_func_gradient1dx, &context };
  pastel_fill(canvas, shader);

}

void pastel_test_gradienty(PastelCanvas* canvas) {
  PastelShaderContextGradient1D context = { PASTEL_RED, PASTEL_GREEN, 0, canvas->height };
  PastelShader shader = { pastel_shader_func_gradient1dy, &context };
  pastel_fill(canvas, shader);
}

void pastel_test_alpha_blending(PastelCanvas* canvas) {
  __fill_bg(canvas, PASTEL_WHITE);

  PastelShaderContextMonochrome context;
  PastelShader shader = { pastel_shader_func_monochrome, &context };

  Vec2i pcircle = { canvas->width/3, canvas->height/3 };
  size_t r = { canvas->width/4 } ; 
  context.color = PASTEL_RGBA(255, 0, 0, 120);
  pastel_fill_circle(canvas, &pcircle, r, shader);

  context.color = PASTEL_RGBA(0, 255, 0, 80);
  Vec2i prect = { canvas->width/3, canvas->height/3 };
  Vec2ui dim = { canvas->width/2, canvas->height/2 };
  pastel_fill_rect(canvas, &prect, &dim, shader);
}

#endif // PASTEL_TEST_IMPLEMENTATION
