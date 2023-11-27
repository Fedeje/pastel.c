#!/bin/sh
echo "\
#!/bin/sh
# Please fill these two lines:
SDL_LIB_PATH=""
SDL_INCLUDE_PATH=""

export SDL_LIB=\$SDL_LIB_PATH
export SDL_INCLUDE=\$SDL_INCLUDE_PATH
export PASTEL_DIR=(\$PWD)

# if [ -z \"\$SDL_LIB\" ]; then export SDL_LIB=\$SDL_LIB_PATH; fi
# if [ -z \"\$SDL_INCLUDE\" ]; then export SDL_INCLUDE=\$SDL_INCLUDE_PATH; fi

echo \"SDL_LIB=\$SDL_LIB\"
echo \"SDL_INCLUDE=\$SDL_INCLUDE\"
echo \"PASTEL_DIR=\$PASTEL_DIR\"
" > .profile
