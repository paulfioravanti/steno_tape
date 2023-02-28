#!/bin/bash

readonly TAPE_FILE="$HOME/Library/Application Support/plover/tapey_tape.txt"
readonly DASH_BACKWARD="COMBO |     W        R B G S  | W-RBGS (D,J,K,L,;) -> {#CONTROL(BACKSPACE)}{^/db}{#RETURN}"
readonly DASH_FORWARD="COMBO |    P         R B G S  | P-RBGS (E,J,K,L,;) -> {#CONTROL(BACKSPACE)}{^/df}{#RETURN}"
readonly DASH_LEFT="COMBO |   K          R B G S  | K-RBGS (S,J,K,L,;) -> {#CONTROL(BACKSPACE)}{^/dl}{#RETURN}"
readonly DASH_RIGHT="COMBO |       R      R B G S  | R-RBGS (F,J,K,L,;) -> {#CONTROL(BACKSPACE)}{^/dr}{#RETURN}"

main() {
  local tape_entry=""
  parse_arg "$@"
  echo "$tape_entry" >> "$TAPE_FILE"
}

parse_arg() {
  case "$1" in
    "DASH_BACKWARD")
      tape_entry="$DASH_BACKWARD";;
    "DASH_FORWARD")
      tape_entry="$DASH_FORWARD";;
    "DASH_LEFT")
      tape_entry="$DASH_LEFT";;
    "DASH_RIGHT")
      tape_entry="$DASH_RIGHT";;
    *)
      echo "Unknown tape entry: $1"
      exit 1
      ;;
  esac
}

main "$@"
