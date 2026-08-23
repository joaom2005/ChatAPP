# Chat app (VERY W.I.P.)

## Description
This is (going to be) a chat application completely written from scratch, with as little dependencies necessaries.

Currently needed dependencies:
- GLAD - Needed to load OpenGL functions.
- GLM - Header only library for math functions and structures.
- STB - Font loading

## How to build
The project is made in CMake. As such, to make the build files, run:

`cmake -S . -B build -DCMAKE_BUILD_TYPE=Release` 

And to build the project run:

`cmake --build build`

You can also just run a editor that can use CMake, such as VS Code with the CMake extension.