#!/bin/bash

path=$HOME/Pictures/screenshots/

declare -A options
options+=(
[Full]="maim"
[Selected]="maim -s"
[Window]="maim -i "$(xdotool getactivewindow)""
)

choice=$(printf "%s\n" "${!options[@]}" | dmenu -i -p Screenshot)
action=$(printf "Clipboard\nSave" | dmenu -i)

case $action in
    "Clipboard")
        ${options[$choice]} | xclip -selection clipboard
        exit 0
        ;;
    "Save")
        ${options[$choice]} $path$(printf "screenshot" | dmenu -p "Save to")
        exit 0
        ;;
esac
