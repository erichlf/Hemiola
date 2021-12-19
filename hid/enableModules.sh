#!/bin/bash

set -e
# This was stolen from https://randomnerdtutorials.com/raspberry-pi-zero-usb-keyboard-hid/

# check to see if anything needs to be done
isInFile=$(grep "dtoverlay=dwc2" /boot/config.txt)
if [ $isInFile -eq 1 ]; then
  # back up files before we change anything
  sudo cp /boot/config.txt /boot/config.txt.bak
  echo "dtoverlay=dwc2" | sudo tee -a /boot/config.txt
fi

isInFile=$(grep "dwc2\|libcomposite" /etc/modules )
if [ $isInFile -eq 1 ]; then
  sudo cp /etc/modules /etc/modules.bak
fi

isInFile=$(grep "dwc2" /etc/modules )
if [ $isInFile -eq 1 ]; then
  echo "dwc2" | sudo tee -a /etc/modules
fi
isInFile=$(grep "libcomposite" /etc/modules )
if [ $isInFile -eq 1 ]; then
  sudo echo "libcomposite" | sudo tee -a /etc/modules
fi
