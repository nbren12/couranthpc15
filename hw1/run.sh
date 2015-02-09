#!/usr/bin/env zsh
CC=gcc
CFLAGS=-lm

${CC} ${CFLAGS} -o laplace laplace.c
${CC} ${CFLAGS} -O0 -o laplace0 laplace.c 
${CC} ${CFLAGS} -O3 -o laplace3 laplace.c 



N=100
time ./laplace $N > /dev/null
time ./laplace0 $N >/dev/null
time ./laplace3 $N > /dev/null
