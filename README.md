# TVP

[![CircleCI](https://circleci.com/gh/venkat24/tvp/tree/master.svg?style=svg&circle-token=0915c0ed7c3b36f5aed8ddcd1b659547c662088c)](https://circleci.com/gh/venkat24/tvp/tree/master)

<h1 align="center">
    <img src="https://github.com/venkat24/tvp/blob/master/images/start.png" width="300"/>
    <img src="https://github.com/venkat24/tvp/blob/master/images/tetris.png" width="300"/><br />
</h1>

This is an incomplete, experimental Nintendo GameBoy emulator written in C++. It runs Tetris!

Build Instructions :

1. Install CMake (> 3.11.1)

2. Install SFML (libsfml-dev) through your distro's package manager, or download / build the library yourself. [https://www.sfml-dev.org/](https://www.sfml-dev.org/)

3. `mkdir build && cd build`

4. `cmake .. -CMAKE_INSTALL_PREFIX=<path_to_someplace_to_install> -DCMAKE_BUILD_TYPE=Release`

5. `make install`
