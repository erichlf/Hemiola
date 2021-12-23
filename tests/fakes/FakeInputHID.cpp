#include "FakeInputHID.h"

#include "Utils.h"

#include <linux/input.h>

using namespace hemiola;

hemiola::FakeInputHID::FakeInputHID()
    : InputHID()
{}

void hemiola::FakeInputHID::open()
{
    m_Opened = true;
}

void hemiola::FakeInputHID::close()
{
    m_Opened = false;
}

void hemiola::FakeInputHID::read ( input_event& event )
{
    if ( m_Data.size() > 0 ) {
        event = m_Data.back();
        m_Data.pop_back();
    } else {
        throw IoException ( "No more data to read.", 42 );
    }
}

void hemiola::FakeInputHID::setData ( const std::vector<input_event>& events )
{
    m_Data = events;
}
