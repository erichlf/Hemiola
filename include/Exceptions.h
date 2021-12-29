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

#include <exception>
#include <string>

namespace hemiola
{
    /*!
     * @brief Exception to be used when something specific to KeyboardEvents should throw.
     */
    class KeyboardException : public std::exception
    {
    public:
        explicit KeyboardException ( const std::string& msg )
            : m_Msg { msg }
        {}

        virtual const char* what() const noexcept { return m_Msg.c_str(); }

    private:
        /*!
         * @brief string associated with the exception
         */
        std::string m_Msg;
    };

    /*!
     * @brief An exception that has an event code associated with it, such as errno.
     * @note This is intended to be the base class for other coded exceptions that likely will use
     *       an enum for those error codes.
     */
    class CodedException : public std::exception
    {
    public:
        CodedException ( const std::string& msg, const int code )
            : m_Msg { msg }
            , m_Code { code }
        {}

        virtual const char* what() const noexcept { return m_Msg.c_str(); }
        virtual int code() const noexcept { return m_Code; }

    private:
        /*!
         * @brief string associated with the exception
         */
        std::string m_Msg;
        /*!
         * @brief error code associated with the exception
         */
        int m_Code;
    };

    /*!
     * @brief An exception used for when trying to execute an external process
     */
    class ExecutionException : public CodedException
    {
    public:
        ExecutionException ( const std::string& msg, const int code )
            : CodedException ( msg, code )
        {}
    };

    /*!
     * @brief An exception used for when trying to do IO
     */
    class IoException : public CodedException
    {
    public:
        IoException ( const std::string& msg, const int code )
            : CodedException ( msg, code )
        {}
    };
}  // namespace hemiola
