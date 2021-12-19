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
     * @brief An exception that has an event code associated with it.
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
