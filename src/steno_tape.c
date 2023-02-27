#include "steno_tape.h"

Tape* steno_tape_init(void) {
  srand(time(NULL));

  Tape *tape = malloc(sizeof(Tape));
  char *home_dir = getenv("HOME");
  char *tape_filepath = malloc(strlen(home_dir) + strlen(TAPE_FILEPATH) + 1);

  if (!tape_filepath || !tape) {
    fprintf(stderr, "Error: memory allocation failed\n");
    exit(1);
  }

  strcpy(tape_filepath, home_dir);
  strcat(tape_filepath, TAPE_FILEPATH);

  tape->filepath = tape_filepath;
  tape->file = fopen(tape_filepath, "a");
  return tape;
}

void steno_tape_error(Tape *tape, const char *message) {
  log_message(tape, ERROR_HEADER, ERROR_EMOJIS, NUM_ERROR_EMOJIS, message);
}

void steno_tape_gaming_mode(Tape *tape) {
  log_message(
    tape,
    GAMING_HEADER,
    GAMING_MODE_EMOJIS,
    NUM_GAMING_MODE_EMOJIS,
    GAMING_MODE_MESSAGE
  );
}

void steno_tape_steno_mode(Tape *tape) {
  log_message(
    tape,
    STENO_HEADER,
    STENO_MODE_EMOJIS,
    NUM_STENO_MODE_EMOJIS,
    STENO_MODE_MESSAGE
  );
}

void steno_tape_mode_unchanged(Tape *tape) {
  steno_tape_error(tape, MODE_UNCHANGED_MESSAGE);
}

void steno_tape_cleanup(Tape *tape) {
  fclose(tape->file);
  free(tape->filepath);
  free(tape);
}

static void log_message(
  Tape *tape,
  const char *header,
  const char* const collection[],
  int num_elements,
  const char *message
) {
  const char *emoji = get_random_collection_element(collection, num_elements);
  const char *tape_entry = build_tape_entry(header, emoji, message);
  fwrite(tape_entry, 1, strlen(tape_entry), tape->file);
}

static const char* get_random_collection_element(
  const char* const collection[],
  int num_elements
) {
  int random_index = rand() % num_elements;
  return collection[random_index];
}

static char* build_tape_entry(
  const char *header,
  const char *emoji,
  const char *message
) {
  static char log_msg[MAX_MESSAGE_LENGTH];
  snprintf(
    log_msg,
    MAX_MESSAGE_LENGTH,
    "%s%s%s%s%s",
    header,
    SEPARATOR,
    emoji,
    SEPARATOR,
    message
  );
  return log_msg;
}
