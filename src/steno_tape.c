#include <stdio.h>         // fclose, fopen, fprintf, fwrite, snprintf
#include <stdlib.h>        // free, getenv, malloc, rand, srand
#include <string.h>        // strcat, strcpy, strlen
#include <time.h>          // time
#include "steno_tape.h"

enum {
  MAX_MESSAGE_LENGTH = 105
};

// REF: https://github.com/rabbitgrowth/plover-tapey-tape
static const char TAPE_FILEPATH[] =
  "/Library/Application Support/plover/tapey_tape.txt";
static const char SEPARATOR[] = "|";

static const char STENO_HEADER[] = "STENO ";
// REF: http://kaomoji.ru/en/
static const char * const STENO_MODE_EMOJIS[] = {
  "⌨️ ｷﾀ━━━━━(ﾟ∀ﾟ)━━━━━!!⌨️ ",
  "⌨️ ☆*:.｡.o(≧▽≦)o.｡.:*☆⌨️ ",
  "⌨️ (￣(￣(￣▽￣)￣)￣)⌨️ ",
  "⌨️  (☞°ヮ°)☞ ☜(°ヮ°☜) ⌨️ ",
  "⌨️ ﾉ≧∀≦)ﾉ ‥…━━━━━━━━━★⌨️ ",
  "⌨️ (ﾉ>ω<)ﾉ’★,｡･:*:･ﾟ’☆⌨️ ",
  "⌨️ (ノ°∀°)ノ⌒.｡.:*゜*☆⌨️ ",
  "⌨️ ╰( ͡° ͜ʖ ͡° )つ─☆*:・ﾟ⌨️ ",
};
static const int NUM_STENO_MODE_EMOJIS =
  sizeof(STENO_MODE_EMOJIS) / sizeof(STENO_MODE_EMOJIS[0]);
static const char STENO_MODE_MESSAGE[] = " STENO mode activated!\n";

static const char GAMING_HEADER[] = "GAMING";
// REF: http://kaomoji.ru/en/
static const char * const GAMING_MODE_EMOJIS[] = {
  "🎮(❁´ω`❁)　✧٩(ˊωˋ*)و✧🎮",
  "🎮 ヽ( ⌒o⌒)人(⌒-⌒ )ﾉ 🎮",
  "🎮ヽ( ⌒ω⌒)人(=^‥^= )ﾉ🎮",
  "🎮ヽ(≧◡≦)八(o^ ^o)ノ 🎮",
  "🎮(*・∀・)爻(・∀・*) 🎮",
  "🎮 (っ˘▽˘)(˘▽˘)˘▽˘ς) 🎮",
  "🎮((*°▽°*)八(*°▽°*)) 🎮",
  "🎮(*＾ω＾)人(＾ω＾*) 🎮",
  "🎮 ٩(๑･ิᴗ･ิ)۶٩(･ิᴗ･ิ๑)۶  🎮",
};
static const int NUM_GAMING_MODE_EMOJIS =
  sizeof(GAMING_MODE_EMOJIS) / sizeof(GAMING_MODE_EMOJIS[0]);
static const char GAMING_MODE_MESSAGE[] = " GAMING mode activated!\n";

static const char ERROR_HEADER[] = "ERROR ";
// REF: http://kaomoji.ru/en/
static const char * const ERROR_EMOJIS[] = {
  "💢     ლ(ಠ_ಠ ლ)      💢",
  "💢  ( ╯°□°)╯ ┻━━┻    💢",
  "💢     (＃`Д´)       💢",
  "💢  (╯°益°)╯彡┻━┻    💢",
  "💢     (￣ω￣;)      💢",
  "💢     ლ(¯ロ¯ლ)      💢",
  "💢     (￢_￢)       💢",
  "💢.｡･ﾟﾟ･(＞_＜)･ﾟﾟ･｡.💢",
  "💢     Σ(▼□▼メ)      💢",
  "💢    ٩(╬ʘ益ʘ╬)۶     💢",
  "💢   ୧((#Φ益Φ#))୨    💢",
  "💢    ლ(¯ロ¯\"ლ)      💢",
};
static const int NUM_ERROR_EMOJIS =
  sizeof(ERROR_EMOJIS) / sizeof(ERROR_EMOJIS[0]);
static const char MODE_UNCHANGED_MESSAGE[] =
  " Attempted mode change unsuccessful!\n";

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

void steno_tape_custom_entry(Tape *tape, const char *entry) {
  if (strcmp(entry, "GAMING_MODE") == 0) {
    steno_tape_gaming_mode(tape);
    return;
  }

  if (strcmp(entry, "STENO_MODE") == 0) {
    steno_tape_steno_mode(tape);
    return;
  }

  for (int i = 0; i < NUM_CUSTOM_ACTIONS; i++) {
    if (strcmp(entry, CUSTOM_ACTIONS[i]) == 0) {
      log_tape_entry(tape, CUSTOM_ACTION_TAPE_ENTRIES[i]);
      return;
    }
  }

  printf("Unknown entry: %s\n", entry);
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
  log_tape_entry(tape, tape_entry);
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

static void log_tape_entry(Tape *tape, const char *tape_entry) {
  fwrite(tape_entry, 1, strlen(tape_entry), tape->file);
}
