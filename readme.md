# Setup
```bash
sudo apt-get install libsdl2-2.0 libsdl2-image-2.0 libsdl2-dev libsdl2-image-dev libxml2 libxml2-dev clang calng-tidy

# run cmake to generate build config and scripts
rm -rf build # if it's there already
mkdir build
cd build
cmake -j3 ..
```

## Enfoce a code style
```bash
# run from inside build folder
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
clang-tidy ../main.c -fix
```

## build & run
```bash
# from inside the build folder
make
./sdl_toy_0
```

## Keys:
```
Move character/hex cursor: up, down, left, right

Run: shift

Toggle HexEditor: tab

Btye Bit Buttons: 1   2   3   4   5   6   7   8
                 128  64  32  16  8   4   2   1

Hex Ops buttons: 9   0   -   =
                Set Add Sub Xor

Memory Address buttons: o p [ ]
                        0 1 2 3
```
