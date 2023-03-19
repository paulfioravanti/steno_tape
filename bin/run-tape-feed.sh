#!/bin/bash

readonly TAPE_FILE="$HOME/Library/Application Support/plover/tapey_tape.txt"
readonly SCPT_SCRIPT_PATTERN="{:COMMAND:SHELL:bash -ci 'osascript \\\$STENO_DICTIONARIES/([a-z/]+)/([a-z\-]+\.scpt)(.*)'}(.*)\$"
readonly SCPT_SCRIPT_REPLACEMENT="{:COMMAND: \2\3}\4"
readonly SHELL_SCRIPT_PATTERN="{:COMMAND:SHELL:bash -ci '\\\$STENO_DICTIONARIES/([a-z/]+)/([a-z\-]+\.sh)(.*)'}(.*)\$"
readonly SHELL_SCRIPT_REPLACEMENT="{:COMMAND: \2\3}\4"
readonly SHELL_COMMAND_PATTERN="{:COMMAND:SHELL:bash -ci '(.+)'}\$"
readonly SHELL_COMMAND_REPLACEMENT="{:COMMAND:SHELL: \1}"
readonly URL_SCHEME_PATTERN="http(s)?://(www\.)?"
readonly GOOGLE_CONSOLE_PATTERN="/links\?resource_id[^\"]+"
readonly PLOVER_PATH_PATTERN="\\\$HOME/Library/Application Support/plover/"
readonly GZDOOM_PATH_PATTERN="~/Documents/GZDoom/Typist/"
readonly CLEAR_TAPE_PATTERN="^.+clear-tape(.|\n)+"

main() {
  local filter=false

  parse_args "$@"

  if [[ "$filter" = true ]]; then
    run_filtered_tape_feed
  else
    run_tape_feed
  fi
}

parse_args() {
  local OPTIND opt
  while getopts ":f-:" opt; do
    case $opt in
      f) filter=true;;
      -)
        case $OPTARG in
          filter) filter=true;;
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
    sed -u -E "s#$SCPT_SCRIPT_PATTERN#$SCPT_SCRIPT_REPLACEMENT#" |
    sed -u -E "s#$SHELL_SCRIPT_PATTERN#$SHELL_SCRIPT_REPLACEMENT#" |
    sed -u -E "s#$SHELL_COMMAND_PATTERN#$SHELL_COMMAND_REPLACEMENT#" |
    sed -u -E "s#$URL_SCHEME_PATTERN##" |
    sed -u -E "s#$PLOVER_PATH_PATTERN##" |
    sed -u -E "s#$GZDOOM_PATH_PATTERN##" |
    sed -u -E "s#$GOOGLE_CONSOLE_PATTERN##" |
    sed -u -E "s#$CLEAR_TAPE_PATTERN##"
}

run_tape_feed() {
  less +F "$TAPE_FILE"
}

main "$@"
