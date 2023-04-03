#!/usr/bin/env bash
# Usage: init.sh [options]
#
# SYNOPSIS:
#
# OPTIONS:
#   -i, --init :
#       renames the files and replaces text in code where needed.
#       
#       Example:
#
#       ./init -i myapp
#
#       Initiatlizes a project in the current directory with the following
#       structure.
#
#       .
#       ├── COPYING
#       ├── Makefile
#       └── src
#           ├── C
#           │   ├── main.c
#           │   ├── myapp-application.c
#           │   └── myapp-window.c
#           ├── H
#           │   ├── myapp-application.h
#           │   └── myapp-window.h
#           ├── myapp.gresource.xml
#           └── ui
#               ├── help-overlay.ui
#               └── myapp-window.ui
#
#   -h, --help :
#       prints this help message
#
#   -t, --test :
#       used for testing functions
#
# NOTES:
#

# Data files needed to build project
DATA_ZIP='.gtk4-proj/data.zip'
DATA_TAR='.gtk4-proj/data.tar.gz'

# For fomatting output
BORDER="======================================================================"

print_help_msg() {
  sed -ne '/^#/!q;s/^#$/# /;/^# /s/^# //p' < "$0" |
    awk -v f="$1" '
      f == "-h" && ($1 == "Usage:" || u) {
        u=1
        if ($0 == "") {
          exit
        } else {
          print
        }
      }
      f != "-h"
      '
  exit 0
}

testing() {
  #echo "${FUNCNAME[0]}( $1 ): Invalid argument $2."
  return 0
}

err_exit(){
  echo "${FUNCNAME[0]}: Invalid argument $1"
  exit 1
}

init_completion_msg(){
  local l_func_name=$(echo "${FUNCNAME[0]}" | tr '[:lower:]' '[:upper:]')
  printf "%s\n" "[ $l_func_name ]"
  echo "$BORDER"
  printf "\t%s\n\n" "You can re-init the project by adding the link again with:"
  printf "\t\t%s\n\n" "ln -s .gtk4-proj/init.sh init"
}

print_refactor_info(){
  local l_step r l_file_name
  l_step="$1"
  r="$2"
  l_file_name="$3"

  # Printing some info
  if [[ "$l_step" -eq 0 ]]; then
    printf "\t%s\n" "[ $l_step ][ $r ] : Replacing text in: $l_file_name"
  else
    printf "\t%s\n" "[ $l_step ][ $r ] : Replacing text in: $l_file_name"
  fi
}

refactor(){
  # ${!1} -- var value
  # $1    -- var name
  local l_file_name="${!1}" # var value
  local l_file_type="$1"    # c or h
  local l_proj_name="$2"
  local l_func_name

  # Convert strings to upper case
  l_func_name=$(echo "${FUNCNAME[0]}" | tr '[:lower:]' '[:upper:]')
  l_file_type=$(echo "$l_file_type" | tr '[:lower:]' '[:upper:]')

  # Convert strings to lower case
  #l_file_name=$(echo "$l_file_name" | tr '[:upper:]' '[:lower:]')

  # Replace the text in the temp files (in place)
  local l_step=0
  re=( 'boilerplate' 'Boilerplate' 'boilerplate-' 'BOILERPLATE' )

  #printf "%s\n" "[ $l_func_name ][ $l_file_type ] $BORDER"
  for r in "${re[@]}"; do

    #print_refactor_info "$l_step" "$r" "$l_file_name"

    case $r in
      'Boilerplate')
        # Create a string with the project name first character to upper case
        local l_first_char="$(echo "${l_proj_name:0:1}" | tr '[:lower:]' '[:upper:]')"
        local l_rest_of_string="${l_proj_name:1}"
        local l_first_char_upper_proj_name="$l_first_char$l_rest_of_string"
        # Replacing the text
        sed -i "s/$r/$l_first_char_upper_proj_name/g" "$l_file_name"
        ;;
      'BOILERPLATE')
        # Create a string with the project name all upper case
        local l_proj_name_all_upper_case=$(echo "$l_proj_name" | tr '[:lower:]' '[:upper:]')
        sed -i "s/$r/$l_proj_name_all_upper_case/g" "$l_file_name"
        ;;
      *)
        sed -i "s/$r/$l_proj_name/g" "$l_file_name"
    esac
    l_step=$((l_step+1))

  done
}

has_file(){
  local l_file_exists
  # Warn before overwriting
  if [ -e 'Makefile' ]; then 
    l_file_exits=1
  elif [ -e 'src' ]; then 
    l_file_exits=1
  else
    l_file_exists=0 
  fi

  echo "$l_file_exits"
}

verify_overwrite_msg(){
  local l_response
  printf "\t%s\n" "The current directory has one or more files that will be overwritten."  
  printf "\t%s"   "Continue? (y/n): "
  read -r l_response
  if [ "$l_response" == 'n' ] || [ "$l_response" == 'N' ]; then
    exit 0
  fi
  echo
}

