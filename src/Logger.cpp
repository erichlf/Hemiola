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
#include "Logger.h"

#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include <chrono>
#include <cstdlib>
#include <string>

using namespace hemiola;

hemiola::Logger::Logger ( const std::string& logFile,
                          const std::size_t maxSize,
                          const std::size_t maxFiles )
    : m_LogFile ( logFile )
    , m_MaxSize ( maxSize )
    , m_MaxFiles ( maxFiles )
{
    // TODO: read in these settings from a config
    spdlog::init_thread_pool ( 8192, 1 );
    auto sink
        = std::make_shared<spdlog::sinks::rotating_file_sink_mt> ( logFile, maxSize, maxFiles );
    m_Log = std::make_shared<spdlog::async_logger> ( "root_logger",
                                                     sink,
                                                     spdlog::thread_pool(),
                                                     spdlog::async_overflow_policy::block );
    spdlog::enable_backtrace ( 32 );
    spdlog::flush_every ( std::chrono::seconds ( 3 ) );
    spdlog::register_logger ( m_Log );
    spdlog::set_default_logger ( m_Log );
}

hemiola::Logger::Logger()
    : Logger ( "/var/log/hemiola/hemiola.log", 10 * 1024 * 1024, 3 )
{}

hemiola::Logger::~Logger()
{
    spdlog::shutdown();
}
