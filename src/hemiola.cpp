/*
  MIT License
  Copyright (c) 2021 Erich L Foster
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#include "Exceptions.h"
#include "InputHID.h"
#include "KeyTable.h"
#include "KeyboardEvents.h"
#include "Logger.h"
#include "OutputHID.h"

#include <csignal>
#include <cstdint>
#include <exception>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

std::condition_variable cv;
std::mutex mutex;

// setup logging here so that the logger works in try catch block
auto logger = hemiola::Logger();

static void signalHandler ( int sig )
{
    // Uninstall this handler, to avoid the possibility of an infinite regress
    signal ( SIGSEGV, SIG_DFL );
    signal ( SIGBUS, SIG_DFL );
    signal ( SIGILL, SIG_DFL );
    signal ( SIGABRT, SIG_DFL );
    signal ( SIGFPE, SIG_DFL );

    LOG ( ERROR, "Caught signal: {}", sig );
    spdlog::dump_backtrace();
    abort();
}

int main()
try {
    signal ( SIGSEGV, signalHandler );
    signal ( SIGBUS, signalHandler );
    signal ( SIGILL, signalHandler );
    signal ( SIGABRT, signalHandler );
    signal ( SIGFPE, signalHandler );

    using namespace hemiola;
    std::unique_lock lock ( mutex );

    // TODO: Implement a config to get our devices and key maps
    auto input = std::make_shared<InputHID>();
    auto output = std::make_shared<OutputHID>();
    auto keys = std::make_shared<KeyTable>();

    // open devices so they can be used
    input->open();
    output->open();

    KeyboardEvents eventHandler ( keys, input );

    // the exception that will be thrown by keys
    std::exception_ptr e;
    auto onError = [&e] ( std::exception_ptr exc ) {
        e = exc;
        cv.notify_all();
    };

    auto onEvent = [&output, &onError] ( KeyReport report, std::string keyRep ) {
        LOG ( DEBUG, keyRep );
        try {
            output->write ( report );
        } catch ( ... ) {
            onError ( std::current_exception() );
        }
    };

    auto captureThread = std::thread ( [&eventHandler, &onEvent, &onError] {
        eventHandler.capture ( std::ref ( onEvent ), std::ref ( onError ) );
        cv.notify_all();
    } );

    // run until a signal is caught
    cv.wait ( lock );
    captureThread.join();

    if ( e != nullptr ) {
        std::rethrow_exception ( e );
    }

    return EXIT_SUCCESS;
} catch ( const hemiola::CodedException& exc ) {
    LOG ( ERROR, "Exception caught: {}, {}", exc.what(), exc.code() );
    spdlog::dump_backtrace();
    return EXIT_FAILURE;
} catch ( const std::exception& exc ) {
    LOG ( ERROR, "Exception caught: {}", exc.what() );
    spdlog::dump_backtrace();
    return EXIT_FAILURE;
}
