# ShadowStack
EECS 573 Project

To build gem5:

cd gem5-stable
scons /build/ALPHA/gem5.opt

To run test cases:
build/ALPHA/gem5.opt configs/example/se.py -c <TEST> --caches --l2cache --cpu-type=detailed
