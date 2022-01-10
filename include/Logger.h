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
#pragma once

#define SPDLOG_FMT_EXTERNAL 1
#include <spdlog/spdlog.h>

#include <memory>

/*!
 * @brief enumerator defining log levels
 */
enum
{
    INFO = 0,
    ERROR,
    WARN,
    CRITICAL,
    DEBUG,
};

/*!
 * @brief macro defining logging
 */
#define LOG( LEVEL, ... )                     \
    switch ( LEVEL ) {                        \
        case ( INFO ):                        \
            spdlog::info ( __VA_ARGS__ );     \
            break;                            \
        case ( ERROR ):                       \
            spdlog::error ( __VA_ARGS__ );    \
            break;                            \
        case ( WARN ):                        \
            spdlog::warn ( __VA_ARGS__ );     \
            break;                            \
        case ( CRITICAL ):                    \
            spdlog::critical ( __VA_ARGS__ ); \
            break;                            \
        case ( DEBUG ):                       \
            spdlog::debug ( __VA_ARGS__ );    \
            break;                            \
        default:                              \
            break;                            \
    }

namespace hemiola
{
    /*
     * @brief simple log class that does the setup of spdlog
     */
    class Logger
    {
    public:
        Logger();

        /*!
         * @brief Logger CTOR
         * @param logFile location of the log file
         * @param maxSize the maximum size of log files in bytes
         * @param maxFiles the maximum number of files to maintain
         * @post creates a daily spdlogger that output to logFile
         */
        Logger ( const std::string& logFile,
                 const std::size_t maxSize,
                 const std::size_t maxFiles );
        Logger ( const Logger& ) = delete;
        Logger ( Logger&& ) = delete;
        Logger& operator= ( const Logger& ) = delete;
        Logger& operator= ( Logger&& ) = delete;
        ~Logger();

    private:
        /*!
         * @brief location of log file
         */
        std::string m_LogFile;

        /*!
         * @brief max log size in bytes
         */
        std::size_t m_MaxSize;

        /*!
         * @brief max number of files to maintain
         */
        std::size_t m_MaxFiles;

        /*!
         * @brief our logger
         */
        std::shared_ptr<spdlog::logger> m_Log;
    };

}