### File structure Reference 
# .
# ├── COPYING
# ├── Makefile
# └── src
#     ├── C
#     │   ├── main.c
#     │   ├── myapp-application.c
#     │   └── myapp-window.c
#     ├── H
#     │   ├── myapp-application.h
#     │   └── myapp-window.h
#     ├── myapp.gresource.xml
#     └── ui
#         ├── help-overlay.ui
#         └── myapp-window.ui
###
  
init() {
  # Format and print out msg
  local l_func_name=$(echo "${FUNCNAME[0]}" | tr '[:lower:]' '[:upper:]')
  printf "%s\n" "[ $l_func_name ]"
  echo "$BORDER"

  if [ -z "$1" ]; then
    echo 'no arg 2'
    exit 1
  fi
  
  # SEE: May exit here
  if [[ "$(has_file)" -eq 1 ]]; then
    verify_overwrite_msg
  fi

  # Confirm the project name
  local l_response l_proj_name
  l_proj_name="$1"
  printf "\t%s\n" "Initializing the project as: [ $l_proj_name ]"
  printf "\t%s"   "Continue? (y/n): "
  read -r l_response
  echo

  # SEE: May exit here
  if [ "$l_response" != 'y' ]; then
    exit 0
  fi

  # Copy the data file into the current directy
  #cp "$DATA_ZIP" ./ || exit 0 
  cp "$DATA_TAR" ./ || exit 0 

  # Unzip the data files
  #unzip -qq "$DATA_ZIP" || exit 0
  tar -xf "$DATA_TAR" || exit 0
  local l_data_dir='data'

  # Set the src dir
  local l_src_dir=$(readlink -f "$l_data_dir/src") 

  # Initiatlize array to store .c and .h files in the directory
  local l_c=()
  local l_h=()

  # Use find command to locate .c files in the directory
  while IFS= read -d $'\0' file; do
      if [[ $file == *.c ]]; then
          l_c+=("$file")
      fi
  done < <(find "$l_src_dir" -name "*.c" -print0)
  
  # Use find command to locate .h files in the directory
  while IFS= read -d $'\0' file; do
      if [[ $file == *.h ]]; then
          l_h+=("$file")
      fi
  done < <(find "$l_src_dir" -name "*.h" -print0)

  # Loop c files and replace text
  for c in "${l_c[@]}"; do
    local l_base_name=$(basename "$c")
    if [ "$l_base_name" != "resources.c" ]; then
      refactor c "$l_proj_name" 
    fi
  done

  # Loop h files and replace text
  for h in "${l_h[@]}"; do
    refactor h "$l_proj_name" 
  done

  # Replace text in the Makefile (1 word at the top)
  local l_tmp_mkfile="$l_data_dir/Makefile"
  sed -i "s/boilerplate/$l_proj_name/g" "$l_tmp_mkfile"
 
  # Set the first char of the project name to upper case
  l_first_char="$(echo "${l_proj_name:0:1}" | tr '[:lower:]' '[:upper:]')"
  l_rest_of_string="${l_proj_name:1}"
  l_first_char_upper_proj_name="$l_first_char$l_rest_of_string"

  # Replace text in the boilerplate-window.ui (line 4)
  local l_window_ui="$l_src_dir/ui/boilerplate-window.ui"
  sed -i "s/Boilerplate/$l_first_char_upper_proj_name/g" "$l_window_ui"

  # Replace text in the boilerplate.gresource.xml
  # <gresource prefix="/org/gnome/Boilerplate">
  # <file preprocess="xml-stripblanks">boilerplate-window.ui</file>
  local l_gresource="$l_src_dir/boilerplate.gresource.xml"
  sed -i "s/Boilerplate/$l_first_char_upper_proj_name/g" "$l_gresource"
  sed -i "s/boilerplate/$l_proj_name/g" "$l_gresource"

  # Rename files
  # --- C files
  mv "$l_src_dir/C/boilerplate-application.c" "$l_src_dir/C/$l_proj_name-application.c" || exit 1
  mv "$l_src_dir/C/boilerplate-window.c" "$l_src_dir/C/$l_proj_name-window.c" || exit 1

  # --- H files
  mv "$l_src_dir/H/boilerplate-application.h" "$l_src_dir/H/$l_proj_name-application.h" || exit 1
  mv "$l_src_dir/H/boilerplate-window.h" "$l_src_dir/H/$l_proj_name-window.h" || exit 1

  # --- xml
  mv "$l_src_dir/boilerplate.gresource.xml" "$l_src_dir/$l_proj_name.gresource.xml" || exit 1

  # --- ui
  mv "$l_src_dir/ui/boilerplate-window.ui" "$l_src_dir/ui/$l_proj_name-window.ui" || exit 1

  # Copy the data files into current directory
  #cp -r "$l_data_dir"/* ./
  cp -r "$l_data_dir"/* ./ || exit 1

  # Clean up the data files
  rm -r data*

  # Remove the init sym-link
  # rm -i ./init
  init_completion_msg
}


if [[ $# -lt 1 ]]; then print_help_msg "$0" "$1"; fi # No Args

# Handle options
case "$1" in
  "-h" | "--help")     print_help_msg;;
  "-t" | "--test")     testing;;
  "-i" | "--init")     init "$2";;
  *) err_exit "$1";;
esac


