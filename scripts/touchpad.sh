#!/bin/bash

read touchpad <<< $( xinput | sed -nre '/TouchPad|Touchpad/s/.*id=([0-9]*).*/\1/p' )
state=$( xinput list-props "$touchpad" | grep "Device Enabled" | grep -o "[01]$" )

if [ "$state" -eq '1' ];then
    xinput --disable "$touchpad"
else
    xinput --enable "$touchpad"
fi
