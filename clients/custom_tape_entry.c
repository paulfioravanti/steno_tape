#include <steno_tape.h>    // steno_tape_cleanup, steno_tape_custom_entry,
                           // steno_tape_init, Tape

int main(int argc, char *argv[]) {
  Tape *tape = steno_tape_init();
  if (!tape) {
    printf("ERROR: Unable to open log file\n");
    steno_tape_cleanup(tape);
    return -1;
  }

  steno_tape_custom_entry(tape, argv[1]);

  steno_tape_cleanup(tape);
  return 0;
}
