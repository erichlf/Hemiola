#include "Exceptions.h"
#include "InputHID.h"
#include "KeyboardEvents.h"
#include "KeyTable.h"
#include "OutputHID.h"

#include <exception>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

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

    std::wstring ws { L"Hemiola\n" };
    std::stringstream ss;

    for ( auto wc : ws ) {
        ss << std::hex << std::uppercase << std::setw ( 2 ) << std::setfill ( '0' ) << wc;
    }
    std::wcout << ss.str().c_str() << std::endl;
    try {
        output->write ( ss.str() );
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
