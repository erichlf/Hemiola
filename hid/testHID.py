#!/usr/bin/env python3

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
