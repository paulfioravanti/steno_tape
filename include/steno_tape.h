#pragma once
#ifndef STENO_TAPE_H
#define STENO_TAPE_H

#include <stdio.h> // FILE

typedef struct tape {
  char *filepath;
  FILE *file;
} Tape;

enum {
  STENO_TAPE_ENTRY_MAX_LENGTH = 105
};

Tape* steno_tape_init(void);
void steno_tape_log_steno_mode(Tape *tape);
void steno_tape_log_gaming_mode(Tape *tape);
void steno_tape_log_mode_unchanged(Tape *tape);
void steno_tape_log_entry(Tape *tape, const char *entry);
void steno_tape_log_error(Tape *tape, const char *entry);
void steno_tape_cleanup(Tape *tape);

#endif
