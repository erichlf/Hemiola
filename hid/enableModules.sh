#!/bin/bash

set -e
# This was stolen from https://randomnerdtutorials.com/raspberry-pi-zero-usb-keyboard-hid/

# back up files before we change anything
sudo cp /boot/config.txt /boot/config.txt.bak
sudo cp /etc/modules /etc/modules.bak

echo "dtoverlay=dwc2" | sudo tee -a /boot/config.txt
echo "dwc2" | sudo tee -a /etc/modules
sudo echo "libcomposite" | sudo tee -a /etc/modules
