#include "HID.h"

#include "Exceptions.h"

#include <cassert>
#include <fcntl.h>
#include <unistd.h>

using namespace hemiola;

hemiola::HID::HID ( const std::string& device )
    : m_HIDId { -1 }
    , m_HIDString { device }
    , m_Opened { false }
{}

hemiola::HID::HID()
    : HID ( "" )
{}

hemiola::HID::~HID()
{
    close();
}

void hemiola::HID::open ( int perms )
{
    assert ( !m_HIDString.empty() );
    assert ( !m_Opened );

    // open input device for reading
    m_HIDId = ::open ( m_HIDString.c_str(), perms );

    if ( m_HIDId == -1 ) {
        throw IoException ( "Error opening input event device '" + m_HIDString + "'", errno );
    }

    m_Opened = true;
}

void hemiola::HID::close()
{
    if ( m_Opened ) {
        ::close ( m_HIDId );
        m_Opened = false;
    }
}
