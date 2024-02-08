#include <errno.h>      // errno
#include <limits.h>     // INT_MAX, INT_MIN
#include <stdio.h>      // printf
#include <stdlib.h>     // strtol
#include <string.h>     // strlen
#include <steno_tape.h> // Tape, steno_tape_*

enum {
  SWITCH_TO_STENO_MODE = 0,
  SWITCH_TO_GAMING_MODE = 1,
  DASH_FORWARD = 2,
  DASH_BACKWARD = 3,
  DASH_LEFT = 4,
  DASH_RIGHT = 5
};

static const char DASH_FORWARD_ENTRY[]  = "|    P         R B G S  | P-RBGS (COMBO: EJKL;) -> /df\n";
static const char DASH_BACKWARD_ENTRY[] = "|     W        R B G S  | W-RBGS (COMBO: DJKL;) -> /db\n";
static const char DASH_LEFT_ENTRY[]     = "|   K          R B G S  | K-RBGS (COMBO: SJKL;) -> /dl\n";
static const char DASH_RIGHT_ENTRY[]    = "|       R      R B G S  | R-RBGS (COMBO: FJKL;) -> /dr\n";

static int parse_arguments(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  int arg = parse_arguments(argc, argv);
  if (arg == -1) {
    return -1;
  }

  Tape *tape = steno_tape_init();
  if (!tape) {
    printf("ERROR: Unable to open log file\n");
    steno_tape_cleanup(tape);
    return -1;
  }

  switch (arg) {
    case SWITCH_TO_STENO_MODE:
      steno_tape_log_steno_mode(tape);
      break;
    case SWITCH_TO_GAMING_MODE:
      steno_tape_log_gaming_mode(tape);
      break;
    case DASH_FORWARD:
      steno_tape_log_entry(tape, DASH_FORWARD_ENTRY);
      break;
    case DASH_BACKWARD:
      steno_tape_log_entry(tape, DASH_BACKWARD_ENTRY);
      break;
    case DASH_LEFT:
      steno_tape_log_entry(tape, DASH_LEFT_ENTRY);
      break;
    case DASH_RIGHT:
      steno_tape_log_entry(tape, DASH_RIGHT_ENTRY);
      break;
    default:
      printf("Unknown argument: %d\n", arg);
      steno_tape_cleanup(tape);
      return -1;
  }

  steno_tape_cleanup(tape);
  return 0;
}

static int parse_arguments(int argc, char *argv[]) {
  // Requires only one argument
  // REF: https://stackoverflow.com/questions/9748393/how-can-i-get-argv-as-int
  if (argc != 2 || strlen(argv[1]) == 0) {
    printf("ERROR: Must provide a single non-empty argument\n");
    return -1;
  }

  char *end_ptr;
  errno = 0;
  // REF: https://devdocs.io/c/string/byte/strtol
  int arg = strtol(argv[1], &end_ptr, 16);

  // Error out if:
  // - an invalid character was found before the end of the string
  // - overflow or underflow errors occurred
  // - number is outside the limited capacity of an int
  if (*end_ptr != '\0' || errno != 0 || arg < INT_MIN || arg > INT_MAX) {
    printf("ERROR: Invalid argument %s\n", argv[1]);
    return -1;
  }

  return arg;
}
