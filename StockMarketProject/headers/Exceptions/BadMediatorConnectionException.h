
#include <stdexcept>

namespace stock
{
    class BadMediatorConnectionException : public std::runtime_error
    {
    public:
        explicit BadMediatorConnectionException(const runtime_error _Message)
            : runtime_error(_Message)
        {
        }

        BadMediatorConnectionException(char const* const _Message)
            : runtime_error(_Message)
        {
        }

        explicit BadMediatorConnectionException(runtime_error const& _Other)
            : runtime_error(_Other)
        {
        }

        char const* what() const noexcept override
        {
            return runtime_error::what();
        };
    };
}
