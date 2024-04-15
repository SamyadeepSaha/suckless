#!/bin/sh

# requires dmenu zathura

books_path=$HOME/Downloads/epapers

getbook() {
    find "$books_path" \( -type f -or -type l \) -and -not -path '*/.*' -printf '%P\0'
}

open() {
    # shellcheck disable=SC2039
    local book
    read -r book
    zathura "$books_path/$book"
}

menu() {
    dmenu -i -p "E-Paper:"
}

choice=$(getbook | sort -z | sed -e 's/\x0/\n/g' | menu)

if [ -z "$choice" ]; then
    exit -1
fi

printf "%s" "$choice" | open
