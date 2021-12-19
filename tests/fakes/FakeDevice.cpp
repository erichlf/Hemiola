#include "FakeDevice.h"

#include "Utils.h"

using namespace hemiola;

hemiola::FakeDevice::FakeDevice()
    : Device()
{}

virtual void hemiola::FakeDevice::open()
{
    m_Opened = true;
}

virtual void hemiola::FakeDevice::close()
{
    m_Opened = false;
}

virtual void hemiolaFakeDevice::read ( input_event& event )
{
    if ( m_Data.size() > 0 ) {
        event = m_Data.pop_back();
    } else {
        throw IoException ( "No more data to read.", 42 );
    }
}
