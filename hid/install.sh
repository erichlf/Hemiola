#!/bin/bash

set -e

# this sets up the raspberry pi to be a human interface device
./enableModules.sh

# this is the actual bit that defines our hid
sudo cp hemiola_usb /usr/bin/hemiola_usb
sudo chmod +x /usr/bin/hemiola_usb

# now have the hid device start on boot
isInFile=$(grep "hemiola_usb" /etc/rc.local )
if [ $isInFile -eq 1 ]; then
  sudo cp /etc/rc.local /etc/rc.local.bak
  sudo sed -i "s/exit 0/\/usr\/bin\/hemiola_usb\nexit 0/" /etc/rc.local
fi
