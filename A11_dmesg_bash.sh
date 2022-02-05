bas#!/bin/bash

# actively read dmesg and search for specific messages

dmesg -w | while read -r line; do

        if [[ $(echo "$line" | grep -e "usb [0-1]-[0-9]: USB disconnected, device number [0-9]") ]]; then
                echo Mouse has been disconnected
        elif [[ $(echo "$line" | grep -e "USB HID v1.10 Mouse") ]]; then
                echo Mouse has been connected
        else
                # No-op when a message that doesn't match anything occurs.
                # You can't leave an `else` clause empty or bash will yell at you
                true
        fi

done
