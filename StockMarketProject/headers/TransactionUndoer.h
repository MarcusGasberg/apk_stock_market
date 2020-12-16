#pragma once
#include <memory>
#include <variant>
#include <boost/signals2/signal.hpp>

#include "Commands/Command.h"
#include "Commands/UndoLatestCommand.h"


namespace stock
{
    template <typename CommandVar>
    class TransactionUndoer
    {
        boost::signals2::signal<void(std::shared_ptr<CommandVar>)>& command_sig_;

    public:
        TransactionUndoer(boost::signals2::signal<void(std::shared_ptr<CommandVar>)>& command_sig)
            : command_sig_(command_sig)
        {
            const std::function<void(std::shared_ptr<CommandVar>)> commands_f = [this](std::shared_ptr<CommandVar> variant)
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


            command_sig_.connect(commands_f);
        }
    };
}
