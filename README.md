# Imagen: Image Generation (or _Imagine_)

This is a collection of visualizations/interactive demos for algorithms that generate pretty-looking images. These are inspired by `OpalMist#4910` on Discord. These demos are all written using [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine), and are written in [C++](https://en.wikipedia.org/wiki/C%2B%2B). The included scripts also compile all of these to WebAssembly using [Emscripten](https://emscripten.org/), and you can find a deployed version [here](https://mustafaquraish.github.io/imagen/).

# Building

## Native

This is only really tested on MacOS as of now, however it should be able to compile on Linux as well, possibly with some tweaks to the `Makefile`. If you're able to make this work anywhere it isn't supported, pull requests are welcome!

Each folder creates it's own executable with the same name:

```bash
$ cd interference
$ make
$ ./interference
```

## WebAssembly

For each folder, the corresponding generated webpage is compiled to `folder/build/`, which you can test by spinning up a local server.

### Using local EMSDK installation:

If you have a local EMSDK toolchain and your environment set up correctly, you ca just invoke the `Makefile`:

```bash
$ cd interference
$ make ems
```

### Using Docker

If you don't have the EMSDK toolchain provided, the included `docker.sh` script at the root of the repo will spin up a docker container with the toolchain and run `make ems` inside it for you. You will need to have [Docker](https://www.docker.com/) installed on your machine.

```bash
$ cd interference
$ ../docker.sh
```