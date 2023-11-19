run TARGET: (compile)
    ./bin/{{TARGET}}

compile:
    mkdir -p ./bin/
    mkdir -p ./imgs/
    mkdir -p ./test/
    clang example.c -Wall -Wextra -std=c99 -o ./bin/example
    clang test.c -Wall -Wextra -std=c99 -o ./bin/test
