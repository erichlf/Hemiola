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

/* a lot of this code was stolen from https://github.com/kernc/logkeys but then modified */

#include "Exceptions.h"

#include <stdio.h>

// these event.value-s aren't defined in <linux/input.h> ?
#define EV_BREAK 0   // when key released
#define EV_MAKE 1    // when key pressed
#define EV_REPEAT 2  // when key switches to repeating after short delay


#ifndef EXE_PS
    #define EXE_PS "/bin/ps"
#endif

#ifndef EXE_GREP
    #define EXE_GREP "/bin/grep"
#endif

#ifndef EXE_DUMPKEYS
    #define EXE_DUMPKEYS "/usr/bin/dumpkeys"
#endif

#define COMMAND_STR_DUMPKEYS \
    ( EXE_DUMPKEYS           \
      " -n | " EXE_GREP      \
      " '^\\([[:space:]]shift[[:space:]]\\)*\\([[:space:]]altgr[[:space:]]\\)*keycode'" )
#define COMMAND_STR_GET_PID                                                     \
    ( ( std::string ( EXE_PS " ax | " EXE_GREP " '" ) + program_invocation_name \
        + "' | " EXE_GREP " -v grep" )                                          \
          .c_str() )
#define COMMAND_STR_CAPSLOCK_STATE                                                               \
    ( "{ { xset q 2>/dev/null | grep -q -E 'Caps Lock: +on'; } || { setleds 2>/dev/null | grep " \
      "-q 'CapsLock on'; }; } && echo on" )

#define INPUT_EVENT_PATH "/dev/input/"  // standard path

namespace hemiola
{
    /*!
     * @brief executes a given command
     * @param cmd the command to execute
     * @return the result of the executed command
     * @throw ExecutionException if the execution of the process fails
     */
    static inline std::string execute ( const std::string& cmd )
    {
        FILE* pipe = popen ( cmd.c_str(), "r" );
        if ( !pipe )
            throw ExecutionException ( "popen error", errno );
        char buffer [4096];  // POSIX compliance suggests lines shouldn't be longer than this
        std::string result = "";
        while ( !feof ( pipe ) ) {
            if ( fgets ( buffer, sizeof ( buffer ), pipe ) != NULL )
                result += buffer;
        }
        pclose ( pipe );

        return result;
    }
}
