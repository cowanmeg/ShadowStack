#!/bin/bash

build/ALPHA/gem5.opt --debug-file=out.txt --debug-flags=Ras configs/example/se.py -c tests/test-progs/hello/bin/alpha/linux/hello --caches --l2cache --cpu-type=detailed
