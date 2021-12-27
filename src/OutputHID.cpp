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

void hemiola::OutputHID::write ( const std::wstring& ws )
{
    assert ( m_Opened );
    assert ( !ws.empty() );

    if ( ::write ( m_HIDId, ws.c_str(), sizeof ( wchar_t ) * ws.size() )
         <= 0 ) {
        throw IoException ( "Unable to write to output device", errno );
    }
}
