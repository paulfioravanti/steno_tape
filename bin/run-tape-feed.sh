#!/bin/bash

readonly TAPE_FILE="$HOME/Library/Application Support/plover/tapey_tape.txt"
readonly SCRIPT_PATTERN="{:COMMAND:SHELL:bash -ci '(osascript )?\\\$STENO_DICTIONARIES/([a-z/]+)/([a-z\-]+\.(scpt|sh))(.*)'}(.*)\$"
readonly SCRIPT_REPLACEMENT="{:COMMAND:SHELL: \3\5}\6"
readonly APPLESCRIPT_PATTERN="{:COMMAND:APPLESCRIPT:\\\$STENO_DICTIONARIES/([a-z/]+)/([a-z\-]+\.scpt)}(.*)\$"
readonly APPLESCRIPT_REPLACEMENT="{:COMMAND:APPLESCRIPT: \2}\3"
readonly URL_SCHEME_PATTERN="http(s)?://(www\.)?"
readonly GOOGLE_CONSOLE_PATTERN="/links\?resource_id[^\"]+"
readonly PLOVER_PATH_PATTERN="\\\$HOME/Library/Application Support/plover/"
readonly GZDOOM_PATH_PATTERN="\\\$HOME/Documents/GZDoom/Typist/"
readonly CLEAR_TAPE_PATTERN="^.+clear-tape(.|\n)+"
readonly PLOVER_FORMATTING_PATTERN="({\^\^}|{MODE:RESET})"
readonly PLOVER_NEWLINE_PATTERN="{\^~\|\\\n\^}"
readonly PLOVER_NEWLINE_REPLACEMENT="\\\n"
readonly GZDOOM_DASH_PATTERN="{\#CONTROL\(BACKSPACE\)}{\^(/d[bflr])}{\#RETURN}"
readonly GZDOOM_DASH_REPLACEMENT="\1"

main() {
  local filter=false
  local game=false

  parse_args "$@"

  if [[ "$filter" = true && "$game" = true ]]; then
    run_filtered_game_tape_feed
  elif [[ "$filter" = true ]]; then
    run_filtered_tape_feed
  else
    run_tape_feed
  fi
}

parse_args() {
  local OPTIND opt
  while getopts ":fg-:" opt; do
    case $opt in
      f) filter=true;;
      g) game=true;;
      -)
        case $OPTARG in
          filter) filter=true;;
          game) game=true;;
          *)
            echo "Unknown option --$OPTARG"
            exit 1
            ;;
        esac
        ;;
      \?)
        echo "Unknown option -$OPTARG"
        exit 1
        ;;
    esac
  done
  shift $((OPTIND - 1))
}

run_filtered_tape_feed() {
  # NOTE: Unbuffered flag (-u) needed for all `sed` commands to force flush
  # their buffers otherwise no log gets output.
  tail --lines=500 -f "$TAPE_FILE" |
    sed -u -E "s#$SCRIPT_PATTERN#$SCRIPT_REPLACEMENT#" |
    sed -u -E "s#$APPLESCRIPT_PATTERN#$APPLESCRIPT_REPLACEMENT#" |
    sed -u -E "s#$URL_SCHEME_PATTERN##" |
    sed -u -E "s#$PLOVER_PATH_PATTERN##" |
    sed -u -E "s#$GZDOOM_PATH_PATTERN##" |
    sed -u -E "s#$GOOGLE_CONSOLE_PATTERN##" |
    sed -u -E "s#$CLEAR_TAPE_PATTERN##"
}

run_filtered_game_tape_feed() {
  run_filtered_tape_feed |
    sed -u -E "s#$PLOVER_FORMATTING_PATTERN##" |
    sed -u -E "s#$PLOVER_NEWLINE_PATTERN#$PLOVER_NEWLINE_REPLACEMENT#" |
    sed -u -E "s#$GZDOOM_DASH_PATTERN#$GZDOOM_DASH_REPLACEMENT#"
}

run_tape_feed() {
  less +F "$TAPE_FILE"
}

main "$@"
