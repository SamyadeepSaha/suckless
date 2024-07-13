#!/bin/bash

screen_res=1366x768
frame_rate=30
path=$HOME/Videos/records/
output=$path$(date "+%s").mp4



ffmpeg -y \
    -video_size "$screen_res" \
    -framerate "$frame_rate" \
    -f x11grab \
    -i :0.0 \
    -pix_fmt yuv420p
    -c:v libx264 \
    -crf 0 -preset ultrafast \
    -c:a aac
    "$output"
