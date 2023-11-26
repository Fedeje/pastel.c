#!/bin/sh
echo "
CompileFlags:
    Add: [-xc,
        -Wall,
        -Wextra,
        -std=c99,
        -I$PASTEL_DIR]
---
If:
    PathMatch: pastel.h
CompileFlags:
    Add: [-DPASTEL_IMPLEMENTATION]
---
If:
    PathMatch: test.h
CompileFlags:
    Add: [-DPASTEL_TEST_IMPLEMENTATION]
---
If:
    PathMatch: pastel_shader_utils.h
CompileFlags:
    Add: [-DPASTEL_SHADER_UTILS_IMPLEMENTATION]
---
If:
    PathMatch: third-party/stb_image.h
CompileFlags:
    Add: [-DSTB_IMAGE_IMPLEMENTATION]
---
If:
    PathMatch: third-party/stb_image_write.h
CompileFlags:
    Add: [-DSTB_IMAGE_WRITE_IMPLEMENTATION]

# Other .clangd example:
# Fragment common to C and C++ source files
# CompileFlags:
#     Add:
#         - "--include-directory=some/directory/to/search/includes/in"
#         - "-D"
#         - "SOME_MACRO_TO_DEFINE"
#         - "-include"
#         - "some/file/to/force/inclusion/of.h"
#
# ---
# Fragment specific to C++ source files
# If:
#     PathExclude: [.*\.c, .*\.h]
# CompileFlags:
#     Add:
#         - "-std=c++17"
" > .clangd
