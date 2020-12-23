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
            std::cout << "========= " << "Undo Latest" << " =========" << "\n";
            for (auto undo_action : undo_actions)
            {
                undo_action();
            }
        }

        void add_undo_action(std::function<void()>& undo_action)
        {
            undo_actions.push_back(undo_action);
        }
    };
}
