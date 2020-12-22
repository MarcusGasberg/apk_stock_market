//
// Created by stud on 12/21/20.
//

#ifndef STOCKMARKETPROJECT_NOPRICEEXCEPTION_H
#define STOCKMARKETPROJECT_NOPRICEEXCEPTION_H
#include <stdexcept>

namespace stock
{
class NoPriceException : public std::runtime_error
{
public:
    NoPriceException() = default;

    explicit NoPriceException(const runtime_error _Message)
            : runtime_error(_Message)
    {
    }

    NoPriceException(const std::string & stockid)
        : std::runtime_error("No price was found for stock with id: " + stockid)
    {

    }

    NoPriceException(char const* const _Message)
            : runtime_error(_Message)
    {
    }

    explicit NoPriceException(runtime_error const& _Other)
    : runtime_error(_Other)
            {
            }

    char const* what() const noexcept override
    {
        return runtime_error::what();
    };
};
}

#endif //STOCKMARKETPROJECT_NOPRICEEXCEPTION_H
