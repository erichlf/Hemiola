#!/bin/bash
: '
  MIT License
  Copyright (c) 2021 Erich L Foster
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
'

set -e
# This was stolen from https://randomnerdtutorials.com/raspberry-pi-zero-usb-keyboard-hid/

# check to see if anything needs to be done
isInFile=$(grep "dtoverlay=dwc2" /boot/config.txt)
if [ "$isInFile" -eq 1 ]; then
  # back up files before we change anything
  sudo cp /boot/config.txt /boot/config.txt.bak
  echo "dtoverlay=dwc2" | sudo tee -a /boot/config.txt
fi

isInFile=$(grep "dwc2\|libcomposite" /etc/modules )
if [ "$isInFile" -eq 1 ]; then
  sudo cp /etc/modules /etc/modules.bak
fi

isInFile=$(grep "dwc2" /etc/modules )
if [ "$isInFile" -eq 1 ]; then
  echo "dwc2" | sudo tee -a /etc/modules
fi
isInFile=$(grep "libcomposite" /etc/modules )
if [ "$isInFile" -eq 1 ]; then
  sudo echo "libcomposite" | sudo tee -a /etc/modules
fi
