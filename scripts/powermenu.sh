#!/bin/sh

menu=$(printf "Shutdown\nReboot\nLock" | dmenu -i -p "Power menu: ")
case $menu in
    "Shutdown") shutdown now ;;
    "Reboot") reboot ;;
    "Lock") slock ;;
esac
