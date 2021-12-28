#include "Exceptions.h"
#include "InputHID.h"
#include "KeyboardEvents.h"
#include "KeyTable.h"
#include "OutputHID.h"

#include <cstdint>
#include <exception>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

int main()
{
    using namespace hemiola;

    // TODO: Implement a config to get out devices and key maps
    // auto input = std::make_shared<InputHID>();
    auto output = std::make_shared<OutputHID>();
    // auto keys = std::make_shared<KeyTable>();

    // open devices so they can be used
    // input->open();
    output->open();

    try {
        // H
        output->write ( std::vector<uint8_t> { 0x20, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00 } );
        output->write ( std::vector<uint8_t> { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );
        // e
        output->write ( std::vector<uint8_t> { 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00 } );
        output->write ( std::vector<uint8_t> { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );
        // m
        output->write ( std::vector<uint8_t> { 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00 } );
        output->write ( std::vector<uint8_t> { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );
        // i
        output->write ( std::vector<uint8_t> { 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00 } );
        output->write ( std::vector<uint8_t> { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );
        // o
        output->write ( std::vector<uint8_t> { 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00 } );
        output->write ( std::vector<uint8_t> { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );
        // l
        output->write ( std::vector<uint8_t> { 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00 } );
        output->write ( std::vector<uint8_t> { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );
        // a
        output->write ( std::vector<uint8_t> { 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00 } );
        output->write ( std::vector<uint8_t> { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );
    } catch ( const IoException& e ) {
        std::cerr << e.what() << " errno " << e.code() << "\n";
    }
    // try {
    //     output->write ( L"\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00" );
    // } catch ( const IoException& e ) {
    //     std::cerr << e.what() << " errno " << e.code() << "\n";
    // }

    // hemiola::KeyboardEvents eventHandler ( keys, input );

    // for now we aren't going to do anything with the data
    // auto onEvent = [] ( std::variant<wchar_t, unsigned short> ) {};

    // // the exception that will be thrown by keys
    // std::exception_ptr e;
    // auto onError = [&e] ( std::exception_ptr exc ) { e = exc; };

    // auto passThrough = [&output, &onError] ( const std::wstring& ws ) {
    //     try {
    //         output->write ( ws );
    //     } catch ( ... ) {
    //         onError ( std::current_exception() );
    //     }
    // };

    // eventHandler.capture ( passThrough, onEvent, onError );
}
