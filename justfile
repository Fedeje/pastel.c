run TARGET: (compile)
    ./bin/{{TARGET}}

compile:
    mkdir -p ./bin/
    mkdir -p ./imgs/
    mkdir -p ./test/diff
    clang example/example.c -Wall -Wextra -ggdb -std=c99 -I. -o ./bin/example
    clang example/misc1.c -Wall -Wextra -O3 -std=c99 -I. -o ./bin/misc1
    clang example/misc2.c -Wall -Wextra -O3 -std=c99 -I. -o ./bin/misc2
    clang test.c -Wall -Wextra -ggdb -std=c99 -I. -o ./bin/test
