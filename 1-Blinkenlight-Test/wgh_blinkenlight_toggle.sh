#!/bin/bash
TGT_DEVICE="/dev/wgh_blinkenlight_0"

if [ "$EUID" -ne 0 ]; then
  echo "Needs to be run as root to write to the /dev/ file."
  exit
fi

echo "Toggling Blinkenlight - Ctrl-C to quit."

Write_To_Device() {
  # Sanity check: Make sure that the file exists, and is a character device.
  # If excluded, and the interface is removed, it just spams 1+0's into a regular file.
  # ... which is the reason this was made a function after 20m of debugging.
  if [ -c "$TGT_DEVICE" ]; then 
    echo "$1" | sudo tee "$TGT_DEVICE" > /dev/null
  fi
}


while : ; do
  Write_To_Device "1"
  sleep 1s
  Write_To_Device "0"
  sleep 1s
done
