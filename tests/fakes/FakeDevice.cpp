#include "FakeDevice.h"

#include "Utils.h"

using namespace hemiola;

hemiola::FakeDevice::FakeDevice()
    : Device()
{}

void hemiola::FakeDevice::open()
{
    m_Opened = true;
}

void hemiola::FakeDevice::close()
{
    m_Opened = false;
}

void hemiola::FakeDevice::read ( input_event& event )
{
    if ( m_Data.size() > 0 ) {
        event = m_Data.back();
        m_Data.pop_back();
    } else {
        throw IoException ( "No more data to read.", 42 );
    }
}

void hemiola::FakeDevice::setData ( const std::vector<input_event>& events )
{
    m_Data = events;
}
