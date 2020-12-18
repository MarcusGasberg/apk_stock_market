#pragma once

#include <iostream>
#include "../Commands/Commands.h"

namespace stock
{
    class AccountManager
    {
    public:
        AccountManager(commands_sig_t& command_sig)
        {
            const std::function<void(std::shared_ptr<commands_var_t>)> commands_f =
                [this](const std::shared_ptr<commands_var_t> variant)
            {
                std::visit([this](auto&& command)
                    {
                        handle_command(command);
                    }, *variant);
            };

            command_sig.connect(commands_f);
        }

        template<typename Command>
        void handle_command(Command& command)
        {
            using T = std::decay_t<decltype(command)>;
            if constexpr (hasExecute<T> && std::is_same_v<T, TraderAccountDepositAction>)
            {
                command.execute();
            }
        }
    };

}
