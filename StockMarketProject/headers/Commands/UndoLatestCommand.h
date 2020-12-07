
#include <iostream>
#include <ostream>
#include <sstream>

namespace stock
{
    template<typename UndoableCommandsVar>
    class UndoLatestCommand
    {
    public:
        
        UndoLatestCommand() = default;

        void execute()
        {
            std::cout << "Doing buy\n";
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
