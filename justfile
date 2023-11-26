run TARGET: (compile)
    ./bin/{{TARGET}}

compile +FLAGS="":
    mkdir -p ./bin/
    mkdir -p ./imgs/
    mkdir -p ./test/diff
    -
    clang example/example.c -fcolor-diagnostics -I. -Wall -Wextra -std=c99 {{FLAGS}} -o ./bin/example
    -
    clang test.c -fcolor-diagnostics -I. -Wall -Wextra -std=c99 -lm {{FLAGS}} -o ./bin/test
    -
    clang example/wasm_triangle.c -I. -Wall -Wextra -Os --target=wasm32 --no-standard-libraries -Wl,--export-all -Wl,--no-entry -Wl,--allow-undefined -o ./bin/triangle.wasm
    -
    clang example/triangle.c -fcolor-diagnostics -I. -I$SDL_INCLUDE -L$SDL_LIB -Wl,-rpath -Wl,$SDL_LIB -lSDL2 -lm -Wall -Wextra -std=c99 {{FLAGS}} -o ./bin/triangle

delete_examples:
    rm -f imgs/*.png

generate_clangd:
    -rm -f .clangd
    export PASTEL_DIR=$(pwd) && ./generate_clangd.sh

