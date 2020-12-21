
#include <stdexcept>

namespace stock
{
    class NoStockException : public std::runtime_error
    {
    public:
        explicit NoStockException(const runtime_error msg)
            : runtime_error(msg)
        {
        }

        NoStockException(const std::string & stock_id)
            : std::runtime_error("No stock was found for stock with id: " + stock_id)
        {

        }

        NoStockException(char const* const msg)
            : runtime_error(msg)
        {
        }

        explicit NoStockException(runtime_error const& other)
            : runtime_error(other)
        {
        }

        char const* what() const noexcept override
        {
            return exception::what();
        };
    };
}
