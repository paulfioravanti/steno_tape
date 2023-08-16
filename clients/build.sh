#!/bin/bash
gcc $(pkg-config --cflags --libs steno_tape) steno_tape_client.c -o steno_tape_client
