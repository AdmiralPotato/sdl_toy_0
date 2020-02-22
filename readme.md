# Setup
```bash
sudo apt-get install libsdl2-2.0 libsdl2-image-2.0 libsdl2-dev libsdl2-image-dev

cmake .
```

## Enfoce a code style
```
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .
clang-tidy main.c -fix
```

## build & run
```bash
make
./sdl_toy_0
```