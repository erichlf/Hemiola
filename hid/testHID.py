#!/usr/bin/env python3
'''
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
'''

'''
This will output:
Hemiola
works
To the device the raspberry pi is connected to.
'''

NULL_CHAR = chr(0)

def write_report(report):
    with open('/dev/hidg0', 'rb+') as fd:
        fd.write(report.encode())

# Press H
write_report(chr(32) + NULL_CHAR + chr(11) + NULL_CHAR*5)
# release keys otherwise the above won't be capitalized
write_report(NULL_CHAR*8)
# Press e
write_report(NULL_CHAR*2 + chr(8) + NULL_CHAR*5)
# Press m key
write_report(NULL_CHAR*2 + chr(16) + NULL_CHAR*5)
# Press i key
write_report(NULL_CHAR*2 + chr(12) + NULL_CHAR*5)
# Press o key
write_report(NULL_CHAR*2 + chr(18) + NULL_CHAR*5)
# Press l key
write_report(NULL_CHAR*2 + chr(15) + NULL_CHAR*5)
# Press a key
write_report(NULL_CHAR*2 + chr(4) + NULL_CHAR*5)

# Press RETURN/ENTER key
write_report(NULL_CHAR*2 + chr(40) + NULL_CHAR*5)

# Release all keys
write_report(NULL_CHAR*8)

# Press w key
write_report(NULL_CHAR*2 + chr(26) + NULL_CHAR*5)
# Press o key
write_report(NULL_CHAR*2 + chr(18) + NULL_CHAR*5)
# Press r key
write_report(NULL_CHAR*2 + chr(21) + NULL_CHAR*5)
# Press k key
write_report(NULL_CHAR*2 + chr(14) + NULL_CHAR*5)
# Press s key
write_report(NULL_CHAR*2 + chr(22) + NULL_CHAR*5)

# Release all keys
write_report(NULL_CHAR*8)
