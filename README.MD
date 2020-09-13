# CMessagePack

## Overview

Object orientated messagepack serializer and deserializer. You can easily serialize and deserialize your objects, arrays, pointers, smart pointers and STL containers to and from messagepack. All primitive, array and STL container types, except sets are supported. To use this library your compiler need to support at least C++11.

## How to use the library?

You need to add the MessagePack.hpp to your include paths.

## How to build the example project?

First you need cmake from https://cmake.org/.
After you have installed cmake you need to create a build directory inside this directory.
Open a terminal and navigate to the build directory, alternative you can use the cmake-gui, but I will describe the terminal version. Since cmake supports many build system I used in this example `ninja`, but you can use whatever you want.

```bash
cmake .. -G "Ninja"
ninja -j4
```

Now you can execute the compiled project.

## License

This library is under the [MIT License](LICENSE)
