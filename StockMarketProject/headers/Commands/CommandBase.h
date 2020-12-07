#pragma once
namespace stock
{
    class CommandBase
    {
        int id_;
        inline static int current_id;
    public:
        virtual ~CommandBase() = default;
        CommandBase() : id_(current_id++) {  }

        [[nodiscard]] int get_id() const
        {
            return id_;
        }

        [[nodiscard]] virtual std::string get_description() const
        {
            return std::to_string(get_id());
        }
    };
}
