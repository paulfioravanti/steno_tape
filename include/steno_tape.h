#pragma once
#ifndef STENO_TAPE_H
#define STENO_TAPE_H

#include <stdio.h> // FILE

typedef struct tape {
  char *filepath;
  FILE *file;
} Tape;

enum {
  MAX_MESSAGE_LENGTH
};

Tape* steno_tape_init(void);
void steno_tape_error(Tape *tape, const char *message);
void steno_tape_gaming_mode(Tape *tape);
void steno_tape_steno_mode(Tape *tape);
void steno_tape_mode_unchanged(Tape *tape);
void steno_tape_custom_entry(Tape *tape, const char *entry);
void steno_tape_cleanup(Tape *tape);

static void log_message(
  Tape *tape,
  const char *header,
  const char* const collection[],
  int num_elements,
  const char *message
);
static const char* get_random_collection_element(
  const char* const collection[],
  int num_elements
);
static char* build_tape_entry(
  const char *header,
  const char *emoji,
  const char *message
);
static void log_tape_entry(Tape *tape, const char *tape_entry);

#endif
