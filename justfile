run TARGET: (compile TARGET)
    ./bin/{{TARGET}}

compile TARGET:
    mkdir -p ./bin/
    mkdir -p ./outputs/
    clang {{TARGET}}.c -Wall -Wextra -std=c99 -o ./bin/{{TARGET}}
