#include <string.h>        // strcmp
#include <steno_tape.h>    // steno_tape_cleanup, steno_tape_init,
                           // steno_tape_log_entry, steno_tape_log_gaming_mode,
                           // steno_tape_log_steno_mode, Tape

static const char * const CUSTOM_ACTIONS[] = {
  "DASH_BACKWARD",
  "DASH_FORWARD",
  "DASH_LEFT",
  "DASH_RIGHT"
};
static const char * const CUSTOM_ACTION_TAPE_ENTRIES[] = {
  "COMBO |     W        R B G S  | W-RBGS (D,J,K,L,;) -> {#CONTROL(BACKSPACE)}{^/db}{#RETURN}\n",
  "COMBO |    P         R B G S  | P-RBGS (E,J,K,L,;) -> {#CONTROL(BACKSPACE)}{^/df}{#RETURN}\n",
  "COMBO |   K          R B G S  | K-RBGS (S,J,K,L,;) -> {#CONTROL(BACKSPACE)}{^/dl}{#RETURN}\n",
  "COMBO |       R      R B G S  | R-RBGS (F,J,K,L,;) -> {#CONTROL(BACKSPACE)}{^/dr}{#RETURN}\n"
};
static const int NUM_CUSTOM_ACTIONS =
  sizeof(CUSTOM_ACTIONS) / sizeof(CUSTOM_ACTIONS[0]);

int main(int argc, char *argv[]) {
  Tape *tape = steno_tape_init();
  if (!tape) {
    printf("ERROR: Unable to open log file\n");
    steno_tape_cleanup(tape);
    return -1;
  }

  const char *action = argv[1];
  int known_action = 0;

  if (strcmp(action, "GAMING_MODE") == 0) {
    known_action = 1;
    steno_tape_log_gaming_mode(tape);
  } else if (strcmp(action, "STENO_MODE") == 0) {
    known_action = 1;
    steno_tape_log_steno_mode(tape);
  } else {
    for (int i = 0; i < NUM_CUSTOM_ACTIONS; i++) {
      if (strcmp(action, CUSTOM_ACTIONS[i]) == 0) {
        known_action = 1;
        steno_tape_log_entry(tape, CUSTOM_ACTION_TAPE_ENTRIES[i]);
      }
    }
  }

  if (!known_action) {
    printf("Unknown action: %s\n", action);
    steno_tape_cleanup(tape);
    return -1;
  }

  steno_tape_cleanup(tape);
  return 0;
}
