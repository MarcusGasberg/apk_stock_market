#pragma once

#include <iostream>
#include <ostream>
#include <sstream>

namespace stock
{
    class UndoLatestCommand
    {
        std::vector<std::function<void()>> undo_actions;
    public:
        
        UndoLatestCommand() = default;

        void execute()
        {
            for (auto undo_action : undo_actions)
            {
                undo_action();
            }
        }

        void add_undo_action(std::function<void()>& undo_action)
        {
            undo_actions.push_back(undo_action);
        }

        [[nodiscard]] std::string get_description() const
        {
            std::stringstream ss;
            ss << "UndoCommand";
            return ss.str();
        }

        friend std::ostream& operator<<(std::ostream& os, const UndoLatestCommand& obj)
        {
            return os << obj.get_description();
        }
    };
}
