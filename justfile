run TARGET: (compile TARGET)
    ./bin/{{TARGET}}

compile TARGET:
    clang {{TARGET}}.c -Wall -Wextra -std=c99 -o ./bin/{{TARGET}}
