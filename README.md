# PASTEL
Credit goes to [Tsoding's Olive.c](https://github.com/tsoding/olive.c).

# Install
For the example that require SDL, please install SDL somewhere on your system.
Check below [how to install SDL](#SDL-install).

If you installed SDL locally (under your home, not `/usr/lib`),
please run the following in the terminal:
- `export SDL_LIB=<place-where-sdl-so-is>`
- `export SDL_INCLUDE=<place-where-sdl-include-files-are>`
Alternatively, run `./generate_profile`.
This will create a `.profile` file which you can edit with the location of SDL.
Then run `source .profile` to have these variables defined in your shell session.

# SDL install
Get a release of SDL [here](https://www.libsdl.org/).
Download it, unpack it somewhere.
If you want to install SDL locally:
- `./configure --prefix=<where-sdl-lib-and-include-will-be-installed>`
- `make install`
If you want to install it globally, omit the prefix and simply `sudo make install`.

# Run
- Compile examples and tests: `just compile`
- Executables: `./bin/example` and `./bin/test`

# Dev Pastel.h
You can generate a handy `.clangd` file by running `./generate_clangd`.
This allows the `clangd` LSP to work properly in the project.
