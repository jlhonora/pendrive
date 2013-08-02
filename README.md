Build your own pendrive
=======================

This repo contains all the necessary material to build your own open-source pendrive, including the software source code, the design files and Gerber prints for the Printed Circuit Board and the 3D model of the plastic enclosure. Feel free to modify it as you like!

The microcontroller used is a TI msp430 of the F55xx family (msp430F5510 for now).

To use this library you need to [install mspgcc](https://github.com/jlhonora/mspgcc-install) and the __rake__ utility.

*WARNING*: The available code has only been tested on the [msp-exp430f5529](http://www.ti.com/tool/msp-exp430f5529) board. The PCB files are only an initial draft, and not a single one has been produced/tested.

## Motivation and principles ##

Technological devices that we use nowadays seem to be very far off from what someone could assemble at home. With this project we try to take down that myth, bringing technology and it's manufacturing process closer to normal people. 

The principles on which this project is built are the following:

- Open source code and designs
- Relatively easy soldering/assembly
- Expandable memory
- Small footprint/dimension

## Building the source code ##

First, build the source code:

    $ rake

And install it:

    $ sudo mspdebug rf2500 "prog bin.elf"

## References ##

The code here is heavily based on TI's msp-exp430F5529 User Experience Source Code, released under a BSD license.
