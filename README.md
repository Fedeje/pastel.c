# PASTEL
<p align="center">
<a href="https://github.com/lmontaut/pastel.c/"><img src="./assets/pastel.png"></a>
</p>

# Dependencies (optional)
Only certain examples use SDL or compile to wasm.
The rest only use `pastel.h`.

## SDL examples
For the examples that require SDL, please install SDL somewhere on your system.
Check below [how to install SDL](#SDL-install).

If you installed SDL locally (under your home, not `/usr/lib`),
please run the following in the terminal:

```console
$ export SDL_LIB=<place-where-sdl-so-is>
$ export SDL_INCLUDE=<place-where-sdl-include-files-are>
```

Alternatively, run:

```console
./generate_profile
```

This will create a `.profile` file which you can edit with the location of SDL.
Then run:

```console
$ source .profile
```

to have these variables defined in your shell session.

## WASM examples
For the wasm example, make sure `wasm-ld` is installed.
On linux run:

```console
$ sudo apt install lld-14
```

if you have `clang-14` installed.
Adapt to your clang version.
On macos, clang should come with its `wasm-ld` by default.

# Compile and run examples
Compile examples and tests:

```console
$ just compile
```

Then run:
```console
$ ./bin/example
$ ./bin/test
```

For the wasm examples:
```console
$ python -m http.server 1234
```
and go to `http://localhost:1234/`.

# SDL install
Full build instructions [here](https://wiki.libsdl.org/SDL2/Installation).
Get a release of SDL [here](https://www.libsdl.org/).
Download it, unpack it somewhere.
If you want to install SDL locally:

```console
$ ./configure --prefix=<where-sdl-lib-and-include-will-be-installed>
$ make install
```

If you want to install it globally, omit the prefix and simply `sudo make install`.

# Dev Pastel.h
You can generate a handy `.clangd` file by running `./generate_clangd`.
This allows the `clangd` LSP to work properly in the project.

# Credit
Inspiration taken from [Tsoding's Olive.c](https://github.com/tsoding/olive.c), notably for stuff related to testing and wasm.
