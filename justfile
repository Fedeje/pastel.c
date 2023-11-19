run TARGET: (compile)
    ./bin/{{TARGET}}

compile:
    mkdir -p ./bin/
    mkdir -p ./imgs/
    mkdir -p ./test/diff
    clang example.c -Wall -Wextra -O3 -std=c99 -o ./bin/example
    clang misc1.c -Wall -Wextra -O0 -std=c99 -o ./bin/misc1
    clang misc2.c -Wall -Wextra -O0 -std=c99 -o ./bin/misc2
    clang test.c -Wall -Wextra -ggdb -std=c99 -o ./bin/test
