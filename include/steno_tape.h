#include <stdio.h>         // FILE, fclose, fopen, fprintf, fwrite, snprintf
#include <stdlib.h>        // free, getenv, malloc, rand, srand
#include <string.h>        // strcat, strcpy, strlen
#include <time.h>          // time

enum {
  MAX_MESSAGE_LENGTH = 105
};

typedef struct {
  char *filepath;
  FILE *file;
} Tape;

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
