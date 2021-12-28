#include "OutputHID.h"

#include "Exceptions.h"

#include <cassert>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

#include <linux/input.h>

using namespace hemiola;

hemiola::OutputHID::OutputHID ( const std::string& device )
    : HID ( device )
{}

void hemiola::OutputHID::open()
{
    HID::open ( O_WRONLY | O_SYNC );
}

void hemiola::OutputHID::write ( const std::vector<uint8_t>& data )
{
    assert ( m_Opened );
    assert ( !data.empty() );

    if ( ::write ( m_HIDId, data.data(), sizeof ( uint8_t ) * data.size() )
         <= 0 ) {
        throw IoException ( "Unable to write to output device", errno );
    }
}
