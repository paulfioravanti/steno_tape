#!/bin/bash
gcc $(pkg-config --cflags --libs steno_tape) custom_tape_entry.c -o custom_tape_entry
