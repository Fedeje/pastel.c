run TARGET: (compile)
    ./bin/{{TARGET}}

compile:
    mkdir -p ./bin/
    mkdir -p ./imgs/
    mkdir -p ./test/diff
    clang example.c -Wall -Wextra -ggdb -std=c99 -o ./bin/example
    clang test.c -Wall -Wextra -ggdb -std=c99 -o ./bin/test
