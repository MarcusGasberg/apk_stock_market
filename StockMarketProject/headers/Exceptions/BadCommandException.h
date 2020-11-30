#pragma once
#include <exception>

namespace stock
{
    class BadCommandException : std::exception
    {
    public:

        BadCommandException() = default;

        explicit BadCommandException(char const* const _Message)
            : exception(_Message)
        {
        }

        BadCommandException(char const* const _Message, const int i)
            : exception(_Message, i)
        {
        }

        explicit BadCommandException(exception const& _Other)
            : exception(_Other)
        {
        }

        char const* what() const override
        {
            return exception::what();
        };
    };
}
