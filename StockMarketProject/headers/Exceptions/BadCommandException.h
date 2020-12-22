#pragma once
#include <stdexcept>

namespace stock
{
    class BadCommandException : public std::runtime_error
    {
    public:
        BadCommandException() = default;

        explicit BadCommandException(const runtime_error _Message)
            : runtime_error(_Message)
        {
        }

        BadCommandException(char const* const _Message)
                : runtime_error(_Message)
        {
        }

        explicit BadCommandException(runtime_error const& _Other)
            : runtime_error(_Other)
        {
        }

        char const* what() const noexcept override
        {
            return runtime_error::what();
        };
    };
}
