#include "InputHID.h"

#include "Exceptions.h"
#include "Utils.h"

#include <algorithm>
#include <cassert>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <unistd.h>
#include <utility>

#include <linux/input.h>

using namespace hemiola;
using ScoredHID = std::pair<std::string, unsigned short>;

hemiola::InputHID::InputHID()
    : HID()
{}

void hemiola::InputHID::open()
{
    m_HIDString = getInputHID();

    HID::open ( O_RDONLY );
}

std::string hemiola::InputHID::getInputHID()
{
    // better be safe than sory: while running other programs, switch user to nobody from root
    setegid ( 65534 );
    seteuid ( 65534 );

    // Look for devices with keybit bitmask that has keys a keyboard does
    // If a bitmask ends with 'e', it supports KEY_2, KEY_1, KEY_ESC, and KEY_RESERVED is set to 0,
    // so it's probably a keyboard keybit:
    // https://github.com/torvalds/linux/blob/02de58b24d2e1b2cf947d57205bd2221d897193c/include/linux/input.h#L45
    // keycodes:
    // https://github.com/torvalds/linux/blob/139711f033f636cc78b6aaf7363252241b9698ef/include/uapi/linux/input-event-codes.h#L75
    // Take the Name, Handlers, and KEY values
    const char* cmd = EXE_GREP " -B8 -E 'KEY=.*e$' /proc/bus/input/devices | " EXE_GREP
                               " -E 'Name|Handlers|KEY' ";
    std::stringstream output;
    try {
        output << execute ( cmd );
    } catch ( const ExecutionException& e ) {
        // log the error and rethrow
        std::cerr << "Failed to grep for keyboard device.\n";
        throw;
    }

    auto comparator = [] ( const ScoredHID& dev1, const ScoredHID& dev2 ) {
        return dev1.second > dev2.second;
    };
    std::priority_queue<ScoredHID, std::vector<ScoredHID>, decltype ( comparator )> devices (
        comparator );
    std::string line;

    std::string device;
    unsigned short lineType = 0;
    unsigned short score = 0;

    while ( std::getline ( output, line ) ) {
        std::transform ( line.begin(), line.end(), line.begin(), ::tolower );

        // Generate score based on device name
        if ( lineType == 0 ) {
            if ( line.find ( "keyboard" ) != std::string::npos ) {
                score += 100;
            }
        } else if ( lineType == 1 ) {  // Add the event handler
            std::string::size_type i = line.find ( "event" );
            if ( i != std::string::npos )
                i += 5;  // "event".size() == 5
            if ( i < line.size() ) {
                int index = atoi ( &line.c_str() [i] );

                if ( index != -1 ) {
                    std::stringstream inputDevicePath;
                    inputDevicePath << INPUT_EVENT_PATH;
                    inputDevicePath << "event";
                    inputDevicePath << index;

                    // save device string for next iteration where we determine score
                    device = inputDevicePath.str();
                }
            }
        } else if ( lineType == 2 ) {  // Generate score based on size of key bitmask
            std::string::size_type i = line.find ( "=" );
            std::string full_key_map = line.substr ( i + 1 );
            score += full_key_map.length();
            devices.push ( std::make_pair ( device, score ) );
            score = 0;
            device = "";
        }
        lineType = ( lineType + 1 ) % 3;
    }

    if ( devices.size() == 0 ) {
        throw KeyboardException ( "Couldn't determine keyboard." );
        std::cerr
            << "Please post contents of your /proc/bus/input/devices file as a new bug report.\n"
               "github.com/erichlf/hemiola\n";
    }

    // now we reclaim those root privileges
    seteuid ( 0 );
    setegid ( 0 );

    // Choose device with the best score
    return devices.top().first;
}

void hemiola::InputHID::read ( input_event& event )
{
    assert ( m_Opened );

    if ( ::read ( m_HIDId, &event, sizeof ( struct input_event ) ) <= 0 ) {
        throw IoException ( "Unable to read from input device.", errno );
    }
}
