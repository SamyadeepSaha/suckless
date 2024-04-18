#! /bin/bash

# The ‘set -e’ option in bash scripts causes the script to exit
# immediately if any command exits with a non-zero status.
set -e

# Variables
urls_file=$HOME/.cache/bookmarks.csv
lines=$(cat $urls_file | wc -l)
browser=librewolf

#cat $urls_file | dmenu -l $(cat $urls_file | wc -l) | awk '{print $(NF)}' | xargs -I '{}' $BROWSER {} ;;

get_list() {
    cat "$urls_file" | cut --delimiter=, --fields=1
}

get_url() {
    local choice
    read -r choice
    if [ -z "$choice" ]; then
        exit -1
    fi
    grep "$choice" "$urls_file" | cut --delimiter=, --fields=2
}

menu() {
    dmenu -i -p "Go to:"
}

url=$(get_list | menu | get_url)

if [ -z "$url" ]; then
    exit -1
fi

#printf "%s" "$url"
"$browser" "$url"
