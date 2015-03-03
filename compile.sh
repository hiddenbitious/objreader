#!/bin/bash

CFLAGS="-O3 -lGL -lglut -lGLU -lm -std=c99 -Wno-unused-result"

gcc helpers.c mesh.c objfile.c objmodel.c tgaLoader/tgaLoader.c plyreader/ply.c $CFLAGS -o objmodel
