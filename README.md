# GPS Library

A small GPS library written in C and designed for use in embedded systems. The goal of this project is to produce a library for both decoding and encoding [NMEA 0183](https://en.wikipedia.org/wiki/NMEA_0183) sentences quickly on systems which lack a floating point coprocessor. This library was also designed to be fast and safe. It's fast in the sense that the decoder functions try and execute as few instructions as possible when parsing a NMEA sentence. It's safe in the sense that all received input is validated against some hard-coded regular expressions.

## Features

Here are the major features which this library provides.

### Decoder

The decoder is the main feature of this library. It takes in a complete NMEA sentence (dollar sign, checksum, CRLF, and all) and decodes the sentence into a time-position-velocity record. You may then check to see if the values stored are valid and, if they are, you may proceed to make use of the data.

### Encoder

This is a utility function. Use it to compose commands intended for sending to a GPS device as valid NMEA sentences.

## Usage

The GPS library is composed of only two files: `gps.c` and `gps.h`. The recommended way to use this library is to copy `gps.c` and `gps.h` directly into your project (aka vendoring).

## Build

> [!NOTE]
> Building the documentation requires [Doxygen](https://www.doxygen.nl/index.html) be preinstalled on your system.

For users who want to generate a static or shared libgps library, API documentation, example programs, or unit tests, then use [Conan](https://conan.io/) and [CMake](https://cmake.org/).

The following commands will build everything in release mode:

```sh
conan install
conan build
```

You can then find artifacts under the `build` directory. For example, you can find the documentation output at `build/html` and the unit test executable at `build/test/Release/tests`. Note, use `build/test/Release/tests '[!benchmark]'` to run benchmarks.

## Simple API

The entire public API is made up of only four functions:

* gps_init_tpv()
* gps_encode()
* gps_decode()
* gps_error_string()

Please generate and refer to the Doxygen documentation for more detailed information on how to use the API.

## Embedded System Notes

This code was written with embedded systems in mind. There is no use of dynamic memory (i.e. malloc) in this library. The only external dependancy which this library has is on the C standard library. All real number data values stored in this library are scaled up by some factor to maintain a certain level of accuracy. This way, the use of floating point data types is avoided.
