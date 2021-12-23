#include "OutputHID.h"

#include "Exceptions.h"

#include <cassert>
#include <fcntl.h>
#include <sstream>
#include <unistd.h>

#include <linux/input.h>

using namespace hemiola;

hemiola::OutputHID::OutputHID ( const std::string& device )
    : HID ( device )
{}

void hemiola::OutputHID::open()
{
    HID::open ( O_WRONLY | O_SYNC );
}

void hemiola::OutputHID::write ( unsigned short code )
{
    assert ( m_Opened );

    std::stringstream s;
    s << code;

    if ( ::write ( m_HIDId, s.str().c_str(), sizeof ( unsigned short ) )
         <= 0 ) {
        throw IoException ( "Unable to write to output device.", errno );
    }
}
