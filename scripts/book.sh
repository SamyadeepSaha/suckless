#!/bin/sh

# requires rofi zathura

books_path=$HOME/Desktop/books

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
    dmenu -i -l 10 -p "Book:" \
        -fn UbuntuMono:size=12:antialias=true \
        -nb black -nf white -sb black -sf cyan
}

choice=$(getbook | sort -z | sed -e 's/\x0/\n/g' | menu)

if [ -z "$choice" ]; then
    exit -1
fi

printf "%s" "$choice" | open
