#pragma once
#include <memory>
#include <variant>
#include <boost/signals2/signal.hpp>

#include "Commands/UndoLatestCommand.h"


namespace stock
{
    class TransactionUndoer
    {

    public:
        TransactionUndoer(boost::signals2::signal<void(std::shared_ptr<commands_var_t>)>& command_sig)
        {
            const std::function<void(std::shared_ptr<commands_var_t>)> commands_f = [this](std::shared_ptr<commands_var_t> variant)
            {
                std::visit([&](auto&& command)
                    {
                        using T = std::decay_t<decltype(command)>;
                        if constexpr (!stock::hasExecute<T>)
                        {
                            return;
                        }
                        if constexpr (std::is_same_v <T, UndoLatestCommand>)
                        {
                            command.execute();
                        }
                    }, *variant);
            };


            command_sig.connect(commands_f);
        }
    };
}
