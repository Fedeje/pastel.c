run TARGET: (compile)
    ./bin/{{TARGET}}

compile:
    mkdir -p ./bin/
    mkdir -p ./imgs/
    mkdir -p ./test/diff
    clang example/example.c -fcolor-diagnostics -I. -Wall -Wextra -ggdb -std=c99 -o ./bin/example
    clang test.c -I. -Wall -Wextra -ggdb -std=c99 -lm -o ./bin/test
    # clang example/triangle.c -I. -Wall -Wextra -Os --target=wasm32 --no-standard-libraries -Wl,--export-all -Wl,--no-entry -Wl,--allow-undefined -o ./bin/triangle.wasm

generate_clangd:
    -rm -f .clangd
    export PASTEL_DIR=$(pwd) && ./generate_clangd.sh
