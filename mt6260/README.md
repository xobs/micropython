Micro Python port to Fernvale
============================

This is a port of Micro Python to the Kosagi Fernvale MT6260 module.

Currently implemented features include:
- REPL (Python prompt) over USB serial.
- 24k heap RAM available for Python code.
- Garbage collector, exceptions.
- Unicode support.
- Builtin modules: gc, array, collections, io, struct, sys.
- C long-long type used as bignum implementation (gives 64 signed ints).

Note that floating-point numbers are not supported.

On the TODO list:
- GPIO support.
- Internal filesystem using the flash.
- ...

Build instructions
------------------

If you're not building on ARM, set CROSS_COMPILE to match your toolchain's
compiler prefix (e.g. arm-none-linux-gnueabi-).  Then type "make".

To load onto a Fernvale board, use fernly-usb-loader.  E.g.:

 ./build/fernly-usb-loader -s \
     /dev/fernvale \
     ./build/usb-loader.bin \
     ./build/firmware.bin \
     ../micropython/mt6260/build/firmware.bin 
