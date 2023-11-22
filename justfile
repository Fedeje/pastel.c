run TARGET: (compile)
    ./bin/{{TARGET}}

compile:
    mkdir -p ./bin/
    mkdir -p ./imgs/
    mkdir -p ./test/diff
    clang example/example.c -fcolor-diagnostics -I. -Wall -Wextra -ggdb -std=c99 -o ./bin/example
    clang example/misc1.c -I. -Wall -Wextra -O3 -std=c99 -o ./bin/misc1
    clang example/misc2.c -I. -Wall -Wextra -O3 -std=c99 -o ./bin/misc2
    clang test.c -Wall -Wextra -ggdb -std=c99 -I. -o ./bin/test
    /opt/homebrew/opt/llvm/bin/clang example/triangle.c -I. -Wall -Wextra -Os --target=wasm32 --no-standard-libraries -Wl,--export-all -Wl,--no-entry -Wl,--allow-undefined -o ./bin/triangle.wasm
    clang test.c -I. -Wall -Wextra -ggdb -std=c99 -lm -o ./bin/test
